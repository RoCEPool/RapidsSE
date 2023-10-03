#!/bin/bash

# The intent of this script file is to invoke SETH against the running RapidsSE.

# This script file is called by the file: ../jobs/testJob.sh

###############################################################################
### Environment Variables                                                   ###
###############################################################################

# If these environment variables are defined and are non-null then they will
# alter how this script executes.
#
# None

###############################################################################
### Constants                                                               ###
###############################################################################

readonly SCRIPT_NAME="$(basename "$0")"
readonly DIR="$(dirname "$0")"
readonly PROJ_ROOT="${DIR}/../../.."
readonly BOOTSTRAP_DIR="${PROJ_ROOT}/bootstrap"
readonly SETH_BIN_DIR="${PROJ_ROOT}/seth/bin"
readonly SHELL_DIR="${PROJ_ROOT}/shell"
readonly SETH_RESULTS_DIR="${DIR}/../results"

readonly COMMON_FN="${DIR}/common.fn"
readonly BOOTSTRAP_COMMON_FN="${BOOTSTRAP_DIR}/common.fn"

# Set to 1 to print debug messages in Bamboo.
readonly DEBUG=0

# How long to run the seth tests for (in seconds) before declaring that they are hung.
# Defaulting to 45 minutes.
//readonly SETH_TIMEOUT="2700"

// ALKA: increased timeout as added new tests
readonly SETH_TIMEOUT="3600"

# Maximum disk space utilisation % that we will accept on the current partition
# before we forcibly stop the tests. This is trying to catch situations where
# we write to the test log in a tight loop and eat up all disk space.
readonly MAX_DISK_USED_PCT="90"


###############################################################################
### Includes                                                                ###
###############################################################################

# Load other common functions

if [ -z "${__BAMBOO_COMMON_FN:-}" ] ; then
  #shellcheck source=common.fn
  source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }
fi

if [ -z "${__COMMON_FN:-}" ] ; then
  #shellcheck source=../../../bootstrap/common.fn
  source "${BOOTSTRAP_COMMON_FN}" || { >&2 echo "Could not load ${BOOTSTRAP_COMMON_FN}"; exit 1; }
fi


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Did the script complete successfully or not?
COMPLETED_SUCCESFULLY=0

# Do we need to stop or force stop RapidsSE?
STOP_REQ=0

# Test suffix to use in Junit results
TEST_SUFFIX=""

# Are we testing debug or release?
TARGET=""

# The path of the config file to be used for installation
CONFIG_FILE=""

# This is the collection of tests that are actually run by bamboo.
TESTLIST_FILE=""

###############################################################################
### Functions                                                               ###
###############################################################################

## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {

  if [ "${STOP_REQ}" -eq 1 ]; then
    # TODO: Stop RapidsSE if it has been installed
    # First, kill any rogue SE and SETH instances owned by the current user.
    killRogueInstances
  fi

  local statusMsg="*** Completed the Test stage                                                 ***"
  if [ "${COMPLETED_SUCCESFULLY}" -eq 0 ]; then
    statusMsg="*** Failed the Test stage                                                    ***"
  fi

  local endTime
  local elapsedTime
  endTime="$(date +%s)"
  elapsedTime="$(secondsToTime $((endTime - STARTTIME)))"

  echo ""
  echo "********************************************************************************"
  echo "${statusMsg}"
  echo "********************************************************************************"
  echo "Elapsed time: ${elapsedTime}"
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Check that the right arguments have been passed in.
## Params: the array of command line arguments

function processArgs() {

  if [ "$#" -lt 3  ] || [ "$#" -gt 5  ]; then
    errorExit "Incorrect arguments provided\n" "Usage: ./${SCRIPT_NAME} " \
              "[ --testsuffix <suffix> ]" \
              "{ *debug* | *release* } " \
              "path/of/cluster.config " \
              "path/of/testlistFile"
  fi

  if [ "$1" == "--testsuffix" ]; then
    TEST_SUFFIX="$2"
    shift
    shift
  fi

  TARGET="$1"

  if ! [[ "${TARGET}" = *debug* || "${TARGET}" = *release* ]]; then
    errorExit "Non-optional parameter 1 must contain the substring \"debug\" or \"release\""
  fi

  CONFIG_FILE="$2"
  if [ ! -f "${CONFIG_FILE}" ]; then
    errorExit "Cannot locate the cluster configuration file at: ${CONFIG_FILE}"
  fi

  TESTLIST_FILE="$3"
  if [ ! -f "${TESTLIST_FILE}" ]; then
    errorExit "Cannot locate the test list file that SETH will execute: ${TESTLIST_FILE}"
  fi

  # Convert to an absolute path
  TESTLIST_FILE="$(realpath "${TESTLIST_FILE}")"
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


###############################################################################
### Main Script                                                             ###
###############################################################################

# Cause Bash to exit on command errors, unset variables and pipe errors.
set -eEuo pipefail

# Check for a supported OS
if [ "$(uname)" != "Linux" ]; then
  errorExit "Unsupported OS. Only Linux is currently supported."
fi

# Record the start time so we can work out the elapsed time.
STARTTIME="$(date +%s)"

echo ""
echo "********************************************************************************"
echo "*** Beginning the Test stage                                                 ***"
echo "********************************************************************************"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Collect/validate the parameters
processArgs "$@"

echo "*** Test Config: ${CONFIG_FILE} ***"
echo "*** Test Build: ${TARGET} ***"
echo ""

# First get the installation dir from the config.
# To do this, load the config (it is just a bash script file)
loadClusterConfig

# Get the name of the first node
FIRST_NODE_NAME="${NODE_LIST[0]}"

# Get the installation dir of the first node
INSTALL_DIR="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "dir")"

if [ -z "${INSTALL_DIR}" ]; then
  errorExit "Couldn't get the installation directory for node ${FIRST_NODE_NAME}"
fi

# The cluster status script that prints out whether the cluster is running or not.
readonly STATUS_SCRIPT="${INSTALL_DIR}/status.sh"

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# We will invoke Seth against the first installed node.
# Extract the details from the cluster configuration.
SE_HOSTNAME="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_HOST_KEY}")"
SE_WIRELINE_PORT="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_CLIENT_PORT_KEY}")"


# Classpath of invoking SETH and the corresponding JDBC driver (in the shell directory).
# (yes, this will also load the SE Shell jar files, but this way is simpler).
# SETH is not installed so we must invoke it from the current repository.
readonly CLASSPATH="${SETH_BIN_DIR}/*:${SHELL_DIR}/*"

readonly SE_LOG_FILE="${INSTALL_DIR}/se.log"

# Additional arguments for SETH. Enable as required.
SETH_ARGS=(
  "--logsteps" 
  "--resultdir" "${SETH_RESULTS_DIR}"
  "--resultname" "results-${TARGET}.xml"
  "--resultformat" "junit"
)

if [ -n "${TEST_SUFFIX}" ]; then
  SETH_ARGS+=("--testsuffix" "${TEST_SUFFIX}")
fi

SETH_ARGS+=("-f" "${TESTLIST_FILE}")

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
readonly URL="jdbc:se://${SE_HOSTNAME}:${SE_WIRELINE_PORT}"


# Check that the cluster is running
echo "Checking that the cluster is running."

if ! CLUSTER_OUTPUT="$("${STATUS_SCRIPT}" 2>&1)" ; then
  >&2 echo -e "RapidsSE cluster is not running. Cluster status output:\n\n${CLUSTER_OUTPUT}"

  if [ -f "${SE_LOG_FILE}" ]; then
    >&2 echo ""
    >&2 echo "Here are the contents of the local RapidsSE log file:"
    cat "${SE_LOG_FILE}"
  fi
  exit 1
fi


echo "Starting SETH to run tests in testlist file: ${TESTLIST_FILE}"

SETH_START_TIME="$(date +%s)"

# Run the SE Test Harness in the background so we can monitor both it and SE.
# We run it in a subshell with "|| true" to avoid problems with set -e if
# it returns a non-zero exit code.
# shellcheck disable=SC2086
( java -cp "${CLASSPATH}" com.rapidsdata.seth.Seth -u "${URL}" "${SETH_ARGS[@]}" || true ) &
SETH_PID=$!  # This will actually be the pid of the subshell

# Now wait until SETH finishes running. If SE exits then it has crashed and we have an error.
while true
do
  # Check if SE is still running
  if ! CLUSTER_OUTPUT="$("${STATUS_SCRIPT}" 2>&1)" ; then

    # print to stderr
    >&2 echo "" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "!!! ERROR: Rapids SE CLUSTER HAS CRASHED!!!" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "" 1>&2
    >&2 echo "Cluster Status:" 1>&2
    >&2 echo "${CLUSTER_OUTPUT}" 1>&2
    >&2 echo "" 1>&2
    >&2 echo "Here are the last 250 lines of its log file:" 1>&2
    >&2 tail -n 250 "${SE_LOG_FILE}" 1>&2

    # Clean up the SETH process
    echo "Cleaning up. Killing SETH pid ${SETH_PID:-(none)}"
    kill -9 "${SETH_PID}" &>/dev/null || true
    exit 1
  fi

  # Check if SETH is finished
  if ! ps -p "${SETH_PID}" &>/dev/null ; then
    echo "SETH has exited."
    break
  fi

  # How long has SETH been running
  CURRENT_TIME="$(date +%s)"
  ELAPSED_TIME="$((CURRENT_TIME - SETH_START_TIME))"

  if [ $((ELAPSED_TIME % 10)) -eq 0 ]
  then
    echo "SETH (pid $SETH_PID) has been running for ${ELAPSED_TIME} seconds."
  fi

  # Check to see if the SETH tests have hung,
  if [ "${ELAPSED_TIME}" -gt "${SETH_TIMEOUT}" ]
  then
    >&2 echo "" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "ERROR: SETH has taken more than ${SETH_TIMEOUT} seconds to execute." 1>&2
    >&2 echo "ERROR: The test is assumed to have hung. Stopping all tasks." 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "" 1>&2

    # Cleanup the SETH process. Don't worry about SE - it'll get cleaned up later.
    echo "Cleaning up. Killing SETH pid ${SETH_PID:-(none)}"
    kill -9 "${SETH_PID}" &>/dev/null || true
    exit 1
  fi

  # Check how much disk space is used on the partition that SE is running from.
  # If it is above a certain percentage then error out.
  DISK_USED_PCT="$(df -h "${INSTALL_DIR}" | awk '{ print $5; }' | tail -n 1 | cut -d% -f1)"
  if [ "${DISK_USED_PCT}" -ge "${MAX_DISK_USED_PCT}" ]
  then
    DISK_PARTITION="$(df -h "${INSTALL_DIR}" | awk '{ print $1; }' | tail -n 1)"
    >&2 echo "" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "ERROR: Disk space is running very low (${DISK_USED_PCT}% used) on partition ${DISK_PARTITION}." 1>&2
    >&2 echo "ERROR: Stopping all tasks before they fail due to lack of disk space." 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "" 1>&2

    # Cleanup the SETH process. Don't worry about SE - it'll get cleaned up later.
    echo "Cleaning up. Killing SETH pid ${SETH_PID:-(none)}"
    kill -9 "${SETH_PID}" &>/dev/null || true
    exit 1;
  fi

  # All good. Wait a second and poll again. 
  sleep 1
done

CURRENT_TIME="$(date +%s)"
SETH_ELAPSED_TIME="$((CURRENT_TIME - SETH_START_TIME))"

if [ "${SETH_ELAPSED_TIME}" -lt 5 ]
then
  >&2 echo "SETH finished too quickly (<5 sec). Something must have gone wrong or there were little or no tests to be run." 1>&2
  exit 1
fi


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# Set this flag that the script completed successfully.
# This will be picked up in the exit hook.
COMPLETED_SUCCESFULLY=1


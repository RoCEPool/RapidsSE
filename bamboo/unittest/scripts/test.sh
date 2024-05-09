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

SCRIPT_NAME="$(basename "$0")"
DIR="$(dirname "$0")"
PROJ_ROOT="${DIR}/../../.."
BOOTSTRAP_DIR="${PROJ_ROOT}/bootstrap"
SETH_BIN_DIR="${PROJ_ROOT}/seth/bin"
SHELL_DIR="${PROJ_ROOT}/shell"
RESULTS_DIR="${DIR}/../results"
CONN_STORM_SH="${DIR}/../harnesses/connectionStorm/connectionStorm.sh"

COMMON_FN="${DIR}/common.fn"
BOOTSTRAP_COMMON_FN="${BOOTSTRAP_DIR}/common.fn"
BOOTSTRAP_BOOT_FN="${BOOTSTRAP_DIR}/boot.fn"

readonly SCRIPT_NAME
readonly DIR
readonly PROJ_ROOT
readonly BOOTSTRAP_DIR
readonly SETH_BIN_DIR
readonly SHELL_DIR
readonly RESULTS_DIR

readonly COMMON_FN
readonly BOOTSTRAP_COMMON_FN
readonly BOOTSTRAP_BOOT_FN

# Set to 1 to print debug messages in Bamboo.
readonly DEBUG=0

# How long to run the seth tests for (in seconds) before declaring that they are hung.
readonly SETH_TIMEOUT_BASE="3800"

# A multiplier that is applied to the above SETH_TIMEOUT_BASE when the tests
# are run in debug mode. This may be a fractional value (e.g. 1.5).
readonly SETH_TIMEOUT_DEBUG_MULTIPLIER=2

# Maximum disk space utilisation % that we will accept on the current partition
# before we forcibly stop the tests. This is trying to catch situations where
# we write to the test log in a tight loop and eat up all disk space.
readonly MAX_DISK_USED_PCT="90"

# The number of log file lines to show when a crash is detected
readonly LOG_FILE_LINES_TO_SHOW=250

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

if [ -z "${__BOOT_FN:-}" ] ; then
  #shellcheck source=../../../bootstrap/boot.fn
  source "${BOOTSTRAP_BOOT_FN}" || { >&2 echo "Could not load ${BOOTSTRAP_BOOT_FN}"; exit 1; }
fi


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# TODO: Add any other tests here
# The array of unit test harnesses to run. The format here is:
#  <testName> <testFunctionToCall>
TEST_TO_RUN=(
  "ConnectionStorm" "runConnectionStormTest"
  "SETH"            "runSethTest"
)

# Track the number of test harnesses that have been run and the number that
# have run to completion successfully or have been aborted due to a problem.
# This is not a measure of whether the tests have passed or failed, just
# whether the harness completed its run.
TEST_HARNESSES_RUN=0
TEST_HARNESSES_COMPLETED=0
TEST_HARNESSES_ABORTED=0

# Do we need to stop or force stop RapidsSE?
STOP_REQ=0

# Test suffix to use in Junit results
TEST_SUFFIX=""

# Are we testing debug or release?
TARGET=""

# The path of the config file to be used for installation
CONFIG_FILE=""

# The path where bamboo log files should be saved
LOG_OUTPUT_DIR=""

# This is the collection of tests that are actually run by bamboo.
TESTLIST_FILE=""

# The actual timeout that Seth should use before believing that the system
# has hung or crashed.
SETH_TIMEOUT="${SETH_TIMEOUT_BASE}"

###############################################################################
### Functions                                                               ###
###############################################################################

## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None
# shellcheck disable=SC2317
function onExit() {

  if [ "${STOP_REQ}" -eq 1 ]; then
    # First, kill any rogue SE and SETH instances owned by the current user.
    killRogueInstances
  fi

  local statusMsg="*** Completed the Test stage                                                 ***"
  if [[ "${TEST_HARNESSES_RUN}" -eq 0 || "${TEST_HARNESSES_ABORTED}" -gt 0 ]]; then
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

  if [ "$#" -lt 4  ] || [ "$#" -gt 6  ]; then
    errorExit "Incorrect arguments provided\n" "Usage: ./${SCRIPT_NAME} " \
              "[ --testsuffix <suffix> ]" \
              "{ *debug* | *release* } " \
              "path/of/cluster.config " \
              "<logfile output dir> " \
              "path/of/testlistFile"
  fi

  if [ "$1" == "--testsuffix" ]; then
    TEST_SUFFIX="$2"
    shift
    shift
  fi

  TARGET="$1"
  shift
  if ! [[ "${TARGET}" = *debug* || "${TARGET}" = *release* ]]; then
    errorExit "Non-optional parameter 1 must contain the substring \"debug\" or \"release\""
  fi

  # Adjust the Seth timeout value if we are running in debug.
  # Note that SETH_TIMEOUT is output as an integer value.
  if [[ "${TARGET}" = *debug* ]]; then
    SETH_TIMEOUT="$(echo "(${SETH_TIMEOUT_BASE} * ${SETH_TIMEOUT_DEBUG_MULTIPLIER}" $ | bc -l)"
    echo "Extending the Seth timeout value from ${SETH_TIMEOUT_BASE} to ${SETH_TIMEOUT} seconds "\
         "because we are running against a debug build of SE."
  fi

  CONFIG_FILE="$1"
  shift
  if [ ! -f "${CONFIG_FILE}" ]; then
    errorExit "Cannot locate the cluster configuration file at: ${CONFIG_FILE}"
  fi

  LOG_OUTPUT_DIR="$1"
  shift
  mkdir -p "${LOG_OUTPUT_DIR}"

  TESTLIST_FILE="$1"
  shift
  if [ ! -f "${TESTLIST_FILE}" ]; then
    errorExit "Cannot locate the test list file that SETH will execute: ${TESTLIST_FILE}"
  fi

  # Convert to an absolute path
  TESTLIST_FILE="$(realpath "${TESTLIST_FILE}")"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Write out a JUnit result file indicating that the test harness encountered
# a significant error and wasn't able to complete.
#
# Parameters:
#   $1      : The test harness name
#   $2      : The elapsed time of the test
#   $3      : The summary of the failure
#   $4      : The detail of the failure
# shellcheck disable=SC2317
function writeHarnessFailureJUnitResult() {
  local -r requiredArguments=3

  if [ "$#" -lt "${requiredArguments}" ]; then
    errorExit "Function \"writeHarnessFailureJUnitResult\" requires at least ${requiredArguments} arguments"
  fi

  local harnessName="$1"
  local elapsedTime="$2"
  local failureSummary="$3"
  local failureDetail="$4"

  local outputFile="${RESULTS_DIR}/harnessFailure-${harnessName// /}-${TARGET}.xml"

  cat << EOF > "${outputFile}"
<?xml version="1.0" encoding="UTF-8"?>
  <testsuite name="Harness Execution" time="${elapsedTime}">
      <testcase name="${harnessName} Harness Failure" classname="" time="${elapsedTime}">
          <failure message="${failureSummary}" type="Execution Error">
              ${failureDetail}
          </failure>
      </testcase>
  </testsuite>
EOF
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Grep the SE log file for a number of known error conditions that will typically
# prevent a test harness from moving forward.
#
# Parameters: None
# shellcheck disable=SC2317
function checkForJobCrash() {

  local grepOptions=(
    "-E"
    "-e" "^@THROW @THROW @THROW"
    "-e" "^INTERNAL ERROR"
    "-e" "^Caught SIG"
    "-e" "^Thread Backtrace"
  )

  # The exit code of grep is the exit code of this function
  grep "${grepOptions[@]}" "${INSTALL_DIR}"/se*.log >/dev/null 2>&1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Runs a generic unit test and monitors for SE hanging and crashing.
#
# Parameters:
#   $1      : A name of the test harness
#   $2      : A timeout in seconds for running the harness
#   $3..$n  : The test harness command line arguments
#
# Returns: exit code 0 if the harness completed successfully or
#          non-zero if it aborted due to an error.
# Note: this is not a measure of whether the tests passed, just whether the test
#       harness ran to completion.
# shellcheck disable=SC2317
function runAndMonitorTest() {
  local -r requiredArguments=3

  if [ "$#" -lt "${requiredArguments}" ]; then
    errorExit "Function \"runAndMonitorTest\" requires at least ${requiredArguments} arguments"
  fi

  local harnessName="$1"
  shift
  local harnessTimeout="$1"
  shift
  local harnessCmdLine=("$@")

  local failureSummary=""
  local failureDetail=""
  local harnessLogOutputDir="${LOG_OUTPUT_DIR}/${harnessName// /}"
  local harnessResultsDir="${RESULTS_DIR}/${harnessName// /}"

  # First, ensure that we are operating in a clean environment. A previous unit
  # test may have failed, for example. We also want to capture separate log files
  # for each unit test being run so that problems are easier to track down.

  # Stop SE if it is already running. This way we can remove the old logs cleanly.
  if isClusterRunning; then
    # shellcheck disable=SC2119
    stopCluster
  fi

  # Remove any SE log files from the install directories of each node
  removeLogs "${CONFIG_FILE}"

  # Now start the cluster. Log files will be created.
  if ! startCluster; then
    failureSummary="Cluster failed to start"
    failureDetail="Check the log files for details"
    writeHarnessFailureJUnitResult "${harnessName}" 0 "${failureSummary}" "${failureDetail}"
    copyLogs "${CONFIG_FILE}" "${harnessLogOutputDir}" "${harnessResultsDir}" || true
    false
    return
  fi

  # Run the test harness
  local harnessStartTime
  harnessStartTime="$(date +%s)"

  # Run the test harness in the background so we can monitor both it and SE.
  # We run it in a subshell with "|| true" to avoid problems with set -e if
  # it returns a non-zero exit code.
  # shellcheck disable=SC2086
  ( "${harnessCmdLine[@]}" || true ) &
  local harnessPid=$!  # This will actually be the pid of the subshell


  # Now wait until the test harness finishes running or something goes wrong with SE.
  # If SE exits then it has crashed and we have an error.
  # We will also grep the log file looking for job crashes because these will cause the
  # test harness to hang and ultimately time out.

  local clusterOutput
  local seProcessCrash=0
  local seJobCrash=0

  local currentTime
  local elapsedTime

  while true
  do
    # Check if the SE process has crashed or if a job has crashed
    if ! clusterOutput="$("${STATUS_SCRIPT}" 2>&1)" ; then
      seProcessCrash=1
      failureSummary="RapidsSE PROCESS has crashed"

      {
        echo ""
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "!!! ERROR: Rapids SE --PROCESS-- HAS CRASHED!!!"
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo ""
        echo "Cluster Status:"
        echo "${clusterOutput}"
        echo ""
        echo "Here are the last ${LOG_FILE_LINES_TO_SHOW} lines of the log file from the first node:"
        echo -e "\n-----------------------------LOG---------------------------------"
        tail -n "${LOG_FILE_LINES_TO_SHOW}" "${SE_LOG_FILE}"
        echo -e "-----------------------------LOG---------------------------------\n"
      } >&2

    elif checkForJobCrash; then
      seJobCrash=1
      failureSummary="RapidsSE JOB has crashed"
      {
        echo ""
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "!!! ERROR: Rapids SE --JOB-- HAS CRASHED!!!"
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo ""
        echo "Here are the last ${LOG_FILE_LINES_TO_SHOW} lines of the log file from the first node:"
        echo -e "\n-----------------------------LOG---------------------------------"
        tail -n "${LOG_FILE_LINES_TO_SHOW}" "${SE_LOG_FILE}"
        echo -e "-----------------------------LOG---------------------------------\n"
      } >&2
    fi

    if [[ "${seProcessCrash}" -eq 1 ||  "${seJobCrash}" -eq 1 ]]; then
      # Clean up the test harness process
      echo "Cleaning up. Killing ${harnessName} pid ${harnessPid:-(none)}"
      kill -9 "${harnessPid}" &>/dev/null || true

      # Write out a JUnit result file indicating the process/job has crashed
      failureDetail="Please refer to the RapidsSE log files for details."
      currentTime="$(date +%s)"
      elapsedTime="$((currentTime - harnessStartTime))"
      writeHarnessFailureJUnitResult "${harnessName}" "${elapsedTime}" "${failureSummary}" "${failureDetail}"

      # Stop SE
      stopCluster || true

      # Copy the logs
      copyLogs "${CONFIG_FILE}" "${harnessLogOutputDir}" "${harnessResultsDir}" || true

      false
      return
    fi

    # Check if the test harness is finished
    if ! ps -p "${harnessPid}" &>/dev/null ; then
      echo "${harnessName} has exited."
      break
    fi

    # How long has the test harness been running
    currentTime="$(date +%s)"
    elapsedTime="$((currentTime - harnessStartTime))"

    if [ $((elapsedTime % 10)) -eq 0 ]
    then
      echo "${harnessName} (pid ${harnessPid}) has been running for ${elapsedTime} seconds."
    fi

    # Check to see if the test harness tests have hung
    if [ "${elapsedTime}" -gt "${harnessTimeout}" ]
    then
      failureSummary="${harnessName} has taken more than ${harnessTimeout} seconds to execute"
      failureDetail="The test is assumed to have hung. Stopping all tasks"
      {
        echo ""
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "ERROR: ${failureSummary}."
        echo "ERROR: ${failureDetail}."
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo ""
      } >&2

      # Cleanup the test harness process. Don't worry about SE - it'll get cleaned up later.
      echo "Cleaning up. Killing ${harnessName} pid ${harnessPid:-(none)}"
      kill -9 "${harnessPid}" &>/dev/null || true

      # Write out a JUnit result file indicating the test has exceeded time limit
      writeHarnessFailureJUnitResult "${harnessName}" "${elapsedTime}" "${failureSummary}" "${failureDetail}"

      # Stop SE
      stopCluster || true

      # Copy the logs
      copyLogs "${CONFIG_FILE}" "${harnessLogOutputDir}" "${harnessResultsDir}" || true

      false
      return
    fi

    # Check how much disk space is used on the partition that SE is running from.
    # If it is above a certain percentage then error out.
    local diskUsedPct
    diskUsedPct="$(df -h "${INSTALL_DIR}" | awk '{ print $5; }' | tail -n 1 | cut -d% -f1)"
    if [ "${diskUsedPct}" -ge "${MAX_DISK_USED_PCT}" ]
    then
      local diskPartition
      diskPartition="$(df -h "${INSTALL_DIR}" | awk '{ print $1; }' | tail -n 1)"
      failureSummary="Disk space is running very low (${diskUsedPct}% used) on partition ${diskPartition}"
      failureDetail="Stopping all tasks before they fail due to lack of disk space"
      {
        echo ""
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "ERROR: ${failureSummary}."
        echo "ERROR: ${failureDetail}."
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo ""
      } >&2

      # Cleanup the test harness process.
      echo "Cleaning up. Killing ${harnessName} pid ${harnessPid:-(none)}"
      kill -9 "${harnessPid}" &>/dev/null || true

      # Write out a JUnit result file indicating the disk has run out of space
      writeHarnessFailureJUnitResult "${harnessName}" "${elapsedTime}" "${failureSummary}" "${failureDetail}"

      # Stop SE
      stopCluster || true

      # Copy the logs
      copyLogs "${CONFIG_FILE}" "${harnessLogOutputDir}" "${harnessResultsDir}" || true

      false
      return
    fi

    # All good. Wait a second and poll again.
    sleep 1
  done

  currentTime="$(date +%s)"
  local harnessElapsedTime="$((currentTime - harnessStartTime))"

  if [ "${harnessElapsedTime}" -lt 5 ]
  then
    failureSummary="${harnessName} finished too quickly (<5 sec)"
    failureDetail="${harnessName} finished too quickly (<5 sec). Something must have gone wrong or there were little or no tests to be run."
    >&2 echo "${failureDetail}" 1>&2

    # Write out a JUnit result file indicating the test finished too quickly
    writeHarnessFailureJUnitResult "${harnessName}" "${elapsedTime}" "${failureSummary}" "${failureDetail}"

    # Stop SE
    stopCluster || true

    # Copy the logs
    copyLogs "${CONFIG_FILE}" "${harnessLogOutputDir}" "${harnessResultsDir}" || true

    false
    return
  fi

  echo "${harnessName} has completed its execution. Logs have not been copied."
  true
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Execute the SETH unit test
# Params: None
#
# shellcheck disable=SC2317
function runSethTest() {
  # Classpath of invoking SETH and the corresponding JDBC driver (in the shell directory).
  # (yes, this will also load the SE Shell jar files, but this way is simpler).
  # SETH is not installed so we must invoke it from the current repository.
  local classpath="${SETH_BIN_DIR}/*:${SHELL_DIR}/*"

  # Additional arguments for SETH. Enable as required.
  local sethCmdLine=(
    "java" "-cp" "${classpath}" "com.rapidsdata.seth.Seth"
    "-u" "${URL}"
    "--logsteps"
    "--resultdir" "${RESULTS_DIR}"
    "--resultname" "seth-results-${TARGET}.xml"
    "--resultformat" "junit"
    "--lognameprefix" "seth"
  )

  if [ -n "${TEST_SUFFIX}" ]; then
    sethCmdLine+=("--testsuffix" "${TEST_SUFFIX}")
  fi

  sethCmdLine+=("-f" "${TESTLIST_FILE}")
  echo "SETH will run with the testlist file: ${TESTLIST_FILE}"

  runAndMonitorTest "SETH" "${SETH_TIMEOUT}" "${sethCmdLine[@]}"
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Execute the Connection Storm unit test
# Params: None
#
# shellcheck disable=SC2317
function runConnectionStormTest() {
  local -r runtimeSec=20
  local -r numConnections=50
  local -r connStormTimeoutSec=60

  local connStormCmdLine=(
    "${CONN_STORM_SH}"
    "-u" "${URL}?logintimeout=10"
    "-t" "${runtimeSec}"
    "-c" "${numConnections}"
    "-d" "${RESULTS_DIR}/connectionStorm-${TARGET}.xml"
  )

  runAndMonitorTest "Connection Storm" "${connStormTimeoutSec}" "${connStormCmdLine[@]}"
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

# Form a JDBC URL against the first installed node.
# Extract the details from the cluster configuration.
# The SE JDBC URL that tells the JDBC driver how to connect to SE.
SE_HOSTNAME="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_HOST_KEY}")"
SE_WIRELINE_PORT="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_CLIENT_PORT_KEY}")"

readonly URL="jdbc:se://${SE_HOSTNAME}:${SE_WIRELINE_PORT}"
readonly SE_LOG_FILE="${INSTALL_DIR}/se.log"

# Check that the cluster is running
echo "Checking that the cluster is running."

if ! STATUS_OUTPUT="$("${STATUS_SCRIPT}" 2>&1)" ; then
  >&2 echo -e "RapidsSE cluster is not running. Cluster status output:\n\n${STATUS_OUTPUT}"

  if [ -f "${SE_LOG_FILE}" ]; then
    >&2 echo ""
    >&2 echo "Here are the contents of the local RapidsSE log file:"
    cat "${SE_LOG_FILE}"
  fi
  exit 1
fi


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Run each of the unit test harnesses

i=0
while [ "${i}" -lt "${#TEST_TO_RUN[@]}" ]; do

  if [ "${i}" -gt 0 ]; then
    # Give a small pause between running subsequent tests
    sleep 5
  fi

  TEST_NAME="${TEST_TO_RUN[$i]}"
  i=$((i+1))
  TEST_FUNC="${TEST_TO_RUN[$i]}"
  i=$((i+1))

  echo -e "\n"
  echo "=========================================================="
  echo "  Starting ${TEST_NAME} test"
  echo "=========================================================="

  TEST_HARNESSES_RUN=$((TEST_HARNESSES_RUN + 1))
  if ${TEST_FUNC}; then
    LAST_RESULT="Success"
    TEST_HARNESSES_COMPLETED=$((TEST_HARNESSES_COMPLETED + 1))
  else
    LAST_RESULT="Failure"
    TEST_HARNESSES_ABORTED=$((TEST_HARNESSES_ABORTED + 1))
  fi

  echo "=========================================================="
  echo "  Finished running ${TEST_NAME} test. Result: ${LAST_RESULT}"
  echo "=========================================================="
  echo -e "\n"

done

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Exit with the appropriate error code

if [[ "${TEST_HARNESSES_RUN}" -eq 0 || "${TEST_HARNESSES_ABORTED}" -gt 0 ]]; then
  exit 1
else
  exit 0
fi
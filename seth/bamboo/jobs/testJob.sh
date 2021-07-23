#!/bin/bash

# The intent of this script file is to test RapidsSE against a specific
# runtime configuration (e.g. 1N4P).

# This script file is called by Bamboo after it has built the RapidsSE installer.

###############################################################################
### Environment Variables                                                   ###
###############################################################################




###############################################################################
### Constants                                                               ###
###############################################################################

readonly DIR="$(dirname "$0")"
readonly SCRIPTS_DIR="${DIR}/../scripts"
readonly PROJ_ROOT="${DIR}/../../.."
readonly BOOTSTRAP_DIR="${PROJ_ROOT}/bootstrap"

readonly COMMON_FN="${SCRIPTS_DIR}/common.fn"
readonly BOOTSTRAP_COMMON_FN="${BOOTSTRAP_DIR}/common.fn"

readonly RELATIVE_BAMBOO_DIR="${DIR}/.."
readonly BAMBOO_RESULTS_DIR="${RELATIVE_BAMBOO_DIR}/results"
readonly BAMBOO_LOGS_DIR="${RELATIVE_BAMBOO_DIR}/logs"
readonly BAMBOO_CONFIGS_DIR="${RELATIVE_BAMBOO_DIR}/configs"
readonly BAMBOO_TESTLISTS_DIR="${RELATIVE_BAMBOO_DIR}/testlists"


###############################################################################
### Includes                                                                ###
###############################################################################

# Load other common functions

if [ -z "${__COMMON_FN:-}" ] ; then
  #shellcheck source=../scripts/common.fn
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

# Which build type do we wish to compile?
# Valid values are "release" and "debug".
BUILD_TYPE=""

# Which configurations do we wish to run and test with.
# These values must match the names of the files in the bamboo/config directory
# as well as the names of the files in the bamboo/testlists directory.
# Examples are: "1N4P" "1N1P" "2N4P"
BUILD_CONFIG=""


###############################################################################
### Functions                                                               ###
###############################################################################

## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {

  # Kill any rogue SE and SETH instances owned by the current user.
  killRogueInstances

  # Compress any large files before they get collected by Bamboo
  compressLargeFiles


  local statusMsg="Completed Successfully"
  if [ "${COMPLETED_SUCCESFULLY}" -eq 0 ]; then
    statusMsg="Failed                "
  fi

  local endTime
  local elapsedTime
  endTime="$(date +%s)"
  elapsedTime="$(secondsToTime $((endTime - STARTTIME)))"

  echo ""
  echo "********************************************************************************"
  echo "*** Test Job ${statusMsg}                                          ***"
  echo "********************************************************************************"
  echo ""
  echo "Elapsed time: ${elapsedTime}"
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

function copyLogs() {

  local buildConfig="$1"
  local buildType="$2"

  local configFile="${BAMBOO_CONFIGS_DIR}/${buildConfig}.config"
  # shellcheck disable=SC1090
  source "${configFile}"

  sleep 5
  mkdir -p "${BAMBOO_LOGS_DIR}/${buildType}/${buildConfig}"

  for nodeName in "${NODE_LIST[@]}"; do
    # Get the installation directory
    local installDir
    installDir="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_DIR_KEY}")"

    cp "${installDir}/se.log" "${BAMBOO_LOGS_DIR}/${buildType}/${buildConfig}/se.${nodeName}.log" || true
  done

  mv "${BAMBOO_RESULTS_DIR}"/log-* "${BAMBOO_LOGS_DIR}/${buildType}/${buildConfig}/seth.log" &>/dev/null || true
}


###############################################################################
### Main Script                                                             ###
###############################################################################

# Cause Bash to exit on command errors, unset variables and pipe errors.
set -eEuo pipefail

# Record the start time so we can work out the elapsed time.
STARTTIME="$(date +%s)"

echo ""
echo "********************************************************************************"
echo "*** Beginning the Test Job                                                   ***"
echo "********************************************************************************"
echo ""
echo "Started at: $(date)"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Check the arguments
if [ "$#" -ne 2 ]; then
  errorExit "Script $0 must be called with two arguments: build_type and build_config.\ne.g. $0 \"release\" \"1N4P\""
fi

BUILD_TYPE="$1"
BUILD_CONFIG="$2"


# Resolve the config file and testlist file to be used
configFile="${BAMBOO_CONFIGS_DIR}/${BUILD_CONFIG}.config"
testlistFile="${BAMBOO_TESTLISTS_DIR}/${BUILD_CONFIG}.testlist"

# Ensure these files exist
if [ ! -f "${configFile}" ]; then
  errorExit "Config file does not exist: ${configFile}"
fi

if [ ! -f "${testlistFile}" ]; then
  errorExit "Testlist file does not exist: ${testlistFile}"
fi


# *** Install RapidsSE ***
if ! "${SCRIPTS_DIR}/install.sh" "${configFile}"; then
  exit 1
fi


# *** Start RapidsSE ***
if ! "${SCRIPTS_DIR}/start.sh" "${configFile}"; then
  # Startup failed. Copy any logs. Stop the whole cluster and exit.
  # We don't try to carry on because it is confusing if Bamboo reports
  # the job failed but any prior tests passed.
  "${SCRIPTS_DIR}/stop.sh" "${configFile}" || true
  ( copyLogs "${BUILD_CONFIG}" "${BUILD_TYPE}" ) || true
  exit 1
fi


# *** Test RapidsSE ***
testNamePrefix="${BUILD_CONFIG}_${BUILD_TYPE}"

if ! "${SCRIPTS_DIR}/test.sh" "${testNamePrefix}" "${configFile}" "${testlistFile}"; then
  # Startup failed. Copy any logs. Stop the whole cluster and exit.
  "${SCRIPTS_DIR}/stop.sh" "${configFile}" || true
  ( copyLogs "${BUILD_CONFIG}" "${BUILD_TYPE}" ) || true
  exit 1
fi


# *** Copy any log files ***
( copyLogs "${BUILD_CONFIG}" "${BUILD_TYPE}" ) || true

# *** Stop the cluster ***
"${SCRIPTS_DIR}/stop.sh" "${configFile}" || true


# Set this flag that the script completed successfully.
# This will be picked up in the exit hook.
COMPLETED_SUCCESFULLY=1
exit 0
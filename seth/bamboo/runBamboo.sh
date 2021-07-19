#!/bin/bash

# The intent of this script file is to build and start RapidsSE and run all
# of the unit tests against it. Bamboo itself should have very little knowledge
# of how to do any of this. That way we can change the layout/nature of tests
# and it remains consistent to a given checkin. i.e., we should be able to 
# check out an older version and re-run the Bamboo tests against it.

# This script file is called by Bamboo after it has checked out the repository.

###############################################################################
### Environment Variables                                                   ###
###############################################################################




###############################################################################
### Constants                                                               ###
###############################################################################

readonly DIR="$(dirname "$0")"
readonly SCRIPTS_DIR="${DIR}/scripts"
readonly PROJ_ROOT="${DIR}/../.."
readonly BIN_DIR="${PROJ_ROOT}/bin"
readonly SRC_DIR="${PROJ_ROOT}/src"
readonly BOOTSTRAP_DIR="${PROJ_ROOT}/bootstrap"

readonly COMMON_FN="${SCRIPTS_DIR}/common.fn"
readonly BOOTSTRAP_COMMON_FN="${BOOTSTRAP_DIR}/common.fn"

readonly RELATIVE_BAMBOO_DIR="seth/bamboo"  # Relative to the project root
readonly BAMBOO_RESULTS_DIR="${RELATIVE_BAMBOO_DIR}/results"
readonly BAMBOO_LOGS_DIR="${RELATIVE_BAMBOO_DIR}/logs"
readonly BAMBOO_CONFIG_DIR="${RELATIVE_BAMBOO_DIR}/config"
readonly BAMBOO_TESTLIST_DIR="${RELATIVE_BAMBOO_DIR}/testlists"

# Set to 1 to print debug messages in Bamboo.
readonly DEBUG=0

readonly NUM_CPUS="$(nproc)"

###############################################################################
### Includes                                                                ###
###############################################################################

# Load other common functions

if [ -z "${__COMMON_FN:-}" ] ; then
  #shellcheck source=scripts/common.fn
  source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }
fi

if [ -z "${__COMMON_FN:-}" ] ; then
  #shellcheck source=../../bootstrap/common.fn
  source "${BOOTSTRAP_COMMON_FN}" || { >&2 echo "Could not load ${BOOTSTRAP_COMMON_FN}"; exit 1; }
fi

###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Did the script complete successfully or not?
COMPLETED_SUCCESFULLY=0

# Which build types do we wish to test?
# Valid values are "release" and "debug".
BUILD_TYPES=( "release" "debug" )

# Which configurations do we wish to run and test with.
# These values must match the names of the files in the bamboo/config directory
# as well as the names of the files in the bamboo/testlists directory.
BUILD_CONFIGS=( "1N4P" "1N1P" "2N4P" )

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

  local statusMsg="Complete"
  if [ "${COMPLETED_SUCCESFULLY}" -eq 0 ]; then
    statusMsg="Failed  "
  fi

  ENDTIME="$(date +%s)"
  ELAPSEDTIME="$(secondsToTime $((ENDTIME - STARTTIME)))"

  echo ""
  echo "********************************************************************************"
  echo "*** Continuous Integration Script ${statusMsg}                                   ***"
  echo "********************************************************************************"
  echo ""
  echo "Elapsed time: ${ELAPSEDTIME}"
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

function copyLogs() {

  local buildConfig="$1"
  local buildType="$2"

  local configFile="${BAMBOO_CONFIG_DIR}/${buildConfig}.config"
  source "${configFile}"

  sleep 5
  mkdir -p "${BAMBOO_LOGS_DIR}/${buildType}/${buildConfig}"

  for nodeName in "${NODE_LIST[@]}"; do
    # Get the installation directory
    local installDir
    installDir="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_DIR_KEY}")"

    cp "${installDir}/se.log" "${BAMBOO_LOGS_DIR}/${buildType}/${buildConfig}/se.${nodeName}.log" || true
  done

  mv "${BAMBOO_RESULTS_DIR}"/log-* "${BAMBOO_LOGS_DIR}/${buildType}/${buildConfig}/seth.log" || true
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
echo "*** Beginning the Continuous Integration Script                              ***"
echo "********************************************************************************"
echo ""
echo "Started at: $(date)"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Check the necessary prerequisites
"${SCRIPTS_DIR}/prerequisites.sh"

# Remove any previous result and log files
rm -rf "${BAMBOO_RESULTS_DIR}" || true
rm -rf "${BAMBOO_LOGS_DIR}"    || true


# Test all build types and configurations.
# Refer to BUILD_TYPES and BUILD_CONFIGS variables above.

for buildType in "${BUILD_TYPES[@]}"; do          # release, debug, etc

  # *** Build RapidsSE ***
  # We can then test it in a number of different configurations.
  "${SCRIPTS_DIR}/build.sh" "${buildType}"


  for buildConfig in "${BUILD_CONFIGS[@]}"; do    # 1N4P, 1N1P, 2N4P, etc

    # Resolve the config file and testlist file to be used
    configFile="${BAMBOO_CONFIG_DIR}/${buildConfig}.config"
    testlistFile="${BAMBOO_TESTLIST_DIR}/${buildConfig}.testlist"

    # Ensure these files exist
    if [ ! -f "${configFile}" ]; then
      printError "Config file does not exist: ${configFile}"
      continue
    fi

    if [ ! -f "${testlistFile}" ]; then
      printError "Testlist file does not exist: ${testlistFile}"
      continue
    fi


    # *** Install RapidsSE ***
    "${SCRIPTS_DIR}/install.sh" "${configFile}"


    # *** Start RapidsSE ***
    if ! "${SCRIPTS_DIR}/start.sh" "${configFile}"; then
      # Startup failed. Copy any logs. Stop the whole cluster and exit.
      ( copyLogs "${buildConfig}" "${buildType}" ) || true
      "${SCRIPTS_DIR}/stop.sh" "${configFile}" || true
      exit 1
    fi


    # *** Test RapidsSE ***
    # If it fails then we continue on with testing another configuration
    testNamePrefix="${buildConfig}_${buildType}"
    "${SCRIPTS_DIR}/test.sh" "${testNamePrefix}" "${configFile}" "${testlistFile}" || true


    # *** Copy any log files ***
    ( copyLogs "${buildConfig}" "${buildType}" ) || true

    # *** Stop the cluster ***
    "${SCRIPTS_DIR}/stop.sh" "${configFile}" || true

  done
done

# Set this flag that the script completed successfully.
# This will be picked up in the exit hook.
COMPLETED_SUCCESFULLY=1


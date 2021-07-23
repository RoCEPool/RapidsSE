#!/bin/bash

# The intent of this script file is to compile RapidsSE and build an installer.
# This script file is called by Bamboo after it has checked out the repository.

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

readonly RELATIVE_BAMBOO_DIR="${DIR}"
readonly BAMBOO_RESULTS_DIR="${RELATIVE_BAMBOO_DIR}/results"
readonly BAMBOO_LOGS_DIR="${RELATIVE_BAMBOO_DIR}/logs"


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

# Build type to compile if none are supplied
DEFAULT_BUILD_TYPE="release"

###############################################################################
### Functions                                                               ###
###############################################################################

## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {

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
  echo "*** Build Job ${statusMsg}                                         ***"
  echo "********************************************************************************"
  echo ""
  echo "Elapsed time: ${elapsedTime}"
  echo ""
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
echo "*** Beginning the Build Job                                                  ***"
echo "********************************************************************************"
echo ""
echo "Started at: $(date)"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Check the arguments
if [ "$#" -eq 0 ]; then
  BUILD_TYPE="${DEFAULT_BUILD_TYPE}"
else
  BUILD_TYPE="$1"
fi

# Check the necessary prerequisites
"${SCRIPTS_DIR}/prerequisites.sh"

# Remove any previous result and log files
echo "Removing any previous results from: ${BAMBOO_RESULTS_DIR}"
rm -rf "${BAMBOO_RESULTS_DIR}" || true

echo "Removing any previous logs from: ${BAMBOO_LOGS_DIR}"
rm -rf "${BAMBOO_LOGS_DIR}"    || true

# *** Build RapidsSE ***
"${SCRIPTS_DIR}/build.sh" "${BUILD_TYPE}"

COMPLETED_SUCCESFULLY=1
exit 0
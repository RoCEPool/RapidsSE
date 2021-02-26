#!/bin/bash

# The intent of this script file is to build RapidsSE and produce an installer.

# This script file is called by Jenkins after it has checked out the repository.

###############################################################################
### Environment Variables                                                   ###
###############################################################################

# If these environment variables are defined and are non-null then they will
# alter how this script executes.
#
# NO_BUILD         Do not build the software (assumes it is already built).

# e.g. NO_BUILD=1 ./build.sh


###############################################################################
### Constants                                                               ###
###############################################################################

readonly SCRIPT_NAME=$(basename "$0")
readonly DIR="$(dirname "$0")"
readonly PROJ_ROOT="${DIR}/../.."
readonly BIN_DIR="${PROJ_ROOT}/bin"
readonly SRC_DIR="${PROJ_ROOT}/src"
readonly PKG_DIR="${PROJ_ROOT}/installer/packaging"
readonly OUT_DIR="${PROJ_ROOT}/installer/output"

readonly COMMON_FN="${DIR}/common.fn"

readonly RELATIVE_JENKINS_DIR="seth/jenkins"  # Relative to the project root

# Set to 1 to print debug messages in Jenkins.
readonly DEBUG=0


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

#TARGET - defined in the parameters

###############################################################################
### Functions                                                               ###
###############################################################################

# Include a file that contains all of the functions used within.
#shellcheck source=common.fn
source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {
  # not needed in this script
  true
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Check that the right arguments have been passed in.
## Params: the array of command line arguments

function processArgs() {

  if [ "$#" -ne 1 ]; then
    errorExit "Insufficient arguments provided\n" "Usage: ./${SCRIPT_NAME} ( debug | release )"
  fi

  declare -g TARGET="$1"

  if [[ "${TARGET}" != "debug" && "${TARGET}" != "release" ]]; then
    errorExit "Parameter 1 must be \"debug\" or \"release\""
  fi
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
echo "*** Beginning the Build stage                                                ***"
echo "********************************************************************************"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Collect/validate the parameters
processArgs "$@"

# First, kill any rogue SE and SETH instances owned by the current user.
killRogueInstances

# Build RapidsSE for a given target type
compileSE "${TARGET}"

# Put together the installer
assembleSE "${TARGET}"

ENDTIME="$(date +%s)"
ELAPSEDTIME="$(secondsToTime $((ENDTIME - STARTTIME)))"

echo ""
echo "********************************************************************************"
echo "*** Completed the Build stage                                                ***"
echo "********************************************************************************"
echo "Elapsed time: ${ELAPSEDTIME}"
echo ""


#!/bin/bash

# The intent of this script file is to check any prerequisites and configure
# any necessary aspects of the system, if applicable.

# This script file is called by Bamboo after it has checked out the repository.

###############################################################################
### Environment Variables                                                   ###
###############################################################################



###############################################################################
### Constants                                                               ###
###############################################################################

readonly SCRIPT_NAME=$(basename "$0")
readonly DIR="$(dirname "$0")"


readonly COMMON_FN="${DIR}/common.fn"

# Set to 1 to print debug messages in Bamboo.
readonly DEBUG=0

###############################################################################
### Includes                                                                ###
###############################################################################

# Load other common functions

if [ -z "${__BAMBOO_COMMON_FN:-}" ] ; then
  #shellcheck source=common.fn
  source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }
fi

###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Did the script complete successfully or not?
COMPLETED_SUCCESFULLY=0

###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {

  local statusMsg="*** Completed the Prerequisites stage                                        ***"
  if [ "${COMPLETED_SUCCESFULLY}" -eq 0 ]; then
    statusMsg="*** Failed the Prerequisites stage                                           ***"
  fi

  ENDTIME="$(date +%s)"
  ELAPSEDTIME="$(secondsToTime $((ENDTIME - STARTTIME)))"

  echo ""
  echo "********************************************************************************"
  echo "${statusMsg}"
  echo "********************************************************************************"
  echo "Elapsed time: ${ELAPSEDTIME}"
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Check that the right arguments have been passed in.
## Params: the array of command line arguments

function processArgs() {

  if [ "$#" -gt 0 ]; then
    errorExit "No arguments are applicable to this script: $*"
  fi
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that we are running on a supported operating system
## Params: None

function checkOS() {
  if [ "$(uname)" != "Linux" ]; then
    errorExit "Unsupported OS. Only Linux is currently supported."
  fi
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
echo "*** Beginning the Prerequisites stage                                        ***"
echo "********************************************************************************"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Collect/validate the parameters
processArgs "$@"


# Check for a supported OS
checkOS

# Check that the TPCH data has been installed
findTpchData


# Set this flag that the script completed successfully.
# This will be picked up in the exit hook.
COMPLETED_SUCCESFULLY=1


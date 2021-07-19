#!/bin/bash

# The intent of this script file is to start RapidsSE from its installed environment.

# This script file is called by Jenkins after it has built RapidsSE from source,
# created the installer, and installed the software locally.

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

readonly SCRIPT_NAME=$(basename "$0")
readonly DIR="$(dirname "$0")"
readonly PROJ_ROOT="${DIR}/../../.."
readonly BIN_DIR="${PROJ_ROOT}/bin"
readonly SRC_DIR="${PROJ_ROOT}/src"
readonly PKG_DIR="${PROJ_ROOT}/installer/packaging"
readonly OUT_DIR="${PROJ_ROOT}/installer/output"
readonly BOOTSTRAP_DIR="${PROJ_ROOT}/bootstrap"

readonly COMMON_FN="${DIR}/common.fn"
readonly BOOTSTRAP_COMMON_FN="${BOOTSTRAP_DIR}/common.fn"

# Set to 1 to print debug messages in Jenkins.
readonly DEBUG=0

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

# The path of the config file to be used for installation
CONFIG_FILE=""

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

  local statusMsg="*** Completed the Start stage                                                ***"
  if [ "${COMPLETED_SUCCESFULLY}" -eq 0 ]; then
    statusMsg="*** Failed the Start stage                                                   ***"
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

  if [ "$#" -ne 1  ]; then
    errorExit "Insufficient arguments provided\n" "Usage: ./${SCRIPT_NAME} path/of/cluster.config"
  fi

  CONFIG_FILE="$1"
  if [ ! -f "${CONFIG_FILE}" ]; then
    errorExit "Cannot locate the cluster configuration file at: ${CONFIG_FILE}"
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
echo "*** Beginning the Start stage                                                ***"
echo "********************************************************************************"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Collect/validate the parameters
processArgs "$@"


# First get the installation dir from the config.
# To do this, load the config (it is just a bash script file)
loadClusterConfig

# Get the name of the first node
FIRST_NODE="${NODE_LIST[0]}"

# Get the installation dir of the first node
INSTALL_DIR="$(nodeCfgGetValue "${FIRST_NODE}" "dir")"

if [ -z "${INSTALL_DIR}" ]; then
  errorExit "Couldn't get the installation directory for node ${FIRST_NODE}"
fi

# Start RapidsSE
"${INSTALL_DIR}/startSE.sh"

# Set this flag that the script completed successfully.
# This will be picked up in the exit hook.
COMPLETED_SUCCESFULLY=1


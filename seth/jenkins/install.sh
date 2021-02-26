#!/bin/bash

# The intent of this script file is to install RapidsSE from the installer.

# This script file is called by Jenkins after it has built RapidsSE from source
# and created the installation package.

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
readonly PROJ_ROOT="${DIR}/../.."
readonly BIN_DIR="${PROJ_ROOT}/bin"
readonly SRC_DIR="${PROJ_ROOT}/src"
readonly PKG_DIR="${PROJ_ROOT}/installer/packaging"
readonly OUT_DIR="${PROJ_ROOT}/installer/output"
readonly BOOTSTRAP_DIR="${PROJ_ROOT}/bootstrap"

readonly COMMON_FN="${DIR}/common.fn"
readonly BOOTSTRAP_COMMON_FN="${BOOTSTRAP_DIR}/common.fn"

readonly RELATIVE_JENKINS_DIR="seth/jenkins"  # Relative to the project root

# Set to 1 to print debug messages in Jenkins.
readonly DEBUG=0


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# The path of the config file to be used for installation
CONFIG_FILE=""

###############################################################################
### Functions                                                               ###
###############################################################################

# Include a file that contains all of the functions used within.
#shellcheck source=common.fn
source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }

# Load the library that will allow us to extract fields from the cluster config file.
#shellcheck source=../../bootstrap/common.fn
source "${BOOTSTRAP_COMMON_FN}" || { >&2 echo "Could not load ${BOOTSTRAP_COMMON_FN}"; exit 1; }

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {
  # Nothing to do here
  true
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
echo "*** Beginning the Install stage CI script                                    ***"
echo "********************************************************************************"
echo ""
echo "Started at: $(date)"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Collect/validate the parameters
processArgs "$@"

# Locate the installation file
INSTALLER="$(ls -1 "${OUT_DIR}"/*.run)" || true

if [ -z "${INSTALLER}" ]; then
  errorExit "RapidsSE installation file could not be found in location: ${OUT_DIR}"
fi

# Simplify the path
INSTALLER="$(realpath "${INSTALLER}")"

# Install it
"${INSTALLER}" -- --script --config "${CONFIG_FILE}"

# We need to set up a symbolic link so that the tests can load data.
# The test files ask SE to load files from ../DATA/TPCH/*. This path
# is relative to the cwd of SE, which will be something like
# /opt/rapidsse/NODE1/commands
# So we need to create /opt/rapidsse/NODE1/DATA and point it to
# the DATA folder in the current repository path.

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

readonly DATA_SYMLINK_SRC="${INSTALL_DIR}/DATA"
readonly DATA_SYMLINK_DEST="$(realpath "${PROJ_ROOT}/DATA")"

# Check if the symbolic link already exists

# If the link exists then lets delete and recreate it.
if [ -e "${DATA_SYMLINK_SRC}" ]; then
  # Is it a symbolic link?
  if [ -h "${DATA_SYMLINK_SRC}" ]; then
    rm -f "${DATA_SYMLINK_SRC}"
  else 
    errorExit "Expected to find a symbolic link at path: ${DATA_SYMLINK_SRC}"
  fi
fi

echo "Making the symbolic link from ${DATA_SYMLINK_SRC} to ${DATA_SYMLINK_DEST}"
ln -s "${DATA_SYMLINK_DEST}" "${DATA_SYMLINK_SRC}"


# Now let's do the same but for the ../TPCH symbolic link

readonly TPCH_SYMLINK_SRC="${INSTALL_DIR}/TPCH"
readonly TPCH_SYMLINK_DEST="/opt/TPCH"

# Check if the data actually exists
if [ ! -d "${TPCH_SYMLINK_DEST}" ]; then
  errorExit "TPC-H SF1 data does not exist at: ${TPCH_SYMLINK_DEST}"
fi

# Check if the symbolic link already exists

# If the link exists then lets delete and recreate it.
if [ -e "${TPCH_SYMLINK_SRC}" ]; then
  # Is it a symbolic link?
  if [ -h "${TPCH_SYMLINK_SRC}" ]; then
    rm -f "${TPCH_SYMLINK_SRC}"
  else
    errorExit "Expected to find a symbolic link at path: ${TPCH_SYMLINK_SRC}"
  fi
fi

echo "Making the symbolic link from ${TPCH_SYMLINK_SRC} to ${TPCH_SYMLINK_DEST}"
ln -s "${TPCH_SYMLINK_DEST}" "${TPCH_SYMLINK_SRC}"


ENDTIME="$(date +%s)"
ELAPSEDTIME="$(secondsToTime $((ENDTIME - STARTTIME)))"

echo ""
echo "********************************************************************************"
echo "*** Install stage CI script complete                                         ***"
echo "********************************************************************************"
echo ""
echo "Elapsed time: ${ELAPSEDTIME}"
echo ""


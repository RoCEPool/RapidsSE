#!/bin/bash

# This file is executed after the installation run file is unpacked.
# It is used to install RapidsSE on a cluster, including each single node installation.

###############################################################################
### Environment Variables                                                   ###
###############################################################################


###############################################################################
### Constants                                                               ###
###############################################################################

# For safety:
#  -e          : exit immediately when a command fails.
#  -E          : don't ignore ERR traps in shell functions and subshells.
#  -o pipefail : exits if any process in a pipeline fails (normally it is just
#                the last one).
#  -u          : treat unset variables as an error and exit immediately.
#  -x          : print each command before executing it (for debugging).
set -Eeou pipefail

#shellcheck disable=2034
readonly DIR=$(dirname "$0")

readonly SETUP_FN="setup.fn"
readonly DEFAULT_CONFIG_PATH="cluster.config"
readonly NODE_NAME_FILE="node.name"

# Default values
readonly NODE_CFG_DEFAULT_NAME_PREFIX="NODE"
readonly NODE_CFG_DEFAULT_PORT="5541"
readonly NODE_CFG_DEFAULT_DIR="/opt/rapidsse"
readonly NODE_CFG_DEFAULT_PARTITIONS="8"  # per node
readonly NODE_CFG_DEFAULT_THREADS="16"    # per node
readonly NODE_CFG_DEFAULT_MEM="32"        # in GB, per node
readonly NODE_CFG_DEFAULT_PARAMS=""

readonly MINIMUM_MEM_SIZE="4"
readonly MINIMUM_NUM_THREADS="8"


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Modes of operation, as dictated by arguments given to the script.
# Values are either 1 or 0.
CLUSTER_INSTALL=1
INTERACTIVE_INSTALL=1
ADV_WIZARD=0
DEBUG=0

# Path to the installation (run) file used.
INSTALL_FILE=""

# Path to the config file used, if any.
CONFIG_FILE=""

# The array of unique node names. The values here are used to help form the
# name of the dynamic associative arrays used to hold the configuration of
# an individual node.
declare -a NODE_LIST

# The prefix given to each node name before its sequence number.
NODE_NAME_PREFIX="${NODE_CFG_DEFAULT_NAME_PREFIX}"

# The unique number used with the above NODE_NAME_PREFIX to form the node name.
NODE_SEQ_NUM=0

# The name of the current node, given in the --name argument to the script.
CURRENT_NODE_NAME=""

###############################################################################
### Functions                                                               ###
###############################################################################

# Include a file that contains all of the functions used within.
#shellcheck source=setup.fn
source "${SETUP_FN}" || { >&2 echo "Could not load ${SETUP_FN}"; exit 1; }


###############################################################################
### Script                                                                  ###
###############################################################################

# The main script starts here.


# Process the arguments given to the script.
processArgs "$@"

if [ -z "${INSTALL_FILE}" ]; then
  findInstallFile
fi

# Print out current settings from the arguments
debugPrint ""
debugPrint "pwd = $(pwd)"
debugPrint "USER_PWD = ${USER_PWD}"
debugPrint "CLUSTER_INSTALL = ${CLUSTER_INSTALL}"
debugPrint "INTERACTIVE_INSTALL = ${INTERACTIVE_INSTALL}"
debugPrint "ADV_WIZARD = ${ADV_WIZARD}"
debugPrint "DEBUG = ${DEBUG}"
debugPrint "INSTALL_FILE = ${INSTALL_FILE}"
debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint 


if [ ${CLUSTER_INSTALL} -eq 1 ]; then
  # cluster installation
  doClusterInstall

else
  # node installation
  doNodeInstall
fi


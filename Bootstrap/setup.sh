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

readonly DIR=$(dirname "$0")


readonly SETUP_FN_FILE="setup.fn"
readonly VERSION_FILE="version"
readonly DEFAULT_CONFIG_PATH="cluster.config"

readonly CONFIG_DEFAULT_VERSION="1"

# Prefix of the name of the associative arrays for each node in the cluster.config
readonly NODE_CFG_PREFIX="NODE_CFG_"

# Names of fields within each node's associative array.
readonly NODE_CFG_NAME_KEY="name"
readonly NODE_CFG_HOST_KEY="host"
readonly NODE_CFG_PORT_KEY="port"
readonly NODE_CFG_DIR_KEY="dir"
readonly NODE_CFG_PARTITIONS_KEY="partitions"
readonly NODE_CFG_THREADS_KEY="threads"
readonly NODE_CFG_MEM_KEY="mem"
readonly NODE_CFG_PARAMS_KEY="params"

# Default values
readonly NODE_CFG_DEFAULT_NAME_PREFIX="NODE"
readonly NODE_CFG_DEFAULT_PORT="5541"
readonly NODE_CFG_DEFAULT_DIR="/opt/rapidsse"
readonly NODE_CFG_DEFAULT_PARTITIONS="8"  # per node
readonly NODE_CFG_DEFAULT_THREADS="16"    # per node
readonly NODE_CFG_DEFAULT_MEM="32"        # in GB, per node
readonly NODE_CFG_DEFAULT_PARAMS=""


readonly MINIMUM_MEM_SIZE="4"


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
source "${SETUP_FN_FILE}"


###############################################################################
### Script                                                                  ###
###############################################################################

# The main script starts here.

# Process the arguments given to the script.
processArgs "$@"

if [ -z "${INSTALL_FILE}" ]; then
  findInstallFile
fi

if [ ${DEBUG} -eq 1 ]; then
  # Print out current settings from the arguments
  echo ""
  echo "pwd = $(pwd)"
  echo "USER_PWD = ${USER_PWD}"
  echo "CLUSTER_INSTALL = ${CLUSTER_INSTALL}"
  echo "INTERACTIVE_INSTALL = ${INTERACTIVE_INSTALL}"
  echo "ADV_WIZARD = ${ADV_WIZARD}"
  echo "DEBUG = ${DEBUG}"
  echo "INSTALL_FILE = ${INSTALL_FILE}"
  echo "CONFIG_FILE = ${CONFIG_FILE}"
  echo
  #echo "$(env)"
fi


if [ ${CLUSTER_INSTALL} -eq 1 ]; then
  # cluster installation
  doClusterInstall

else
  # node installation
  doNodeInstall
fi



# Arguments
#  cluster/node installation?
#   - In cluster install mode (the default) the user is prompted for information about the cluster before
#     each node gets installed.
#   - In node install mode, a config file is supplied along with a parameter indicating which node 
#     represents the current machine. The software is unpacked, installed and configured for that node.
#  basic/advanced mode
#  script/interactive mode
#  disable sanity checks (e.g. SSH access, isRunning, etc)
#  debug/verbose

# What hosts do you wish to install this on (assuming the current host too)
#  NB: Can't use localhost, 127.0.0.1, etc.

# (Advanced: What node name prefix is to be used?) Default: "NODE"

# What directory do you wish to install this in? Default: /opt/rapidsse
# (Advanced: different directories for each node)

# How much shared memory should SE reserve on each node? Default: 4GB

# (Advanced: what port will SE listen on, on each node?) Default: 5541

# (Advanced: how many partitions on each node will SE allocate?) Default: 8

# (Advanced: how many threads will SE allocate?) Default: 16


# Confirm / Edit all information

# Run sanity checks
#  Check if all hosts are reachable by SSH.
#  Check if SE is running in any of these host:directory combinations.
#  Check if the port is not being used on any of the hosts.
#  Check if a conflicting SE SHM file exists on any of the hosts.
#  Check if the SE reserved memory size exceeds RAM.
#  Check if the SE reserved memory size exceeds /dev/shm total size (if this exists).
#  Check if the SE reserved memory size exceeds the available SHM space.
#  Check if the number of threads is more than 2x number of physical cores.
#  Check that the installation dir can be created (if not already) on each node,
#  Check that the installation dir on each node is writable.
#  If an installation already exists on any node, confirm that the user wishes to overwrite it.
#  Check that node names don't contain spaces or special characters.
#  Check that node names are unique.
#  Check that there is at least one hostname.
#  Check that each hostname is resolvable.
#  Check that NODE_SEQ_NUM is always non-zero if we read in an existing config

# If there are no problems with the sanity checks then continue.
# Otherwise prompt the user and ask if they wish to edit the information entered.


# Write out config file. Include version number of config file.

# Install to each node


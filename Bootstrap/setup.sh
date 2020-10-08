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
set -Eeo pipefail

readonly DIR=$(dirname "$0")


readonly SETUP_FN_FILE="setup.fn"


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

CLUSTER_INSTALL=1
INTERACTIVE_INSTALL=1
ADV_WIZARD=0
DEBUG=0

CONFIG_FILE_PATH=""

###############################################################################
### Functions                                                               ###
###############################################################################

# Include a file that contains all of the functions used within.
source "${SETUP_FN_FILE}"


###############################################################################
### Script                                                                  ###
###############################################################################

# The main script starts here.

# First process the arguments given to the script.
processArgs "$@"


echo "Starting the RapidsSE installation wizard"



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

# If there are no problems with the sanity checks then continue.
# Otherwise prompt the user and ask if they wish to edit the information entered.


# Write out config file. Include version number of config file.

# Install to each node


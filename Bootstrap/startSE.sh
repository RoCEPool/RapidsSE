#!/bin/bash
# This file starts RapidsSE for the entire cluster, or just a single node. 

###############################################################################
### Constants                                                               ###
###############################################################################

# Get the relative path to this file.
readonly DIR=$(dirname "$0")
readonly ROOT_DIR="${DIR}"

readonly BOOT_FN="${DIR}/boot.fn"
readonly SCRIPT_NAME=$(basename "$0")


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Default modes of operation
CLUSTER_OP=1
CLUSTER_INIT=0
CONFIG_FILE="${DIR}/cluster.config"
NAME_FILE="${DIR}/node.name"
DEBUG=0

# Will be set after loading node name file.
CURRENT_NODE_NAME=""

###############################################################################
### Includes                                                                ###
###############################################################################

# Load bootstrapping functions
#shellcheck source=boot.fn
source "${BOOT_FN}" || { >&2 echo "Could not load ${BOOT_FN}"; exit 1; }


###############################################################################
### Functions                                                               ###
###############################################################################

# None


###############################################################################
### Main Script                                                             ###
###############################################################################

# Process the arguments given to the script.
processStartStopArgs "$@"

debugPrint "CLUSTER_OP = ${CLUSTER_OP}"
debugPrint "CLUSTER_INIT = ${CLUSTER_INIT}"
debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint "NAME_FILE = ${NAME_FILE}"
debugPrint "DEBUG = ${DEBUG}"

# Import the cluster configuration
loadClusterConfig

if [ ${CLUSTER_OP} -eq 1 ]; then
  if [ "${CLUSTER_INIT}" -eq 1 ]; then
    remoteClusterInit
    exit
  fi

  startCluster

else
  loadNodeName

  if [ "${CLUSTER_INIT}" -eq 1 ]; then
    localClusterInit
    exit
  fi

  startCurrentNode
fi


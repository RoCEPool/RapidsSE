#!/usr/bin/env bash

# Description:
# ------------
#   Writes to stdout a simple JSON object with key parameters of the first
#   node of the cluster config file. For example:
#
#     {
#       "numNodes": 2,
#       "shm": 32,
#       "threads": 16,
#       "partitions": 8,
#       "params": ""
#     }
#
#  This can then be easily parsed by Python as JSON output
#
# Arguments:
# ----------
#   $1: The path to the common.fn file which is used to load and read the
#       cluster configuration file.
#   $2: The path to the cluster config file to be read.
#

# Check for the minimum version of bash
if [ "${BASH_VERSINFO[0]}" -lt 4 ]; then
  >&2 echo "ERROR: The version of bash must be >= 4. Currently running ${BASH_VERSION}"
  exit 1
fi

# Get the path to common.fn and the cluster config file
# from the arguments

if [ "$#" -ne 2 ]; then
  >&2 echo "ERROR: Expected 2 arguments to be passed to this script. Got $# instead."
  exit 1
fi

COMMON_FN="$1"
CONFIG_FILE="$2"

if [ ! -f "${COMMON_FN}" ]; then
  >&2 echo "ERROR: File not found: ${COMMON_FN}" && exit 1
fi

source "${COMMON_FN}"

loadClusterConfig

# Get the name of the first node
FIRST_NODE_NAME="${NODE_LIST[0]}"

[ -n "${FIRST_NODE_NAME}" ] || errorExit "Could not get the name of the first node in the cluster"

numNodes="${#NODE_LIST[@]}"
shm="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_MEM_KEY}")"
threads="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_THREADS_KEY}")"
partitions="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_PARTITIONS_KEY}")"
params="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_PARAMS_KEY}")"

# Write out the results in a JSON format for easy parsing by Python
cat << EOF
{
  "numNodes": ${numNodes},
  "shm": ${shm},
  "threads": ${threads},
  "partitions": ${partitions},
  "params": "${params}"
}
EOF
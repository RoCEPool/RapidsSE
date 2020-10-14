#!/bin/bash

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
CONFIG_FILE="${DIR}/cluster.config"
NAME_FILE="${DIR}/node.name"
DEBUG=0

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



###############################################################################
### Main Script                                                             ###
###############################################################################

# Process the arguments given to the script.
processArgs "$@"

if [ ${DEBUG} -eq 1 ]; then
  echo "CLUSTER_OP = ${CLUSTER_OP}"
  echo "CONFIG_FILE = ${CONFIG_FILE}"
  echo "NAME_FILE = ${NAME_FILE}"
  echo "DEBUG = ${DEBUG}"
fi

# Import the cluster configuration
loadClusterConfig

if [ ${CLUSTER_OP} -eq 1 ]; then
  startCluster

else
  loadNodeName
  startCurrentNode
fi










#if [ "$#" -ne 4 ]
#then
#  echo "Usage: $0 <nodeName> <#partitions> <size> <#threads>"
#  echo "E.g.: $0 NODE1 8 32GB 32"
#  exit 1
#fi
#
#NODE_NAME=$1
#PARTITIONS=$2
#SIZE=$3
#THREADS=$4
#
#NUM_NODES=4
##HOST_PREFIX="boray0"
#HOST_PREFIX="se"
#HOST_SUFFIX=""
#PORT="5541"
#
## Generate the list of nodes in the cluster
#declare -a NODES
#for i in $(seq -s ' ' 1 ${NUM_NODES})
#do
#  NODES+=("--node NODE${i} ${HOST_PREFIX}${i}${HOST_SUFFIX} ${PORT} ")
#done
#
#EXE_PATH="${ROOT_DIR}/Linux/bin"
#EXE="${EXE_PATH}/rapidsse"
#
#START_PARAMS="--partitionsPerNode ${PARTITIONS} --threads ${THREADS} --size ${SIZE} --name ${NODE_NAME} ${NODES[*]} --start --munged --script --metadata --commands ${ROOT_DIR}/Commands"
#
##set the library path for the rapidsSE library
#export LD_LIBRARY_PATH="${EXE_PATH}":$LD_LIBRARY_PATH
#
## SE environment variables
#export SE_ENV_METACLUE=1
#export SE_ENV_INTKEY=1
#
## how long to wait after SE has started to check that it is still running.
#SLEEP_SEC=10
#
## Declare a function to check if SE is running and if not, remove the pid file.
## If SE is running, print an error and then exit.
#function checkIfSEIsRunning() {
#
  #if [ ! -e "${PIDFILE}" ]
  #then
    ## no pid file, so we should be safe to start SE.
    #return
  #fi
#
  ## Pid file exists, but is the process running?
  #OLD_PID=$(cat "${PIDFILE}")
  #ps -p "${OLD_PID}" > /dev/null 2>&1
  #IS_RUNNING=$?
#
  #if [ "${IS_RUNNING}" -eq 0 ]
  #then
    ## print to stderr
    #>&2 echo "ERROR: Rapids SE is already running (pid ${OLD_PID})." 1>&2
    #exit 1
  #fi
#
  ## The pid in the pidfile is not running. Remove the pidfile.
  #rm -f "${PIDFILE}"
#}
#
#
## Check if a previous SE is still running or is not shut down properly.
#checkIfSEIsRunning
#
## Remove the nohup.out file if it exists so we start clean and don't keep accumulating into this file.
#rm "${NOHUP_FILE}" &>/dev/null
#
#
#echo "Starting SE."  | tee -a "${NOHUP_FILE}"
#echo "nohup ${EXE} ${START_PARAMS}  >> ${NOHUP_FILE}"
#nohup "${EXE}" ${START_PARAMS}  >> "${NOHUP_FILE}" 2>&1 &
#
## get the last pid and save it to a file
#PID=$!
#echo "${PID}" > "${PIDFILE}"
#
#echo "RapidsSE has been started as a background process (pid ${PID})." | tee -a "${NOHUP_FILE}"
#
#
## Wait for 10 seconds for it to start up
#echo "Waiting ${SLEEP_SEC} seconds for SE to start up."
#sleep ${SLEEP_SEC}
#
## Check that SE started correctly
#ps -p "${PID}" > /dev/null 2>&1
#IS_RUNNING=$?
#
#if [ "${IS_RUNNING}" -ne 0 ]
#then
  ## print to stderr
  #>&2 echo "ERROR: Rapids SE (pid ${PID}) did not startup correctly. Here is its nohup.out file:" 1>&2
  #cat "${NOHUP_FILE}"
  #exit 1
#fi
#
#echo "SE started successfully (pid ${PID})."

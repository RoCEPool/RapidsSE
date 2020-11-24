#!/bin/bash

# This is a script file for starting two nodes of SE in the Bamboo continuous 
# integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."

EXE_PATH="${ROOT_DIR}/bin"
EXE="${EXE_PATH}/rapidsse"

START_CLUSTER_OUTPUT_FILE="${ROOT_DIR}/startCluster.log"

# Declare some arrays to hold custom values for each node.
# We can't use an associative array because it won't work on MacOS due to the old
# version of bash (v3).
declare -a NODES
declare -a START_PARAMS_ARR
declare -a START_CLUSTER_PARAMS_ARR
declare -a NOHUP_FILE_ARR
declare -a PIDFILE_ARR
declare -a PIDS_ARR

# Initialise the nodes array
NODES=("NODE0" "NODE1")

### Common / Base commands ###
BASE_START_PARAMS="--size 4GB --start --progress --threads 24 --munged --script --metadata --commands ${ROOT_DIR}/commands --progress"
BASE_START_CLUSTER_PARAMS="--munged --script --commands ${ROOT_DIR}/commands --command _connectCluster --appendTrace"

i=0
for NODE in "${NODES[@]}"
do
  START_PARAMS_ARR+=("--name ${NODE} --node NODE0 127.0.0.1 5541 --node NODE1 127.0.0.1 5551 ${BASE_START_PARAMS}")
  START_CLUSTER_PARAMS_ARR+=("--name ${NODE} ${BASE_START_CLUSTER_PARAMS}")
  NOHUP_FILE_ARR+=("${ROOT_DIR}/nohupSE-${NODE}.out")
  PIDFILE_ARR+=("${ROOT_DIR}/se-${NODE}.pid")
  i=$((i+1))
done

#set the library path for the rapidsSE library
export LD_LIBRARY_PATH="${EXE_PATH}":$LD_LIBRARY_PATH

# how long to wait after SE has started to check that it is still running.
SLEEP_SEC=10

# Declare a function to check if SE is running and if not, remove the pid file.
# If SE is running, print an error and then exit.
function checkIfSEIsRunning() {
  # $1 = index of the node to check
  idx=$1
  NODE="${NODES[$idx]}"
  PIDFILE="${PIDFILE_ARR[$idx]}"

  if [ ! -e "${PIDFILE}" ]
  then
    # no pid file, so we should be safe to start SE.
    return
  fi

  # Pid file exists, but is the process running?
  OLD_PID=$(cat "${PIDFILE}")
  ps -p "${OLD_PID}" > /dev/null 2>&1
  IS_RUNNING=$?

  if [ "${IS_RUNNING}" -eq 0 ]
  then
    # print to stderr
    >&2 echo "ERROR: Rapids SE (${NODE}) is already running (pid ${OLD_PID})." 1>&2
    exit 1
  fi

  # The pid in the pidfile is not running. Remove the pidfile.
  rm -f "${PIDFILE}"
}


# Check if a previous SE is still running or is not shut down properly.
for ((i=0; i<${#NODES[@]}; i++))
do
  NODE="${NODES[${i}]}"
  checkIfSEIsRunning "${i}"

  # Remove the nohup.out file if it exists so we start clean and don't keep accumulating into this file.
  rm "${NOHUP_FILE_ARR[${i}]}" &>/dev/null
done


for ((i=0; i<${#NODES[@]}; i++))
do
  NODE="${NODES[${i}]}"

  echo "Starting SE (${NODE})."  | tee -a "${NOHUP_FILE_ARR[${i}]}"
  echo "nohup ${EXE} ${START_PARAMS_ARR[${i}]} $@"
  nohup ${EXE} ${START_PARAMS_ARR[${i}]} $@ >> "${NOHUP_FILE_ARR[${i}]}" 2>&1 &

  # get the last pid and save it to a file
  PID=$!
  PIDS_ARR+=("${PID}")
  echo "${PID}" > "${PIDFILE_ARR[${i}]}"

  echo "RapidsSE (${NODE}) has been started as a background process (pid ${PID})." | tee -a "${NOHUP_FILE_ARR[${i}]}"


  # Wait for 10 seconds for it to start up
  echo "Waiting ${SLEEP_SEC} seconds for SE (${NODE}) to start up."
  sleep ${SLEEP_SEC}

  # Check that SE started correctly
  ps -p "${PID}" > /dev/null 2>&1
  IS_RUNNING=$?

  if [ "${IS_RUNNING}" -ne 0 ]
  then
    # print to stderr
    >&2 echo "ERROR: Rapids SE (${NODE}, pid ${PID}) did not startup correctly. Here is its nohup.out file:" 1>&2
    cat "${NOHUP_FILE_ARR[${i}]}"
    exit 1
  fi
done


# Run the start cluster command

echo "Starting the SE cluster"

rm "${START_CLUSTER_OUTPUT_FILE}" 2>/dev/null
timeout -k 15s 15s "${EXE}" ${START_CLUSTER_PARAMS_ARR[0]} 1>${START_CLUSTER_OUTPUT_FILE} 2>&1 &
wait $!
START_CLUSTER_EXIT_STATUS="$?"
OUTPUT=$(cat "${START_CLUSTER_OUTPUT_FILE}" 2>/dev/null)

if [ "${START_CLUSTER_EXIT_STATUS}" -eq 124 ]
then
    >&2 echo "ERROR: The _bootCluster command timed out on RapidsSE. RapidsSE may have crashed. Here is the output of the _bootCluster command:" 1>&2
  echo "${OUTPUT}"

  >&2 echo "And here is the output of RapidsSE's nohup file (${NODES[0]}):" 1>&2
  cat "${NOHUP_FILE_ARR[${NODES[0]}]}"

  exit 1

elif [ "${START_CLUSTER_EXIT_STATUS}" -ne 0 ]
then
  >&2 echo "ERROR: Could not run the startcluster command on RapidsSE. Here is the output of the startcluster command:" 1>&2
  echo "${OUTPUT}"

  >&2 echo "And here is the output of RapidsSE's nohup file (${NODES[0]}):" 1>&2
  cat "${NOHUP_FILE_ARR[${NODES[0]}]}"

  exit 1
fi

for ((i=0; i<${#NODES[@]}; i++))
do
  NODE="${NODES[${i}]}"
  echo "SE (${NODE}) started successfully (pid ${PIDS_ARR[${i}]})."
done

# Wait for 10 seconds for cluster to start (ALKA added)
echo "Waiting ${SLEEP_SEC} seconds for SE CLUSTER to connect up."
sleep ${SLEEP_SEC}


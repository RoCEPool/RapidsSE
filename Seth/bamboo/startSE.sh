#!/bin/bash

# This is a script file for starting SE in the Bamboo continuous integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."

NOHUP_FILE="${ROOT_DIR}/nohupSE.out"
PIDFILE="${ROOT_DIR}/se.pid"

EXE_PATH="${ROOT_DIR}/Linux/bin"
EXE="${EXE_PATH}/rapidsse"
START_PARAMS="--name NODE0 --node NODE0 127.0.0.1 5541 --size 4GB --start --munged --script --metadata --commands ${ROOT_DIR}/Commands"
START_CLUSTER_PARAMS="--name NODE0 --munged --script --commands ${ROOT_DIR}/Commands --command _connectCluster --appendTrace"

START_CLUSTER_OUTPUT_FILE="${ROOT_DIR}/startCluster.log"

#set the library path for the rapidsSE library
export LD_LIBRARY_PATH="${EXE_PATH}":$LD_LIBRARY_PATH

# how long to wait after SE has started to check that it is still running.
SLEEP_SEC=10

# Declare a function to check if SE is running and if not, remove the pid file.
# If SE is running, print an error and then exit.
function checkIfSEIsRunning() {

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
    >&2 echo "ERROR: Rapids SE is already running (pid ${OLD_PID})." 1>&2
    exit 1
  fi

  # The pid in the pidfile is not running. Remove the pidfile.
  rm -f "${PIDFILE}"
}


# Check if a previous SE is still running or is not shut down properly.
checkIfSEIsRunning

# Remove the nohup.out file if it exists so we start clean and don't keep accumulating into this file.
rm "${NOHUP_FILE}" &>/dev/null


echo "Starting SE."  | tee -a "${NOHUP_FILE}"
nohup "${EXE}" ${START_PARAMS} $@ >> "${NOHUP_FILE}" 2>&1 &

# get the last pid and save it to a file
PID=$!
echo "${PID}" > "${PIDFILE}"

echo "RapidsSE has been started as a background process (pid ${PID})." | tee -a "${NOHUP_FILE}"


# Wait for 10 seconds for it to start up
echo "Waiting ${SLEEP_SEC} seconds for SE to start up."
sleep ${SLEEP_SEC}

# Check that SE started correctly
ps -p "${PID}" > /dev/null 2>&1
IS_RUNNING=$?

if [ "${IS_RUNNING}" -ne 0 ]
then
  # print to stderr
  >&2 echo "ERROR: Rapids SE (pid ${PID}) did not startup correctly. Here is its nohup.out file:" 1>&2
  cat "${NOHUP_FILE}"
  exit 1
fi

# Run the start cluster command

echo "Starting the SE cluster"

rm "${START_CLUSTER_OUTPUT_FILE}" 2>/dev/null
timeout -k 15s 15s "${EXE}" ${START_CLUSTER_PARAMS} 1>${START_CLUSTER_OUTPUT_FILE} 2>&1 &
wait $!
START_CLUSTER_EXIT_STATUS="$?"
OUTPUT=$(cat "${START_CLUSTER_OUTPUT_FILE}" 2>/dev/null)

if [ "${START_CLUSTER_EXIT_STATUS}" -eq 124 ]
then
    >&2 echo "ERROR: The _bootCluster command timed out on RapidsSE. RapidsSE may have crashed. Here is the output of the _bootCluster command:" 1>&2
  echo "${OUTPUT}"

  >&2 echo "And here is the output of RapidsSE's nohup file:" 1>&2
  cat "${NOHUP_FILE}"

  exit 1

elif [ "${START_CLUSTER_EXIT_STATUS}" -ne 0 ]
then
  >&2 echo "ERROR: Could not run the _bootCluster command on RapidsSE. Here is the output of the _bootCluster command:" 1>&2
  echo "${OUTPUT}"

  >&2 echo "And here is the output of RapidsSE's nohup file:" 1>&2
  cat "${NOHUP_FILE}"

  exit 1
fi

echo "SE started successfully (pid ${PID})."

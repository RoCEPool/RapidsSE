#!/bin/bash

# This is a script file for starting the SE prompt.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."

NOHUP_FILE="${ROOT_DIR}/nohupSE.out"
PIDFILE="${ROOT_DIR}/se.pid"

EXE_PATH="${ROOT_DIR}/Linux/bin"
EXE="${EXE_PATH}/rapidsse"
START_PARAMS="--name NODE0  --prompt --metadata --commands ${ROOT_DIR}/Commands --appendTrace"

#set the library path for the rapidsSE library
export LD_LIBRARY_PATH="${EXE_PATH}":$LD_LIBRARY_PATH


# Declare a function to check if SE is running and if not, remove the pid file.
# If SE is running, print an error and then exit.
function checkIfSEIsRunning() {

  if [ ! -e "${PIDFILE}" ]
  then
    >&2 echo "ERROR: Rapids SE does not appear to be running (no pid file present: se.pid)." 1>&2
    exit 1
  fi

  # Pid file exists, but is the process running?
  OLD_PID=$(cat "${PIDFILE}")
  ps -p "${OLD_PID}" > /dev/null 2>&1
  IS_RUNNING=$?

  if [ "${IS_RUNNING}" -ne 0 ]
  then
    >&2 echo "ERROR: Rapids SE does not appear to be running (no pid file present: se.pid)." 1>&2
    exit 1
  fi
}


# Check if a previous SE is still running or is not shut down properly.
checkIfSEIsRunning

"${EXE}" ${START_PARAMS} $@ 


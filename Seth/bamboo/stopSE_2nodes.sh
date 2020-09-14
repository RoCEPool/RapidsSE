#!/bin/bash

# This is a script file for stopping a 2 node SE cluster in the Bamboo continuous integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."

EXE_DIR="${ROOT_DIR}/Linux/bin"
EXE="${EXE_DIR}/rapidsse"

EXIT_SLEEP_TIME=0.2   # How long to sleep (in seconds) between checking if the pid has exited.
EXIT_GIVE_UP_TIME=5   # After this time (in seconds) just kill the pid.
EXIT_ITERATIONS=$(bc <<< "${EXIT_GIVE_UP_TIME} / ${EXIT_SLEEP_TIME}")

declare -a NODES
declare -a STOP_PARAMS_ARR
declare -a PIDFILE_ARR
declare -a PIDS_ARR

# Initialise the nodes array
NODES=("NODE0" "NODE1")

i=0
for NODE in "${NODES[@]}"
do
  STOP_PARAMS_ARR+=("--name ${NODE} --stop --commands ${ROOT_DIR}/Commands --appendTrace")
  PIDFILE_ARR+=("${ROOT_DIR}/se-${NODE}.pid")
  i=$((i+1))
done

#set the library path for the rapidsSE library
ANTLR_PATH="${ROOT_DIR}/ThirdParty/antlr/lib/linux/centos7"
export LD_LIBRARY_PATH="${EXE_DIR}":"${ANTLR_PATH}":$LD_LIBRARY_PATH

if [ ! -f "${EXE}" ]
then
  echo "Not stopping SE because the binary does not exist."
  exit 1
fi


for ((i=0; i<${#NODES[@]}; i++))
do
  NODE="${NODES[${i}]}"

  if [ -f "${PIDFILE_ARR[${i}]}" ]
  then
  
    # Get the pid
    PIDS_ARR+=($(cat "${PIDFILE_ARR[${i}]}" 2>/dev/null))
  
    # This will set a flag in the SE shared memory segment to tell any servers to unlink and exit.
    echo "Stopping RapidsSE (${NODE}) with pid ${PIDS_ARR[${i}]}."
    ${EXE} ${STOP_PARAMS_ARR[${i}]} "$@"

    # Now wait for the process to stop. While loop exits if ps -p has a non-zero exit code.
    COUNTER=0
  
    while ps -p "${PIDS_ARR[${i}]}" &> /dev/null
    do
  
      if [ "${COUNTER}" -ge "${EXIT_ITERATIONS}" ]
      then
        break;
      fi
  
      (( COUNTER++ ))
      sleep "${EXIT_SLEEP_TIME}"
    done
  
    # Check again that the process has been stopped. If it hasn't then we shall kill it.
    if ps -p "${PIDS_ARR[${i}]}" &> /dev/null    # Exit code 0 means se is still running. The 'if' fires on a 0 exit code.
    then
      echo "Killing RapidsSE (${NODE}) process ${PIDS_ARR[${i}]}."
      kill -9 "${PIDS_ARR[${i}]}" &>/dev/null
    fi
  
    # Remove the pid file
    rm -f "${PIDFILE_ARR[${i}]}" &>/dev/null
  
  else
  
    # Kill by process name, restricted to processes owned by the current user.
    # We need to set the next 2 variables to avoid "illegal byte sequence" errors.
    echo "PID file missing, so stopping RapidsSE by killing all processes by that name."
    export LC_CTYPE=C
    export LANG=C
  
    pkill -u $UID -x rapidsse 2>/dev/null
  
  fi

  echo "Rapids SE (${NODE}) has been stopped."
done


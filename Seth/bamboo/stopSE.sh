#!/bin/bash

# This is a script file for stopping SE in the Bamboo continuous integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."

PIDFILE="${ROOT_DIR}/se.pid"

EXE_DIR="${ROOT_DIR}/bin"
EXE="${EXE_DIR}/rapidsse"
STOP_PARAMS="--name NODE0 --stop --commands ${ROOT_DIR}/Commands --appendTrace"

EXIT_SLEEP_TIME=0.2   # How long to sleep (in seconds) between checking if the pid has exited.
EXIT_GIVE_UP_TIME=5   # After this time (in seconds) just kill the pid.
EXIT_ITERATIONS=$(bc <<< "${EXIT_GIVE_UP_TIME} / ${EXIT_SLEEP_TIME}")

#set the library path for the rapidsSE library
export LD_LIBRARY_PATH="${EXE_DIR}":$LD_LIBRARY_PATH

if [ ! -f "${EXE}" ]
then
  echo "Not stopping SE because the binary does not exist."
  exit 1
fi

if [ -f "${PIDFILE}" ]
then

  # Get the pid
  PID=$(cat "${PIDFILE}") 2>/dev/null

  # This will set a flag in the SE shared memory segment to tell any servers to unlink and exit.
  echo "Stopping RapidsSE with pid ${PID}."
  ${EXE} ${STOP_PARAMS} "$@"

  # Now wait for the process to stop. While loop exits if ps -p has a non-zero exit code.
  COUNTER=0

  while ps -p "${PID}" &> /dev/null
  do

    if [ "${COUNTER}" -ge "${EXIT_ITERATIONS}" ]
    then
      break;
    fi

    (( COUNTER++ ))
    sleep "${EXIT_SLEEP_TIME}"
  done

  # Check again that the process has been stopped. If it hasn't then we shall kill it.
  if ps -p "${PID}" &> /dev/null    # Exit code 0 means se is still running. The 'if' fires on a 0 exit code.
  then
    echo "Killing RapidsSE process ${PID}."
    kill -9 "${PID}" &>/dev/null
  fi

  # Remove the pid file
  rm -f "${PIDFILE}" &>/dev/null

else

  # Kill by process name, restricted to processes owned by the current user.
  # We need to set the next 2 variables to avoid "illegal byte sequence" errors.
  echo "PID file missing, so stopping RapidsSE by killing all processes by that name."
  export LC_CTYPE=C
  export LANG=C

  pkill -u $UID -x rapidsse 2>/dev/null

fi

echo "Rapids SE has been stopped."

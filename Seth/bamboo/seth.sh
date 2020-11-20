#!/bin/bash

# This is the script file that runs in the Bamboo continuous integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."
CLASSPATH=".:*:${DIR}:${DIR}/*:${DIR}/..:${DIR}/../*"

# This is the colleciton of tests that are actually run by bamboo.
TESTLIST_FILE="${DIR}/allTestsBamboo"

# How long to run the seth tests for (in seconds) before declaring that they are hung.
# Defaulting to 15 minutes.
SETH_TIMEOUT="1800"

#SE_PATH="../OS X/Build/Products/Debug/SE"
NODE="NODE0"
SE_DIR="${ROOT_DIR}/bin"
SE_PATH="${SE_DIR}/rapidsse"
SE_NOHUP_FILE="${ROOT_DIR}/nohupSE.out"
SE_ARGS="--name ${NODE} --node ${NODE} 127.0.0.1 5541 --munged --commands ./Commands --appendTrace"  # in addition to --metadata and --script

export LD_LIBRARY_PATH="${SE_DIR}":$LD_LIBRARY_PATH

# Additional arguments for SETH. Enable as required.
SETH_ARGS="--logsteps --resultdir ${DIR}/results --resultformat junit -f ${TESTLIST_FILE}" 

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:rapidsse:${SE_PATH}?extraargs=${SE_ARGS}"

SE_PIDFILE="${ROOT_DIR}/se.pid"

if [ ! -f "${SE_PIDFILE}" ]
then
  >&2 echo "Not starting SETH because SE does not appear to be running. The pidfile does not exist." 1>&2
  exit 1
fi

SE_PID=$(cat "${SE_PIDFILE}") 2>/dev/null

# Check that SE is running
ps -p "${SE_PID}" > /dev/null 2>&1
IS_SE_RUNNING=$?

if [ "${IS_SE_RUNNING}" -ne 0 ]
then
  # print to stderr
  >&2 echo "ERROR: Rapids SE (pid ${PID}) did not startup correctly. Here is its nohup.out file:" 1>&2
  cat "${SE_NOHUP_FILE}"
  exit 1
fi

SETH_START_TIME="$(date +%s)"

# Run the SE Test Harness in the background so we can monitor both it and SE.
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS} "$@" &

SETH_PID=$!

# Now wait until SETH finishes running. If SE exits then it has crashed and we have an error.
while true
do
  # Check if SE is still running
  ps -p "${SE_PID}" &>/dev/null
  IS_SE_RUNNING=$?

  if [ "${IS_SE_RUNNING}" -ne 0 ]
  then
    # print to stderr
    >&2 echo "" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "!!! ERROR: Rapids SE (pid ${SE_PID}) HAS CRASHED!!!" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "" 1>&2
    >&2 echo "Here are the last 200 lines of its nohup.out file:" 1>&2
    >&2 tail -n 200 "${SE_NOHUP_FILE}" 1>&2

    # Were there any core files produced?
    CORE_FILES=$(find . -name "core.*" 2>/dev/null)
    if [ -z "${CORE_FILES}" ]
    then
      >&2 echo "No core file was produced." 1>&2
    else
      >&2 echo "Core file was produced as: ${CORE_FILES}" 1>&2
    fi

    # Clean up the SETH process and its subordinate SE prompt
    SE_PROMPT_PID=$(ps -o pid= --ppid ${SETH_PID})
    kill -9 "${SETH_PID}" &>/dev/null
    echo "${SE_PROMPT_PID}" | xargs -r kill -9 
    exit 1
  fi

  # Check if SETH is finished
  ps -p "${SETH_PID}" &>/dev/null
  IS_SETH_RUNNING=$?

  if [ "${IS_SETH_RUNNING}" -ne 0 ]
  then
    echo "SETH has exited."
    break
  fi

  # How long has SETH been running
  CURRENT_TIME="$(date +%s)"
  ELAPSED_TIME="$((CURRENT_TIME - SETH_START_TIME))"

  if [ $((ELAPSED_TIME % 10)) -eq 0 ]
  then
    echo "SETH (pid $SETH_PID) has been running for ${ELAPSED_TIME} seconds."
  fi

  # Check to see if the SETH tests have hung,
  if [ "${ELAPSED_TIME}" -gt "${SETH_TIMEOUT}" ]
  then
    >&2 echo "" 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "ERROR: SETH has taken more than ${SETH_TIMEOUT} seconds to execute." 1>&2
    >&2 echo "ERROR: The test is assumed to have hung. Stopping all tasks." 1>&2
    >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
    >&2 echo "" 1>&2

    # Ensure we kill any SE prompt with SETH. Don't worry about SE - it'll get cleaned up later.
    SE_PROMPT_PID=$(ps -o pid= --ppid ${SETH_PID})
    kill -9 "${SETH_PID}" &>/dev/null
    kill -9 "${SE_PROMPT_PID}"
    exit 1
  fi

  # All good. Wait a second and poll again. 
  sleep 1
done

CURRENT_TIME="$(date +%s)"
ELAPSED_TIME="$((CURRENT_TIME - SETH_START_TIME))"

if [ "${ELAPSED_TIME}" -lt 5 ] 
then
  >&2 echo "SETH finished too quickly (<5 sec). Something must have gone wrong." 1>&2
  exit 1
fi

# successful finish
true

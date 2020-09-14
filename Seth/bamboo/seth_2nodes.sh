#!/bin/bash

# This is the script file that runs in the Bamboo continuous integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."
CLASSPATH=".:*:${DIR}:${DIR}/*:${DIR}/..:${DIR}/../*"

# This is the colleciton of tests that are actually run by bamboo.
# Run the special multi-node test list.
TESTLIST_FILE="${DIR}/allTestsBambooMN"

# How long to run the seth tests for (in seconds) before declaring that they are hung.
# Defaulting to 15 minutes.
SETH_TIMEOUT="500"

# The minimum time (in seconds) that we expect the seth tests to run for. If they take 
# less than this then something has gone wrong.
SETH_MIN_RUNTIME="0"

declare -a NODES
NODES=("NODE0" "NODE1")

SE_DIR="${ROOT_DIR}/Linux/bin"
SE_PATH="${SE_DIR}/rapidsse"
SE_ARGS="--name ${NODES[0]} --munged --commands ${ROOT_DIR}/Commands --appendTrace"  # in addition to --metadata and --script

ANTLR_DIR="${ROOT_DIR}/ThirdParty/antlr/lib/linux/centos7"
export LD_LIBRARY_PATH="${SE_DIR}":"${ANTLR_DIR}":$LD_LIBRARY_PATH

# Additional arguments for SETH. Enable as required.
SETH_ARGS="--logsteps --resultdir ${DIR}/results --resultformat junit -f ${TESTLIST_FILE}" 

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:rapidsse:${SE_PATH}?extraargs=${SE_ARGS}"

declare -a PIDFILE_ARR
declare -a PIDS
declare -a NOHUP_FILE_ARR

for NODE in "${NODES[@]}"
do
  NOHUP_FILE_ARR+=("${ROOT_DIR}/nohupSE-${NODE}.out")
  PIDFILE_ARR+=("${ROOT_DIR}/se-${NODE}.pid")
done

# Check that there is a pidfile for each node.
for ((i=0; i<${#NODES[@]}; i++))
do
  node="${NODES[${i}]}"
  pidfile="${PIDFILE_ARR[${i}]}"

  if [ ! -f "${pidfile}" ]
  then
    >&2 echo "Not starting SETH because SE (${node}) does not appear to be running. The pidfile (${pidfile}) does not exist." 1>&2
    exit 1
  fi

  pid=$(cat "${pidfile}") 2>/dev/null
  PIDS+=("${pid}")

  # Check that this SE node is running
  ps -p "${pid}" > /dev/null 2>&1
  isSERunning=$?

  if [ "${isSERunning}" -ne 0 ]
  then
    # print to stderr
    >&2 echo "ERROR: Rapids SE (${NODES[${i}]}, pid ${pid}) did not startup correctly. Here is its nohup file (${NOHUP_FILE_ARR[${i}]}):" 1>&2
    cat "${NOHUP_FILE_ARR[${i}]}"
    exit 1
  fi
done


SETH_START_TIME="$(date +%s)"

# Run the SE Test Harness in the background so we can monitor both it and SE.
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS} "$@" &

SETH_PID=$!

# Now wait until SETH finishes running. If either SE node exits then it has crashed and we have an error.
while true
do
  for ((i=0; i<${#NODES[@]}; i++))
  do

    node="${NODES[${i}]}"
    pid="${PIDS[${i}]}"

    # Check if SE is still running
    ps -p "${pid}" &>/dev/null
    isSERunning=$?
  
    if [ "${isSERunning}" -ne 0 ]
    then
      # print to stderr
      >&2 echo "" 1>&2
      >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
      >&2 echo "!!! ERROR: Rapids SE (${node}, pid ${pid}) HAS CRASHED!!!" 1>&2
      >&2 echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 1>&2
      >&2 echo "" 1>&2
      >&2 echo "Here are the last 200 lines of its nohup.out file:" 1>&2
      >&2 tail -n 200 "${NOHUP_FILE_ARR[${i}]}" 1>&2
  
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
  done  # for each node
  
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

if [ "${ELAPSED_TIME}" -lt "${SETH_MIN_RUNTIME}" ] 
then
  >&2 echo "SETH finished too quickly (<${SETH_MIN_RUNTIME} sec). Something must have gone wrong." 1>&2
  exit 1
fi

# successful finish
true

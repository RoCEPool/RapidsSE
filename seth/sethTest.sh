#!/bin/bash

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/.."
CLASSPATH=".:*:${DIR}:${DIR}/*"

SE_DIR="${ROOT_DIR}/bin"
SE_PATH="${SE_DIR}/rapidsse"
SE_ARGS="--name NODE0 --sen ../sen --commands ../commands --appendTrace"  # in addition to --metadata and --script

# Additional arguments for SETH. Enable as required.
#SETH_ARGS="--clean --logtests"   # log all test executions to a file
SETH_ARGS="--clean --logsteps"   # log executions of all steps within each test to a file.

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:rapidsse:${SE_PATH}?extraargs=${SE_ARGS}"

# Run the SE Test Harness
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS}  $@

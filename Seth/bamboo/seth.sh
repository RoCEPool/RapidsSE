#!/bin/bash

# This is the script file that runs in the Bamboo continuous integration environment.

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/../.."
CLASSPATH=".:*:${DIR}:${DIR}/*:${DIR}/..:${DIR}/../*"

# This is the colleciton of tests that are actually run by bamboo.
TESTLIST_FILE="${DIR}/allTestsBamboo"

#SE_PATH="../OS X/Build/Products/Debug/SE"
SE_DIR="${ROOT_DIR}/Linux/bin"
SE_PATH="${SE_DIR}/rapidsse"
SE_ARGS="--name NODE0 --node NODE0 127.0.0.1 5541 --munged --commands ../Commands"  # in addition to --metadata and --script

export LD_LIBRARY_PATH="${SE_DIR}:${LD_LIBRARY_PATH}"

# Additional arguments for SETH. Enable as required.
SETH_ARGS="--clean --logsteps --resultdir ${DIR}/results --resultformat junit -f ${TESTLIST_FILE}" 

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:rapidsse:${SE_PATH}?extraargs=${SE_ARGS}"

# Run the SE Test Harness
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS} $@


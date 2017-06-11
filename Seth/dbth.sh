#!/bin/bash
# Runs seth against RapidsDB.

# Get the relative path to this file.
DIR=$(dirname "$0")
CLASSPATH=".:*:${DIR}:${DIR}/*"

DB_HOST="127.0.0.1"
DB_PORT="5334"

# Additional arguments for SETH. Enable as required.
#SETH_ARGS="--clean --logtests"   # log all test executions to a file
SETH_ARGS="--clean --logsteps"   # log executions of all steps within each test to a file.

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:rdp://${DB_HOST}:${DB_PORT}"

# Run the SE Test Harness
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS}  $@


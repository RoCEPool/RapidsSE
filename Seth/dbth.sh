#!/bin/bash
# Runs seth against RapidsDB.

# Get the relative path to this file.
DIR=$(dirname "$0")
CLASSPATH=".:*:${DIR}:${DIR}/*"

DB_HOST="127.0.0.1"
DB_PORT="5334"

# Additional arguments for SETH. Enable as required.
SETH_ARGS="--clean --logsteps"   # log executions of all steps within each test to a file.

# The JDBC URL (and therefore the JDBC driver) to be used.
URL="jdbc:rdp://${DB_HOST}:${DB_PORT}"

# Credentials for RapidsDB
export RDP_USERNAME=rapids
export RDP_PASSWORD=rapids

# Run the SE Test Harness
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS}  $@


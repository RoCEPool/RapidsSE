#!/bin/bash
# A script file for running Seth against Postgres

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/.."
CLASSPATH="${DIR}/bin:${DIR}/bin/*"

PG_HOST="ubuntu22a"
PG_PORT="5432"
PG_USER="craig"
PG_PASS="craig"
PG_DB="sf10"

# Additional arguments for SETH. Enable as required.
#SETH_ARGS="--clean --logtests"   # log all test executions to a file

# Log executions of all steps within each test to a file.
# Also Postgres returns string values padded with whitespace, so
# this needs to be ignored in any comparisons.
SETH_ARGS="--clean --logsteps --opt ignoreTrailingSpace"

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:postgresql://${PG_HOST}:${PG_PORT}/${PG_DB}?user=${PG_USER}&password=${PG_PASS}"

if [ "$#" -eq 0 ]; then
  >&2 echo "ERROR: $0 must be called with at least one argument."
  >&2 echo
  >&2 echo "e.g. to run Seth with a single test file:"
  >&2 echo "  $0 path/to/test/file.test"
  >&2 echo
  >&2 echo "e.g. to run Seth with a testlist file that references many test files:"
  >&2 echo "  $0 -f path/to/testlist"
  exit 1
fi

# Run the SE Test Harness
java -cp ${CLASSPATH} com.rapidsdata.seth.Seth -u "${URL}" ${SETH_ARGS}  $@

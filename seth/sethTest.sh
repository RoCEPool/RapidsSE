#!/bin/bash

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/.."
CLASSPATH="${DIR}/bin:${DIR}/bin/*:${ROOT_DIR}/shell/*"

JAVA_ARGS=("-Djava.net.preferIPv4Stack=true" "-Djava.net.preferIPv6Addresses=false")
JAVA_ARGS+=("-cp" "${CLASSPATH}")

# Additional arguments for SETH. Enable as required.
#SETH_ARGS=("--clean" "--logtests")   # log all test executions to a file
SETH_ARGS=("--clean" "--logsteps")   # log executions of all steps within each test to a file.

# Connection details for RapidsSE wireline protocol
# We use 127.0.0.1 over localhost because localhost could be resolved
# as an IPv6 address on some machines. Using 127.0.0.1 forces IPv4.
HOST="127.0.0.1"
PORT="9123"

# The SE JDBC URL that tells the JDBC driver how to connect to SE.
URL="jdbc:se://${HOST}:${PORT}?continueBatchOnError=true"

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
java "${JAVA_ARGS[@]}" com.rapidsdata.seth.Seth -u "${URL}" "${SETH_ARGS[@]}"  "$@"

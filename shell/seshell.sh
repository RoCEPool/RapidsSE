#!/bin/bash

# Get the relative path to this file.
DIR="$(dirname "$0")"
CLASSPATH="${CLASSPATH}:${DIR}/*:${DIR}/shell/*"
# DEBUG="-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"

# Authentication not yet supported in SE
export SE_USERNAME=""
export SE_PASSWORD=""

OPTIONS="--continueBatchOnError"

java -cp "${CLASSPATH}" com.rapidsdata.shell.RapidsShell ${OPTIONS} "$@"

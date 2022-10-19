#!/bin/bash

# Get the relative path to this file.
DIR="$(dirname "$0")"
CLASSPATH="${CLASSPATH}:${DIR}/*:${DIR}/../shell/*"

# Authentication not yet supported in SE
export SE_USERNAME=""
export SE_PASSWORD=""

java -cp "${CLASSPATH}" com.rapidsdata.shell.RapidsShell "$@"
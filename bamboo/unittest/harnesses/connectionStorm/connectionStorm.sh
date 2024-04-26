#!/bin/bash

DIR=$(dirname "$0")
REPO_ROOT="${DIR}/../../../.."

# Java Parameters
CONNECTION_STORM_MAIN="com.rapidsdata.seConnectionStorm.Main"
JDBC_DRIVER_CP="${REPO_ROOT}/shell/*"
CLASSPATH="${DIR}/*:${JDBC_DRIVER_CP}"

# Connection storm parameters
JDBC_URL="jdbc:se://127.0.0.1:9123"
RUNTIME_SEC="30"
NUM_CONNECTIONS="16"

JAVA_ARGS=("-cp" "${CLASSPATH}" "${CONNECTION_STORM_MAIN}" "-Djava.net.preferIPv4Stack=true" "-Djava.net.preferIPv6Stack=false")
APP_ARGS=("-u" "${JDBC_URL}" "-t" "${RUNTIME_SEC}" "-c" "${NUM_CONNECTIONS}")

if [ "$#" -gt 0 ]; then
  APP_ARGS+=("$@")
fi

java "${JAVA_ARGS[@]}" "${APP_ARGS[@]}"
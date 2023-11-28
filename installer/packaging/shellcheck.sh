#!/bin/bash

if ! which shellcheck &>/dev/null; then
  echo "Cannot check scripts: shellcheck not installed."
  exit 1
fi

DIR="$(dirname "$0")"
PROJ_DIR="${DIR}/../.."

# Files to be checked
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/setup.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/startSE.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/stopSE.sh"
# shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/prompt.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/seshell.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/generateData.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap "${PROJ_DIR}/bootstrap/collectLogs.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap -e SC2031 "${PROJ_DIR}/bootstrap/addNode.sh"
shellcheck -s bash -x -P "${PROJ_DIR}"/bootstrap -e SC2031 "${PROJ_DIR}/bootstrap/removeNode.sh"

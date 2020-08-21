#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: $0 <nodeName>"
  exit 1
fi

NODE_NAME="$1"

# Get the relative path to this file.
DIR=$(dirname "$0")
ROOT_DIR="${DIR}/.."

EXE_PATH="${ROOT_DIR}/Linux/bin"
EXE="${EXE_PATH}/rapidsse"
START_PARAMS="--name ${NODE_NAME} --metadata --commands ${ROOT_DIR}/Commands --prompt --appendTrace"

#set the library path for the rapidsSE library
export LD_LIBRARY_PATH="${EXE_PATH}":$LD_LIBRARY_PATH

"${EXE}" ${START_PARAMS}

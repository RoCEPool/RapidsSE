#!/bin/bash

# Packages up all the necessary bits of SE for benchmarking/tuning.

DIR=$(dirname "$0")
SE_DIR="${DIR}/.."

OUTPUT_FILE="se.tar.gz"

VERSION="version.txt"

(
  cd "${SE_DIR}" || exit
  
  rm "${OUTPUT_FILE}" 2>/dev/null
  
  if which git &>/dev/null
  then
    git rev-parse --short HEAD > version.txt
  fi
  
  tar cvzf "${OUTPUT_FILE}" Linux Commands Demo dbgen Source/LogicUnit.cpp Source/*.h "${VERSION}"
  
  rm -f "${VERSION}" &>/dev/null
)

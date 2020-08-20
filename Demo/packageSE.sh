#!/bin/bash

# Packages up all the necessary bits of SE for benchmarking/tuning.

DIR=$(dirname "$0")
SE_DIR="${DIR}/.."

VERSION="version.txt"

(
  cd "${SE_DIR}" || exit
  
  which git &>/dev/null
  if [ "$?" -eq 0 ]
  then
    git rev-parse --short HEAD > version.txt
  fi
  
  tar cvzf se.tar.gz Linux Commands Demo Source/LogicUnit.cpp "${VERSION}"
  
  rm -f "${VERSION}" &>/dev/null
)

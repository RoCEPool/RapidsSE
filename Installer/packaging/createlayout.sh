#!/bin/bash

# This file is responsible for copying all files to be installed into the layout directory
# before the installer is created.

if [ -z "${ROOT_DIR}" ] || [ -z "${LAYOUT_DIR}" ] || [ -z "${OUTPUT_DIR}" ] || [ -z "${SOURCE_DIR}" ] 
then 
  >&2 echo "Cannot execute this script individually. It must be executed by the package.sh script." 
  exit 1 
fi


# Copy binaries
mkdir -p "${LAYOUT_DIR}/Linux/bin"
cp "${ROOT_DIR}/Linux/bin/"* "${LAYOUT_DIR}/Linux/bin"

# Commands directory
mkdir -p "${LAYOUT_DIR}/Commands"
cp -r "${ROOT_DIR}/Commands" "${LAYOUT_DIR}"

# tcc thirdparty library
TCC_DIR="ThirdParty/tcc/lib/linux/centos7/tcc"
mkdir -p "${LAYOUT_DIR}/${TCC_DIR}"
mkdir -p "${LAYOUT_DIR}/${TCC_DIR}/include"
cp "${ROOT_DIR}/${TCC_DIR}/libtcc1.a" "${LAYOUT_DIR}/${TCC_DIR}"
cp "${ROOT_DIR}/${TCC_DIR}/include/"* "${LAYOUT_DIR}/${TCC_DIR}/include"

# dbgen
mkdir -p "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dbgen"     "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dists.dss" "${LAYOUT_DIR}/dbgen"

# Source
mkdir -p "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/LogicUnit.cpp"   "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/EnumGroups.h"    "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/Ent_int.h"       "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/InflateSet.h"    "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/LIB_interface.h" "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/DEV_interface.h" "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/RLU_interface.h" "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/RluCommon.h"     "${LAYOUT_DIR}/Source"
cp "${ROOT_DIR}/Source/eEO.h"           "${LAYOUT_DIR}/Source"

# Bootstrap
cp "${ROOT_DIR}/Bootstrap/setup.sh"     "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/setup.fn"     "${LAYOUT_DIR}"


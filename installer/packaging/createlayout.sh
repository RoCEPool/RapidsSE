#!/bin/bash

# This file is responsible for copying all files to be installed into the layout directory
# before the installer is created.

if [ -z "${ROOT_DIR}" ] || [ -z "${LAYOUT_DIR}" ] || [ -z "${OUTPUT_DIR}" ] || [ -z "${SOURCE_DIR}" ] 
then 
  >&2 echo "Cannot execute this script individually. It must be executed by the package.sh script." 
  exit 1 
fi


# Copy binaries
mkdir -p "${LAYOUT_DIR}/bin"
cp "${ROOT_DIR}/bin/rapidsse"       "${LAYOUT_DIR}/bin"
cp "${ROOT_DIR}/bin/libRapidsSE.so" "${LAYOUT_DIR}/bin"
cp "${ROOT_DIR}/bin/se.man"         "${LAYOUT_DIR}/bin"

# commands directory
mkdir -p "${LAYOUT_DIR}/commands"
cp -r "${ROOT_DIR}/commands" "${LAYOUT_DIR}"

# tcc thirdparty library
TCC_DIR="thirdparty/tcc/lib/tcc"
mkdir -p "${LAYOUT_DIR}/${TCC_DIR}"
mkdir -p "${LAYOUT_DIR}/${TCC_DIR}/include"
cp "${ROOT_DIR}/${TCC_DIR}/libtcc1.a"     "${LAYOUT_DIR}/${TCC_DIR}"
cp "${ROOT_DIR}/${TCC_DIR}/include/"*     "${LAYOUT_DIR}/${TCC_DIR}/include"

# dbgen
mkdir -p "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dbgen"                "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dists.dss"            "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/bootstrap/generateData.sh"  "${LAYOUT_DIR}/dbgen"

# Source
mkdir -p "${LAYOUT_DIR}/src"
mkdir -p "${LAYOUT_DIR}/src/expressions"
mkdir -p "${LAYOUT_DIR}/src/resources"
mkdir -p "${LAYOUT_DIR}/src/dataBuffers"
cp "${ROOT_DIR}/src/expressions/LogicUnit.cpp"    "${LAYOUT_DIR}/src/expressions"
cp "${ROOT_DIR}/src/resources/EnumGroups.h"       "${LAYOUT_DIR}/src/resources"
cp "${ROOT_DIR}/src/resources/eEO.h"              "${LAYOUT_DIR}/src/resources"
cp "${ROOT_DIR}/src/dataBuffers/Ent_int.h"        "${LAYOUT_DIR}/src/dataBuffers"
cp "${ROOT_DIR}/src/expressions/InflateSet.h"     "${LAYOUT_DIR}/src/expressions"
cp "${ROOT_DIR}/src/expressions/LIB_interface.h"  "${LAYOUT_DIR}/src/expressions"
cp "${ROOT_DIR}/src/expressions/DEV_interface.h"  "${LAYOUT_DIR}/src/expressions"
cp "${ROOT_DIR}/src/expressions/RLU_interface.h"  "${LAYOUT_DIR}/src/expressions"
cp "${ROOT_DIR}/src/expressions/RluCommon.h"      "${LAYOUT_DIR}/src/expressions"

# eo_rtText
cp -r "${ROOT_DIR}/eo_text"               "${LAYOUT_DIR}/eo_text"

# Bootstrap
cp "${ROOT_DIR}/bootstrap/setup.sh"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/setup.fn"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/envCheck.fn"           "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/common.fn"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/cluster.config.sample" "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/startSE.sh"            "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/stopSE.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/boot.fn"               "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/status.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/prompt.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/collectLogs.sh"        "${LAYOUT_DIR}"


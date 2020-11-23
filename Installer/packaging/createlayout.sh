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

# Commands directory
mkdir -p "${LAYOUT_DIR}/Commands"
cp -r "${ROOT_DIR}/Commands" "${LAYOUT_DIR}"

# tcc thirdparty library
TCC_DIR="ThirdParty/tcc/lib/tcc"
mkdir -p "${LAYOUT_DIR}/${TCC_DIR}"
mkdir -p "${LAYOUT_DIR}/${TCC_DIR}/include"
cp "${ROOT_DIR}/${TCC_DIR}/libtcc1.a"     "${LAYOUT_DIR}/${TCC_DIR}"
cp "${ROOT_DIR}/${TCC_DIR}/include/"*     "${LAYOUT_DIR}/${TCC_DIR}/include"

# dbgen
mkdir -p "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dbgen"                "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dists.dss"            "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/Bootstrap/generateData.sh"  "${LAYOUT_DIR}/dbgen"

# Source
mkdir -p "${LAYOUT_DIR}/src"
mkdir -p "${LAYOUT_DIR}/src/Expressions"
mkdir -p "${LAYOUT_DIR}/src/eNums"
mkdir -p "${LAYOUT_DIR}/src/Entries"
cp "${ROOT_DIR}/src/Expressions/LogicUnit.cpp"    "${LAYOUT_DIR}/src/Expressions"
cp "${ROOT_DIR}/src/eNums/EnumGroups.h"           "${LAYOUT_DIR}/src/eNums"
cp "${ROOT_DIR}/src/Entries/Ent_int.h"            "${LAYOUT_DIR}/src/Entries"
cp "${ROOT_DIR}/src/Expressions/InflateSet.h"     "${LAYOUT_DIR}/src/Expressions"
cp "${ROOT_DIR}/src/Expressions/LIB_interface.h"  "${LAYOUT_DIR}/src/Expressions"
cp "${ROOT_DIR}/src/Expressions/DEV_interface.h"  "${LAYOUT_DIR}/src/Expressions"
cp "${ROOT_DIR}/src/Expressions/RLU_interface.h"  "${LAYOUT_DIR}/src/Expressions"
cp "${ROOT_DIR}/src/Expressions/RluCommon.h"      "${LAYOUT_DIR}/src/Expressions"
cp "${ROOT_DIR}/src/eNums/eEO.h"                  "${LAYOUT_DIR}/src/eNums"

# EO_Text
cp -r "${ROOT_DIR}/EO_Text"               "${LAYOUT_DIR}/EO_Text"

# Bootstrap
cp "${ROOT_DIR}/Bootstrap/setup.sh"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/setup.fn"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/envCheck.fn"           "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/common.fn"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/cluster.config.sample" "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/startSE.sh"            "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/stopSE.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/boot.fn"               "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/status.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/prompt.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/Bootstrap/collectLogs.sh"        "${LAYOUT_DIR}"


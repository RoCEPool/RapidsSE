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
# Refer to #includes in LogicCompiler::v_DynPreliminary()
mkdir -p "${LAYOUT_DIR}/src"
mkdir -p "${LAYOUT_DIR}/src/store"
mkdir -p "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/store/LogicUnit.cpp"    "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/base/BaseTypes_tcc.h"   "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/EnumGroups.h"      "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/eEO.h"             "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/eEA.h"             "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/eDM.h"             "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/FunMacros.h"       "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/MathLib.h"         "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/TccStuff.h"        "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/store/Ent_int.h"        "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/InflateSet.h"     "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/LIB_interface.h"  "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/DEV_interface.h"  "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/RLU_interface.h"  "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/RluCommon.h"      "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_InflateColumn.h"   "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_Parameters.h"      "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_PreliminaryCrw.h"  "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_SkipColumn.h"      "${LAYOUT_DIR}/src/store"

# Asm
cp -r "${ROOT_DIR}/asm"                          "${LAYOUT_DIR}/asm"

# eo_Text
cp -r "${ROOT_DIR}/eo_text"                      "${LAYOUT_DIR}/eo_text"

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
cp "${ROOT_DIR}/bootstrap/addNode.sh"            "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/removeNode.sh"         "${LAYOUT_DIR}"

# Resources
mkdir -p "${LAYOUT_DIR}/res"
cp "${ROOT_DIR}/src/store/parser/graph/PlannerDef.json5" "${LAYOUT_DIR}/res"

# demo files
mkdir -p "${LAYOUT_DIR}/demo"
cp "${ROOT_DIR}/demo/drop1_sql"                 "${LAYOUT_DIR}/demo"
cp "${ROOT_DIR}/demo/drop2_sql"                 "${LAYOUT_DIR}/demo"
cp "${ROOT_DIR}/demo/drop3_sql"                 "${LAYOUT_DIR}/demo"
cp -r "${ROOT_DIR}/demo/demoData"               "${LAYOUT_DIR}/demo/demoData"

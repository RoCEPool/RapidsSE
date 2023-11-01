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
cp "${ROOT_DIR}/bin/rapidsse"      "${LAYOUT_DIR}/bin"
cp "${ROOT_DIR}/bin/se.man"        "${LAYOUT_DIR}/bin"

# SE Shell
# Note: We put seshell.sh in both the $LAYOUT_DIR and $LAYOUT_DIR/shell so that
#       the $LAYOUT_DIR/shell directory can be copied to another machine and run
#       as it is.
mkdir -p "${LAYOUT_DIR}/shell"
cp "${ROOT_DIR}/shell/"*.jar       "${LAYOUT_DIR}/shell"
cp "${ROOT_DIR}/shell/seshell.sh"  "${LAYOUT_DIR}/shell"
cp "${ROOT_DIR}/shell/seshell.bat" "${LAYOUT_DIR}/shell"
cp "${ROOT_DIR}/shell/seshell.sh"  "${LAYOUT_DIR}"

# commands directory
mkdir -p "${LAYOUT_DIR}/commands"
cp -r "${ROOT_DIR}/commands"       "${LAYOUT_DIR}"

# dbgen
mkdir -p "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dbgen"                "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/dbgen/dists.dss"            "${LAYOUT_DIR}/dbgen"
cp "${ROOT_DIR}/bootstrap/generateData.sh"  "${LAYOUT_DIR}/dbgen"

# Source
# Refer to #includes in LogicCompiler::v_DynPreliminary()
mkdir -p "${LAYOUT_DIR}/src"
mkdir -p "${LAYOUT_DIR}/src/store"
mkdir -p "${LAYOUT_DIR}/src/store/Fragments"
mkdir -p "${LAYOUT_DIR}/src/base"
mkdir -p "${LAYOUT_DIR}/src/primitives"
cp "${ROOT_DIR}/src/store/LogicUnit.cpp"    "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/primitives/BaseTypes_general.h" "${LAYOUT_DIR}/src/primitives"
cp "${ROOT_DIR}/src/base/CastMacros.h"      "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/CodeSynthCommon.h" "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/ClangStuff.h"      "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/ClangDynamic.h"    "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/EnumGroups.h"      "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/eEC.h"             "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/eDM.h"             "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/FunctionMacros.h"  "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/LogicalMacros.h"   "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/MathMacros.h"      "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/base/RelationalMacros.h" "${LAYOUT_DIR}/src/base"
cp "${ROOT_DIR}/src/store/Ent_int.h"        "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/RluCommon.h"      "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_InflateColumn.h"   "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_SkipColumn.h"      "${LAYOUT_DIR}/src/store"

cp "${ROOT_DIR}/src/store/vFLAT_Parameters.h"      "${LAYOUT_DIR}/src/store"
cp "${ROOT_DIR}/src/store/vFLAT_PreliminaryCrw.h"  "${LAYOUT_DIR}/src/store"

cp "${ROOT_DIR}/src/store/Fragments/Aggreg.cpp"          "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/CommonCode.cpp"      "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/fileHeader.cpp"      "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/Filter.cpp"          "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/functionFooters.cpp" "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/functionHeaders.cpp" "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/GbyAgg.cpp"          "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/Index.cpp"           "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/IndexJoin.cpp"       "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/IndexScan.cpp"       "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/InflateCrw.cpp"      "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/InflateRow.cpp"      "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/Project.cpp"         "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/Stencil.cpp"         "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/vFLAT_FunctionLocals.h" "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/vFLAT_Header.h"         "${LAYOUT_DIR}/src/store/Fragments"
cp "${ROOT_DIR}/src/store/Fragments/Limit.cpp"              "${LAYOUT_DIR}/src/store/Fragments"

# Bootstrap
cp "${ROOT_DIR}/bootstrap/setup.sh"               "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/setup.fn"               "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/envCheck.fn"            "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/common.fn"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/cluster.config.sample"  "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/startSE.sh"             "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/stopSE.sh"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/boot.fn"                "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/status.sh"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/prompt.sh"              "${LAYOUT_DIR}"
cp "${ROOT_DIR}/bootstrap/collectLogs.sh"         "${LAYOUT_DIR}"
# cp "${ROOT_DIR}/bootstrap/addNode.sh"            "${LAYOUT_DIR}"
# cp "${ROOT_DIR}/bootstrap/removeNode.sh"         "${LAYOUT_DIR}"

# Resources
mkdir -p "${LAYOUT_DIR}/res"
cp "${ROOT_DIR}/src/store/parser/graph/PlannerDef.json5" "${LAYOUT_DIR}/res"

# Documentation
if [ -z "${NO_DOCS}" ] || [ "${NO_DOCS}" -ne 1 ]; then
  mkdir -p "${LAYOUT_DIR}/doc"
  if [ -d "${ROOT_DIR}/doc/build" ]; then
    # HTML documentation
    cp -r "${ROOT_DIR}/doc/build/html" "${LAYOUT_DIR}/doc/html"
    rm -rf "${LAYOUT_DIR}/doc/html/_sources"
    rm -f  "${LAYOUT_DIR}/doc/html/.buildinfo"
    rm -f  "${LAYOUT_DIR}/doc/html/objects.inv"

    # PDF documentation
    mkdir -p "${LAYOUT_DIR}/doc/pdf"
    cp "${ROOT_DIR}/doc/build/latex/rapidsse-"*.pdf "${LAYOUT_DIR}/doc/pdf"
  fi
fi

# demo files
mkdir -p "${LAYOUT_DIR}/demo"
cp "${ROOT_DIR}/demo/drop1_sql"                 "${LAYOUT_DIR}/demo"
cp "${ROOT_DIR}/demo/drop2_sql"                 "${LAYOUT_DIR}/demo"
cp "${ROOT_DIR}/demo/drop3_sql"                 "${LAYOUT_DIR}/demo"
cp -r "${ROOT_DIR}/demo/demoData"               "${LAYOUT_DIR}/demo/demoData"

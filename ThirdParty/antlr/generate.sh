#!/bin/bash

DIR="$(dirname $0)"
GEN_DIR="${DIR}/generated"  # Where the generated source files will be written out to.
CPP_NAMESPACE="Parser"  # Namespace for the C++ files.
ANTLR_JAR=$(ls -1 "${DIR}"/antlr*-complete.jar) 

if ! which java > /dev/null
then
  >&2 echo "Cannot find java runtime"
  exit 1
fi

if [ ! -f "${ANTLR_JAR}" ]
then
  >&2 echo "Cannot find antlr jar file"
  exit 1
fi

mkdir -p "${GEN_DIR}"
rm -f "${GEN_DIR}/*"

java -jar "${ANTLR_JAR}" -Dlanguage=Cpp -visitor -o "${GEN_DIR}" -no-listener -package "${CPP_NAMESPACE}" "${DIR}/sql.g4"

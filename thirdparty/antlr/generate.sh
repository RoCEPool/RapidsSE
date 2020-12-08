#!/bin/bash

DIR="$(dirname $0)"

(
  # Change into the directory and run from there otherwise ANTLR messes up
  # the output directory.

  cd "${DIR}"
  DIR="."

  GEN_DIR="${DIR}/generated"  # Where the generated source files will be written out to.
  CPP_NAMESPACE="Parser"  # Namespace for the C++ files.
  ANTLR_JAR=$(ls -1 "${DIR}"/antlr*-complete.jar) 

  echo "Generating SQL parser from grammar."

  if ! which java > /dev/null
  then
    >&2 echo "error: Cannot find java runtime to generate SQL parser."
    exit 1
  fi

  if [ ! -f "${ANTLR_JAR}" ]
  then
    >&2 echo "error: Cannot find antlr jar file to generate SQL parser."
    exit 1
  fi

  mkdir -p "${GEN_DIR}"
  rm -f "${GEN_DIR}/*"

  java -jar "${ANTLR_JAR}" -Dlanguage=Cpp -visitor -o "${GEN_DIR}" -no-listener -package "${CPP_NAMESPACE}" "${DIR}/sql.g4"

  echo "Generated SQL parser at: $(pwd)/${GEN_DIR:2}"
)

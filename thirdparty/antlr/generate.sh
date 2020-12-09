#!/bin/bash

DIR="$(dirname "$0")"

if [ $# -ne 1 ]; then
  >&2 echo "Usage: \$0 <grammarFile.g4>"
  exit 1
fi

file="$1"

if [ ! -f "${file}" ]; then
  >&2 echo "Error: file does not exist: ${file}"
  exit 1
fi

# Get the absolute path of the grammar file
file="$(cd "$(dirname "${file}")"; pwd -P)/$(basename "${file}")"

(
  # Change into the directory and run from there otherwise ANTLR messes up
  # the output directory.

  cd "${DIR}" || exit 1
  DIR="."

  GEN_DIR="${DIR}/generated"  # Where the generated source files will be written out to.
  CPP_NAMESPACE="Parser"  # Namespace for the C++ files.
  ANTLR_JAR=$(ls -1 "${DIR}"/antlr*-complete.jar)

  echo "Generating parser/lexer from grammar file: ${file}"

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
  #rm -f "${GEN_DIR}/*"

  java -jar "${ANTLR_JAR}" -Dlanguage=Cpp -visitor -o "${GEN_DIR}" -no-listener -package "${CPP_NAMESPACE}" "${file}"

  echo "Generated parser/lexer files at: $(pwd)/${GEN_DIR:2}"
)

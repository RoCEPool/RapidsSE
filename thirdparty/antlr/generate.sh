#!/usr/bin/env bash

DIR="$(realpath -q "$(dirname "$0")")"
GEN_DIR="$(realpath -q "${DIR}/generated")"  # Where the generated source files will be written out to.
FORCE_SYNTAX="--force"
FORCE=0

function printUsageAndExit() {
  >&2 echo "Usage: \$0 [${FORCE_SYNTAX}] <grammarFile.g4>"
  exit 1
}

# Check for the minimum version of bash
if [ -z "${BASH_VERSINFO[0]}" ]; then
  >&2 echo "ERROR: This script requires bash v4. Currently you are running ${SHELL}."
  if [ "$(uname)" == "Darwin" ]; then
    >&2 echo "Please install bash v4 via homebrew (brew install bash)"
  fi
  exit 1
fi

# Check for the minimum version of bash
if [ "${BASH_VERSINFO[0]}" -lt 4 ]; then
  >&2 echo "ERROR: The version of bash must be >= 4. Currently running ${BASH_VERSION}"
  if [ "$(uname)" == "Darwin" ]; then
    >&2 echo "Please install bash v4 via homebrew (brew install bash)"
  fi
  exit 1
fi

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
  printUsageAndExit
fi

if [ $# -eq 1 ]; then
  file="$1"
  if [ "${file,,}" == "${FORCE_SYNTAX}" ]; then
    printUsageAndExit
  fi

else
  # $# must be 2
  arg="$1"
  if [ "${arg,,}" != "${FORCE_SYNTAX}" ]; then
    printUsageAndExit
  fi

  FORCE=1
  file="$2"
fi


if [ ! -f "${file}" ]; then
  >&2 echo "Error: file does not exist: ${file}"
  exit 1
fi

# Prefix is "sql" or "json5"
prefix="$(basename "${file//.g4}")"

# Don't generate the parser/lexer from the grammar unless
# the grammar file has modifications or the generated files
# don't exit. Since the generated parser/lexer is checked in,
# this avoids issues where they are regenerated on each system
# with a slight change in one of the file comments.
#
# So if the grammar file does not have any local modifications
# then we will not generate the parser/lexer code.
#
# Ref: https://stackoverflow.com/questions/5143795/how-can-i-check-in-a-bash-script-if-my-local-git-repository-has-changes

if [ "${FORCE}" -eq 1 ]; then
  echo "Forcing generation of parser/lexer from grammar: ${file}"

elif [ ! -d "${GEN_DIR}" ] || [ -z "$(ls -A "${GEN_DIR}/${prefix}."* 2>/dev/null)" ]; then
  echo "Output directory or files are missing for grammar: ${file}"

else
  git update-index -q --refresh

  if git diff-index --quiet HEAD "${file}"; then
    echo "No need to generate parser/lexer from unmodified grammar: ${file}"
    exit 0
  else
    echo "Grammar file has local modifications: ${file}"
  fi
fi

# Get the absolute path of the grammar file
file="$(cd "$(dirname "${file}")"; pwd -P)/$(basename "${file}")"

(
  # Change into the directory and run from there otherwise ANTLR messes up
  # the output directory.

  cd "${DIR}" || exit 1
  DIR="."

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

  echo "Generated parser/lexer files at: ${GEN_DIR}"
)

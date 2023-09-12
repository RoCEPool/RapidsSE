#!/usr/bin/env bash
# A script to create a ReStructuredText fragment file (that is included into
# another rST file) that contains all the Thrift wireline message definitions.
#
# Arguments:
# $1 : The directory whose subdirectories contain *.thrift files to be included.
# $2 : The path the rst fragment file to be written

# For safety:
#  -e          : exit immediately when a command fails.
#  -E          : don't ignore ERR traps in shell functions and subshells.
#  -o pipefail : exits if any process in a pipeline fails (normally it is just
#                the last one).
#  -u          : treat unset variables as an error and exit immediately.
#  -x          : print each command before executing it (for debugging).
# Note: We don't set pipefail globally because we can expect some piped
#       commands to fail (e.g. if you enter an invalid hostname).
set -Eeu -o pipefail

# set -x    # Enable, for debugging

# -----------------------------------------------------------------------
function printUsage() {
  echo "Usage:"
  echo "  $0 <base_path_of_thrift_files> <output_rst_fragment_file>"
  echo ""
}

# -----------------------------------------------------------------------
function appendMessageDef() {
  filePath="$1"
  fileName="$(basename "${filePath}")"

  # We need to indent the file so that it is associated with the rst code block
  indentedFile="$(sed 's/^/  /' "${filePath}")"

  (
    echo ""
    echo ".. rubric:: ${fileName}"
    echo ""
    echo ".. code:: thrift"
    echo "${indentedFile}"
    echo ""
  ) >> "${TGT}"
}

# -----------------------------------------------------------------------

if ((BASH_VERSINFO[0] < 4)); then
  echo -e "Sorry, you need at least bash version 4.0 or later to run "\
          "this script.\n"\
          "Try installing a newer version of bash through Homebrew "\
          "and setting homebrew at the start of your \${PATH}.\n"\
          "(Don't forget to restart your terminal too)" >&2
  exit 1
fi

if ! which realpath &>/dev/null; then
  echo "The utility \"realpath\" is required." >&2
  exit 1
fi


if [ "$#" -ne 2 ]; then
  echo "ERROR: Expected 2 arguments to script, got $# instead." >&2
  printUsage
  exit 1
fi

readonly DIR="$(realpath "$(dirname "$0")")"
readonly SRC="$1"
readonly TGT="$2"

if [ ! -d "${SRC}" ]; then
  echo "Input source path is not a directory or does not exist: ${SRC}" >&2
  exit 1
fi

if [ ! -d "$(dirname "${TGT}")" ]; then
  echo "Parent of output path is not a directory or does not exist: $(dirname "${TGT}")" >&2
  exit 1
fi

rm -f "${TGT}"

# Find the latest version directory that matches "v<num>".
EXTENDED_REGEX_OPT=""
[ "$(uname)" = "Darwin" ] && EXTENDED_REGEX_OPT="-E"

THRIFT_DIR_COMPAT="${SRC}/compatibility"
THRIFT_DIR_LATEST="$(find ${EXTENDED_REGEX_OPT} "${SRC}" -maxdepth 1 -type d -regex ".*/v[0-9]+" | sort -n -k 2 -t v | tail -n 1)"
echo "Latest thrift dir is: ${THRIFT_DIR_LATEST}"

if [ ! -d  "${THRIFT_DIR_COMPAT}" ]; then
  echo "ERROR: Could not find the 'compatibility' subdirectory at: ${THRIFT_DIR_COMPAT}" >&2
  exit 1
fi

THRIFT_FILES_COMPAT=()
THRIFT_FILES_LATEST=()
while IFS= read -r line; do THRIFT_FILES_COMPAT+=("${line}"); done < <(find "${THRIFT_DIR_COMPAT}" -type f -name "*.thrift")
while IFS= read -r line; do THRIFT_FILES_LATEST+=("${line}"); done < <(find "${THRIFT_DIR_LATEST}" -type f -name "*.thrift")

touch "${TGT}" || exit 1

(
  echo "Protocol Compatibility Message Definitions"
  echo "::::::::::::::::::::::::::::::::::::::::::"
) >> "${TGT}"

for f in "${THRIFT_FILES_COMPAT[@]}"
do
  appendMessageDef "${f}"
done

(
  echo ""
  echo "Protocol $(basename "${THRIFT_DIR_LATEST}") Message Definitions"
  echo ":::::::::::::::::::::::::::::::::"
) >> "${TGT}"

for f in "${THRIFT_FILES_LATEST[@]}"
do
    appendMessageDef "${f}"
done
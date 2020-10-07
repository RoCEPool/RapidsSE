#!/bin/bash
# This script builds a RapidsSE installer.

DIR=$(dirname "$0")

# For safety:
#  -e          : exit immediately when a command fails.
#  -E          : don't ignore ERR traps in shell functions and subshells.
#  -o pipefail : exits if any process in a pipeline fails (normally it is just
#                the last one).
#  -u          : treat unset variables as an error and exit immediately.
#  -x          : print each command before executing it (for debugging).
set -Eeo pipefail

###############################################################################
### Environment Variables                                                   ###
###############################################################################
# Set these environment variables to non-null values to use them:
#  DEBUG        - print extra debugging information
#  NO_MOD_CHECK - don't check for modified files that haven't been committed.
#  NO_BUILD     - don't compile the software


###############################################################################
### Constants                                                               ###
###############################################################################

ROOT_DIR="${DIR}/../.."
LAYOUT_DIR="${DIR}/../layout"
OUTPUT_DIR="${DIR}/../output"
SOURCE_DIR="${ROOT_DIR}/Source"

MAKESELF_DIR=$(find "${ROOT_DIR}/ThirdParty/" -type d -name "makeself*")

GIT_CHECK_MODIFIED_FILES_CMD="git status -uno --porcelain"
#GIT_HASH_CMD="git rev-parse --short HEAD"
GIT_DESCRIBE_CMD="git describe --long"
GIT_BRANCH_CMD="git rev-parse --abbrev-ref HEAD"

VERSION_FILE="${LAYOUT_DIR}/version.txt"
CREATE_LAYOUT_FILE="${DIR}/createlayout.sh"

SETUP_SCRIPT="./setup.sh"  # relative to the directory of extracted files
MAKESELF_ARGS=""

###############################################################################
### Functions                                                               ###
###############################################################################

# Print an error message to stderr and exit
errorexit() {
  >&2 echo -e "$1"
  exit 1
}

extractVersions() {
  # SE_VERSION_STR will contain a value that looks something like this:
  #  v0.5.0-SNAPSHOT-0-ga7c5a26
  # First validate that we've got a value that can be parsed.

  local regex
  local git_version_str
  
  regex="^v([0-9]+)\.([0-9]+)\.([0-9]+)(-[^-]+)?-([0-9]+)-([[:alnum:]]+)$"
  git_version_str="$(${GIT_DESCRIBE_CMD})"

  if [[ "${git_version_str}" =~ ${regex} ]]
  then
    SE_MAJOR_VER="${BASH_REMATCH[1]}"
    SE_MINOR_VER="${BASH_REMATCH[2]}"
    SE_PATCH_VER="${BASH_REMATCH[3]}"
    SE_VER_LABEL="${BASH_REMATCH[4]}"
    SE_BUILD_NUM="${BASH_REMATCH[5]}"
    SE_COMMIT_HASH="${BASH_REMATCH[6]}"

  else
    errorexit "Could not parse the SE version string: ${SE_VERSION_STR}"
  fi

  # Form a string of the form x.y.z-label
  SE_VERSION_STR="${SE_MAJOR_VER}.${SE_MINOR_VER}.${SE_PATCH_VER}${SE_VER_LABEL}"

  # If the version label is not null then strip off the leading '-'
  if [ -n "${SE_VER_LABEL}" ] 
  then
    SE_VER_LABEL="${SE_VER_LABEL:1}"
  fi
}


###############################################################################
### Script                                                                  ###
###############################################################################

### Check the version of bash ###
if [ -n "${DEBUG}" ] ; then echo "Checking the version of bash: ${BASH_VERSION}"; fi;

if [ "${BASH_VERSINFO:-0}" -lt 4 ]
then
  errorexit "Bash version 4 or later must be used. You have ${BASH_VERSION}"
fi


### Check the OS ###
if [ -n "${DEBUG}" ] ; then echo "Checking the OS type: ${OSTYPE}"; fi;

case "${OSTYPE}" in
  linux*)
    ;;

  darwin*)
    errorexit "Packaging for MacOS not yet supported."
    ;;
 
  *)
    errorexit "Unable to determine the OS type: ${OSTYPE}"
    ;;
esac 


### Check for any modifications to git-tracked files ###
if [ -n "${DEBUG}" ] ; then echo "Checking for any modified files to committed to git"; fi;

# Get an array of modified files (reading lines). Ignore untracked files.
mapfile -t MODIFIED_FILES < <(${GIT_CHECK_MODIFIED_FILES_CMD})

if [[ ${#MODIFIED_FILES[@]} -ne 0 && -z "${NO_MOD_CHECK}" ]]
then
  MSG=$(
    echo "Cannot package SE while there are modified files that haven't been committed to git:"
    printf '%s\n' "${MODIFIED_FILES[@]}"
  )
  errorexit "${MSG}"
fi


### Check for the existence of Makeself ###
if [ ! -d "${MAKESELF_DIR}" ]
then
  errorexit "Couldn't find the directory for the makeself installer: ${MAKESELF_DIR}"
fi


### Remove & recreate the layout and output directories to have a clean slate ###
if [ -n "${DEBUG}" ] ; then echo "Cleaning & setting up the layout directory"; fi;

rm -rf "${LAYOUT_DIR}/"* &>/dev/null || true
rm -rf "${OUTPUT_DIR}/"* &>/dev/null || true
mkdir -p "${LAYOUT_DIR}"
mkdir -p "${OUTPUT_DIR}"


### Get the version number of SE ###
if [ -n "${DEBUG}" ] ; then echo "Getting the RapidsSE version number"; fi;

SE_VERSION_STR=""
SE_MAJOR_VER=""
SE_MINOR_VER=""
SE_PATCH_VER=""
SE_VER_LABEL=""
SE_BUILD_NUM=""
SE_COMMIT_HASH=""
SE_BRANCH="$(${GIT_BRANCH_CMD})"

extractVersions

if [ -n "${DEBUG}" ] 
then
  echo "SE_VERSION_STR = ${SE_VERSION_STR}"
  echo "SE_MAJOR_VER   = ${SE_MAJOR_VER}"
  echo "SE_MINOR_VER   = ${SE_MINOR_VER}"
  echo "SE_PATCH_VER   = ${SE_PATCH_VER}"
  echo "SE_VER_LABEL   = ${SE_VER_LABEL}"
  echo "SE_BUILD_NUM   = ${SE_BUILD_NUM}"
  echo "SE_COMMIT_HASH = ${SE_COMMIT_HASH}"
  echo "SE_BRANCH      = ${SE_BRANCH}"
fi


### Generate version.txt file ###
if [ -n "${DEBUG}" ] ; then echo "Generating the version.txt file"; fi;
(
  echo "RapidsSE build information:"
  echo "  version     ${SE_VERSION_STR}"
  echo "  from branch ${SE_BRANCH}"
  echo "  by          ${USER}"
  echo "  on          ${HOSTNAME}"
  echo "  at          $(date)"
  echo ""
)> "${VERSION_FILE}"



### (Optional) Compile the software ###
if [ -n "${DEBUG}" ] ; then echo "Compiling RapidsSE"; fi;

if [ -z "${NO_BUILD}" ] 
then
  (
    cd "${SOURCE_DIR}"
    scl enable devtoolset-7 'make clean && make -j 8'
  )
fi


### Build the layout directory ###
if [ -n "${DEBUG}" ] ; then echo "Building the layout"; fi;

source "${CREATE_LAYOUT_FILE}"


### Build the installer's run file ###
if [ -n "${DEBUG}" ] ; then echo "Generating the run file"; fi;

OUTPUT_FILE="${OUTPUT_DIR}/rapidsse-${SE_VERSION_STR},run"

"${MAKESELF_DIR}/makeself.sh" ${MAKESELF_ARGS} "${LAYOUT_DIR}" "${OUTPUT_FILE}" "RapidsSE files..." "${SETUP_SCRIPT}"


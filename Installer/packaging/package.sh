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
### Constants                                                               ###
###############################################################################

ROOT_DIR="${DIR}/../.."
LAYOUT_DIR="${DIR}/../layout"
OUTPUT_DIR="${DIR}/../output"

MAKESELF_DIR=$(find "${ROOT_DIR}/ThirdParty/" -type d -name "makeself*")

GIT_CHECK_MODIFIED_FILES_CMD="git status -uno --porcelain"
#GIT_HASH_CMD="git rev-parse --short HEAD"
GIT_DESCRIBE_CMD="git describe --long"
GIT_BRANCH_CMD="git rev-parse --abbrev-ref HEAD"

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

  local regex="^v([0-9]+)\.([0-9]+)\.([0-9]+)(-[^-]+)?-([0-9]+)-([[:alnum:]]+)$"

  if [[ "${SE_VERSION_STR}" =~ ${regex} ]]
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

  # If the version label is not null then strip off the leading '-'
  if [ -n "${SE_VER_LABEL}" ] 
  then
    SE_VER_LABEL="${SE_VER_LABEL:1}"
  fi
}


##############################################################################r
### Script                                                                  ###
###############################################################################

### Check the version of bash ###
if [ "${BASH_VERSINFO:-0}" -lt 4 ]
then
  errorexit "Bash version 4 or later must be used. You have ${BASH_VERSION}"
fi


### Check the OS ###
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
rm -rf "${LAYOUT_DIR}" &>/dev/null || true
rm -rf "${OUTPUT_DIR}" &>/dev/null || true
mkdir -p "${LAYOUT_DIR}"
mkdir -p "${OUTPUT_DIR}"


### Get the version number of SE ###
SE_VERSION_STR="$(${GIT_DESCRIBE_CMD})"
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


### TODO: Generate version.txt file ###



### TODO: (Optional) Compile the software ###



### TODO: Build layout ###



### TODO: Build run file ###



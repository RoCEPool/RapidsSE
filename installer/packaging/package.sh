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
#  DEBUG         - print extra debugging information
#  NO_MOD_CHECK  - don't check for modified files that haven't been committed.
#  NO_BUILD      - don't compile the software
#  NO_SHELLCHECK - don't run shellcheck on the scripts beforehand.

###############################################################################
### Constants                                                               ###
###############################################################################

readonly ROOT_DIR="${DIR}/../.."
readonly LAYOUT_DIR="${DIR}/../layout"
readonly OUTPUT_DIR="${DIR}/../output"
readonly SOURCE_DIR="${ROOT_DIR}/src"

readonly MAKESELF_DIR=$(find "${ROOT_DIR}/ThirdParty/" -type d -name "makeself*")

readonly GIT_CHECK_MODIFIED_FILES_CMD="git status -uno --porcelain"
readonly GIT_DESCRIBE_CMD="git describe --long"
readonly GIT_BRANCH_CMD="git rev-parse --abbrev-ref HEAD"

readonly VERSION_FILE="${LAYOUT_DIR}/version"
readonly CREATE_LAYOUT_FILE="${DIR}/createlayout.sh"

readonly SETUP_SCRIPT="./setup.sh"  # relative to the directory of extracted files
readonly MAKESELF_ARGS=() 
#readonly MAKESELF_ARGS=( "--bzip2" "--threads" "8")

readonly SHELLCHECK_CMD="${DIR}/shellcheck.sh"

###############################################################################
### Functions                                                               ###
###############################################################################

# Print an error message to stderr and exit
# Params: 
#   $1 : Error message
function errorExit() {
  >&2 echo -e "$1"
  exit 1
}

function extractVersions() {
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
    errorExit "Could not parse the SE version string: ${SE_VERSION_STR}"
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
  errorExit "Bash version 4 or later must be used. You have ${BASH_VERSION}"
fi


### Check the OS ###
if [ -n "${DEBUG}" ] ; then echo "Checking the OS type: ${OSTYPE}"; fi;

case "${OSTYPE}" in
  linux*)
    ;;

  darwin*)
    errorExit "Packaging for MacOS not yet supported."
    ;;
 
  *)
    errorExit "Unable to determine the OS type: ${OSTYPE}"
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
  errorExit "${MSG}"
fi


### Check for the existence of Makeself ###
if [ ! -d "${MAKESELF_DIR}" ]
then
  errorExit "Couldn't find the directory for the makeself installer: ${MAKESELF_DIR}"
fi


### Run shellcheck against the setup files
if [[ -z "${NO_SHELLCHECK}" || "${NO_SHELLCHECK}" -ne 1 ]] && (which shellcheck &>/dev/null); then
  echo "Running ShellCheck..."
  if ! ${SHELLCHECK_CMD}; then
    errorExit "Packaging failed due to shellcheck issues with the setup scripts. Please fix these first."
  fi
  echo "ShellCheck passed successfully!"

else
  echo "Skipping ShellCheck"
fi


### Remove & recreate the layout and output directories to have a clean slate ###
if [ -n "${DEBUG}" ] ; then echo "Cleaning & setting up the layout directory"; fi;

rm -rf "${LAYOUT_DIR:?}/"* &>/dev/null || true
rm -f  "${OUTPUT_DIR:?}/"*.run &>/dev/null || true
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


### Generate version file ###
if [ -n "${DEBUG}" ] ; then echo "Generating the version file"; fi;
(
  echo "# RapidsSE build information:"
  echo "#   version     ${SE_VERSION_STR}"
  echo "#   from branch ${SE_BRANCH}"
  echo "#   by          ${USER}"
  echo "#   on          ${HOSTNAME}"
  echo "#   at          $(date)"
  echo ""
  echo "SE_VERSION_STR=\"${SE_VERSION_STR}\""
  echo "SE_MAJOR_VER=\"${SE_MAJOR_VER}\""
  echo "SE_MINOR_VER=\"${SE_MINOR_VER}\""
  echo "SE_PATCH_VER=\"${SE_PATCH_VER}\""
  echo "SE_VER_LABEL=\"${SE_VER_LABEL}\""
  echo "SE_BUILD_NUM=\"${SE_BUILD_NUM}\""
  echo "SE_COMMIT_HASH=\"${SE_COMMIT_HASH}\""
  echo "SE_BRANCH=\"${SE_BRANCH}\""
)> "${VERSION_FILE}"



### (Optional) Compile the software ###
if [ -n "${DEBUG}" ] ; then echo "Compiling RapidsSE"; fi;

if [ -z "${NO_BUILD}" ] || [ "${NO_BUILD}" -ne 1 ] 
then

  gccver="$(gcc -dumpversion | cut -f1 -d.)"
  if [[ -n "${gccver}" && "${gccver}" -lt 5 ]] && which scl 2>/dev/null; then
    (
      cd "${SOURCE_DIR}"
      scl enable devtoolset-7 'make clean && make -j 8'
    )

  else
    (
      cd "${SOURCE_DIR}"
      make clean && make -j 
    )
  fi
fi


### Build the layout directory ###
if [ -n "${DEBUG}" ] ; then echo "Building the layout"; fi;

source "${CREATE_LAYOUT_FILE}"


### Build the installer's run file ###
if [ -n "${DEBUG}" ] ; then echo "Generating the run file"; fi;

OUTPUT_FILE="rapidsse-${SE_VERSION_STR}.run"

"${MAKESELF_DIR}/makeself.sh" "${MAKESELF_ARGS[@]}" "${LAYOUT_DIR}" "${OUTPUT_DIR}/${OUTPUT_FILE}" "RapidsSE files..." "${SETUP_SCRIPT}" 

FILESIZE="$(du -k "${OUTPUT_DIR}/${OUTPUT_FILE}" | cut -f1)"
printf "Size of installer: %'d kB\n" "${FILESIZE}"

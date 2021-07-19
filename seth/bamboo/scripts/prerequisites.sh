#!/bin/bash

# The intent of this script file is to check any prerequisites and configure
# any necessary aspects of the system, if applicable.

# This script file is called by Bamboo after it has checked out the repository.

###############################################################################
### Environment Variables                                                   ###
###############################################################################



###############################################################################
### Constants                                                               ###
###############################################################################

readonly SCRIPT_NAME=$(basename "$0")
readonly DIR="$(dirname "$0")"
readonly PROJ_ROOT="${DIR}/../../.."

readonly TPCH_DIR="$(readlink -f "${PROJ_ROOT}/TPCH")"

# An array of possible places the TPC-H data could be kept, depending on the environment
readonly TPCH_LINK_DESTS=( "/mnt/spinningDisk/bamboo/TPCH"
                           "/opt/TPCH" )


readonly COMMON_FN="${DIR}/common.fn"

# Set to 1 to print debug messages in Bamboo.
readonly DEBUG=0

###############################################################################
### Includes                                                                ###
###############################################################################

# Load other common functions

if [ -z "${__BAMBOO_COMMON_FN:-}" ] ; then
  #shellcheck source=common.fn
  source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }
fi

###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Did the script complete successfully or not?
COMPLETED_SUCCESFULLY=0

###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {

  local statusMsg="*** Completed the Prerequisites stage                                        ***"
  if [ "${COMPLETED_SUCCESFULLY}" -eq 0 ]; then
    statusMsg="*** Failed the Prerequisites stage                                           ***"
  fi

  ENDTIME="$(date +%s)"
  ELAPSEDTIME="$(secondsToTime $((ENDTIME - STARTTIME)))"

  echo ""
  echo "********************************************************************************"
  echo "${statusMsg}"
  echo "********************************************************************************"
  echo "Elapsed time: ${ELAPSEDTIME}"
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Check that the right arguments have been passed in.
## Params: the array of command line arguments

function processArgs() {

  if [ "$#" -gt 0 ]; then
    errorExit "No arguments are applicable to this script."
  fi
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that we are running on a supported operating system
## Params: None

function checkOS() {
  if [ "$(uname)" != "Linux" ]; then
    errorExit "Unsupported OS. Only Linux is currently supported."
  fi
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Looks for the TPC-H SF1 data somewhere on the system. Writes the path out
## to stdout and returns a zero exit code if the data is found.
## Params: None

function findTpchData() {

  for dataDir in "${TPCH_LINK_DESTS[@]}"; do
    if isTpchDir "${dataDir}"; then
      echo "${dataDir}"
      return 0
    fi
  done

  printError "Could not find TPC-H SF1 data necessary for these tests. Tried looking in the following directories:"
  >&2 printf "  %s\n" "${TPCH_LINK_DESTS[@]}"
  exit 1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Returns a zero exit code if the given path is a TPCH directory with data.
## Params:
##  $1 - path of the directory

function isTpchDir() {

  [ "$#" -eq 1 ] || errorExit "Function isTpchDir() needs to be called with 1 parameter."

  local tpchDir="$1"

  if [ ! -e "${tpchDir}" ]; then
    echo "Looking for TPC-H data found in path: ${tpchDir}"
    return 1;
  fi

  if [ ! -d "${tpchDir}" ]; then
    echo "Path ${tpchDir} is not a TPC-H data directory because it is not a directory."
    return 1;
  fi

  # It must have a SF0 and SF1 subdirectories
  local subdirs=( "SF0" "SF1" )

  for subdir in "${subdirs[@]}"; do
    if [ ! -d "${tpchDir}/${subdir}" ]; then
      echo "Path ${tpchDir} is not a TPC-H data directory because it does not have a ${subdir} subdirectory."
      return 1;
    fi
  done

  # Check for data files
  local -a files=( "SF1/customer.tbl"
                   "SF1/lineitem.tbl"
                   "SF1/nation.tbl"
                   "SF1/orders.tbl"
                   "SF1/part.tbl"
                   "SF1/partsupp.tbl"
                   "SF1/region.tbl"
                   "SF1/supplier.tbl" )

  for file in "${files[@]}"; do
    if ! [ -f "${tpchDir}/${file}" ]; then
      echo "Path ${tpchDir} is not a TPC-H data directory because it is missing the file: ${file}"
      return 1
    fi
  done

  echo "Found TPC-H data at path: ${tpchDir}"
  return 0
}

###############################################################################
### Main Script                                                             ###
###############################################################################

# Cause Bash to exit on command errors, unset variables and pipe errors.
set -eEuo pipefail


# Record the start time so we can work out the elapsed time.
STARTTIME="$(date +%s)"

echo ""
echo "********************************************************************************"
echo "*** Beginning the Prerequisites stage                                        ***"
echo "********************************************************************************"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# Collect/validate the parameters
processArgs "$@"


# Check for a supported OS
checkOS

# Check that the TPCH data has been installed
checkTpchData


# Set this flag that the script completed successfully.
# This will be picked up in the exit hook.
COMPLETED_SUCCESFULLY=1


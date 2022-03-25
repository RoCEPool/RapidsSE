#!/bin/bash

# A script for copying previously generated TPC-H data from an S3 bucket and
# arranging it appropriately on each node in the cluster so it can be loaded
# in a distributed fashion (using dload).
#
# The priority is to get the fastest load possible for large data sets. This
# means doing a distributed load of files on each SSD in parallel (the bottleneck
# being the disk read speed). So we want to make use of each disk drive as much
# as possible.
#
# Assuming there is at least 1 SSD then data will be written out to:
#   /mnt/disk<1..N>/data/tpch/sf<N>/<tableName>/<tableName>.tbl
#
# If there are no SSDs then data will be written out to:
#   <installDir>/data/tpch/sf<N>/<tableName>/<tableName>.tbl
#
# Assumptions:
#  (1) All nodes have the same hardware.
#  (2) All local SSDs are mounted at /mnt/disk1, /mnt/disk2, etc
#
# Relies on the following files in the installation directory:
#   - common.fn
#   - cluster.config
#   - node.name
#

set -euo pipefail

###############################################################################
### Constants                                                               ###
###############################################################################
readonly DIR=$(dirname "$0")
readonly SCRIPT_NAME=$(basename "$0")
readonly S3_BUCKET="s3://rapidsse-perf-test-data"

###############################################################################
### Significant Global Variables                                            ###
###############################################################################
# Scale factor
SF=0

# The root of the installation dir
PROJ_ROOT=""

# The path of the common.fn file in the installation dir
COMMON_FN=""

# The path of the cluster.config file
CONFIG_FILE=""

# How many chunks the original data is split into
NUMCHUNKS=0

# The number of nodes in the cluster
NUMNODES=0

# The base of the paths where the data gets written out to
BASE_OUTPUT_PATHS=()

# The number of file chunks that each BASE_OUTPUT_PATH above
# on each node will need to process. The sum of these should add up to NUMCHUNKS.
# e.g. if there are 3 nodes that each have 4 base paths then there will
# be 3*4=12 entries here.
CHUNKS_PER_NODEPATH=()

###############################################################################
### Functions                                                               ###
###############################################################################

## Prints out the valid arguments to the script and exits.
## Params: None
## Note: The variable ${SCRIPT_NAME} must be defined as the name of the script filename.

function printUsageAndExit() {

  echo ""
  echo "Usage: ./${SCRIPT_NAME} --sf <scaleFactor>"
  echo ""
  echo "Details:"
  echo "  --sf <scaleFactor>  The scale factor of the TPC-H data to be copied."
  echo ""
  echo "  --help              Displays this message."
  exit 1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Process the arguments for the script.
## Params: None

function processArgs() {
  local argSf=0

  while [[ $# -gt 0 ]]
  do
    key="$1"
    case "${key}" in

      --sf)
        argSf=1
        SF="$2"
        shift # past argument
        shift # past value
        ;;

      --help|-h)
        shift
        printUsageAndExit
        ;;

      *)
        shift
        printUsageAndExit
        ;;
    esac
  done

  if [[ "${argSf}" -eq 0 ]]; then
    printError "Scale factor (--sf) must be specified."
    printUsageAndExit
  fi

  if ! [[ "${SF}" =~ ^[0-9]+$ ]]; then
    printError "Scale factor must be a positive integer only."
    printUsageAndExit
  fi

  if [[ "${SF}" -le 0 ]]; then
    printError "Scale factor must be a positive integer only."
    printUsageAndExit
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Search for and source the common.fn file in the installation directory
## Params: None

function findAndLoadCommon() {
  local searchDir
  searchDir="$(realpath "${DIR}")"

  while [ -n "${searchDir}" ] && [ "${searchDir}" != "/" ]
  do
    if [ -f "${searchDir}/common.fn" ]; then
      COMMON_FN="${searchDir}/common.fn"
      PROJ_ROOT="${searchDir}"
      CONFIG_FILE="${PROJ_ROOT}/cluster.config"

      # shellcheck disable=SC1090
      source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }
      return
    fi

    searchDir="$(realpath "${searchDir}/..")"
  done

  >&2 echo "ERROR: Could not find common.fn in the installation directory"
  exit 1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Returns the number of chunks in the data
## Params: None

function getDataChunks() {
  local url="${S3_BUCKET}/tpch/sf${SF}/lineitem/"
  chunks="$(aws s3 ls "${url}" | wc -l)"
  echo "${chunks}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Sets the variable BASE_OUTPUT_PATHS with the base mount point paths of each
# disk drive we will write to.
## Params: None

function getBaseOutputPaths() {
  local i=0
  local mountPoint
  local arbitraryMaximum

  arbitraryMaximum=20

  while [ $i -lt ${arbitraryMaximum} ];
  do
    i=$((i+1))

    # This is based on how our cloud-init script mounts local drives
    mountPoint="/mnt/disk${i}"

    if ! [ -w "${mountPoint}" ]; then
      break
    fi

    # Add it to the list and keep looking for more drives
    BASE_OUTPUT_PATHS+=("${mountPoint}")
  done

  if [ "${#BASE_OUTPUT_PATHS[@]}" -eq 0 ]; then
    BASE_OUTPUT_PATHS+=("${PROJ_ROOT}")
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Calculates how many data chunks each path will need to download and merge.
## Updates the CHUNKS_PER_NODEPATH variable.
## Params: None

function calculate_chunks_per_path() {
  local numPaths
  local factor
  local remainder

  numPaths=$((${#BASE_OUTPUT_PATHS[@]} * NUMNODES))
  factor=$((NUMCHUNKS / numPaths))
  remainder=$((NUMCHUNKS % numPaths))

  for i in $(seq ${numPaths});
  do
    chunks="${factor}"

    if [ ${remainder} -gt 0 ]; then
      chunks=$((chunks + 1))
      remainder=$((remainder - 1))
    fi

    CHUNKS_PER_NODEPATH+=("${chunks}")
  done
}

###############################################################################
### Main Script                                                             ###
###############################################################################

# Process the arguments given to the script.
processArgs "$@"

findAndLoadCommon
loadClusterConfig   # common.fn
loadNodeName        # common.fn

# Determine how many chunks there are in the data
NUMCHUNKS="$(getDataChunks)"

# Determine how many nodes in the cluster
NUMNODES="${#NODE_LIST[@]}"

# Determine how many drives are in the cluster and where they are mounted
getBaseOutputPaths

# Determine how many files need to be downloaded to each drive on each node.
# This will set the CHUNKS_PER_NODEPATH variable
calculate_chunks_per_path

# TODO: Run a command on each machine to:
# TODO:   1. create folders
# TODO:   2. download the data from S3
# TODO:   3. unzip the data
# TODO:   4. merge files together
# TODO:   5. rename files, cleanup etc

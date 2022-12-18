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
#   /mnt/disk<1..N>/data<N>/tpch/sf<N>/<tableName>/<tableName>.tbl
#
# (The non-partitioned nation and region tables will be written out to the
#  first disk of each node).
#
# If there are no SSDs then data will be written out to:
#   <installDir>/data1/tpch/sf<N>/<tableName>/<tableName>.tbl
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
readonly S3_BUCKET_PREFIX="rapidsse-tpch-sf"
readonly TABLES=("customer" "lineitem" "orders" "supplier" "part" "partsupp" "nation" "region")
readonly PARALLEL_ARGS=("--xapply")
#readonly PARALLEL_ARGS=("-j" "20" "--xapply")
readonly S3_CP_ARGS=("--region" "us-east-1" "--no-progress" "--cli-connect-timeout" "10" "--cli-read-timeout" "10")

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

# The file that contains the name of the current node.
NAME_FILE=""

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

# Params when run in copyData mode
COPY_DATA_MODE=0
START_IDX=0
TARGETS=()
INSTALL_DIR=""

# How many ways do we split the data for a given disk?
# This allows for multiple paths of data per disk.
PTNS_PER_DISK=1

S3_BUCKET=""

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
  echo "  --partitionsPerDisk <count>  How many ways to split the data for each disk. Default=1" # Multiple paths per disk
  echo ""
  echo "  --copyData                  <internal use> Runs the script in copyData mode to download and layout the data."
  echo "  --installDir                <internal use> The RapidsSE installation directory on the current node."
  echo "  --start                     <internal use> The 1-based start index of the data chunks to copy on this node."
  echo "  --target <path> <count>     <internal use> How many chunks to put on a given target path."
  echo ""
  echo "  --help              Displays this message."
  exit 1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Print an error message to stderr.
## This is also provided in common.fn, but it is required before common.fn is loaded.
## Params:
##   $1 : Error message
##   $2..$n : (optional) other arguments to print.

function printError() {
  >&2 echo -e "ERROR: $*"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Process the arguments for the script.
## Params: None

function processArgs() {
  # Did we get these arguments?
  local argSf=0
  local argCopyData=0
  local argInstallDir=0
  local argStart=0
  local argTarget=0
  local argPtnsPerDisk=0

  while [[ $# -gt 0 ]]
  do
    local key="$1"
    case "${key}" in

      --sf)
        argSf=1
        SF="$2"
        shift # past argument
        shift # past value
        ;;

      --copyData)
        argCopyData=1
        COPY_DATA_MODE=1
        shift # past argument
        ;;

      --installDir)
        argInstallDir=1
        INSTALL_DIR="$2"
        shift # past argument
        shift # past value
        ;;

      --start)
        argStart=1
        START_IDX="$2"
        shift # past argument
        shift # past value
        ;;

      --target)
        argTarget=1
        TARGETS+=("$2" "$3")
        shift # past argument
        shift # past first value
        shift # past second value
        ;;

      --partitionsPerDisk)
        argPtnsPerDisk=1
        PTNS_PER_DISK="$2"
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

  if [[ "${PTNS_PER_DISK}" -le 0 ]]; then
    printError "partitionsPerDisk must be a positive integer only."
    printUsageAndExit
  fi

  if [[ "${argCopyData}" -eq 1 ]]; then
    # Check that other arguments are also supplied

    # --installDir must be specified and must exist
    if [[ "${argInstallDir}" -eq 0 ]]; then
      printError "RapidsSE installation dir argument (--installDir) must be specified when the --copyData argument is specified."
      printUsageAndExit
    fi

    if [[ -z "${INSTALL_DIR}" ]] || ! [[ -d "${INSTALL_DIR}" ]]; then
      printError "Value for --installDir argument does not exist: ${INSTALL_DIR}"
      printUsageAndExit
    fi

    # --start must exist and must be a positive integer
    if [[ "${argStart}" -eq 0 ]]; then
      printError "Start index argument (--start) must be specified when the --copyData argument is specified."
      printUsageAndExit
    fi

    if ! [[ "${START_IDX}" =~ ^[0-9]+$ ]]; then
      printError "The value for the --start argument must be a positive integer only."
      printUsageAndExit
    fi

    if [[ "${argTarget}" -eq 0 ]]; then
      printError "Argument --target must be specified when the --copyData argument is specified."
      printUsageAndExit
    fi

    # The --target values must be a directory followed by a count
    local i=0
    while [[ "${i}" -lt "${#TARGETS[@]}" ]];
    do
      if [[ -z "${TARGETS[$i]}" ]] || [[ -z "${TARGETS[$((i+1))]}" ]]; then
        printError "The --target argument must specify a location and a count of data chunks."
        printUsageAndExit
      fi

      if ! [[ "${TARGETS[$((i+1))]}" =~ ^[0-9]+$ ]]; then
        printError "The second value of the --target argument must be a positive integer."
        printUsageAndExit
      fi

      i=$((i + 2))
    done

  else
    # Not running in copyData mode. Check that certain arguments have not been specified.

    if [[ "${argStart}" -eq 1 ]]; then
      printError "The --start argument can only be specified in conjunction with the --copyData argument."
      printUsageAndExit
    fi

    if [[ "${argTarget}" -eq 1 ]]; then
      printError "The --target argument can only be specified in conjunction with the --copyData argument."
      printUsageAndExit
    fi
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Search for common.fn file in the installation directory and sets the following
## variables appropriately: COMMON_FN, PROJ_ROOT, CONFIG_FILE, NAME_FILE.
## Params: None

function findCommonFn() {

  # Do we have a known installation dir we can use?
  if [[ -n "${INSTALL_DIR}" ]]; then
    COMMON_FN="${INSTALL_DIR}/common.fn"
    PROJ_ROOT="${INSTALL_DIR}"
    CONFIG_FILE="${PROJ_ROOT}/cluster.config"
    NAME_FILE="${PROJ_ROOT}/node.name"
    return
  fi

  # Fall back to searching upwards from the current path.
  local searchDir
  searchDir="$(realpath "${DIR}")"

  while [ -n "${searchDir}" ] && [ "${searchDir}" != "/" ]
  do
    if [ -f "${searchDir}/common.fn" ]; then
      COMMON_FN="${searchDir}/common.fn"
      PROJ_ROOT="${searchDir}"
      CONFIG_FILE="${PROJ_ROOT}/cluster.config"
      NAME_FILE="${PROJ_ROOT}/node.name"
      return
    fi

    searchDir="$(realpath "${searchDir}/..")"
  done

  >&2 echo "ERROR: Could not find common.fn in the installation directory"
  exit 1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Check for any required prerequisites
## Params: None

function checkPrerequisites() {
  if ! which parallel &>/dev/null; then
    errorExit "This script requires the GNU parallels utility."
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Returns the number of chunks in the data
## Params: None

function getDataChunks() {
  local url="${S3_BUCKET}"
  chunks="$(aws s3 ls "${url}" | wc -l)"
  chunks=$((chunks - 1))  # Don't count the generateData.sh script
  echo "${chunks}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Sets the variable BASE_OUTPUT_PATHS with the base mount point paths of each
# disk drive we will write to.
## Params: None

function getBaseOutputPaths() {
  local i=0
  local j
  local mountPoint
  local arbitraryMaximum

  # We wouldn't expect more disks that this.
  arbitraryMaximum=20

  while [ $i -lt ${arbitraryMaximum} ];
  do
    i=$((i+1))

    # This is based on how our cloud-init script mounts local drives
    mountPoint="/mnt/disk${i}"

    if ! [ -w "${mountPoint}" ]; then
      break
    fi

    j=0
    while [ "$j" -lt "${PTNS_PER_DISK}" ]; do
      j=$((j+1))

      # Add it to the list and keep looking for more drives
      BASE_OUTPUT_PATHS+=("${mountPoint}/data${j}")
    done

  done

  if [ "${#BASE_OUTPUT_PATHS[@]}" -eq 0 ]; then
    BASE_OUTPUT_PATHS+=("${PROJ_ROOT}")
  fi

  echo "Found ${#BASE_OUTPUT_PATHS[@]} common filesystem locations for saving data:"
  printf "  %s\n" "${BASE_OUTPUT_PATHS[@]}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Calculates how many data chunks each path will need to download and merge.
## Updates the CHUNKS_PER_NODEPATH variable.
## Params: None

function calculateChunksPerPath() {
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

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Print how many data chunks each node/path will get
## Params: None
function printChunksPerPath() {
  local chunkIdx=-1
  local nodeName
  local host
  local dir
  local chunksForCurrentPath

  local pathExt
  pathExt="/tpch/sf${SF}"

  readonly fmtStr="  %-40s    %-7s    %-7s\n"

  echo "The following paths will each get these number of data chunks:"
  # shellcheck disable=SC2059
  printf "${fmtStr}" "Path" "#Chunks" "Index"
  echo "  ----------------------------------------    -------    -------"


  for nodeName in "${NODE_LIST[@]}";
  do
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"

    for dir in "${BASE_OUTPUT_PATHS[@]}"
    do
      chunkIdx=$((chunkIdx + 1))
      chunksForCurrentPath="${CHUNKS_PER_NODEPATH[${chunkIdx}]}"

      # shellcheck disable=SC2059
      printf "${fmtStr}" "${host}:${dir}${pathExt}" "${chunksForCurrentPath}" "${chunkIdx}"
    done
  done
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Copies the file to each node in the cluster
## Params:

function copyScriptToNodes() {

  local nodeName
  local host

  for nodeName in "${NODE_LIST[@]}";
  do
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"

    echo "Copying script to host: ${host}"
    scp "-o" "BatchMode=yes" "-o" "StrictHostKeyChecking=no" "${DIR}/${SCRIPT_NAME}" "${host}:/tmp/${SCRIPT_NAME}"

    # ensure the script is runnable
    ssh "-o" "BatchMode=yes" "-o" "StrictHostKeyChecking=no" "${host}" chmod +x "/tmp/${SCRIPT_NAME}"
  done
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Invokes the file on each node in the cluster, in parallel.
## Params:

function invokeScriptOnNodes() {

  local commonArgs=("--copyData" "--sf" "${SF}")

  local chunkIdx=-1
  local chunkCount=1
  local chunksForCurrentPath
  local nodeName
  local host
  local nodeArgs
  local installDir

  local pathExt
  pathExt="/tpch/sf${SF}"

  for nodeName in "${NODE_LIST[@]}";
  do
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"
    installDir="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_DIR_KEY}")"

    # Build the target directories and chunk counts for this node
    nodeArgs=("--installDir" "${installDir}" "--start" "${chunkCount}")

    for dir in "${BASE_OUTPUT_PATHS[@]}"
    do
      # Get the number of chunks for this node and path
      chunkIdx=$((chunkIdx + 1))
      chunksForCurrentPath="${CHUNKS_PER_NODEPATH[${chunkIdx}]}"

      nodeArgs+=("--target" "${dir}${pathExt}" "${chunksForCurrentPath}")

      # Increment the chunk count, which sets the starting index for the next node
      chunkCount=$((chunkCount + chunksForCurrentPath))
    done

    # Invoke the script in the background
    echo "Invoking script on host: ${host}"
    echo ssh "-o" "BatchMode=yes" "-o" "StrictHostKeyChecking=no" "${host}" "/tmp/${SCRIPT_NAME}" "${commonArgs[@]}" "${nodeArgs[@]}" &
    ssh "-o" "BatchMode=yes" "-o" "StrictHostKeyChecking=no" "${host}" "/tmp/${SCRIPT_NAME}" "${commonArgs[@]}" "${nodeArgs[@]}" &

  done

  # Wait for all background jobs to complete
  wait
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Downloads and unpacks it appropriately.
## Params: None

function copyData() {
  # create folders
  makeTargets

  # download the data from S3
  time downloadData
  echo ""

  # Unzip, merge and rename files appropriate
  time processData
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Makes the directories that will hold the data
## Params: None

function makeTargets() {
  local i=0
  local dir

  echo "Making target directories on host: $(hostname)"

  while [ "${i}" -lt "${#TARGETS[@]}" ]
  do
    dir="${TARGETS[$i]}"

    # Delete the directory if it already exists
    if [ -d "${dir}" ]; then
      rm -rf "${dir}"
    fi

    mkdir -p "${dir}"

    i=$((i + 2))
  done
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Downloads data from S3 for each target.
## Params: None

function downloadData() {
  local i=0
  local dir
  local count
  local index
  local paddedNum

  local tables=("customer" "lineitem" "orders" "part" "partsupp" "supplier")

  index="${START_IDX}"

  # General Notes:
  # --------------
  #
  # When retrieving from AWS S3 it can sometimes slow down significantly, from 3-4Gbps down
  # to 120Mbps. At this speed you cannot retrieve a large dataset in any reasonable time.
  # The reason for this is that S3 internally partitions the data on S3 key prefixes. These
  # prefixes are the leading part of the "path" where a file is stored in a bucket.
  # e.g. for a file /a/b/c/d.txt in a given bucket the prefix is "/a/b/c/".
  # S3 appears to use the prefix for partitioning, but not all prefixes become partitions.
  # This is apparently done with ML depending on use. Therefore:
  #   1. We cannot completely avoid slowdowns, but hopefully S3 learns the usage behaviour
  #      over time and partitions correctly.
  #   2. We cannot manually specify the partitions.
  #   3. When a slowdown happens, it occurs to the whole bucket and there is nothing you can
  #      do except stop using the bucket for a period of time.
  #   4. Having a wide distribution of prefixes that are unique quickly does help.
  #
  # e.g. regarding (4) above, this schema didn't work well:
  #  Bucket: rapidsse-tpch-data
  #    /tpch/sf100/lineitem/lineitem.tbl.001.gz
  #    /tpch/sf100/lineitem/lineitem.tbl.002.gz
  #    /tpch/sf100/lineitem/lineitem.tbl.003.gz
  #                         ...
  #
  # However this schema worked much better:
  #  Bucket: rapidsse-tpch-sf100
  #    /001/lineitem/lineitem.tbl.001.gz
  #    /001/customer/customer.tbl.001.gz
  #    /001/partsupp/partsupp.tbl.001.gz
  #         ...
  #    /002/lineitem/lineitem.tbl.002.gz
  #    /002/customer/customer.tbl.002.gz
  #         ...
  #    /003/lineitem/lineitem.tbl.003.gz
  #
  # i.e. Make the first part of the prefix random or unique. And files can be placed
  # in their own folders too if that helps get a better unique prefix.

  aws configure set default.s3.max_concurrent_requests 10
  aws configure set default.s3.max_queue_size 100000
  aws configure set default.s3.multipart_threshold 256MB
  aws configure set default.s3.multipart_chunksize 256MB

  echo "About to download data for SF${SF} from S3 on host: $(hostname)"

  for tableName in "${tables[@]}"; do
    echo
    echo
    echo "Downloading table: ${tableName} @ SF${SF}"
    echo

    local i=0
    local j=0
    local index="${START_IDX}"
    local numTargets="${#TARGETS[@]}"
    local -a srcList=()
    local -a destList=()

    # Build a list of source and destination paths which we will then give to gnu parallel.
    # We want to alternate each path from a different target so that when parallel executes
    # them in order we get reasonably even writing to different paths (& different disks)
    # at the same time.

    # First find the maximum number of chunks for any of the target paths.
    # Also make the directories for each table for each target path.
    local maxCount=0

    while [ "${i}" -lt "${#TARGETS[@]}" ]; do
      local dir="${TARGETS[$i]}"
      local count="${TARGETS[$((i+1))]}"
      i=$((i + 2))

      [ "${count}" -gt "${maxCount}" ] && maxCount="${count}"

      # ${dir} already contains the tpch/sf${SF} part.
      mkdir -p "${dir}/${tableName}"
    done

    j=0
    while [ "${j}" -le "${maxCount}" ]; do
      i=0
      while [ "${i}" -lt "${numTargets}" ]; do
        local dir="${TARGETS[$i]}"
        local count="${TARGETS[$((i+1))]}"
        i=$((i + 2))

        if [ "${j}" -ge "${count}" ]; then
          # No more for this target
          break
        fi

        paddedNum="$(printf "%03d" "${index}")"
        srcList+=("${paddedNum}/${tableName}/${tableName}.tbl.${paddedNum}.gz")
        destList+=("${dir}/${tableName}/${tableName}.tbl.${paddedNum}.gz")

        index=$((index + 1))
      done

      j=$((j + 1))
    done


    # Old code that assigns chunks to targets in blocks
#    while [ "${i}" -lt "${numTargets}" ]; do
#      local dir="${TARGETS[$i]}"
#      local count="${TARGETS[$((i+1))]}"
#
#      # ${dir} already contains the tpch/sf${SF} part.
#      mkdir -p "${dir}/${tableName}"
#
#      for j in $(seq "${index}" "$((index + count - 1))" )
#      do
#        paddedNum="$(printf "%03d" "${j}")"
#        srcList+=("${paddedNum}/${tableName}/${tableName}.tbl.${paddedNum}.gz")
#        destList+=("${dir}/${tableName}/${tableName}.tbl.${paddedNum}.gz")
#
#        # Original commands
#        # mkdir -p "${dir}/tpch/sf${SF}/${tableName}/"
#        # aws s3 cp "${S3_BUCKET}/tpch/sf${SF}/${tableName}/${tableName}.tbl.${paddedNum}.gz" "${dir}/${tableName}/" --no-progress &
#
#        # Experiments
#        # aws s3api get-object --region us-east-1 --output text --bucket ${S3_BUCKET} --key "${paddedNum}/${tableName}/${tableName}.tbl.${paddedNum}.gz" "${dir}/${tableName}/${tableName}.tbl.${paddedNum}.gz" &
#        # aws s3api get-object --region us-east-1 --output text --bucket rapidsse-perf-test-data --key tpch/sf100/lineitem/lineitem.tbl.001.gz /tmp/lineitem.tbl.001.gz &
#        # aws s3 cp "${S3_BUCKET}/tpch/sf${SF}/${tableName}/${tableName}.tbl.${paddedNum}.gz" "/dev/shm/${dir}/${tableName}/" --no-progress &
#      done
#
#      i=$((i + 2))
#      index=$((index + count))
#    done

    # wait $(jobs -p) # Wait for all data for the current table to be downloaded
    parallel "${PARALLEL_ARGS[@]}" aws s3 cp "${S3_CP_ARGS[@]}" "s3://${S3_BUCKET}/{1}" "{2}" ::: "${srcList[@]}" ::: "${destList[@]}"
    # parallel "${PARALLEL_ARGS[@]}" aws s3api get-object --region us-east-1 --output text --bucket "${S3_BUCKET}" --key "{1}" "{2}" ::: "${srcList[@]}" ::: "${destList[@]}"
    echo
    echo "Finished downloading table: ${tableName} @ SF${SF}"

  done

  # Also download the nation and region tables onto the first target on each node
  # (it's not necessary on each node but these tables are small).
  dir="${TARGETS[0]}"
  for tableName in "nation" "region"; do
    mkdir -p "${dir}/${tableName}"
    aws s3 cp "${S3_CP_ARGS[@]}" "s3://${S3_BUCKET}/001/${tableName}/${tableName}.tbl.001.gz" "${dir}/${tableName}/${tableName}.tbl.001.gz" &
#    aws s3api get-object --region us-east-1 --output text --bucket "${S3_BUCKET}" --key "001/${tableName}/${tableName}.tbl.001.gz" "${dir}/${tableName}/${tableName}.tbl.001.gz" &
  done

  wait
  echo ""
  echo ""
  echo "Finished downloading data for SF${SF} from S3 on host: $(hostname)"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Unzips, merges data and renames files appropriately
## Params: None

function processData() {

  # For each target path on this node:
    # Go into each directory (customer, lineitem, etc)
    # unzip the data
    # merge files together
    # rename files, cleanup etc

  local i=0
  local dir

  echo
  echo "About to decompress and merge data chunks on host: $(hostname)"

  while [ "${i}" -lt "${#TARGETS[@]}" ]; do
    dir="${TARGETS[$i]}"
    i=$((i + 2))

    # For each table
    for tableName in "${TABLES[@]}"; do

      if ! [ -d "${dir}/${tableName}" ]; then
        # nation and region table data doesn't exist on each drive
        continue
      fi

      {
        cd "${dir}/${tableName}"

        if [ "${tableName}" == "nation" ] || [ "${tableName}" == "region" ]; then
          # Decompress and rename file. Compressed file will be automatically removed.
          gunzip "${tableName}.tbl.001.gz"
          mv "${tableName}.tbl.001" "${tableName}.tbl"

        else
          # Tables: lineitem, orders, supplier, customer, part, parsupp
          # Decompress file, append to a single file and then remove the compressed file.
          # find . -name "${tableName}.tbl.*.gz" -print | sort | xargs gunzip -c >> "${tableName}.tbl"
          local -a fileList=$(find . -name "${tableName}.tbl.*.gz" -print | sort)
          parallel "${PARALLEL_ARGS[@]}" gunzip ::: "${fileList[@]}"

          for f in "${tableName}.tbl."*; do
            cat "${f}" >> "${tableName}.tbl"
            rm -f "${f}"
          done
        fi
      } &

    done

  done

  wait
  echo ""
  echo ""
  echo "Finished decompressing and merging data chunks on host: $(hostname)"
}


###############################################################################
### Main Script                                                             ###
###############################################################################

# Debugging
# set -x
# export PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

# Process the arguments given to the script.
processArgs "$@"

# Set the bucket name
S3_BUCKET="${S3_BUCKET_PREFIX}${SF}"

# We only need to find common.fn. We must source it from outside a function
# because otherwise any declared variables in common.fn will only be local
# to that function and not global, unless explicitly declared that way.
findCommonFn

checkPrerequisites

# shellcheck disable=SC1090
source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }

if [ "${COPY_DATA_MODE}" -eq 1 ]; then
  copyData
  exit
fi

# Pre-copy-data mode. Determine what needs to be copied where.
echo "Loading cluster config at ${CONFIG_FILE}"
loadClusterConfig   # common.fn

echo "Loading name file at ${NAME_FILE}"
loadNodeName        # common.fn

# Determine how many chunks there are in the data
echo "Determining the chunkiness of the data at SF${SF}..."
NUMCHUNKS="$(getDataChunks)"
echo "There are ${NUMCHUNKS} chunks for the SF${SF} data."

# Determine how many nodes in the cluster
NUMNODES="${#NODE_LIST[@]}"

# Determine how many drives are in the cluster and where they are mounted
getBaseOutputPaths

# Determine how many files need to be downloaded to each drive on each node.
# This will set the CHUNKS_PER_NODEPATH variable
calculateChunksPerPath
printChunksPerPath

# Copy this file to each node
copyScriptToNodes

# Invoke this file on each node
invokeScriptOnNodes

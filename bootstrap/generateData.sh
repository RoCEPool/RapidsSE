#!/bin/bash

# A script file for generating TPC-H data in parallel across all machines
# in the cluster.

###############################################################################
### Constants                                                               ###
###############################################################################

# Get the relative path to this file.
#shellcheck disable=SC2155
readonly DIR=$(dirname "$0")
readonly ROOT_DIR="${DIR}/.."
readonly DBGEN_DIR="${ROOT_DIR}/dbgen"
readonly DBGEN_EXE="${DBGEN_DIR}/dbgen"
readonly DISTS="${DBGEN_DIR}/dists.dss"

readonly COMMON_FN="${DIR}/../common.fn"
#shellcheck disable=SC2155
readonly SCRIPT_NAME=$(basename "$0")


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Default modes of operation
CLUSTER_OP=1
CONFIG_FILE="${DIR}/../cluster.config"
NAME_FILE="${DIR}/../node.name"
DEBUG=0
SF=1
TOTAL_CHUNKS=-1
FIRST_OFFSET=-1

declare -a OUTPUT_DIRS=()
declare -a CORE_COUNTS=()
declare -a CHUNK_COUNTS_PER_PATH=()  # Chunks per path for each node

TOTAL_CORES=0

CURRENT_NODE_NAME=""

###############################################################################
### Includes                                                                ###
###############################################################################

# Load bootstrapping functions
#shellcheck source=common.fn
source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }


###############################################################################
### Functions                                                               ###
###############################################################################

## Prints out the valid arguments to the script and exits.
## Params: None
## Note: The variable ${SCRIPT_NAME} must be defined as the name of the script filename.

function printUsageAndExit() {

  # Note: not showing the internal parameters (--cluster, --node, --chunks, --offset)
  # so users don't mess with them!

  echo ""
  echo "Usage: ./${SCRIPT_NAME} [--config <file>]"
  echo "                        --dir <dirName> [--dir <dirName> ...]"
  echo "                        [--debug] [--norename]"
  echo "Details:"
  echo "  --config <file>     Uses a given cluster configuration file."
  echo "                      Default is: ${CONFIG_FILE}"
  echo ""
  echo "  --sf <scaleFactor>  The scale factor of the TPC-H data to be generated."
  echo ""
  echo "  --dir <dirName>     The name of a directory where output files will be written"
  echo "                      on all nodes."
  echo ""
  echo "  --norename          Do not remove the \".1\", \",2\" extensions off the data files."
  echo ""
  echo "  --debug             Displays additional debugging messages during execution."
  echo ""
  echo "  --help              Displays this message."
  exit 1
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Process the arguments for the script.
## Params: None

function processArgs() {

  # Check that some globals are specified with defaults
  if [ -z "${CLUSTER_OP:-}" ];   then errorExit "No default value for \$CLUSTER_OP specified"; fi;

  # Indicates if a given argument was specified.
  local argCluster=0
  local argNode=0
  local argConfig=0
  local argSf=0
  local argDir=0
  local argChunks=0
  local argOffset=0

  while [[ $# -gt 0 ]]
  do
    local key="$1"
    case "${key}" in

      --cluster)
        CLUSTER_OP=1
        argCluster=1
        shift # past argument
        ;;

      --node)
        CLUSTER_OP=0
        argNode=1
        shift # past argument
        ;;

      --config)
        argConfig=1
        CONFIG_FILE="$2"
        shift # past argument
        shift # past value
        ;;

      --config=*)
        argConfig=1
        CONFIG_FILE="${key#*=}"
        shift # past argument=value
        ;;

      --sf)
        argSf=1
        SF="$2"
        shift # past argument
        shift # past value
        ;;

      --dir)
        argDir=$((argDir + 1))
        OUTPUT_DIRS+=( "$2" )
        shift # past argument
        shift # past value
        ;;

      --dir=*)
        argDir=$((argDir + 1))
        OUTPUT_DIRS+=( "${key#*=}" )
        shift # past argument=value
        ;;

      --chunks)
        argChunks=1
        TOTAL_CHUNKS="$2"
        shift # past argument
        shift # past value
        ;;

      --chunks=*)
        argChunks=1
        TOTAL_CHUNKS="${key#*=}"
        shift # past argument and value
        ;;

      --offset)
        argOffset=1
        FIRST_OFFSET="$2"
        shift # past argument
        shift # past value
        ;;

      --offset=*)
        argOffset=1
        FIRST_OFFSET="${key#*=}"
        shift # past argument and value
        ;;

      --debug)
        DEBUG=1
        shift # past argument
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

  # Now we can check for invalid combinations of arguments.
    # --cluster     and --node
    # --sf not specified
    # --config      and no file specified
    # --dir         and no path specified
    # --chunks      and --cluster
    # --offset      and --cluster


  local errmsg="Invalid combination of arguments specified: "

  if [[ ${argCluster} -eq 1  && ${argNode} -eq 1 ]]; then
    printError "${errmsg}" "--cluster" "--node"
    printUsageAndExit
  fi

  if [[ ${argConfig} -eq 1 && -z "${CONFIG_FILE:-}" ]]; then
    printError "${errmsg}" "No config file specified"
    printUsageAndExit
  fi

  # Check that the config file exists
  if [[ ! -f "${CONFIG_FILE}" ]]; then
    printError "Config file does not exist: ${CONFIG_FILE}"
    printUsageAndExit
  fi

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

  if [[ "${argDir}" -eq 0 || "${#OUTPUT_DIRS[@]}" -eq 0 ]]; then
    printError "No output directory specified (use the --dir argument)."
    printUsageAndExit
  fi

  if [[ "${argDir}" -ne "${#OUTPUT_DIRS[@]}" ]]; then
    printError "Mismatched number of --dir arguments (${argDir}) and directory paths (${#OUTPUT_DIRS})."
    printUsageAndExit
  fi

  # Convert any relative directories to absolute
  for i in "${!OUTPUT_DIRS[@]}"
  do
    local dir="${OUTPUT_DIRS[$i]}"

    if [ "${dir:0:1}" != "/" ]; then
      OUTPUT_DIRS[i]="$(readlink -f "./${dir}")"
    fi
  done

  if [[ ${argCluster} -eq 1  && ${argChunks} -eq 1 ]]; then
    printError "${errmsg}" "--cluster" "--chunks"
    printUsageAndExit
  fi

  if [[ ${argCluster} -eq 1  && ${argOffset} -eq 1 ]]; then
    printError "${errmsg}" "--cluster" "--offset"
    printUsageAndExit
  fi


  # Argument dependencies:
    # --node and --chunks
    # --node and --offset
  if [[ ${argNode} -eq 1  && ${argChunks} -eq 0 ]]; then
    printError "${errmsg}" "--node requires --chunks"
    printUsageAndExit
  fi

  if [[ ${argNode} -eq 1  && ${argOffset} -eq 0 ]]; then
    printError "${errmsg}" "--node requires --offset"
    printUsageAndExit
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks for any required tools/utilities
## Params: None

function checkPrerequisites() {
  if ! which parallel &>/dev/null; then
    errorExit "The utility parallel is required and must be installed."
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Generates TPC-H data on all nodes in the cluster.
## Params: None

function generateClusterData() {

  local startTime
  startTime="$(date +%s)"

  # Create all of the output directories and ensure they are writable.
  makeOutputDirs

  # Get the CPU core counts on each node in the cluster.
  # Then use that to calculate how many chunks each node should generate
  getClusterCoreCounts
  getClusterChunkCounts

  local offset=1

  echo "Generating TPC-H data for SF${SF}. Writing the data out to these directories:"
  printIndentArray 2 "${OUTPUT_DIRS[@]}"

  local i=0
  for nodeName in "${NODE_LIST[@]}"
  do
    invokeNodeGeneration "${nodeName}" "${offset}" &
    offset=$(( offset + ${#OUTPUT_DIRS[@]} * ${CHUNK_COUNTS_PER_PATH[$i]} ))
    i=$(( i+1 ))
  done

  # Wait for all parallel jobs to finish.
  wait

  local endTime
  local elapsedTime

  endTime="$(date +%s)"
  elapsedTime=$((endTime - startTime))


  echo ""
  echo "Generated TPC-H SF${SF} data across ${#NODE_LIST[@]} nodes in ${elapsedTime} seconds."
  echo ""
  echo "Refer to the following directories on each node:"

  for dir in "${OUTPUT_DIRS[@]}"
  do
    echo -n "  ls -lh ${dir} ;"
  done
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Calculates the number of CPU cores in the cluster and writes this to the CORE_COUNTS array.
## Params: None

function getClusterCoreCounts() {
  local nodeName
  local host
  local coreCmd=""
  local count=""
  declare -a sshCmd=()

  for nodeName in "${NODE_LIST[@]}"
  do
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"

    coreCmd="{ which nproc &>/dev/null && nproc; } || { getconf _NPROCESSORS_ONLN 2>/dev/null; } || { echo 0; }"
    sshCmd=( "ssh" "${SSH_COMMON_OPTIONS[@]}" "${host}" "bash -c '${coreCmd}'" )
    debugPrint "About to get core count on node ${nodeName} with the command: ${sshCmd[*]}"
    count="$("${sshCmd[@]}")"

    if [ -z "${count}" ] || [ "${count}" -ne "${count}" ]; then
      errorExit "Could not get the CPU core count for node ${nodeName} (host: ${host})."
    fi

    # Append to the CORE_COUNTS array and add to the total count
    CORE_COUNTS+=("${count}")
    TOTAL_CORES=$((TOTAL_CORES + count))
  done
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Gets the number of CPU cores on the current machine and writes this to stdout.
## Params: None

function getCurrentCoreCount() {

  local host
  host="$(nodeCfgGetValue "${CURRENT_NODE_NAME}" "${NODE_CFG_HOST_KEY}")"

  local count
  count="$({ which nproc &>/dev/null && nproc; } || { getconf _NPROCESSORS_ONLN 2>/dev/null; } || { echo 0; })"

  if [ -z "${count}" ] || [ "${count}" -ne "${count}" ]; then
    errorExit "Could not get the CPU core count for node ${CURRENT_NODE_NAME} (host: ${host})."
  fi

  echo "${count}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Calculates how many chunks per path each node should create
## Params: None

function getClusterChunkCounts() {
  local chunks
  local total=0

  for cpus in "${CORE_COUNTS[@]}"; do
    chunks=$((cpus / ${#OUTPUT_DIRS[@]}))

    if [ "${chunks}" -le 0 ]; then
      chunks=1
    fi

    CHUNK_COUNTS_PER_PATH+=("${chunks}")
    total=$((total + chunks * ${#OUTPUT_DIRS[@]}))
  done

  TOTAL_CHUNKS="${total}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Get the number of chunks to be used per output directory for the current node.
## Params: None

function getCurrentChunkCountPerPath() {
  local cpus="$(getCurrentCoreCount)"

  local chunks="$((cpus / ${#OUTPUT_DIRS[@]}))"

  if [ "${chunks}" -le 0 ]; then
    chunks=1
  fi

  echo "${chunks}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Makes the directories where the output files will reside, if applicable, and
## checks that they are writable. Returns a non-zero exit status if there is
## an error.
## Params: None

function makeOutputDirs() {
  local nodeName
  local host
  local makeDirCmd=""
  local checkWritableCmd=""
  declare -a sshCmd=()

  for nodeName in "${NODE_LIST[@]}"
  do
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"

    makeDirCmd=""
    checkWritableCmd=""

    for dir in "${OUTPUT_DIRS[@]}"
    do
      # Make the directory if it doesn't exist. Then check if the directory is writable.
      makeDirCmd="${makeDirCmd} if ! mkdir -p \"${dir}\"; then echo \"Cannot make directory on host ${host}: ${dir}\"; exit 1; fi ; "
      checkWritableCmd="${checkWritableCmd} if [ ! -w \"${dir}\" ]; then echo \"Directory is not writable on host ${host}: ${dir}\"; exit 1; fi; "
    done

    sshCmd=( "ssh" "${SSH_COMMON_OPTIONS[@]}" "${host}" "bash -c '${makeDirCmd}'" )
    debugPrint "About to create output directories on node ${nodeName} with the command: ${sshCmd[*]}"
    "${sshCmd[@]}" || exit 1

    sshCmd=( "ssh" "${SSH_COMMON_OPTIONS[@]}" "${host}" "bash -c '${checkWritableCmd}'" )
    debugPrint "About to check the writability of output directories on node ${nodeName} with the command: ${sshCmd[*]}"
    "${sshCmd[@]}" || exit 1
  done

  echo "All output directories created."
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Calls this script on each node to kick off the data generation.
## Params:
##  $1 : The name of the node to generate data on.
##  $2 : The first chunk offset for this node.

function invokeNodeGeneration() {
  local nodeName=$1
  local offset=$2

  local host
  local installDir
  local remoteCmd
  local sshCmd

  host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"
  installDir="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_DIR_KEY}")"

  remoteCmd="\"${installDir}/dbgen/${SCRIPT_NAME}\" --node --sf ${SF} --chunks ${TOTAL_CHUNKS} --offset ${offset}"

  for dir in "${OUTPUT_DIRS[@]}"
  do
    remoteCmd="${remoteCmd} --dir \"${dir}\""
  done

  if [ ${DEBUG} -eq 1 ]; then
    remoteCmd="${remoteCmd} --debug"
  fi

  sshCmd=( "ssh" "${SSH_COMMON_OPTIONS[@]}" "${host}" "bash" "-c" "'${remoteCmd}'" )

  debugPrint "About to generate TPC-H data on node ${nodeName} with the command: ${sshCmd[*]}"

  "${sshCmd[@]}" || errorExit "Could not generate TPC-H data on node ${nodeName}."
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Use dbgen to generate data in chunks for a given output directory.
## This should be a standalone function that can be invoked from parallel
## (i.e. no dependencies on global variables, etc).
##
## Params:
##  (1): The dbgen executable
##  (2): Path to the dists.dss file
##  (3): Scale factor
##  (4): Output directory path
##  (5): The chunk number to generate
##  (6): The total number of chunks that will be generated in the cluster
##  (7): The name of the table to generate data for
##  (8): The name of the current node
##  (9): The hostname of the current node

function generateChunk() {
  [ $# -eq 9 ] || { echo "Invalid number of arguments: $#"; exit 1; }

  local dbgen=$1
  local dists=$2
  local sf=$3
  local dir=$4
  local chunkNum=$5
  local totalChunks=$6
  local tableName=$7
  local nodeName=$8
  local host=$9

  local tableCode

  case "${tableName}" in
    customer)
      tableCode="c"
      ;;

    lineitem)
      tableCode="L"
      ;;

    orders)
      tableCode="O"
      ;;

    partsupp)
      tableCode="S"
      ;;

    part)
      tableCode="P"
      ;;

    supplier)
      tableCode="s"
      ;;

    nation)
      tableCode="n"
      ;;

    region)
      tableCode="r"
      ;;

    *)
      echo "Invalid table: ${tableName}"
      exit 1;
      ;;
  esac

  {
    cd "${dir}" || exit

    local paddedChunkNum=$(printf "%03d" "${chunkNum}")

    echo "$(date +%T) : Table: ${tableName} : Generating chunk #${paddedChunkNum} / ${totalChunks} : Path ${dir} : Node ${nodeName} [host: ${host}]"

    if [ "${totalChunks}" -ne 1 ]; then
      "${dbgen}" -b "${dists}" -f -q -s "${sf}" -C "${totalChunks}" -S "${chunkNum}" -T "${tableCode}"
    else
      "${dbgen}" -b "${dists}" -f -q -s "${SF}" -T "${tableCode}"
    fi
  }
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
##
## Params:
##  (1): Scale factor
##  (2): Output directory path
##  (3): The name of the table to generate data for
##  (4): The name of the current node
##  (5): The hostname of the current node

function combineChunks() {
  [ $# -eq 5 ] || { echo "Invalid number of arguments: $#"; exit 1; }

  local sf=$1
  local dir=$2
  local tableName=$3
  local nodeName=$4
  local host=$5

  echo "$(date +%T) : Table: ${tableName} : Combining chunks for path ${dir}: Node ${nodeName} [host: ${host}]"

  {
    cd "${dir}" || exit

    local fileList=()
    mapfile -t fileList < <(find . -type f -name "${tableName}.tbl.*" | sort -V)

#    echo "Sorting: ${fileList[*]}"

    for f in "${fileList[@]}"; do
      if [ "${f}" == "${fileList[0]}" ]; then
        mv "${f}" "${tableName}.tbl"

      else
        cat "${f}" >> "${tableName}.tbl"
        rm -f "${f}"
      fi
    done
  }
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Generates TPC-H data on the current node in the cluster.
## Params: None

function generateNodeData() {

  readonly chunkableTables=("customer" "lineitem" "orders" "partsupp" "part" "supplier")
  readonly smallTables=("nation" "region")

  local host
  host="$(nodeCfgGetValue "${CURRENT_NODE_NAME}" "${NODE_CFG_HOST_KEY}")"

  local nCpus
  nCpus="$(getCurrentCoreCount)"

  local chunksPerPath
  chunksPerPath="$(getCurrentChunkCountPerPath)"

  local chunksOnThisNode=$((${#OUTPUT_DIRS[@]} * chunksPerPath))
  debugPrint "Generating data for SF${SF} for chunks $(seq "${FIRST_OFFSET}" $((FIRST_OFFSET + chunksOnThisNode - 1)) | xargs) of ${TOTAL_CHUNKS} on node ${CURRENT_NODE_NAME} [host: ${host}]."

  # Needed/called by parallel
  export -f generateChunk
  export -f combineChunks

  # Let's say that this machine has 8 CPUs and 2 output directories. It needs to generate
  # 8*2=16 chunks for each output directories and then consolidate them into a single file
  # per table per output path.
  # Say we know that this node needs to start generating chunks from offset 33. Therefore we
  # need to generate chunks #33 to (33 + 16 - 1) = #48 inclusively.
  # We want to invoke parallel to use all available CPUs to generate chunks for each output
  # path. i.e. chunks #33 to #40 are for disk A and chunks #41 to #48 are for disk B.
  # We will do this by specifying 2 input parameters to parallel
  #  1. an array of size 16 of output paths (values are repeated evenly)
  #  2. an array of the chunk numbers to generate
  # GNU parallel will invoke dbgen for each value of the inputs.

  local lastOffset=$((FIRST_OFFSET + chunksOnThisNode - 1))
  local chunkArray=()
  mapfile -t chunkArray < <(seq "${FIRST_OFFSET}" "${lastOffset}")

  local dirArray=()
  for dir in "${OUTPUT_DIRS[@]}"; do
    for ((i = 1; i <= "${chunksPerPath}"; i++)); do
      dirArray+=("${dir}")
    done
  done

  # If we have more paths than cpus then let's spawn extra jobs and let the OS take care of
  # scheduling them. This will give faster results than if we only run exactly one job per
  # cpu, as per default parallel behaviour.
  local extraParallelArgs=""
  if [ "${#OUTPUT_DIRS[@]}" -gt "${nCpus}" ]; then
    extraParallelArgs=("-j" "${#OUTPUT_DIRS[@]}")
  fi

  # Generate each table sequentially
  for table in "${chunkableTables[@]}"; do
    parallel --xapply --shuf "${extraParallelArgs[@]}" generateChunk "${DBGEN_EXE}" "${DISTS}" "${SF}" "{1}" "{2}" "${TOTAL_CHUNKS}" "${table}" "${CURRENT_NODE_NAME}" "${host}" ::: "${dirArray[@]}" ::: "${chunkArray[@]}"

    # Combine all of the individual chunks into a single *.tbl file for each path.
    # Since this is I/O heavy and CPU light, we can do this in the background while
    # we generate the next table.
    parallel combineChunks "${SF}" "{1}" "${table}" "${CURRENT_NODE_NAME}" "${host}" ::: "${OUTPUT_DIRS[@]}" &
  done

  # Generate data for the nation and region tables
  parallel --xapply generateChunk "${DBGEN_EXE}" "${DISTS}" "${SF}" "{1}" "1" "1" "{2}" "${CURRENT_NODE_NAME}" "${host}" ::: "${OUTPUT_DIRS[0]}" ::: "${smallTables[@]}"

  wait

  echo "TPC-H SF${SF} data generation completed on node ${CURRENT_NODE_NAME} (host: ${host})"
}


###############################################################################
### Main Script                                                             ###
###############################################################################

# Process the arguments given to the script.
processArgs "$@"

debugPrint "CLUSTER_OP = ${CLUSTER_OP}"
debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint "NAME_FILE = ${NAME_FILE}"
debugPrint "DEBUG = ${DEBUG}"
debugPrint "SF = ${SF}"
debugPrint "CHUNKS = ${TOTAL_CHUNKS}"
debugPrint "FIRST_OFFSET = ${FIRST_OFFSET}"
debugPrint "OUTPUT_DIRS = "
debugPrintArray 2 "${OUTPUT_DIRS[@]}"


# Import the cluster configuration
loadClusterConfig

checkPrerequisites

if [ ${CLUSTER_OP} -eq 1 ]; then
  generateClusterData

else
  loadNodeName
  time generateNodeData
fi


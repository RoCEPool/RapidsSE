#!/bin/bash

# A script file for generating TPC-H data in parallel across all machines
# in the cluster.

###############################################################################
### Constants                                                               ###
###############################################################################

# Get the relative path to this file.
readonly DIR=$(dirname "$0")
readonly ROOT_DIR="${DIR}/.."
readonly DBGEN_DIR="${ROOT_DIR}/dbgen"
readonly DBGEN_EXE="${DBGEN_DIR}/dbgen"
readonly DISTS="${DBGEN_DIR}/dists.dss"

readonly COMMON_FN="${DIR}/../common.fn"
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
CHUNKS=-1
FIRST_OFFSET=-1
NO_RENAME=0

declare -a OUTPUT_DIRS

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

  echo ""
  echo "Usage: ./${SCRIPT_NAME} [--cluster | --node]"
  echo "                        [--config <file>]"
  echo "                        --dir <dirName> [--dir <dirName> ...]"
  echo "                        [--debug]"
  echo "Details:"
  echo "  --cluster           [Default] Generates TPC-H data on all nodes in the cluster."
  echo "  --node              Generates TPC-H data for the current node only."
  echo ""
  echo "  --config <file>     Uses a given cluster configuration file."
  echo "                      Default is: ${CONFIG_FILE}"
  echo ""
  echo "  --sf <scaleFactor>  The scale factor of the TPC-H data to be generated."
  echo ""
  echo "  --dir <dirName>     The name of a directory where output files will be written"
  echo "                      on all nodes."
  echo ""
  echo "  --chunks <val>      The total number of ways the data is split."
  echo "                      Requires --node."
  echo "  --offset <val>      The first (1-based) chunk number that this node will create."
  echo "                      Requires --node."
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
  if [ -z "${CLUSTER_OP}" ];   then errorExit "No default value for \$CLUSTER_OP specified"; fi;

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
    key="$1"
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
        CHUNKS="$2"
        shift # past argument
        shift # past value
        ;;

      --chunks=*)
        argChunks=1
        CHUNKS="${key#*=}"
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

      --norename)
        NO_RENAME=1
        shift # past argument
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

  if [[ ${argConfig} -eq 1 && -z "${CONFIG_FILE}" ]]; then
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
      OUTPUT_DIRS[$i]="$(readlink -f ".")/${dir}"
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
## Generates TPC-H data on all nodes in the cluster.
## Params: None

function generateClusterData() {

  local startTime
  startTime="$(date +%s)"

  # Calculate how many chunks there will be
  CHUNKS=$(( ${#NODE_LIST[@]} * ${#OUTPUT_DIRS[@]} ))

  # Create all of the output directories and ensure they are writable.
  makeOutputDirs

  local offset=1

  echo "Generating TPC-H data for SF${SF}. Writing the data out to these directories:"
  printIndentArray 2 "${OUTPUT_DIRS[@]}"

  for nodeName in "${NODE_LIST[@]}"
  do
    invokeNodeGeneration "${nodeName}" "${offset}" &
    offset=$(( offset + ${#OUTPUT_DIRS[@]} ))
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
## Makes the directories where the output files will reside, if applicable, and
## checks that they are writable. Returns a non-zero exit status if there is
## an error.
## Params: None

function makeOutputDirs() {
  local nodeName
  local host
  local remoteCmd=""
  declare -a sshCmd

  for nodeName in "${NODE_LIST[@]}"
  do
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"
    
    for dir in "${OUTPUT_DIRS[@]}" 
    do
      # Make the directory if it doesn't exist. Then check if the directory is writable.
      remoteCmd="${remoteCmd} mkdir -p \"${dir}\" || { echo \"Cannot make directory on host ${host}: ${dir}\"; exit 1; } ; if [ ! -w \"${dir}\" ]; then echo \"Directory is not writable on host ${host}: ${dir}\"; exit 1; fi; true;"
    done

    sshCmd=( "ssh" "-q" "${host}" "bash -c '${remoteCmd}'" )

    debugPrint "About to create output directories on node ${nodeName} with the command: ${sshCmd[*]}"

    "${sshCmd[@]}" || errorExit "Could not create output directories on node ${nodeName}."
  done
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

  remoteCmd="\"${installDir}/dbgen/${SCRIPT_NAME}\" --node --sf ${SF} --chunks ${CHUNKS} --offset ${offset}"

  if [ "${NO_RENAME}" -eq 1 ]; then
    remoteCmd="${remoteCmd} --norename"
  fi

  for dir in "${OUTPUT_DIRS[@]}"
  do
    remoteCmd="${remoteCmd} --dir \"${dir}\""
  done

  if [ ${DEBUG} -eq 1 ]; then
    remoteCmd="${remoteCmd} --debug"
  fi
 
  sshCmd=( "ssh" "-q" "${host}" "bash" "-c" "'${remoteCmd}'" )

  debugPrint "About to generate TPC-H data on node ${nodeName} with the command: ${sshCmd[*]}"

  "${sshCmd[@]}" || errorExit "Could not generate TPC-H data on node ${nodeName}."
 
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Generates TPC-H data on the current node in the cluster.
## Params: None

function generateNodeData() {
 
  local host
  host="$(nodeCfgGetValue "${CURRENT_NODE_NAME}" "${NODE_CFG_HOST_KEY}")"

  local counter=0

  debugPrint "Generating data for SF${SF} for chunks $(seq "${FIRST_OFFSET}" $((FIRST_OFFSET + ${#OUTPUT_DIRS[@]} -1)) | xargs) of ${CHUNKS} on node ${CURRENT_NODE_NAME} [host: ${host}]."

  for dir in "${OUTPUT_DIRS[@]}"
  do
    local offset
    offset=$((FIRST_OFFSET + counter))
    counter=$((counter + 1))

    (
      cd "${dir}" || exit

      echo "$(date +%T) : Generating chunk #${offset} for customers table on node ${CURRENT_NODE_NAME} [host: ${host}]..."
      "${DBGEN_EXE}" -b "${DISTS}" -f -q -s "${SF}" -C "${CHUNKS}" -S "${offset}" -T c || exit
      echo "$(date +%T) : Generation of chunk #${offset} for customers table complete."

      echo "$(date +%T) : Generating chunk #${offset} for orders table on node ${CURRENT_NODE_NAME} [host: ${host}]..."
      "${DBGEN_EXE}" -b "${DISTS}" -f -q -s "${SF}" -C "${CHUNKS}" -S "${offset}" -T O || exit
      echo "$(date +%T) : Generation of chunk #${offset} for orders table complete."

      echo "$(date +%T) : Generating chunk #${offset} for lineitem table on node ${CURRENT_NODE_NAME} [host: ${host}]..."
      "${DBGEN_EXE}" -b "${DISTS}" -f -q -s "${SF}" -C "${CHUNKS}" -S "${offset}" -T L || exit
      echo "$(date +%T) : Generation of chunk #${offset} for lineitem table complete."
    ) &
  done

  wait

  # Rename the data files to remove the .1, .2, .3... extensions
  if [ "${NO_RENAME}" -ne 1 ]; then
    removeFileChunkExt
  fi

  echo "$(date +%T) : Data generation complete on node ${CURRENT_NODE_NAME} [host: ${host}]."
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Removes the chunking/offset extension from the file name. So instead of 
## having a file lineitem.tbl.3 you simply have lineitem.tbl.
## Params: None

function removeFileChunkExt() {
  
  for dir in "${OUTPUT_DIRS[@]}"
  do
    declare -a files
    mapfile -t files < <(find "${dir}" -type f -name "*.tbl.*")

    for f in "${files[@]}"
    do
      debugPrint "Renaming file: \"${f}\" --> \"${f%.*}\""
      mv "${f}" "${f%.*}" || errorExit "Error renaming file: ${f}"
    done
  done
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
debugPrint "CHUNKS = ${CHUNKS}"
debugPrint "FIRST_OFFSET = ${FIRST_OFFSET}"
debugPrint "NO_RENAME = ${NO_RENAME}"
debugPrint "OUTPUT_DIRS = "
debugPrintArray 2 "${OUTPUT_DIRS[@]}"


# Import the cluster configuration
loadClusterConfig

if [ ${CLUSTER_OP} -eq 1 ]; then
  generateClusterData

else
  loadNodeName
  generateNodeData
fi


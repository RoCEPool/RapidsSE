#!/bin/bash
# This file starts RapidsSE for the entire cluster, or just a single node. 

###############################################################################
### Constants                                                               ###
###############################################################################

# Get the relative path to this file.
#shellcheck disable=SC2155
readonly DIR=$(dirname "$0")
readonly ROOT_DIR="${DIR}"

readonly BOOT_FN="${DIR}/boot.fn"
#shellcheck disable=SC2155
readonly SCRIPT_NAME=$(basename "$0")


###############################################################################
### Includes                                                                ###
###############################################################################

# Load bootstrapping functions
#shellcheck source=boot.fn
source "${BOOT_FN}" || { >&2 echo "Could not load ${BOOT_FN}"; exit 1; }


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Default modes of operation
CLUSTER_OP=1
CLUSTER_INIT=0
CONFIG_FILE="${DIR}/${DEFAULT_CONFIG_PATH}"
NAME_FILE="${DIR}/${DEFAULT_NAME_FILE}"
DEBUG=0
RELOAD_PATH=""
EXTRA_START_ARGS=()

# Will be set after loading node name file.
CURRENT_NODE_NAME=""


###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Process the arguments for the start script.
## Params: None

function processStartArgs() {

  # Check that some globals are specified with defaults
  if [ -z "${CLUSTER_OP:-}" ];   then errorExit "No default value for \$CLUSTER_OP specified"; fi;
  if [ -z "${CLUSTER_INIT:-}" ]; then errorExit "No default value for \$CLUSTER_INIT specified"; fi;

  # Indicates if a given argument was specified.
  local argCluster=0
  local argNode=0
  local argConfig=0
  local argReload=0

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

      --reload)
        argReload=1
        RELOAD_PATH="$2"
        EXTRA_START_ARGS+=("$1" "$2")
        shift # past argument
        shift # past value
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

      --debug)
        DEBUG=1
        shift # past argument
        ;;

      --clusterinit)
        #argClusterInit=1
        CLUSTER_INIT=1
        shift # past argument
        ;;

      --help|-h)
        shift
        printStartStopUsageAndExit
        ;;

      *)
        shift
        printStartStopUsageAndExit
        ;;
    esac
  done

  # Now we can check for invalid combinations of arguments.
    # --cluster     and --node
    # --config      and no file specified

  local errmsg="Invalid combination of arguments specified: "

  if [[ ${argCluster} -eq 1  && ${argNode} -eq 1 ]]; then
    printError "${errmsg}" "--cluster" "--node"
    printStartStopUsageAndExit
  fi

  if [[ ${argConfig} -eq 1 && -z "${CONFIG_FILE:-}" ]]; then
    printError "${errmsg}" "No config file specified"
    printStartStopUsageAndExit
  fi

  # Check that the config file exists
  if [[ ! -f "${CONFIG_FILE}" ]]; then
    printError "Config file does not exist: ${CONFIG_FILE}"
    printStartStopUsageAndExit
  fi

  if [[ ${argReload} -eq 1 && -z "${RELOAD_PATH:-}" ]]; then
      printError "${errmsg}" "No reload snapshot was specified"
      printStartStopUsageAndExit
  fi
}

###############################################################################
### Main Script                                                             ###
###############################################################################

# Process the arguments given to the script.
processStartArgs "$@"

debugPrint "CLUSTER_OP = ${CLUSTER_OP}"
debugPrint "CLUSTER_INIT = ${CLUSTER_INIT}"
debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint "NAME_FILE = ${NAME_FILE}"
debugPrint "DEBUG = ${DEBUG}"
debugPrint "RELOAD_PATH = ${RELOAD_PATH}"
debugPrint "EXTRA_START_ARGS = ${EXTRA_START_ARGS[*]}"

# Import the cluster configuration
loadClusterConfig

if [ ${CLUSTER_OP} -eq 1 ]; then
  if [ "${CLUSTER_INIT}" -eq 1 ]; then
    remoteClusterXConnect
    exit
  fi

  if [ -n "${RELOAD_PATH:-}" ]; then
    # Validate the reload path exists on all nodes
    validateReloadPath "${RELOAD_PATH}"
  fi

  #shellcheck disable=SC2119
  startCluster

else
  loadNodeName

  if [ "${CLUSTER_INIT}" -eq 1 ]; then
    localClusterXConnect
    exit
  fi

  startCurrentNode "${EXTRA_START_ARGS[@]}"
fi

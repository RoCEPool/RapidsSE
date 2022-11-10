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
ARG_EXTRA=""
ARG_EXTRA_VALUE=""

# Will be set after loading node name file.
CURRENT_NODE_NAME=""


###############################################################################
### Functions                                                               ###
###############################################################################

# None


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
debugPrint "ARG_EXTRA = ${ARG_EXTRA}"
debugPrint "ARG_EXTRA_VALUE = ${ARG_EXTRA_VALUE}"

# Import the cluster configuration
loadClusterConfig

if [ ${CLUSTER_OP} -eq 1 ]; then
  if [ "${CLUSTER_INIT}" -eq 1 ]; then
    remoteClusterXConnect
    exit
  fi

  #shellcheck disable=SC2119
  startCluster ${ARG_EXTRA} ${ARG_EXTRA_VALUE}

else
  loadNodeName

  if [ "${CLUSTER_INIT}" -eq 1 ]; then
    localClusterXConnect
    exit
  fi

  startCurrentNode ${ARG_EXTRA} ${ARG_EXTRA_VALUE}
fi

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
  local argExtra=0
  #local argClusterInit=0

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
        argExtra=1
        ARG_EXTRA="$1"
        ARG_EXTRA_VALUE="$2"
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
}
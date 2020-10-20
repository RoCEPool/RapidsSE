#!/bin/bash

###############################################################################
### Constants                                                               ###
###############################################################################

# Get the relative path to this file.
readonly DIR=$(dirname "$0")
readonly ROOT_DIR="${DIR}"

readonly BOOT_FN="${DIR}/boot.fn"
readonly SCRIPT_NAME=$(basename "$0")


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Default modes of operation
CLUSTER_OP=1
CONFIG_FILE="${DIR}/cluster.config"
NAME_FILE="${DIR}/node.name"
DEBUG=0

CURRENT_NODE_NAME=""

###############################################################################
### Includes                                                                ###
###############################################################################

# Load bootstrapping functions
#shellcheck source=boot.fn
source "${BOOT_FN}" || { >&2 echo "Could not load ${BOOT_FN}"; exit 1; }


###############################################################################
### Functions                                                               ###
###############################################################################

## Prints out the valid arguments to the start & stop scripts and exits.
## Params: None
## Note: The variable ${SCRIPT_NAME} must be defined as the name of the start
##       or stop script filename.

function printUsageAndExit() {

  echo ""
  echo "Usage: ./${SCRIPT_NAME} [--cluster | --node]"
  echo "                    [--config <file>]"
  echo "                    [--debug]"
  echo "Details:"
  echo "  --cluster         [Default] Displays status for all nodes in the cluster."
  echo "  --node            Displays status for the current node only."
  echo ""
  echo "  --config <file>   Uses a given cluster configuration file."
  echo "                    Default is: ${CONFIG_FILE}"
  echo ""
  echo "  --debug           Displays additional debugging messages during execution."
  echo ""
  echo "  --help            Displays this message."
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
    # --config      and no file specified

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

# Import the cluster configuration
loadClusterConfig

if [ ${CLUSTER_OP} -eq 1 ]; then
  printClusterStatus

else
  loadNodeName
  printNodeStatus
fi


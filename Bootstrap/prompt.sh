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

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Prints out the valid arguments to the start & stop scripts and exits.
## Params: None
## Note: The variable ${SCRIPT_NAME} must be defined as the name of the start
##       or stop script filename.

function printUsageAndExit() {

  echo ""
  echo "Usage: ./${SCRIPT_NAME} [--config <file>]"
  echo "                   [--debug]"
  echo "Details:"
  echo "  --config <file>   Uses a given cluster configuration file."
  echo "                    Default is: ${CONFIG_FILE}"
  echo ""
  echo "  --debug           Displays additional debugging messages during execution."
  echo ""
  echo "  --help            Displays this message."
  exit 1
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Process the arguments for the start and stop scripts.
## Params: None

function processArgs() {

  # Indicates if a given argument was specified.
  local argConfig=0

  while [[ $# -gt 0 ]]
  do
    key="$1"
    case "${key}" in

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

  # Now check for invalid argument combinations and settings

  # If --config was specified then so must the config file.
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


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Starts the SE Prompt
## Params: None

function startPrompt() {

  declare -a promptCmd
  promptCmd+=("${EXE}")
  promptCmd+=("--name")
  promptCmd+=("${CURRENT_NODE_NAME}")
  promptCmd+=("--prompt")
  promptCmd+=("--metadata")
  promptCmd+=("--Commands")
  promptCmd+=("${ROOT_DIR}/Commands")
  promptCmd+=("--appendTrace")

  # Set the library path so the RapidsSE library can be found and loaded
  export LD_LIBRARY_PATH="${EXE_DIR}:${LD_LIBRARY_PATH}"

  # Run the prompt
  "${promptCmd[@]}"
}

###############################################################################
### Main Script                                                             ###
###############################################################################

# Process the arguments given to the script.
processArgs "$@"

debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint "NAME_FILE = ${NAME_FILE}"
debugPrint "DEBUG = ${DEBUG}"

# Import the cluster configuration and node name
loadClusterConfig
loadNodeName

# Check that the local SE node is running
if ! isLocalSeRunning; then
  errorExit "Cannot start SE Prompt - RapidsSE is not running on this node."
fi

# Start the SE Prompt
startPrompt

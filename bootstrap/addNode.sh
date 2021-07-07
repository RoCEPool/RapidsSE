#!/bin/bash

# This file is used to dynamically add one or more nodes to a cluster.
# It will install the new nodes, reconfigure the cluster, and start the
# new nodes if necessary.
# This can be called whether the cluster is running or stopped.

###############################################################################
### Environment Variables                                                   ###
###############################################################################

# NO_RESOLVE_HOSTS=1  Skip the check that hostnames are resolvable


###############################################################################
### Constants                                                               ###
###############################################################################

# For safety:
#  -e          : exit immediately when a command fails.
#  -E          : don't ignore ERR traps in shell functions and subshells.
#  -o pipefail : exits if any process in a pipeline fails (normally it is just
#                the last one).
#  -u          : treat unset variables as an error and exit immediately.
#  -x          : print each command before executing it (for debugging).
# Note: We don't set pipefail globally because we can expect some piped
#       commands to fail (e.g. if you enter an invalid hostname).
set -Eeu 

#shellcheck disable=2034
readonly DIR=$(dirname "$0")
readonly THIS_FILE=$(basename "$0")
readonly SETUP_FN="setup.fn"
readonly BOOT_FN="boot.fn"


###############################################################################
### Common Functions                                                        ###
###############################################################################

# Include a file that contains all of the functions used within.
#shellcheck source=setup.fn
source "${SETUP_FN}" || { >&2 echo "Could not load ${SETUP_FN}"; exit 1; }

#shellcheck source=boot.fn
source "${BOOT_FN}" || { >&2 echo "Could not load ${BOOT_FN}"; exit 1; }


###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Modes of operation, as dictated by arguments given to the script.
# Values are either 1 or 0.
CLUSTER_INSTALL=1
INTERACTIVE_INSTALL=1
ADV_WIZARD=0
DEBUG=0

# Path to the installation (run) file used.
INSTALL_FILE=""

# Path to the config file used, if any.
CONFIG_FILE=""

# Has value 1 if any nodes in the cluster are running, otherwise 0.
CLUSTER_IS_RUNNING=0

# The array of unique node names. The values here are used to help form the
# name of the dynamic associative arrays used to hold the configuration of
# an individual node.
declare -ga NODE_LIST=()

# The prefix given to each node name before its sequence number.
NODE_NAME_PREFIX="${NODE_CFG_DEFAULT_NAME_PREFIX}"

# The unique number used with the above NODE_NAME_PREFIX to form the node name.
NODE_SEQ_NUM=0

# The name of the current node, given in the --name argument to the script.
CURRENT_NODE_NAME=""

# Associative array of the number of cores on each node, keyed by node name.
declare -gA NODE_CORE_COUNT

# Associative array of the amount of memory (in GB) on each node, keyed by node name.
# Values can be decimal values.
declare -gA NODE_TOTAL_MEM

# Associative array of the maximum size of the SHM segment (in GB) on each node, 
# keyed by node name. Values can be decimal values.
declare -gA NODE_TOTAL_SHM

# Associative array of the current size of the SHM segment (in GB) that is 
# available on each node, keyed by node name.
declare -gA NODE_FREE_SHM


###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Prints the acceptable arguments to the script and then exits.
## Params: None

function printAddNodeUsageAndExit() {
  echo ""
  echo "Usage: ./${THIS_FILE} [--interactive | --script]"
  echo "                    [--config <file>]"
  echo "                    [--basic | --adv | --advanced]"
  echo "                    [--debug]"
  echo "Details:"
  echo "  --interactive   Uses a wizard to ask questions and prompt for answers"
  echo "                  in order to build a cluster configuration."
  echo "  --script        Does not prompt the user for input and uses the supplied"
  echo "                  configuration. The --config option must be specified."
  echo ""
  echo "  --config <file> Specifies the path to an existing configuration"
  echo "                  file that contains the cluster settings."
  echo ""
  echo "  --basic         [Default] Hides advanced questions during the cluster"
  echo "                  installation wizard (if being used)."
  echo "  --adv"
  echo "  --advanced      Asks additional questions during the cluster"
  echo "                  installation wizard."
  echo ""
  echo "  --installer     Path to the installation *.run file."
  echo ""
  echo "  --debug         Displays more messages during execution."
  echo ""
  echo "  --help          Displays this message."
  exit 1
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Inspect all of the arguments, checking for incompatibilities, and set
## the appropriate global variables
## Params: the arguments to the script

function processAddNodeArgs() {
  # Local variables have value 1 if that argument has been specified.
#  local argCluster=0
#  local argNode=0
  local argInteractive=0
  local argScript=0
  local argConfig=0
  local argBasic=0
  local argAdvanced=0
  local argInstaller=0

  while [[ $# -gt 0 ]]
  do
    key="$1"
    case "${key}" in
  
      --interactive)
        INTERACTIVE_INSTALL=1
        argInteractive=1
        shift # past argument
        ;;
  
      --script)
        INTERACTIVE_INSTALL=0
        argScript=1
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
  
      --basic)
        ADV_WIZARD=0
        argBasic=1
        shift # past argument
        ;;
  
      --adv|--advanced)
        ADV_WIZARD=1
        argAdvanced=1
        shift # past argument
        ;;

      --installer)
        argInstaller=1
        INSTALL_FILE="$2"
        shift # past argument
        shift # past value
        ;;
  
      --installer=*)
        argInstaller=1
        INSTALL_FILE="${key#*=}"
        shift # past argument=value
        ;;

      --debug)
        DEBUG=1
        shift # past argument
        ;;

      --help|-h)
        shift
        printAddNodeUsageAndExit
        ;;

      *)
        shift
        printAddNodeUsageAndExit
        ;;
    esac
  done


  # Now we can check for invalid combinations of arguments.
    # --interactive and --script
    # --basic       and --script
    # --adv         and --script
    # --config      and no file specified
    # --config      and file not found
    # --installer   and no file specified
    # --installer   and file not found

  local errmsg="Invalid combination of arguments specified: "

  if [[ ${argInteractive} -eq 1 && ${argScript} -eq 1 ]]; then 
    printError "${errmsg}" "--interactive" "--script"
    printAddNodeUsageAndExit
  fi

  if [[ ${argBasic} -eq 1    && ${INTERACTIVE_INSTALL} -eq 0 ]]; then 
    printError "${errmsg}" "--basic" "--script" 
    printAddNodeUsageAndExit
  fi

  if [[ ${argAdvanced} -eq 1 && ${INTERACTIVE_INSTALL} -eq 0 ]]; then 
    printError "${errmsg}" "--advanced" "--script"
    printAddNodeUsageAndExit
  fi

  if [[ ${argConfig} -eq 1 && -z "${CONFIG_FILE}" ]]; then 
    printError "${errmsg}" "No config file specified" 
    printAddNodeUsageAndExit
  fi

  if [[ ${argConfig} -eq 1 && ! -f "${CONFIG_FILE}" ]]; then 
    printError "Config file does not exist: ${CONFIG_FILE}"
    printAddNodeUsageAndExit 
  fi

  if [[ ${argInstaller} -eq 1 && -z "${INSTALL_FILE}" ]]; then 
    printError "${errmsg}" "No installation *.run file specified" 
    printAddNodeUsageAndExit
  fi

  if [[ ${argInstaller} -eq 1 && ! -f "${INSTALL_FILE}" ]]; then 
    printError "Installation *.run file does not exist: ${INSTALL_FILE}"
    printAddNodeUsageAndExit 
  fi



  # Check for dependencies between combinations of arguments.
    # --script must have --config

  if [[ ${argScript} -eq 1 && ${argConfig} -eq 0  ]]; then 
    printError "Missing argument dependency: argument --script must also specify argument --config"
    printAddNodeUsageAndExit
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


###############################################################################
### Script                                                                  ###
###############################################################################

# The main script starts here.


# Process the arguments given to the script.
processAddNodeArgs "$@"

# Print out current settings from the arguments
debugPrint ""
debugPrint "pwd = $(pwd)"
debugPrint "USER_PWD = ${USER_PWD}"
debugPrint "CLUSTER_INSTALL = ${CLUSTER_INSTALL}"
debugPrint "INTERACTIVE_INSTALL = ${INTERACTIVE_INSTALL}"
debugPrint "ADV_WIZARD = ${ADV_WIZARD}"
debugPrint "DEBUG = ${DEBUG}"
debugPrint "INSTALL_FILE = ${INSTALL_FILE}"
debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint


# Load the config file

# Find the installation file and check that it's version matches the current version
if [ -z "${INSTALL_FILE}" ]; then
  findInstallFile

else
  # Check that this file matches the current version

  [ -f "${DIR:+${DIR}/}${VERSION_FILE}" ] || errorExit "Could not find the file ${VERSION_FILE} in the installation directory."
  #shellcheck source=common.fn
  currentVer="$(source "${DIR:+${DIR}/}${VERSION_FILE}" && echo "${SE_VERSION_STR}")"
  [ -n "${currentVer}" ] || errorExit "Could not extract out the current RapidsSE version number."

  checkInstallFileVersion "${INSTALL_FILE}" "${currentVer}" || errorExit "RapidsSE installation file version does not match the currently installed version (${currentVer})."
fi

# Check if the cluster is running
isClusterRunning && CLUSTER_IS_RUNNING=1


# Prompt for details on new node (host, port, directory, etc)
# Validate new config
# Backup existing config
# Install new nodes
# Write out new config to all nodes
# If cluster is running
#   Start new nodes, wait for them to be ready
#   Wait for nodes to join the cluster
#   Rebalance node data
# end
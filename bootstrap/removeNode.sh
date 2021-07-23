#!/bin/bash

# This file is used to dynamically remove one or more nodes from a cluster.
# It will inactivate the nodes, stop them (if necessary) and remove them from
# the cluster.
# This can be called whether the cluster is running or stopped.

###############################################################################
### Environment Variables                                                   ###
###############################################################################



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
readonly ROOT_DIR="${DIR}"
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
NAME_FILE="${DIR}/${DEFAULT_NAME_FILE}"

# Path to the config file used, if any.
CONFIG_FILE=""

# Has value 1 if any nodes in the cluster are running, otherwise 0.
CLUSTER_IS_RUNNING=0

# The array of unique node names. The values here are used to help form the
# name of the dynamic associative arrays used to hold the configuration of
# an individual node.
declare -ga NODE_LIST=()

# The array of node names before the change, including the nodes being removed
declare -ga ORIGINAL_NODE_LIST=()

# The array of node names of nodes that are being removed
declare -ga REMOVED_NODE_LIST=()

# The prefix given to each node name before its sequence number.
NODE_NAME_PREFIX="${NODE_CFG_DEFAULT_NAME_PREFIX}"

# The unique number used with the above NODE_NAME_PREFIX to form the node name.
NODE_SEQ_NUM=0

# The name of the current node, given in the --name argument to the script.
CURRENT_NODE_NAME=""


###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Prints the acceptable arguments to the script and then exits.
## Params: None

function printRemoveNodeUsageAndExit() {
  echo ""
  echo "Usage: ./${THIS_FILE} [--interactive | --script]"
  echo "                       [--config <file>]"
  echo "                       [--basic | --adv | --advanced]"
  echo "                       [--debug]"
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
  local argInteractive=0
  local argScript=0
  local argConfig=0
  local argBasic=0
  local argAdvanced=0

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

      --debug)
        DEBUG=1
        shift # past argument
        ;;

      --help|-h)
        shift
        printRemoveNodeUsageAndExit
        ;;

      *)
        shift
        printRemoveNodeUsageAndExit
        ;;
    esac
  done


  # Now we can check for invalid combinations of arguments.
    # --interactive and --script
    # --basic       and --script
    # --adv         and --script
    # --config      and no file specified
    # --config      and file not found

  local errmsg="Invalid combination of arguments specified: "

  if [[ ${argInteractive} -eq 1 && ${argScript} -eq 1 ]]; then 
    printError "${errmsg}" "--interactive" "--script"
    printRemoveNodeUsageAndExit
  fi

  if [[ ${argBasic} -eq 1    && ${INTERACTIVE_INSTALL} -eq 0 ]]; then 
    printError "${errmsg}" "--basic" "--script" 
    printRemoveNodeUsageAndExit
  fi

  if [[ ${argAdvanced} -eq 1 && ${INTERACTIVE_INSTALL} -eq 0 ]]; then 
    printError "${errmsg}" "--advanced" "--script"
    printRemoveNodeUsageAndExit
  fi

  if [[ ${argConfig} -eq 1 && -z "${CONFIG_FILE}" ]]; then 
    printError "${errmsg}" "No config file specified" 
    printRemoveNodeUsageAndExit
  fi

  if [[ ${argConfig} -eq 1 && ! -f "${CONFIG_FILE}" ]]; then 
    printError "Config file does not exist: ${CONFIG_FILE}"
    printRemoveNodeUsageAndExit 
  fi


  # Check for dependencies between combinations of arguments.
    # --script must have --config

  if [[ ${argScript} -eq 1 && ${argConfig} -eq 0  ]]; then 
    printError "Missing argument dependency: argument --script must also specify argument --config"
    printRemoveNodeUsageAndExit
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Welcomes the user to the interactive addNode wizard.
## Params: None

function displayRemoveNodeWelcome() {
  clear
  #shellcheck disable=1090
  source "${VERSION_FILE}"

  echo "Welcome to the Remove Node wizard for RapidsSE version ${SE_VERSION_STR}."
  echo "(git ${SE_COMMIT_HASH})"
  echo ""
  echo "This will guide you through the process of removing one or more nodes from"
  echo "a RapidsSE cluster."
  echo ""
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Displays a list of node names, host and installation directories.
## Params:
##  $@ - the list of node names

function displayNodes() {

  [ "$#" -gt 0 ] || errorExit "Function displayNodes() cannot have 0 arguments."

  local -a nodeNames=("$@")

  echo ""

  printf "  %-10s  %s:%s\n" "Node Name" "Hostname" "Installation_Path"
  printf "  %-10s  %s\n\n"  "---------" "---------------------------------------------------"


  for nodeName in "${nodeNames[@]}"; do

    local host
    local dir

    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"
    dir="$(nodeCfgGetValue  "${nodeName}" "${NODE_CFG_DIR_KEY}")"

    # Assertions
    [ "${host:-x}" != x ] || errorExit "Could not lookup the hostname for node named ${nodeName}"
    [ "${dir:-x}"  != x ] || errorExit "Could not lookup the installation directory for node named ${nodeName}"

    printf "  %-10s  %s:%s\n" "${nodeName}" "${host}" "${dir}"
  done
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks if the given node name is in the REMOVED_NODES_LIST. If so, an error
## message is written out and a non-zero exit status is returned. If not, a zero
## exit status is returned.
## Params:
##  $1 - the node name in question

function validateNotInRemoveList() {
  [ "$#" -eq 1 ] || errorExit "Function validateNotInRemoveList() must be called with 1 parameter"
  
  local nodeName="$1"
  
  for removeNode in "${REMOVED_NODE_LIST[@]}"; do
    # Case insensitive match
    if [ "${removeNode,,}" = "${nodeName,,}" ]; then
      echo "Cannot remove node ${nodeName} as it is already set to be removed."
      echo ""
      return 1
    fi
  done
  
  # All good
  return 0
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Returns the array key associated in NODE_LIST associated with a given node name.
## If the node name cannot be found then -1 is returned on stdout.
## Should be called like this:
##   val="$(findNodeIndex "${nodeName}")"
## Params:
##  $1 - the node name in question

function findNodeIndex() {
  [ "$#" -eq 1 ] || errorExit "Function findNodeIndex() must be called with 1 parameter"

  local searchName="$1"

  for i in "${!NODE_LIST[@]}"; do
    local nodeName
    nodeName="${NODE_LIST[$i]}"

    # Case insensitive match
    if [ "${searchName,,}" = "${nodeName,,}" ]; then
      echo "$i"
      return
    fi
  done

  # Not found
  echo "-1"
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Alters the in-memory cluster configuration interactively by prompting the user.
## Params: None

function alterConfiguration() {

  # (advanced) node name prefix
  [ -z "${NODE_NAME_PREFIX}" ] && errorExit "NODE_NAME_PREFIX is not set"
  [ -z "${NODE_SEQ_NUM:-}" ]   && errorExit "NODE_SEQ_NUM is not set"
  [ "${NODE_SEQ_NUM}" -le 0 ]  && errorExit "NODE_SEQ_NUM must be a positive value"

  # Record a copy of the original node name list
  ORIGINAL_NODE_LIST=("${NODE_LIST[@]}")

  # Display header comments
  echo ""
  echo "Existing Nodes"
  echo "--------------"
  echo ""
  echo "Let's choose which nodes are to be removed from the RapidsSE cluster."

  local isFirst=1
  local doDisplayNodes=1

  while true; do

    # If we are down to 1 node left then we cannot remove it!
    if [ "${#NODE_LIST[@]}" -eq 1 ]; then
      echo "No more nodes can be removed from the cluster."
      break
    fi

    # display list of nodes
    if [ "${doDisplayNodes}" -eq 1 ]; then
      displayNodes "${NODE_LIST[@]}"
      echo ""
    fi

    doDisplayNodes=1

    # Capture the user input
    if [ "${isFirst}" -eq 1 ]; then
      echo -n "Enter the name of a node to be removed and press return : "
    else
      echo -n "Enter another node name, or enter an empty line to finish : "
    fi

    IFS=$'\n' read -r line

    if [[ "${line}" = *[[:space:]]* ]]; then
      echo "Invalid input: node name cannot contain whitespace."
      echo ""
      doDisplayNodes=0
      continue
    fi

    if [ -z "${line:-}" ]; then

      if [ "${isFirst}" -eq 1 ]; then
        echo "Invalid input: must specify a first node name."
        echo ""
        doDisplayNodes=0
        continue

      else
        # Empty line = exit
        break
      fi
    fi

    # Validate the user input
    
    # Validate: cannot remove a node more than once from the grid.
    if ! validateNotInRemoveList "${line}"; then
      continue
    fi

    # Validate/lookup node name
    local nodeIndex
    nodeIndex="$(findNodeIndex "${line}")"
    if [ "${nodeIndex}" -eq -1 ]; then
      echo "Invalid node name entered: ${line}"
      echo ""
      doDisplayNodes=0
      continue
    fi

    # Validate: cannot remove all remaining nodes from the grid.
    if [ "${#NODE_LIST[@]}" -eq 1 ]; then
      echo "Cannot remove the last node in the RapidsSE cluster."
      echo ""
      doDisplayNodes=0
      continue
    fi

    # Add node name to the removed nodes list.
    # Use the one from NODE_LIST so we get case sensitivity right.
    node="${NODE_LIST[$nodeIndex]}"
    REMOVED_NODE_LIST+=( "${node}" )

    # Remove node name from list
    unset 'NODE_LIST[nodeIndex]'

    isFirst=0
    echo "${node} will be removed from the cluster."
  done


  # Assertions
  [ "${#REMOVED_NODE_LIST[@]}" -gt 0 ] || errorExit "Line ${LINENO}: Removed node list must not be empty"
  [ "${#ORIGINAL_NODE_LIST[@]}" -gt "${#NODE_LIST[@]}" ] || errorExit "Line ${LINENO}: Original node list must be > current node list"
  [ "${#NODE_LIST[@]}" -gt 0 ] || errorExit "Line ${LINENO}: Node list must not be empty"

  # At this point ${NODE_LIST[@]} will represent the list of remaining nodes in the cluster,
  # while ${REMOVED_NODE_LIST[@]} will represent the nodes to be removed and
  # ${ORIGINAL_NODE_LIST[@]} represents the node list that we started with.

}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Asks the user to confirm that they wish to proceed with the node removal.
## Params: None

function confirmNodeRemoval() {
  echo ""
  echo ""
  echo "Node Removal"
  echo "------------"
  echo ""
  if [ "${#REMOVED_NODE_LIST[@]}" -eq 1 ]; then
    echo "RapidsSE is now ready to remove ${#REMOVED_NODE_LIST[@]} node from the cluster."
  else
    echo "RapidsSE is now ready to remove ${#REMOVED_NODE_LIST[@]} nodes from the cluster."
  fi

  if [ "${#NODE_LIST[@]}" -eq 1 ]; then
    echo "The cluster will have "${#NODE_LIST[@]}" node remaining."
  else
    echo "The cluster will have "${#NODE_LIST[@]}" nodes remaining."
  fi

  echo ""
  echo "The following nodes will be removed:"
  displayNodes "${REMOVED_NODE_LIST[@]}"

  local line

  while true
  do
    echo ""
    echo -n "Do you wish to proceed with the node removal? (y/n) [default: y] : "
    IFS=$'\n' read -r line

    if [[ -z "${line}" || "${line}" = "y" || "${line}" = "Y" || "${line}" = "yes" || "${line}" = "YES" ]]; then
      break
    fi

    if [[ "${line}" = "n" || "${line}" = "N" || "${line}" = "no" || "${line}" = "NO" ]]; then
      echo "Aborting node removal."
      exit 1
    fi

    echo "Invalid input: y/n expected."
  done
}


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
debugPrint "CLUSTER_INSTALL = ${CLUSTER_INSTALL}"
debugPrint "INTERACTIVE_INSTALL = ${INTERACTIVE_INSTALL}"
debugPrint "ADV_WIZARD = ${ADV_WIZARD}"
debugPrint "DEBUG = ${DEBUG}"
debugPrint "CONFIG_FILE = ${CONFIG_FILE}"
debugPrint

# Use the default config file path if the config file was not specified
[ -n "${CONFIG_FILE}" ] || CONFIG_FILE="${DIR:+${DIR}/}${DEFAULT_CONFIG_PATH}"

# Load the config file and node name
loadClusterConfig
loadNodeName

# Check if the cluster is running
isClusterRunning && CLUSTER_IS_RUNNING=1

if [ "${#NODE_LIST[@]}" -eq 1 ]; then
  errorExit "Cannot remove nodes when the cluster only has 1 node."
fi

# Is this an interactive or scripted install?
if [ ${INTERACTIVE_INSTALL} -eq 1 ]; then
  displayRemoveNodeWelcome

  # Alter the in-memory configuration by prompting the user
  # for details on nodes being removed.
  alterConfiguration

  # Confirm config
  confirmNodeRemoval

  # Backup existing configs on all existing nodes in the cluster
  backupSuffix="$(date +%Y%m%dT%H%M%S)"
  backupConfig "${backupSuffix}" "${ORIGINAL_NODE_LIST[@]}"

  if [ "${CLUSTER_IS_RUNNING}" -eq 1 ]; then
    # TODO: Issue __forgetNode command

    # TODO: Wait until the status prompt no longer shows rows for the nodes being removed.

    # TODO: kill the nodes

    true
  fi

  # Write out new config on the current node
  writeClusterConfig "${CONFIG_FILE}"

  # Write out new config to all existing nodes
  distributeFile "${CONFIG_FILE}" "${DEFAULT_CONFIG_PATH}" "${NODE_LIST[@]}"

  # Remove the cluster.config on the removed nodes (so they cannot easily be started up)
  removeConfig "" "${REMOVED_NODE_LIST[@]}"

  # Indicate that SE installation directories can be cleaned up
  echo ""
  echo "The installation directories of the removed nodes can be deleted, if no longer needed:"
  for nodeName in "${REMOVED_NODE_LIST[@]}"; do
    
    hostname="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"
    installDir="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_DIR_KEY}")"

    printf "  %s:%s\n" "${hostname}" "${installDir}"
  done

  echo ""
  if [ ${#REMOVED_NODE_LIST[@]} -eq 1 ]; then
    echo "The removal of ${#REMOVED_NODE_LIST[@]} node from RapidsSE is complete!"
  else
    echo "The removal of ${#REMOVED_NODE_LIST[@]} nodes from RapidsSE is complete!"
  fi


else
  # TODO: Scripted removal
  errorExit "Scripted node removal is not yet supported"
fi


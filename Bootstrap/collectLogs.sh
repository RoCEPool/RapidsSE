#!/bin/bash

###############################################################################
### Constants                                                               ###
###############################################################################

# Get the relative path to this file.
readonly DIR=$(dirname "$0")
readonly COMMON_FN="${DIR}/common.fn"
readonly SCRIPT_NAME=$(basename "$0")

###############################################################################
### Significant Global Variables                                            ###
###############################################################################

# Default modes of operation
CONFIG_FILE="${DIR}/cluster.config"

DEBUG=0

###############################################################################
### Includes                                                                ###
###############################################################################

# Load bootstrapping functions
#shellcheck source=common.fn
source "${COMMON_FN}" || { >&2 echo "Could not load ${COMMON_FN}"; exit 1; }


###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Prints out the valid arguments to the script and exits.
## Params: None

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
    printError "No config file specified"
    printUsageAndExit
  fi

  # Check that the config file exists
  if [[ ! -f "${CONFIG_FILE}" ]]; then
    printError "Config file does not exist: ${CONFIG_FILE}"
    printUsageAndExit
  fi

}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Collect all of the logs from each node and tar them up.
## Params: 

function collectLogs() {

  local timestamp
  timestamp="$(date +%Y%m%dT%H%M%S)"

  # Make the output filename  
  local outputFile="logs-${timestamp}.tar.gz"

  # Make a temporary directory in which to collect all of the copied logs.
  local tempDir
  tempDir="$(mktemp -d)"
  
  debugPrint "Collecting logfiles to path: ${tempDir}"

  # Make a working directory. We will tar this one up so when it is extracted
  # all of the logs go into a nicely named directory.
  local workingDir="${tempDir}/logs-${timestamp}"
  mkdir -p "${workingDir}"
  
  
  # Copy in the cluster.config file
  cp "${CONFIG_FILE}" "${workingDir}"
  
  local nodeName
  local remoteCmd

  remoteCmd="'"
  remoteCmd+="echo -e \"Machine Info:\n\"; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: hostname -f\"; "
  remoteCmd+="hostname -f; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Contents of: /etc/redhat-release\"; "
  remoteCmd+="cat /etc/redhat-release; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: uname -a\"; "
  remoteCmd+="uname -a; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: lscpu\"; "
  remoteCmd+="lscpu; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: vmstat -s -S M -w\"; "
  remoteCmd+="vmstat -s -S M -w; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Contents of: /proc/meminfo\"; "
  remoteCmd+="cat /proc/meminfo; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: lsblk\"; "
  remoteCmd+="lsblk; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: df -h\"; "
  remoteCmd+="df -h; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: top -b -n 1\"; "
  remoteCmd+="top -b -n 1; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: ifconfig\"; "
  remoteCmd+="ifconfig || /usr/sbin/ifconfig; "
  remoteCmd+="echo -e \"\n*******************************************************\n\"; "
  remoteCmd+="echo \"Output of: netstat -antp\"; "
  remoteCmd+="netstat -antp; "
  remoteCmd+="'"

  # Copy logs for each node
  for nodeName in "${NODE_LIST[@]}"
  do
    local host
    local installDir
    
    host="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_HOST_KEY}")"
    installDir="$(nodeCfgGetValue "${nodeName}" "${NODE_CFG_DIR_KEY}")"

    # Create a subdirectory of the output dir for each node
    local outputDir="${workingDir}/${nodeName}"  
    mkdir -p "${outputDir}"

    # Copy some files
    echo "Copying logs from ${nodeName} [host ${host}]"
    scp    "${host}:${installDir}/se.log"         "${outputDir}" &>/dev/null || true
    scp    "${host}:${installDir}/version"        "${outputDir}" &>/dev/null || true
    scp    "${host}:${installDir}/cluster.config" "${outputDir}" &>/dev/null || true
    scp -r "${host}:${installDir}/tmp"            "${outputDir}" &>/dev/null || true
    ssh    "${host}" bash -c "${remoteCmd}"    &> "${outputDir}/machine.txt" || true
  done

  # Tar up the collected log files.
  
  tar cvzf "${outputFile}" -C "${tempDir}" . 1>/dev/null

  # Remove the temporary directory and all contents
  rm -rf "${tempDir:?}" || true
  debugPrint "Cleaned up temporary logfile path: ${tempDir}"
  
  echo ""
  echo "Copied all logs to the file: ${outputFile}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

# Collect all of the logs and tar them up
collectLogs


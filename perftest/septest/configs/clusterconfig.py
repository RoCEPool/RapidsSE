import string
import decimal
import subprocess
import logging
import json
import os
import stat
import pathlib

from .configurationerror import ConfigurationError

LOG = logging.getLogger(__name__)


class ClusterConfig:
    """Encapsulates a cluster config file and hides how it is implemented and parsed."""

    C_PARSER_FILE    = "clusterConfigParser.sh"
    C_PARSER_PATH    = str((pathlib.Path(__file__).parent / C_PARSER_FILE).resolve())
    C_COMMON_FN_PATH = "../bootstrap/common.fn"
    C_SHM            = "shm"
    C_THREADS        = "threads"
    C_PARTITIONS     = "partitions"
    C_PARAMS         = "params"

    def __init__(self, path: string):
        self.path = path
        self.shm = -1
        self.threads = -1
        self.partitions = -1
        self.params = ""

        # Make the clusterConfigParser.sh file executable
        file_mode = os.stat(self.C_PARSER_PATH).st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH
        os.chmod(self.C_PARSER_PATH, mode=file_mode)

        cmd = [self.C_PARSER_PATH, self.C_COMMON_FN_PATH, self.path]

        try:
            status: subprocess.CompletedProcess = subprocess.run(cmd,
                                                                 shell=False,
                                                                 capture_output=True,
                                                                 check=True,
                                                                 text=True)

        except subprocess.CalledProcessError as e:
            msg = f"Could not parse the cluster config file at: {self.path}\n" \
                  f"Got the following stderr output:\n  {e.stderr}"
            raise ConfigurationError(msg)

        status.check_returncode()

        if len(status.stderr) > 0:
            raise ConfigurationError(f"Error encountered while parsing cluster config file: {path}\n{status.stderr}")

        if len(status.stdout) == 0:
            raise ConfigurationError(f"Parsing of cluster config file resulted in no output: {path}")

        output = json.loads(status.stdout, strict=False)
        self.shm        = output[self.C_SHM]
        self.threads    = output[self.C_THREADS]
        self.partitions = output[self.C_PARTITIONS]
        self.params     = output[self.C_PARAMS]

    def __str__(self):
        s = f"<ClusterConfig>\n" \
            f"  shared mem = {self.get_shared_mem()}\n" \
            f"  threads    = {self.get_threads()}\n" \
            f"  partitions = {self.get_partitions()}\n" \
            f"  params     = \"{self.get_params()}\""
        return s

    def get_shared_mem(self) -> decimal:
        return self.shm

    def get_threads(self) -> int:
        return self.threads

    def get_partitions(self) -> int:
        return self.partitions

    def get_params(self) -> string:
        return self.params

    @staticmethod
    def __get_parse_script() -> string:
        return r'''
#!/usr/bin/env bash

# Description:
# ------------
#   Writes to stdout a simple JSON object with key parameters of the first
#   node of the cluster config file. For example:
#
#     {
#       "shm": 32,
#       "threads": 16,
#       "partitions": 8,
#       "params": ""
#     }
#
#  This can then be easily parsed by Python as JSON output
# 
# Arguments:
# ----------
#   $1: The path to the common.fn file which is used to load and read the
#       cluster configuration file.
#   $2: The path to the cluster config file to be read.
#

# Check for the minimum version of bash
if [ "${BASH_VERSINFO[0]}" -lt 4 ]; then
  >&2 echo "ERROR: The version of bash must be >= 4. Currently running ${BASH_VERSION}"
  exit 1
fi

# Get the path to common.fn and the cluster config file
# from the arguments

if [ "$#" -ne 2 ]; then
  &>2 echo "ERROR: Expected 2 arguments to be passed to this script. Got $# instead."
  exit 1
fi

COMMON_FN="$1"
CONFIG_FILE="$2"

if [ ! -f "${COMMON_FN}" ]; then
  >&2 echo "ERROR: File not found: ${COMMON_FN}" && exit 1
fi

source "${COMMON_FN}"

loadClusterConfig

# Get the name of the first node
FIRST_NODE_NAME="${NODE_LIST[0]}"

[ -n "${FIRST_NODE_NAME}" ] || errorExit "Could not get the name of the first node in the cluster"

shm="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_MEM_KEY}")"
threads="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_THREADS_KEY}")"
partitions="$(nodeCfgGetValue "${FIRST_NODE_NAME}" "${NODE_CFG_PARTITIONS_KEY}")"

# Write out the results in a JSON format for easy parsing by Python
cat << EOF
{
  "shm": ${shm},
  "threads": ${threads},
  "partitions": ${partitions},
  "params": "${params}"
}
EOF
        '''.strip()

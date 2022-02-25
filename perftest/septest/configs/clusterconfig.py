import string
import decimal
import subprocess
import logging
import json
import os
import stat
import pathlib

from septest.errors import ConfigurationError

LOG = logging.getLogger(__name__)


class ClusterConfig:
    """Encapsulates a cluster config file and hides how it is implemented and parsed."""

    C_PARSER_FILE    = "clusterConfigParser.sh"
    C_PARSER_PATH    = str((pathlib.Path(__file__).parent / C_PARSER_FILE).resolve())
    C_COMMON_FN_PATH = "../bootstrap/common.fn"
    C_NUM_NODES      = "numNodes"
    C_SHM            = "shm"
    C_THREADS        = "threads"
    C_PARTITIONS     = "partitions"
    C_DIR            = "dir"
    C_PORT           = "port"
    C_PARAMS         = "params"

    def __init__(self, path: string):
        self.path = path
        self.num_nodes = -1
        self.shm = -1
        self.threads = -1
        self.partitions = -1
        self.dir = ""
        self.port = -1
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
        self.num_nodes  = output[self.C_NUM_NODES]
        self.shm        = output[self.C_SHM]
        self.threads    = output[self.C_THREADS]
        self.partitions = output[self.C_PARTITIONS]
        self.dir        = output[self.C_DIR]
        self.port       = output[self.C_PORT]
        self.params     = output[self.C_PARAMS]

    def __str__(self):
        s = f"<ClusterConfig>\n" \
            f"  num nodes  = {self.get_num_nodes()}\n" \
            f"  shared mem = {self.get_shared_mem()}\n" \
            f"  threads    = {self.get_threads()}\n" \
            f"  partitions = {self.get_partitions()}\n" \
            f"  dir        = {self.get_dir()}\n" \
            f"  port       = {self.get_port()}\n" \
            f"  params     = \"{self.get_params()}\""
        return s

    def get_cluster_name(self) -> string:
        """Returns the name of the cluster file without the last extension"""
        return os.path.basename(self.path).rsplit(sep=".", maxsplit=1)[0]

    def get_num_nodes(self) -> int:
        return self.num_nodes

    def get_shared_mem(self) -> decimal:
        return self.shm

    def get_threads(self) -> int:
        return self.threads

    def get_partitions(self) -> int:
        return self.partitions

    def get_dir(self) -> string:
        return self.dir

    def get_port(self) -> int:
        return self.port

    def get_params(self) -> string:
        return self.params

""""Runs the build job"""
import string
import logging
import pathlib

import common.ec2 as ec2
import common.errors as errors
from common.releaseconfig import ReleaseConfig

LOG = logging.getLogger(__name__)

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_EC2_BOOT_TIMEOUT_SEC = 300


class BuildRunner:

    def __init__(self,
                 result_path: pathlib.Path,
                 artifact_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 config: ReleaseConfig):

        self.result_path = result_path
        self.artifact_path = artifact_path
        self.repo_path = repo_path
        self.config = config

    def run(self):
        """
        Runs the logic that builds SE on a remote node with a particular image.
        """
        pass

import string
import logging

from .jobconfig import JobConfig
from .hardwareconfig import HardwareConfig
from .clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)


class AllConfigs:
    """A simple class that encapsulates all configuration associated with a job"""

    def __init__(self, job_path: string):
        self.job_config = None
        self.hardware_config = None
        self.cluster_config = None

        # Read in all of the config files and save them into the above members
        self.__load(job_path)

    def get_job(self) -> JobConfig:
        return self.job_config

    def get_hardware(self) -> HardwareConfig:
        return self.hardware_config

    def get_cluster(self) -> ClusterConfig:
        return self.cluster_config

    def __load(self, job_path: string) -> None:
        self.job_config = JobConfig(job_path)
        self.hardware_config = HardwareConfig(self.job_config.get_resolved_hardware_config_path())
        self.cluster_config = ClusterConfig(self.job_config.get_resolved_cluster_config_path())

        LOG.debug("Using the following job config:\n%s", str(self.job_config))
        LOG.debug("Using the following hardware config:\n%s", str(self.hardware_config))
        LOG.debug("Using the following cluster config:\n%s", str(self.cluster_config))


""""Runs a job"""
import string
import logging
from configs.jobconfig import JobConfig
from configs.hardwareconfig import HardwareConfig
from configs.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)


def run_job(job_path: string, bamboo_build_str: string, git_hash: string, tags: dict):
    # Parse the job file
    job_config = JobConfig(job_path)
    LOG.debug("Using the following job config:\n%s", str(job_config))

    # Get the path to the hardware config and parse that too
    hardware_config_path = job_config.get_resolved_hardware_config_path()
    hw_config = HardwareConfig(hardware_config_path)
    LOG.debug("Using the following hardware config:\n%s", str(hw_config))

    # Get the paths to the cluster config and the list of test files
    cluster_config_path = job_config.get_resolved_cluster_config_path()
    test_paths = job_config.get_resolved_test_paths()

    cluster_config = ClusterConfig(cluster_config_path)
    LOG.debug("Using the following cluster config:\n%s", str(cluster_config))

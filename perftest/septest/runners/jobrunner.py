""""Runs a job"""
import string
import logging
from configs.jobconfig import JobConfig

LOG = logging.getLogger(__name__)


def run_job(job_path: string, bamboo_build_str: string, git_hash: string, tags: dict):
    job_config = JobConfig(job_path)

    # global LOG
    LOG.debug(str(job_config))
    # logging.getLogger(__name__).debug("Using the following job config:\n%s", str(job_config))

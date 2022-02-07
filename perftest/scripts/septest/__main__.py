import sys
import os
import pathlib
import logging
import logging.config
import os.path
import septest.runners.jobrunner as jobrunner

C_REL_REPO_ROOT = "../.."
C_REPO_ROOT = (pathlib.Path.cwd() / C_REL_REPO_ROOT).resolve()
C_CWD_SUFFIX = "perftest/scripts"

C_LOG_CONFIG_FILE = "logging.ini"
LOG = None

def main() -> int:
    """A simple main entry point for executing the SE Performance Test script."""
    """Run with pipenv run python -m septest"""
    """from the outer septest directory."""

    # Set up the logger configuration
    if os.path.exists(C_LOG_CONFIG_FILE):
        logging.config.fileConfig(C_LOG_CONFIG_FILE)

    # Create a main logger
    global LOG
    LOG = logging.getLogger(__name__)

    job_path = ""
    bamboo_build_str = ""
    tags = {}

    LOG.debug(f"cwd = {os.getcwd()}")
    LOG.debug(f"repo root = {C_REPO_ROOT}")

    # Validate the path
    if not os.getcwd().endswith(C_CWD_SUFFIX):
        LOG.error(f"This file must be executed from the following subdirectory of the project root: {C_CWD_SUFFIX}")
        return 1

    # Validate the arguments

    # jobrunner.runJob(job_path, bamboo_build_str, tags)
    return 0


if __name__ == '__main__':
    sys.exit(main())

import string
import sys
import os
import pathlib
import logging
import logging.config
import os.path
import argparse
import subprocess

import configs.configurationerror
import septest.runners.jobrunner as jobrunner

C_PROG_RUN = "pipenv run python -m septest"
C_REL_REPO_ROOT = "../"
C_REPO_ROOT = (pathlib.Path.cwd() / C_REL_REPO_ROOT).resolve()
C_CWD_SUFFIX = "perftest"

# Create a main logger
LOG = logging.getLogger(__name__)


def main() -> int:
    """A simple main entry point for executing the SE Performance Test script."""
    """Run with pipenv run python -m septest"""
    """from the outer septest directory."""

    LOG.debug(f"cwd = {os.getcwd()}")
    LOG.debug(f"repo root = {C_REPO_ROOT}")

    # Validate the path
    if not os.getcwd().endswith(C_CWD_SUFFIX):
        LOG.error(f"This file must be executed from the following subdirectory of the project root: {C_CWD_SUFFIX}")
        return 1

    # Extract the command line arguments
    arg_parser = argparse.ArgumentParser(C_PROG_RUN, description="Run the SE Performance Test")
    arg_parser.add_argument("--job",   "-j",  type=non_empty_string, required=True)
    arg_parser.add_argument("--build", "-b",  type=non_empty_string, required=True)
    arg_parser.add_argument("--hash",         type=ascii, required=False, default='')
    arg_parser.add_argument("--tags",  "-t",  nargs='*', action=KeyValue)

    args = arg_parser.parse_args()
    LOG.debug("Got the following arguments:  %s", args)

    if not os.path.exists(args.job):
        LOG.error("Job file does not exist: %s", args.job)
        return 1

    job_path = args.job
    bamboo_build_str = args.build
    tags = args.tags
    git_hash = ""

    if len(git_hash) == 0:
        git_hash = get_git_hash_from_system()

    jobrunner.run_job(job_path, bamboo_build_str, git_hash, tags)
    return 0


def non_empty_string(s: string) -> string:
    """Method for evaluating string as being non-empty in argument processing"""
    if len(s) == 0:
        raise ValueError("Cannot specify an empty string value")

    return s


# A key/value class for parsing command line arguments
# Modified from source: https://www.geeksforgeeks.org/python-key-value-pair-using-argparse/
class KeyValue(argparse.Action):
    """Class for building and extracting key/value pairs in argument processing"""

    # Constructor calling
    def __call__(self, parser, namespace, values, option_string=None):

        # Append to the output object if it already exists
        if getattr(namespace, self.dest) is None:
            setattr(namespace, self.dest, dict())

        for value in values:
            # Split it into key and value and cater for keys only
            if '=' in value:
                key, val = value.split('=')
            else:
                key = value
                val = None

            # print(f"Key = {key}, Value = {val}")
            # Assign into dictionary
            getattr(namespace, self.dest)[key] = val


def get_git_hash_from_system() -> string:
    """Returns the current git hash. Must be in a git repository."""
    global LOG
    command = 'git rev-parse --short=16 HEAD'.split(' ')

    try:
        completion_status: subprocess.CompletedProcess = \
            subprocess.run(command,
                           capture_output=True,
                           check=True,
                           text=True)

    except subprocess.CalledProcessError:
        LOG.error("Could not get the git hash")
        raise

    completion_status.check_returncode()
    return completion_status.stdout.strip()


if __name__ == '__main__':
    try:
        sys.exit(main())
    except configs.configurationerror.ConfigurationError as e:
        LOG.error(e)
        sys.exit(1)

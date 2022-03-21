import string
import sys
import os
import pathlib
import logging
import logging.config
import os.path
import argparse
import subprocess

import septest.errors as errors
from septest.runners.jobrunner import JobRunner

C_PROG_RUN = "pipenv run python -m septest"
C_REL_REPO_ROOT = "../"
C_REPO_ROOT = (pathlib.Path.cwd() / C_REL_REPO_ROOT).resolve()
C_CWD_SUFFIX = "perftest"
C_DEFAULT_RESULT_DIR = "./results"

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
    arg_parser.add_argument("--job",         type=non_empty_string, required=True,
                            help="Path to the job file to be executed.")
    arg_parser.add_argument("--planKey",     type=non_empty_string, required=True,
                            help="The name of the Bamboo that this run belongs to (e.g. SE-PERF).")
    arg_parser.add_argument("--buildNum",    type=int, required=True,
                            help="The Bamboo build number within the Bamboo plan that this run corresponds to.")
    arg_parser.add_argument("--hash",        type=str, required=False, default="",
                            help="The git hash of being tested.")
    arg_parser.add_argument("--branch",      type=str, required=False, default="",
                            help="The git branch being tested.")
    arg_parser.add_argument("--user",        type=str, required=False, default="SYSTEM",
                            help="The name of the user who manually invoked this run.")
    arg_parser.add_argument("--tags",  "-t", nargs='*', action=KeyValue,
                            help="A set of key/value pairs that are associated and saved with this run.")
    arg_parser.add_argument("--archiveMetrics", action='store_true', required=False, default=False,
                            help="If present, the raw metric files will always be collected and archived in Bamboo.")
    arg_parser.add_argument("--resultDir",   type=str, required=False, default=C_DEFAULT_RESULT_DIR,
                            help="The directory to write results and files to be archived")

    args = arg_parser.parse_args()
    LOG.debug("Got the following arguments:  %s", args)

    if not os.path.exists(args.job):
        LOG.error("Job file does not exist: %s", args.job)
        return 1

    if not os.path.exists(args.resultDir):
        LOG.error("resultDir path does not exist: %s", args.resultDir)
        return 1

    job_path = args.job
    bamboo_plan_key = args.planKey
    bamboo_build_num = args.buildNum
    tags = args.tags
    git_hash = args.hash
    git_branch = args.branch
    invoking_user = args.user
    archive_metrics = args.archiveMetrics
    result_path = args.resultDir

    if len(git_hash) == 0:
        git_hash = get_git_hash_from_system()

    if len(git_branch) == 0:
        git_branch = get_git_branch_from_system()

    jobrunner = JobRunner(job_path,
                          bamboo_plan_key,
                          bamboo_build_num,
                          git_hash,
                          git_branch,
                          invoking_user,
                          archive_metrics,
                          result_path,
                          tags)
    jobrunner.run_job()
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
    git_hash = completion_status.stdout.strip()
    LOG.debug(f"Got git hash from system: {git_hash}")
    return git_hash


def get_git_branch_from_system() -> string:
    command = 'git branch --show-current'.split(' ')

    try:
        completion_status: subprocess.CompletedProcess = \
            subprocess.run(command,
                           capture_output=True,
                           check=True,
                           text=True)

    except subprocess.CalledProcessError:
        LOG.error("Could not get the git branch")
        raise

    completion_status.check_returncode()
    git_branch = completion_status.stdout.strip()
    LOG.debug(f"Got git branch from system: {git_branch}")
    return git_branch


if __name__ == '__main__':
    try:
        sys.exit(main())

    except errors.NonTracebackError as e:
        LOG.error(e)
        sys.exit(1)

    except errors.SeptestError as e:
        LOG.error(e, exc_info=e.should_print_trace)
        sys.exit(1)

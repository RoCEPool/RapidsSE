import string
import sys
import os
import pathlib
import logging
import logging.config
import os.path
import argparse
import subprocess

import releasecommon.errors as errors

C_PROG_RUN = "pipenv run python -m buildrelease"
C_CWD_SUFFIX = "makerelease"

C_DEFAULT_RESULT_DIR = "./results"
C_DEFAULT_ARTIFACT_DIR = "./artifacts"

# Create a main logger
LOG = logging.getLogger(__name__)


def main() -> int:
    """A simple main entry point for executing the buildrelease test script."""
    """Run with pipenv run python -m septest"""
    """from the outer makerelease directory."""

    # Validate the path
    if not os.getcwd().endswith(C_CWD_SUFFIX):
        LOG.error(f"This file must be executed from the following subdirectory of the project root: {C_CWD_SUFFIX}")
        return 1

    # Extract the command line arguments
    arg_parser = argparse.ArgumentParser(C_PROG_RUN, description="Run the buildrelease job")
    arg_parser.add_argument("--resultDir",   type=str, required=False, default=C_DEFAULT_RESULT_DIR,
                            help="The directory to write results files")
    arg_parser.add_argument("--artifactDir", type=str, required=False, default=C_DEFAULT_ARTIFACT_DIR,
                            help="The directory to store artifacts to be archived")
    arg_parser.add_argument("--repoDir", type=str, required=True,
                            help="The root directory of the repository")
    arg_parser.add_argument("--config", type=str, required=True,
                            help="The path of the config file to use")

    args = arg_parser.parse_args()
    LOG.debug("Got the following arguments:  %s", args)

    if not os.path.exists(args.resultDir):
        LOG.error("Result directory does not exist: %s", args.resultDir)
        return 1

    if not os.path.isdir(args.resultDir):
        LOG.error("Result path is not a directory: %s", args.resultDir)
        return 1

    if not os.path.exists(args.artifactDir):
        LOG.error("Artifact directory does not exist: %s", args.artifactDir)
        return 1

    if not os.path.isdir(args.artifactDir):
        LOG.error("Artifact path is not a directory: %s", args.artifactDir)
        return 1

    if not os.path.exists(args.repoDir):
        LOG.error("Repository directory does not exist: %s", args.repoDir)
        return 1

    if not os.path.isdir(args.repoDir):
        LOG.error("Repository path is not a directory: %s", args.repoDir)
        return 1

    if not os.path.exists(args.config):
        LOG.error("Config file does not exist: %s", args.config)
        return 1

    if not os.path.isfile(args.config):
        LOG.error("Config file path is not a file: %s", args.config)
        return 1

    result_dir = args.resultDir
    if not pathlib.Path.is_absolute(result_dir):
        result_dir = (pathlib.Path.cwd() / result_dir).resolve()

    artifact_dir = args.artifactDir
    if not pathlib.Path.is_absolute(artifact_dir):
        artifact_dir = (pathlib.Path.cwd() / artifact_dir).resolve()

    repo_dir = args.repoDir
    if not pathlib.Path.is_absolute(repo_dir):
        repo_dir = (pathlib.Path.cwd() / repo_dir).resolve()

    config_path = args.config
    if not pathlib.Path.is_absolute(config_path):
        config_path = (pathlib.Path.cwd() / config_path).resolve()

    LOG.debug(f"cwd = {os.getcwd()}")
    LOG.debug(f"result dir = {result_dir}")
    LOG.debug(f"artifact dir = {artifact_dir}")
    LOG.debug(f"repo root = {repo_dir}")
    LOG.debug(f"config file = {config_path}")

    # jobrunner = JobRunner(job_path,
    #                       bamboo_plan_key,
    #                       bamboo_build_num,
    #                       git_hash,
    #                       git_branch,
    #                       invoking_user,
    #                       archive_metrics,
    #                       result_path,
    #                       tags,
    #                       not no_build)
    # jobrunner.run_job()
    return 0


if __name__ == '__main__':
    try:
        sys.exit(main())

    except errors.NonTracebackError as e:
        LOG.error(e)
        sys.exit(1)

    except errors.MakeReleaseError as e:
        LOG.error(e, exc_info=e.should_print_trace)
        sys.exit(1)

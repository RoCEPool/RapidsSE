import shutil
import string
import sys
import os
import pathlib
import logging
import logging.config
import os.path
import argparse
import subprocess

import common.errors as errors
from common.releaseconfig import ReleaseConfig
from buildrunner import BuildRunner

C_PROG_RUN = "pipenv run python -m buildrelease"
C_CWD_SUFFIX = "makerelease"

C_DEFAULT_RESULT_DIR = "./results"
C_DEFAULT_ARTIFACT_DIR = "./artifacts"

# Create a main logger
LOG = logging.getLogger(__name__)


def main() -> int:
    """A simple main entry point for executing the buildrelease test script.
       Run with: pipenv run python -m buildrelease
       from the outer makerelease directory.
       Expected arguments:
         --resultDir    path to write the results file
         --artifactDir  path to save any artifacts
         --repoDir      path to the root of the repository
         --config       path of the file that contains build configuration information
    """

    # Validate the path
    if not os.getcwd().endswith(C_CWD_SUFFIX):
        LOG.error(f"This file must be executed from the following subdirectory of the project root: {C_CWD_SUFFIX}")
        return 1

    # Validate the Python arguments
    (result_path, artifact_path, repo_path, config_path) = validate_arguments()

    LOG.debug(f"cwd          = {os.getcwd()}")
    LOG.debug(f"result dir   = {result_path}")
    LOG.debug(f"artifact dir = {artifact_path}")
    LOG.debug(f"repo root    = {repo_path}")
    LOG.debug(f"config file  = {config_path}")

    # Read in the config file and validate it
    config = ReleaseConfig(str(config_path))

    # Clone the repository.
    # We do this because bamboo uses a cached repository strategy and the current repo
    # cannot be copied to another node as it uses hard links for files to fixed paths
    # on the current node. So instead we will clone the cached repo in full to a temporary
    # folder and then we can copy that to other nodes and use it. We must remember to
    # clean up the temporary cloned repo before we exit.
    clone_repo_path = clone_repo(repo_path)

    try:
        buildrunner = BuildRunner(result_path, artifact_path, clone_repo_path, config)
        buildrunner.run()

        # testrunner = TestRunner(result_path, artifact_path, clone_repo_path, config)
        # testrunner.run()

    finally:
        # Clean up the cloned repository
        shutil.rmtree(clone_repo_path, ignore_errors=True)

    return 0


def validate_arguments() -> (pathlib.Path, pathlib.Path, pathlib.Path, pathlib.Path):
    # Extract the command line arguments
    arg_parser = argparse.ArgumentParser(C_PROG_RUN, description="Run the buildrelease job")
    arg_parser.add_argument("--resultDir", type=str, required=False, default=C_DEFAULT_RESULT_DIR,
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
        sys.exit(1)

    if not os.path.isdir(args.resultDir):
        LOG.error("Result path is not a directory: %s", args.resultDir)
        sys.exit(1)

    if not os.path.exists(args.artifactDir):
        LOG.error("Artifact directory does not exist: %s", args.artifactDir)
        sys.exit(1)

    if not os.path.isdir(args.artifactDir):
        LOG.error("Artifact path is not a directory: %s", args.artifactDir)
        sys.exit(1)

    if not os.path.exists(args.repoDir):
        LOG.error("Repository directory does not exist: %s", args.repoDir)
        sys.exit(1)

    if not os.path.isdir(args.repoDir):
        LOG.error("Repository path is not a directory: %s", args.repoDir)
        sys.exit(1)

    if not os.path.exists(args.config):
        LOG.error("Config file does not exist: %s", args.config)
        sys.exit(1)

    if not os.path.isfile(args.config):
        LOG.error("Config file path is not a file: %s", args.config)
        sys.exit(1)

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

    return result_dir, artifact_dir, repo_dir, config_path


def clone_repo(repo_path: pathlib.Path) -> pathlib.Path:
    """
    Clones the repo in full from the cached repo at repo_path.
    :param repo_path: The current cloned repo that uses hard links to the local
                      cache and therefore cannot be copied across nodes.
    :return: the path to the cloned repo that can be copied across nodes.
    """
    # Get the current git hash

    # Get the URL to the local cached repo remote. This will be a file:// URL.

    # Clone this repo locally

    # Check out the above git hash

    # Return the path to the cloned repo
    

if __name__ == '__main__':
    try:
        sys.exit(main())

    except errors.NonTracebackError as e:
        LOG.error(e)
        sys.exit(1)

    except errors.MakeReleaseError as e:
        LOG.error(e, exc_info=e.should_print_trace)
        sys.exit(1)

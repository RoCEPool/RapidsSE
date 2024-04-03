import shutil
import sys
import os
import pathlib
import logging.config
import os.path
import argparse
import fnmatch

import pyutils.errors as errors
from releasebuilder.releaseconfig import ReleaseConfig
from releasebuilder.buildrunner import BuildRunner
from releasebuilder.buildrunner import C_INSTALLER_FILESPEC
from releasebuilder.testrunner import TestRunner
from releasebuilder.testrunner import terminate_instances_from_file
import pyutils.repo as repo
import releasebuilder.results as results


C_PROG_RUN = "pipenv run python -m releasebuilder"
C_CWD_SUFFIX = "releasetest"

C_DEFAULT_RESULT_DIR = "./results"
C_DEFAULT_ARTIFACT_DIR = "./artifacts"
C_DEFAULT_RESULT_FILENAME = "buildResults.xml"
C_TEST_SUITE_NAME = "Release Test"

# Create a main logger
LOG = logging.getLogger(__name__)


def main() -> int:
    """A simple main entry point for executing the releasebuilder test script.
       Run with: pipenv run python -m releasebuilder
       from the outer releasetest directory.
       Expected arguments:
         --resultDir    path to write the results file
         --artifactDir  path to save any artifacts
         --repoDir      path to the root of the repository
         --config       path of the file that contains build configuration information
         --skipBuild    don't build SE, instead find the pre-built installer. For development purposes only.
         --skipTest     build SE only, don't test it. For development purposes only.
    """

    # Validate the path
    if not os.getcwd().endswith(C_CWD_SUFFIX):
        LOG.error(f"This file must be executed from the following subdirectory of the project root: {C_CWD_SUFFIX}")
        return 1

    # First check for the cleanup directive
    if len(sys.argv) == 2 and sys.argv[1].lower() == "--cleanup":
        terminate_instances_from_file()
        return 0

    # Validate the Python arguments
    (result_path, artifact_path, repo_path, config_path, skip_build, skip_test) = validate_arguments()

    LOG.debug(f"cwd          = {os.getcwd()}")
    LOG.debug(f"result dir   = {result_path}")
    LOG.debug(f"artifact dir = {artifact_path}")
    LOG.debug(f"repo root    = {repo_path}")
    LOG.debug(f"config file  = {config_path}")
    LOG.debug(f"skip build   = {skip_build}")
    LOG.debug(f"skip test    = {skip_test}")

    # Read in the config file and validate it
    config = ReleaseConfig(str(config_path))

    # Create a result object and let the buildrunner and testrunner fill it in appropriately.
    result_list: [results.TestResult] = []

    # Clone the repository.
    # We do this because bamboo uses a cached repository strategy and the current repo
    # cannot be copied to another node as it uses hard links for files to fixed paths
    # on the current node. So instead we will clone the cached repo in full to a temporary
    # folder and then we can copy that to other nodes and use it. We must remember to
    # clean up the temporary cloned repo before we exit.
    clone_repo_path = repo.clone_repo(repo_path)

    try:
        installer_path: pathlib.Path

        if skip_build:
            LOG.info("Skipping the build stage")

            # Find the existing installer
            installer_path = __find_installer(artifact_path)
        else:
            buildrunner = BuildRunner(artifact_path, clone_repo_path, config, result_list)
            buildrunner.run()
            installer_path = buildrunner.installer_path

            # Don't do the test stage if we don't pass the build stage
            if not buildrunner.test_result.test_passed:
                skip_test = True

        if skip_test:
            LOG.info("Skipping the test stage")
        else:
            testrunner = TestRunner(artifact_path, result_path, clone_repo_path, config, result_list, installer_path)
            testrunner.run()

    finally:
        # Clean up the cloned repository
        LOG.debug(f"Cleaning up cloned repository at: {str(clone_repo_path)}")
        shutil.rmtree(clone_repo_path, ignore_errors=True)

        # Write out the results to result_path
        if len(result_list) > 0:
            result_file = result_path / C_DEFAULT_RESULT_FILENAME
            results.summarise_to_junit(result_list, result_file, C_TEST_SUITE_NAME)
        else:
            LOG.warning("There are no results to write out.")

    return 0


def validate_arguments() -> (pathlib.Path, pathlib.Path, pathlib.Path, pathlib.Path):
    # Extract the command line arguments
    arg_parser = argparse.ArgumentParser(C_PROG_RUN, description="Run the releasebuilder script")
    arg_parser.add_argument("--resultDir", type=str, required=False, default=C_DEFAULT_RESULT_DIR,
                            help="The directory to write results files")
    arg_parser.add_argument("--artifactDir", type=str, required=False, default=C_DEFAULT_ARTIFACT_DIR,
                            help="The directory to store artifacts to be archived")
    arg_parser.add_argument("--repoDir", type=str, required=True,
                            help="The root directory of the repository")
    arg_parser.add_argument("--config", type=str, required=True,
                            help="The path of the config file to use")
    arg_parser.add_argument("--skipBuild", action='store_true', required=False, default=False,
                            help="Skip the building and go straight to testing")
    arg_parser.add_argument("--skipTest", action='store_true', required=False, default=False,
                            help="Skip the testing phase")

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

    result_dir = pathlib.Path(args.resultDir)
    if not result_dir.is_absolute():
        result_dir = (pathlib.Path.cwd() / result_dir).resolve()

    artifact_dir = pathlib.Path(args.artifactDir)
    if not artifact_dir.is_absolute():
        artifact_dir = (pathlib.Path.cwd() / artifact_dir).resolve()

    repo_dir = pathlib.Path(args.repoDir)
    if not repo_dir.is_absolute():
        repo_dir = (pathlib.Path.cwd() / repo_dir).resolve()

    config_path = pathlib.Path(args.config)
    if not config_path.is_absolute():
        config_path = (pathlib.Path.cwd() / config_path).resolve()

    return result_dir, artifact_dir, repo_dir, config_path, args.skipBuild, args.skipTest


def __find_installer(artifact_path: pathlib.Path) -> pathlib.Path:
    """Finds the path of the RapidsSE installer in the artifact directory."""
    for root, dirs, files in os.walk(artifact_path):
        for name in files:
            if fnmatch.fnmatch(name, C_INSTALLER_FILESPEC):
                installer_path = pathlib.Path(root) / name
                LOG.info(f"The RapidsSE installer is found at {str(installer_path)}")
                return installer_path

    assert False, f"RapidsSE installer could not be found in path: {str(artifact_path)}"


if __name__ == '__main__':
    try:
        sys.exit(main())

    except errors.NonTracebackError as e:
        LOG.error(e)
        sys.exit(1)

    except errors.RapidsError as e:
        LOG.error(e, exc_info=e.should_print_trace)
        sys.exit(1)

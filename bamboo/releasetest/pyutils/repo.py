import pathlib
import shutil
import subprocess
import re
import os.path
import tempfile
import logging

import pyutils.errors as errors

LOG = logging.getLogger(__name__)


def clone_repo(repo_path: pathlib.Path) -> pathlib.Path:
    """
    Clones the repo in full from the cached repo at repo_path.
    May throw a pyutils.errors.GitError.
    :param repo_path: The current cloned repo that uses hard links to the local
                      cache and therefore cannot be copied across nodes.
    :return: the path to the cloned repo that can be copied across nodes.
    """
    # Get the current git hash
    git_hash = __get_current_git_hash(repo_path)

    # Get the URL to the local cached repo remote. This will be a file:// URL.
    git_remote_url = __get_git_remote_url(repo_path)

    # Clone this repo locally
    cloned_repo_path = __clone_repo(git_remote_url)

    # Check out the above git hash
    try:
        __checkout_hash(cloned_repo_path, git_hash)

    except errors.GitError:
        # Clean up the created directory
        shutil.rmtree(cloned_repo_path)
        raise

    # Return the path to the cloned repo
    return cloned_repo_path


def __get_current_git_hash(repo_path: pathlib.Path) -> str:
    """
    Returns the git hash of the repo at repo_path
    May throw a pyutils.errors.GitError.
    :param repo_path: path to the root of a repository
    :return: the git hash
    """

    LOG.info(f"About to get current git hash of local repo at path: {str(repo_path)}")

    git_command = ["git", "rev-parse", "HEAD"]

    try:
        result = subprocess.run(git_command,
                                cwd=str(repo_path),
                                capture_output=True,
                                check=True,
                                text=True
                                )
    except subprocess.CalledProcessError as e:
        msg = f"Could not get the git hash for the current repository at: {str(repo_path)}" \
              f"\nError message returned: {e.stderr}"
        raise errors.GitError(msg) from e

    git_hash = result.stdout.strip()

    # Check that the git hash is sane
    match = re.match(r"^[a-fA-F\d]+$", git_hash)

    if match is None:
        msg = f"Could not get the git hash for the current repository at: {str(repo_path)}" \
              f"\nGot an invalid git hash: {git_hash}"
        raise errors.GitError(msg)

    LOG.info(f"Got current git hash ({git_hash}) of local repo at path: {str(repo_path)}")
    return git_hash


def __get_git_remote_url(repo_path: pathlib.Path) -> str:
    """
    Returns the URL of the origin remote for the repository given by repo_path.
    This is expected to be a file:// URL because bamboo uses cached git repositories.
    May throw a pyutils.errors.GitError.
    :param repo_path: path to the root of the local cloned repository
    :return: the url to the cached remote origin
    """

    LOG.info(f"About to get the remote origin URL of local repo at path: {str(repo_path)}")

    git_command = ["git", "remote", "-v"]

    try:
        result = subprocess.run(git_command,
                                cwd=str(repo_path),
                                capture_output=True,
                                check=True,
                                text=True
                                )
    except subprocess.CalledProcessError as e:
        msg = f"Could not get the git remote URL for the current repository at: {str(repo_path)}" \
              f"\nError message returned: {e.stderr}"
        raise errors.GitError(msg) from e

    prefix = "file://"
    git_url = None
    lines = result.stdout.splitlines()

    for line in lines:
        words = line.split()
        if len(words) >= 2 and words[0] == "origin" and words[1].startswith(prefix):
            git_url = words[1]
            break

    if git_url is None:
        msg = f"Could not find an appropriate git remote URL for the current repository at: {str(repo_path)}" \
              f"\nOutput returned:\n{result.stdout}"
        raise errors.GitError(msg)

    # Check that the git_url exists and is a directory
    if not os.path.exists(git_url[len(prefix):]):
        msg = f"Remote git repository URL does not exist at path: {git_url}"
        raise errors.GitError(msg)

    if not os.path.isdir(git_url[len(prefix):]):
        msg = f"Remote git repository URL is not a directory: {git_url}"
        raise errors.GitError(msg)

    LOG.info(f"Got the remote origin URL of local repo at path {str(repo_path)}. URL is {git_url}")

    return git_url


def __clone_repo(git_remote_url: str) -> pathlib.Path:
    """
    Clones the repository from git_remote_url into a temporary directory
    :param git_remote_url: the url of the local cached repository
    :return: the path to the cloned repo
    """

    dest_path = pathlib.Path(tempfile.mkdtemp(prefix="rapidsse_repo_"))
    LOG.info(f"About to clone the repository so it can be copied to remote nodes. Path: {str(dest_path)}")

    git_command = ["git", "clone", "--no-local", git_remote_url, str(dest_path)]

    try:
        subprocess.run(git_command,
                       capture_output=True,
                       check=True,
                       text=True
                       )
    except subprocess.CalledProcessError as e:
        # Clean up the created directory
        shutil.rmtree(dest_path)
        msg = f"Could not clone the repository at {git_remote_url} to {str(dest_path)}" \
              f"\nError message returned: {e.stderr}"
        raise errors.GitError(msg) from e

    LOG.info(f"Finished cloning the repository at: {str(dest_path)}")

    return dest_path


def __checkout_hash(cloned_repo_path: pathlib.Path, git_hash: str) -> None:
    """
    Checks out a given hash on a repo cloned at a given path.
    :param cloned_repo_path: the repo to work on
    :param git_hash: the hash to checkout
    """

    LOG.info(f"About to checkout git hash {git_hash} in repo at path: {str(cloned_repo_path)}")

    git_command = ["git", "checkout", git_hash]

    try:
        subprocess.run(git_command,
                       cwd=str(cloned_repo_path),
                       capture_output=True,
                       check=True,
                       text=True
                       )
    except subprocess.CalledProcessError as e:
        msg = f"Could not checkout the git hash {git_hash} in repo at path: {str(cloned_repo_path)}" \
              f"\nError message returned: {e.stderr}"
        raise errors.GitError(msg) from e

    LOG.info(f"Finished checkout of git hash {git_hash} in repo at path: {str(cloned_repo_path)}")

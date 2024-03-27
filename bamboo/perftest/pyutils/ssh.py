import subprocess
import logging

import pyutils.errors as errors

LOG = logging.getLogger(__name__)


def run_ssh(host: str, command: str, timeout: float = None, ctx: str = "") -> subprocess.CompletedProcess:
    """
    Runs a given command on a remote host, returning a subprocess.CompletedProcess object.
    A timeout is an optional value.
    May throw a SshError or a subprocess.TimeoutExpired exception.
    """

    ssh_command = ["ssh",
                   "-q",
                   "-o", "BatchMode=yes",
                   "-o", "StrictHostKeyChecking=no",
                   host,
                   command]

    LOG.debug(f"{ctx}About to run SSH command: {' '.join(ssh_command)}")

    result = subprocess.run(ssh_command,
                            capture_output=True,
                            check=False,
                            timeout=timeout,
                            text=True)

    if result.returncode != 0:
        msg = f"{ctx}Error while executing the command: {' '.join(ssh_command)}"
        raise errors.SshError(msg, result.stdout, result.stderr, result.returncode)

    return result


def run_scp(src: str, dest: str, timeout: float = None, recursive: bool = False, ctx: str = ""):
    """
    Copies a src filespec to a dest filespec using SCP.
    Either src or dest should be in the form "<host>:<path>".
    May throw a SshError or a subprocess.TimeoutExpired exception.
    """

    scp_command = ["scp",
                   "-q",
                   "-o", "BatchMode=yes",
                   "-o", "StrictHostKeyChecking=no"
                   ]

    if recursive:
        scp_command.append("-r")

    scp_command.append(src)
    scp_command.append(dest)

    LOG.debug(f"{ctx}About to run SCP command: {' '.join(scp_command)}")

    result = subprocess.run(scp_command,
                            capture_output=True,
                            check=False,
                            timeout=timeout,
                            text=True)

    if result.returncode != 0:
        msg = f"{ctx}Error while executing the command: {' '.join(scp_command)}\n" + \
              f"Return code: {str(result.returncode)}\n" + \
              f"Stdout: {result.stdout}\n" + \
              f"Stderr: {result.stderr}"
        raise errors.SshError(msg, result.stdout, result.stderr, result.returncode)

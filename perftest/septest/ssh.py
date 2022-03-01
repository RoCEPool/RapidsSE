import subprocess
import string
import logging

LOG = logging.getLogger(__name__)


def run_ssh(host: string, command: string, timeout: float = None) -> subprocess.CompletedProcess:
    """
    Runs a given command on a remote host, returning a subprocess.CompletedProcess object.
    A timeout is an optional value.
    May throw a subprocess.CalledProcessError or a subprocess.TimeoutExpired exception.
    """

    ssh_command = ["ssh",
                   "-q",
                   "-o", "BatchMode=yes",
                   "-o", "StrictHostKeyChecking=no",
                   host,
                   command]

    LOG.debug(f"About to run SSH command: {' '.join(ssh_command)}")

    result = subprocess.run(ssh_command,
                            capture_output=True,
                            check=True,
                            timeout=timeout,
                            text=True)

    return result


def run_scp(src: string, dest: string, timeout: float = None):
    """
    Copies a src filespec to a dest filespec using SCP.
    Either src or dest should be in the form "<host>:<path>".
    May throw a subprocess.CalledProcessError or a subprocess.TimeoutExpired exception.
    """

    scp_command = ["scp",
                   "-q",
                   "-o", "BatchMode=yes",
                   "-o", "StrictHostKeyChecking=no",
                   src,
                   dest]

    LOG.debug(f"About to run SCP command: {' '.join(scp_command)}")

    subprocess.run(scp_command,
                   capture_output=True,
                   check=True,
                   timeout=timeout,
                   text=True)

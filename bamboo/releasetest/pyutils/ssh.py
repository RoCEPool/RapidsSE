import subprocess
import logging

import pyutils.errors as errors

LOG = logging.getLogger(__name__)


def run_ssh(host: str, command: str, timeout: float = None, ssh_opts: [] = None, ctx: str = "") \
        -> subprocess.CompletedProcess:
    """
    Runs a given command on a remote host, returning a subprocess.CompletedProcess object.
    A timeout is an optional value.
    May throw a SshError or a subprocess.TimeoutExpired exception.
    """

    ssh_command = ["ssh",
                   "-q",
                   "-o", "BatchMode=yes",
                   "-o", "StrictHostKeyChecking=no",
                   "-o", "CheckHostIP=no",
                   "-o", "RequestTTY=force",
                   ]

    if ssh_opts is not None:
        ssh_command.extend(ssh_opts)

    ssh_command.append(host)
    ssh_command.append('"' + command.replace('"', '\\"') + '"')
    cmd_str: str = ' '.join(ssh_command)

    # LOG.debug(f"{ctx}About to run SSH command: {cmd_str}")

    result = subprocess.run(cmd_str,
                            capture_output=True,
                            check=False,
                            shell=True,
                            timeout=timeout,
                            text=True)

    if result.returncode != 0:
        # LOG.debug(f"Exit code = {result.returncode} ;"
        #           f"\nstdout = {result.stdout} ;"
        #           f"\nstderr = {result.stderr} ;"
        #           f"\nargs = {result.args}")

        msg = f"{ctx}Error while executing the command: {cmd_str}"
        raise errors.SshError(msg, result.stdout, result.stderr, result.returncode)

    return result


def run_scp(src: str, dest: str, timeout: float = None, recursive: bool = False, ssh_opts: [] = None, ctx: str = ""):
    """
    Copies a src filespec to a dest filespec using SCP.
    Either src or dest should be in the form "<host>:<path>".
    May throw a SshError or a subprocess.TimeoutExpired exception.
    """

    scp_command = ["scp",
                   "-q",
                   "-o", "BatchMode=yes",
                   "-o", "StrictHostKeyChecking=no",
                   "-o", "CheckHostIP=no",
                   ]

    if ssh_opts is not None:
        scp_command.extend(ssh_opts)

    if recursive:
        scp_command.append("-r")

    scp_command.append('"' + src + '"')
    scp_command.append('"' + dest + '"')
    cmd_str: str = ' '.join(scp_command)

    # LOG.debug(f"{ctx}About to run SCP command: {cmd_str}")

    result = subprocess.run(cmd_str,
                            capture_output=True,
                            check=False,
                            shell=True,
                            timeout=timeout,
                            text=True)

    if result.returncode != 0:
        LOG.error(f"Error in SCP command."
                  f"\nExit code = {result.returncode} ;"
                  f"\nstdout = {result.stdout} ;"
                  f"\nstderr = {result.stderr} ;"
                  f"\nargs = {result.args}")

        msg = f"{ctx}Error while executing the command: {cmd_str}\n" + \
              f"Return code: {str(result.returncode)}\n" + \
              f"Stdout: {result.stdout}\n" + \
              f"Stderr: {result.stderr}"
        raise errors.SshError(msg, result.stdout, result.stderr, result.returncode)

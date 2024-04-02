import os.path
import pathlib
import logging
import subprocess
import tempfile
import shutil

import pyutils.errors as errors
import pyutils.ssh as ssh
import pyutils.repo as repo
from pyutils.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)

# How long to wait before killing the RapidsSE installation?
C_INSTALL_TIMEOUT_SEC = 120
C_GENERAL_SSH_TIMEOUT_SEC = 120
C_CLUSTER_START_CMD = "startSE.sh"
C_CLUSTER_SESHELL_CMD = "seshell.sh"
C_CLUSTER_COLLECT_LOGS_CMD = "collectLogs.sh"
C_CLUSTER_STATUS_CMD = "status.sh"


def copy_repo(repo_root: pathlib.Path, host: str, remote_path: pathlib.Path, ctx: str = "") -> None:
    """
    Copies the repository from the local repo_root directory to the remote node
    on host at directory remote_path.
    """
    # We first need to clone the repo because Bamboo uses a "file://" origin with hard links
    # to this cache for space/performance reasons. We need a fully reconstructed repo that we
    # can copy to another machine.

    LOG.info(f"{ctx}About to clone the repository at {str(repo_root)}.")
    cloned_repo_root: pathlib.Path = repo.clone_repo(repo_root)
    LOG.info(f"{ctx}Repository cloned to {str(cloned_repo_root)}.")

    try:
        LOG.info(f"{ctx}About to copy the repository from {str(cloned_repo_root)} to {host}:{str(remote_path)}.")
        timeout = 60

        try:
            ssh.run_scp(str(cloned_repo_root), f"{host}:{str(remote_path)}", timeout, True, ctx)

        except (errors.SshError, subprocess.TimeoutExpired) as e:
            msg = f"{ctx}{'Timeout' if type(e) is subprocess.TimeoutExpired else 'Error'} " \
                  f"while copying the repository to the remote node at {host}." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.CompilationError(msg) from e

        LOG.info(f"{ctx}Finished copying the repository from {str(cloned_repo_root)} to {host}:{str(remote_path)}.")

    finally:
        # Cleanup the temporarily clone repo
        shutil.rmtree(cloned_repo_root)
        LOG.info(f"{ctx}Cleaned up cloned repository at {str(cloned_repo_root)}.")


def build_installer_remotely(host: str,
                             remote_repo_path: pathlib.Path,
                             local_result_dir: str,
                             ctx: str = "") -> pathlib.Path:
    """
    Compiles RapidsSE and builds the installer on a given remote node.
    Returns the file path of the installer.
    Assumes that the repository has already been copied to the remote node.
    """
    LOG.info(f"{ctx}About to invoke the RapidsSE installer packager script to remotely "
             f"build and package RapidsSE.")

    output_path = f"{str(remote_repo_path)}/installer/packaging/installer.log"
    command = f"{str(remote_repo_path)}/installer/packaging/package.sh > {output_path} 2>&1"

    try:
        ssh.run_ssh(host, command, ctx=ctx)

    except (errors.SshError, subprocess.TimeoutExpired) as e:
        msg = f"{ctx}{'Timeout' if type(e) is subprocess.TimeoutExpired else 'Error'} " \
              f"while remotely compiling and building the RapidsSE installer."

        # Copy the log of the installer output as a system artifact
        try:
            ssh.run_scp(f"{host}:{output_path}", local_result_dir, 10, False, ctx)
        except errors.SshError:
            LOG.error(f"Could not copy the remote compilation log from {host}:{output_path} to {local_result_dir}")

        raise errors.CompilationError(msg) from e

    return find_installer_remotely(host, remote_repo_path, ctx)


def build_installer_locally(ctx: str = "") -> pathlib.Path:
    """
    Compiles RapidsSE and builds the installer. Returns the file path of the installer.
    Assumes we are running from the <proj_root>/perftest directory
    """
    LOG.info(f"{ctx}About to invoke the RapidsSE installer packager script to locally "
             f"build and package RapidsSE.")

    # Note: For debugging only
    # command = "NO_MOD_CHECK=1 NO_BUILD=1 ../../installer/packaging/package.sh"

    command = "../../installer/packaging/package.sh"

    try:
        subprocess.run(command,
                       capture_output=True,
                       check=True,
                       shell=True,
                       text=True)

    except subprocess.CalledProcessError as e:
        msg = f"{ctx}Error while locally compiling RapidsSE and building the installer." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.CompilationError(msg) from e

    return find_installer_locally(ctx)


def find_installer_remotely(host: str, remote_repo_path: pathlib.Path, ctx: str = "") -> pathlib.Path:
    """
    Attempts to find the RapidsSE installer on a remote node and returns the path to it.
    Throws an assertion error if the installer cannot be found.
    """
    # Find the installer file that was built
    install_file_path: pathlib.Path = remote_repo_path / "installer" / "output" / "rapidsse-*.run"

    timeout = 30
    command = f"/bin/sh -c 'ls -1 {str(install_file_path)}'"

    try:
        result: subprocess.CompletedProcess = ssh.run_ssh(host, command, timeout, ctx)

    except (errors.SshError, subprocess.TimeoutExpired) as e:
        msg = f"{ctx}{'Timeout' if type(e) is subprocess.TimeoutExpired else 'Error'} " \
              f"while trying to find the RapidsSE installer on the remote node at {host}."
        raise errors.CompilationError(msg) from e

    if result.stdout is None:
        msg = f"{ctx}Could not find the RapidsSE installer on the remote node at {host}."
        raise errors.CompilationError(msg)

    installer_files: list = str(result.stdout).strip().split("\n")
    if len(installer_files) > 1:
        msg = f"{ctx}Found more than one RapidsSE installer on the remote node at {host}.\n"
        msg += f"  {str(installer_files)}"
        raise errors.CompilationError(msg)

    installer_file = pathlib.Path(installer_files[0].strip())
    LOG.info(f"{ctx}Installation file successfully built at: {str(installer_file)} on host {host}")
    return installer_file


def find_installer_locally(ctx: str = "") -> pathlib.Path:
    """
    Attempts to find the RapidsSE installer and returns the path to it.
    Throws an assertion error if the installer cannot be found.
    """
    # Find the installer file that was built
    install_file_path = pathlib.Path("../../installer/output").resolve()
    install_file_glob = "rapidsse-*.run"

    run_files_iter = pathlib.Path(install_file_path).glob(install_file_glob)

    installer_file: pathlib.Path

    try:
        installer_file = next(run_files_iter)

    except StopIteration:
        assert False, f"{ctx}Could not find the installation file in path: {install_file_path}"
        pass

    assert next(run_files_iter, None) is None, f"{ctx}Found more than one installer file in path: {install_file_path}"
    assert installer_file.exists()

    LOG.info(f"{ctx}Installation file successfully built at: {str(installer_file)}")
    return installer_file


def customise_cluster_cfg_hostnames(cluster_cfg_path: pathlib.Path,
                                    ip_addresses: [str],
                                    new_cluster_id: int = -1,
                                    ctx: str = "") \
        -> tempfile.NamedTemporaryFile:
    """
    The cluster config file doesn't know ahead of time what the hostnames are going to be
    of the new AWS nodes being started. So we will take a copy of the cluster config,
    and modify it to use the known AWS hostnames that we are going to run on.
    Returns a NamedTemporaryFile that will be auto-deleted when it is garbage collected.
    """

    LOG.info(f"{ctx}Customising the cluster config file for the hostnames of the AWS instances that were started")

    # Get the config file specified in the job
    cluster_cfg_path = pathlib.Path(cluster_cfg_path)
    assert cluster_cfg_path.exists()
    assert cluster_cfg_path.stat().st_size > 0

    # Create an empty file that we will copy the contents of the cluster config into
    config_copy = tempfile.NamedTemporaryFile(mode="wt", prefix="septest_cluster_", suffix=".config", delete=True)

    ip_iter = iter(ip_addresses)
    new_lines = []

    with open(cluster_cfg_path) as config_orig:
        for line in config_orig:
            # Copy all the lines except those that define the hostname.
            # We will change them to the IP addresses we got from the EC2 instances.

            if not line.startswith("NODE_CFG_"):
                # Not the line we are looking for
                new_lines.append(line)
                continue

            splits: [str] = line.split("=")
            if len(splits) != 2:
                new_lines.append(line)
                continue

            if splits[0].endswith('["host"]'):
                # It is the line we are looking for!
                next_ip = next(ip_iter, None)
                assert next_ip is not None, f"{ctx}Mismatch in count of machines started and number of nodes in cluster"
                new_lines.append(f'{splits[0]}="{next_ip}"\n')
                continue

            if new_cluster_id != -1 and splits[0].endswith('["clusterid"]'):
                new_lines.append(f'{splits[0]}="{str(new_cluster_id)}"\n')
                continue

            # Everything else
            new_lines.append(line)

    new_content = "".join(new_lines)
    config_copy.write(new_content)
    config_copy.flush()

    LOG.info(f"{ctx}Customised temporary cluster config file written out to {config_copy.name}")
    LOG.debug(f"{ctx}Customised config file contents:\n{new_content}")
    return config_copy


def install_rapidsse_remotely(host: str,
                              remote_install_file: pathlib.Path,
                              local_cluster_config_path: pathlib.Path,
                              ctx: str = ""):
    """
    Installs RapidsSE using the installer file and cluster config
    """

    assert local_cluster_config_path.exists()

    # Copy the cluster.config to the remote node
    remote_cluster_config_path: pathlib.Path = pathlib.Path("/tmp") / local_cluster_config_path.name
    timeout = 30

    try:
        ssh.run_scp(str(local_cluster_config_path),
                    f"{host}:{str(remote_cluster_config_path)}",
                    timeout,
                    False,
                    ctx)

    except (errors.SshError, subprocess.TimeoutExpired) as e:
        msg = f"{ctx}{'Timeout' if type(e) is subprocess.TimeoutExpired else 'Error'} " \
              f"while copying the cluster.config to the remote node at {host}."
        raise errors.InstallationError(msg) from e

    # Note: Can add in "--debug" as a parameter to the below command
    command = f"{str(remote_install_file)} --quiet -- --script --config {str(remote_cluster_config_path)}"
    timeout = 60

    try:
        result = ssh.run_ssh(host, command, timeout, ctx)

    except (errors.SshError, subprocess.TimeoutExpired) as e:
        msg = f"{ctx}{'Timeout' if type(e) is subprocess.TimeoutExpired else 'Error'} " \
              f"while remotely installing RapidsSE at {host}."
        raise errors.InstallationError(msg) from e

    LOG.debug(f"{ctx}Installation output (stdout):\n{result.stdout}")

    LOG.info(f"{ctx}RapidsSE cluster installed successfully")


def install_rapidsse(install_file: pathlib.Path, cluster_config_path: pathlib.Path, ctx: str = ""):
    """
    Installs RapidsSE using the installer file and cluster config
    """

    assert install_file.exists()
    assert cluster_config_path.exists()

    command = [str(install_file), "--quiet", "--",
               "--script", "--config", str(cluster_config_path)]

    # Note: Can add in "--debug" as a parameter to the above list

    LOG.info(f"{ctx}About to install the RapidsSE cluster with the following command: {' '.join(command)}")

    try:
        result = subprocess.run(command,
                                capture_output=True,
                                check=True,
                                shell=False,
                                timeout=C_INSTALL_TIMEOUT_SEC,
                                text=True)

    except subprocess.CalledProcessError as e:
        msg = f"{ctx}Error while installing RapidsSE." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.InstallationError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"{ctx}Installation of RapidsSE timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.InstallationError(msg) from e

    LOG.debug(f"{ctx}Installation output (stdout):\n{result.stdout}")

    LOG.info(f"{ctx}RapidsSE cluster installed successfully")


def start_rapidsse(host: str, cluster_cfg: ClusterConfig, ctx: str = ""):

    start_command = f"{cluster_cfg.get_dir()}/{C_CLUSTER_START_CMD}"
    timeout = 90

    LOG.info(f"{ctx}About to start the RapidsSE cluster with the following command: {start_command}")

    try:
        result = ssh.run_ssh(host, start_command, timeout)

    except errors.SshError as e:
        msg = f"{ctx}Error while starting the RapidsSE cluster."
        raise errors.ClusterError(msg) from e

    LOG.debug(f"{ctx}Cluster start output (stdout):\n{result.stdout}")
    LOG.info(f"{ctx}RapidsSE cluster started successfully")


def run_dummy_statement(host: str, cluster_cfg: ClusterConfig, ctx: str = ""):

    dummy_command = f"echo -e \"status;\" | {cluster_cfg.get_dir()}/{C_CLUSTER_SESHELL_CMD}"
    command_timeout = 10  # seconds

    LOG.info(f"{ctx}About to run a dummy command on the RapidsSE cluster: {dummy_command}")

    try:
        result = ssh.run_ssh(host, dummy_command, command_timeout)

    except errors.SshError as e:
        msg = f"{ctx}Error while running a dummy command on the RapidsSE cluster."
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"{ctx}Execution of a dummy command on the RapidsSE cluster timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.debug(f"{ctx}Cluster dummy command output (stdout):\n{result.stdout}")
    LOG.info(f"{ctx}RapidsSE dummy command ran successfully")


def get_rapidsse_logs(host: str, cluster_cfg: ClusterConfig, dest_path: str, ctx: str = ""):
    """
    Gets the log files from each RapidsSE node and copies them to a local directory.
    """
    assert os.path.exists(dest_path), f"dest_path does not exist: {dest_path}"

    LOG.debug(f"{ctx}About to collect logs files from the RapidsSE cluster")

    command = f"cd {cluster_cfg.get_dir()} && " \
              f"./{C_CLUSTER_COLLECT_LOGS_CMD} >/dev/null && " \
              f"ls -1 logs-*.tar.gz | sort -r | head -n 1"

    try:
        result = ssh.run_ssh(host, command, C_GENERAL_SSH_TIMEOUT_SEC)

    except errors.SshError as e:
        msg = f"{ctx}Error while collecting the RapidsSE log files."
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"{ctx}Collecting the RapidsSE log files timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    log_file_name = result.stdout.strip()
    src_file = f"{host}:{cluster_cfg.get_dir()}/{log_file_name}"
    LOG.debug(f"{ctx}About to copy the RapidsSE log file from: {src_file}")

    try:
        ssh.run_scp(src_file, dest_path, C_GENERAL_SSH_TIMEOUT_SEC)

    except errors.SshError as e:
        msg = f"{ctx}Error while copying RapidsSE log files from the cluster."
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"{ctx}Copying of the RapidsSE log files from the cluster timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e


def is_rapidsse_running(host: str, cluster_cfg: ClusterConfig, ctx: str = "") -> (bool, str):
    """
    Returns a tuple containing a boolean result of whether RapidsSE is running
    and a string with an error message if it is not.
    e.g. (True, None) if SE is running, (False, "some_error_msg") if SE is not running.
    """

    command = f"{cluster_cfg.get_dir()}/{C_CLUSTER_STATUS_CMD}"

    try:
        ssh.run_ssh(host, command, 30)

    except errors.SshError as e:
        msg = f"{ctx}The RapidsSE cluster is not running." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        return False, msg

    except subprocess.TimeoutExpired as e:
        msg = f"{ctx}The RapidsSE cluster may not be running as the status check timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        return False, msg

    LOG.debug(f"{ctx}RapidsSE is still running!")
    return True, None

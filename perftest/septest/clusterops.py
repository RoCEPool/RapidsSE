import os.path
import pathlib
import logging
import subprocess
import tempfile
import string

import septest.errors as errors
import septest.ssh as ssh
from septest.configs.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)

# How long to wait before killing the RapidsSE installation?
C_INSTALL_TIMEOUT_SEC = 60
C_GENERAL_SSH_TIMEOUT_SEC = 60
C_CLUSTER_START_CMD = "startSE.sh"
C_CLUSTER_PROMPT_CMD = "prompt.sh"
C_CLUSTER_COLLECT_LOGS_CMD = "collectLogs.sh"
C_CLUSTER_STATUS_CMD = "status.sh"

C_JDBC_DRIVER_DIR = pathlib.Path("../seth/bin")
C_JDBC_DRIVER_GLOB = "rapidsse-jdbc-*.*.*.jar"

# Cached path to the found JDBC driver
C_JDBC_DRIVER: pathlib.Path = None
C_JDBC_URL_FMT = 'jdbc:rapidsse:{0}/bin/rapidsse?extraargs="--name {1} ' \
                 '--sen {0}/sen --commands {0}/commands --appendTrace"'

C_PERF_TESTS_DIR = "perftests"
C_SUPPORT_FILES_DIR = f"{C_PERF_TESTS_DIR}/supportFiles"


def build_installer() -> pathlib.Path:
    """
    Compiles RapidsSE and builds the installer. Returns the file path of the installer.
    Assumes we are running from the <proj_root>/perftest directory
    """
    LOG.info("About to invoke the RapidsSE installer packager script to build and package RapidsSE.")

    # Note: For debugging only
    # command = "NO_MOD_CHECK=1 NO_BUILD=1 ../installer/packaging/package.sh"

    command = "../installer/packaging/package.sh"

    try:
        subprocess.run(command,
                       capture_output=True,
                       check=True,
                       shell=True,
                       text=True)

    except subprocess.CalledProcessError as e:
        msg = f"Error while compiling RapidsSE and building the installer." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.CompilationError(msg) from e

    # Find the installer file that was built
    install_file_path = pathlib.Path("../installer/output").resolve()
    install_file_glob = "rapidsse-*.run"

    run_files_iter = pathlib.Path(install_file_path).glob(install_file_glob)

    installer_file: pathlib.Path

    try:
        installer_file = next(run_files_iter)

    except StopIteration:
        assert False, f"Could not find the installation file in path: {install_file_path}"
        pass

    assert next(run_files_iter, None) is None, f"Found more than one installer file in path: {install_file_path}"
    assert installer_file.exists()

    LOG.info(f"Installation file successfully built at: {str(installer_file)}")
    return installer_file


def customise_cluster_cfg_hostnames(cluster_cfg_path: pathlib.Path,
                                    ip_addresses: [string]) \
        -> tempfile.NamedTemporaryFile:
    """
    The cluster config file doesn't know ahead of time what the hostnames are going to be
    of the new AWS nodes being started. So we will take a copy of the cluster config,
    and modify it to use the known AWS hostnames that we are going to run on.
    Returns a NamedTemporaryFile that will be auto-deleted when it is garbage collected.
    """

    LOG.info("Customising the cluster config file for the hostnames of the AWS instances that were started")

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
            # Copy all of the lines except those that define the hostname.
            # We will change them to the IP addresses we got from the EC2 instances.

            if not line.startswith("NODE_CFG_"):
                # Not the line we are looking for
                new_lines.append(line)
                continue

            splits: [string] = line.split("=")
            if len(splits) != 2:
                new_lines.append(line)
                continue

            if not splits[0].endswith('["host"]'):
                new_lines.append(line)
                continue

            # It is the line we are looking for!
            next_ip = next(ip_iter, None)
            assert next_ip is not None, "Mismatch in count of machines started and number of nodes in cluster"
            new_lines.append(f'{splits[0]}="{next_ip}"\n')

    new_content = "".join(new_lines)
    config_copy.write(new_content)
    config_copy.flush()

    LOG.info(f"Customised temporary cluster config file written out to {config_copy.name}")
    LOG.debug(f"Customised config file contents:\n{new_content}")
    return config_copy


def install_rapidsse(install_file: pathlib.Path, cluster_config_path: pathlib.Path):
    """
    Installs RapidsSE using the installer file and cluster config
    """

    assert install_file.exists()
    assert cluster_config_path.exists()

    command = [str(install_file), "--quiet", "--",
               "--script", "--config", str(cluster_config_path)]

    # Note: Can add in "--debug" as a parameter to the above list

    LOG.info(f"About to install the RapidsSE cluster with the following command: {' '.join(command)}")

    try:
        result = subprocess.run(command,
                                capture_output=True,
                                check=True,
                                shell=False,
                                timeout=C_INSTALL_TIMEOUT_SEC,
                                text=True)

    except subprocess.CalledProcessError as e:
        msg = f"Error while installing RapidsSE." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.InstallationError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Installation of RapidsSE timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.InstallationError(msg) from e

    LOG.debug(f"Installation output (stdout):\n{result.stdout}")

    LOG.info("RapidsSE cluster installed successfully")


def start_rapidsse(host: string, cluster_cfg: ClusterConfig):

    start_command = f"{cluster_cfg.get_dir()}/{C_CLUSTER_START_CMD}"
    timeout = 90

    LOG.info(f"About to start the RapidsSE cluster with the following command: {start_command}")

    try:
        result = ssh.run_ssh(host, start_command, timeout)

    except subprocess.CalledProcessError as e:
        msg = f"Error while starting the RapidsSE cluster." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.debug(f"Cluster start output (stdout):\n{result.stdout}")
    LOG.info("RapidsSE cluster started successfully")


def run_dummy_statement(host: string, cluster_cfg: ClusterConfig):

    dummy_command = f"echo -e \"SELECT 1;\nbye;\" | {cluster_cfg.get_dir()}/{C_CLUSTER_PROMPT_CMD}"
    command_timeout = 10  # seconds

    LOG.info(f"About to run a dummy command on the RapidsSE cluster: {dummy_command}")

    try:
        result = ssh.run_ssh(host, dummy_command, command_timeout)

    except subprocess.CalledProcessError as e:
        msg = f"Error while running a dummy command on the RapidsSE cluster." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Execution of a dummy command on the RapidsSE cluster timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.debug(f"Cluster dummy command output (stdout):\n{result.stdout}")
    LOG.info("RapidsSE dummy command ran successfully")


def copy_jdbc_driver(host: string, cluster_cfg: ClusterConfig):
    """
    Copy the RapidsSE JDBC driver to the installation directory of the given host.
    """

    src = find_local_jdbc_driver()
    dest = f"{host}:{cluster_cfg.get_dir()}/{src.name}"

    LOG.info(f"About to copy the JDBC driver to the first node of the cluster at: {dest}")

    try:
        ssh.run_scp(str(src), dest, 10)

    except subprocess.CalledProcessError as e:
        msg = f"Error while copying RapidsSE JDBC Driver to the first node." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Copying of RapidsSE JDBC Driver to the first node timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.info("Copied the JDBC driver successfully")


def copy_support_files(host: string, cluster_cfg: ClusterConfig, src_folder: string):
    """
    Copy the supportFiles directory to the <install_dir>/perftests/supportFiles directory of the given host.
    """
    assert os.path.exists(src_folder), f"src_folder does not exist: {src_folder}"

    dest = f"{host}:{cluster_cfg.get_dir()}/{C_SUPPORT_FILES_DIR}"

    LOG.info(f"About to copy the test supportFiles folder to the first node of the cluster at: {dest}")

    try:
        ssh.run_scp(src_folder, dest, 10, recursive=True)

    except subprocess.CalledProcessError as e:
        msg = f"Error while copying the test supportFiles folder to the first node." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Copying of the test supportFiles folder to the first node timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.info("Copied the test supportFiles folder successfully")


def make_perftests_dir(host: string, cluster_cfg: ClusterConfig):
    """
    Makes the common perftests directory, to hold all tests, support files and their metrics
    """

    dest = f"{cluster_cfg.get_dir()}/{C_PERF_TESTS_DIR}"
    ssh_command = f"mkdir -p {dest}"

    LOG.info(f"About to create the perftests folder on the first node of the cluster at: {host}:{dest}")

    try:
        ssh.run_ssh(host, ssh_command, 10)

    except subprocess.CalledProcessError as e:
        msg = f"Error while creating the perftests folder on the first node." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Creating of the perftests folder on the first node timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.info("Created the perftests folder successfully")


def find_local_jdbc_driver() -> pathlib.Path:
    """
    Finds the SE JDBC driver in the repository. Returns a Path object for it.
    """

    global C_JDBC_DRIVER

    if C_JDBC_DRIVER is not None:
        return C_JDBC_DRIVER

    matches = sorted(C_JDBC_DRIVER_DIR.glob(C_JDBC_DRIVER_GLOB), reverse=True)
    assert len(matches) > 0, f"No SE JDBC driver found in path: {C_JDBC_DRIVER_DIR}"

    C_JDBC_DRIVER = pathlib.Path(matches[0])
    assert C_JDBC_DRIVER.exists()

    return C_JDBC_DRIVER


def write_jmeter_property_file(host: string, cluster_cfg: ClusterConfig):
    """
    Generates a JMeter user property file and writes it to the node
    """

    jdbc_url = C_JDBC_URL_FMT.format(cluster_cfg.get_dir(), cluster_cfg.get_first_node_name())

    lines = [
        "# Classpath to find the RapidsSE JDBC driver",
        f"user.classpath={cluster_cfg.get_dir()}",
        "",
        "# JDBC driver URL to use",
        f'JDBC_URL={jdbc_url}',
        ""
    ]
    content = "\n".join(lines)

    with tempfile.NamedTemporaryFile(mode="w", delete=True, encoding="utf-8") as local_file:
        local_file.write(content)
        local_file.flush()

        dest = f"{host}:{get_remote_jmeter_property_file_path(cluster_cfg)}"

        LOG.info(f"About to copy the JMeter user properties file to the first node of the cluster at: {dest}")

        try:
            ssh.run_scp(local_file.name, dest, 10)

        except subprocess.CalledProcessError as e:
            msg = f"Error while JMeter user properties file to the first node." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of JMeter user properties file to the first node timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

    LOG.info("Copied the JMeter user properties file successfully")


def get_remote_jmeter_property_file_path(cluster_cfg: ClusterConfig) -> string:
    """
    Returns the full path and filename of the jmeter user properties file
    to be used when executing jmeter.
    """
    return f"{cluster_cfg.get_dir()}/jmeter.properties"


def get_rapidsse_logs(host: string, cluster_cfg: ClusterConfig, dest_path: string):
    """
    Gets the log files from each RapidsSE node and copies them to a local directory.
    """
    assert os.path.exists(dest_path), f"dest_path does not exist: {dest_path}"

    LOG.debug("About to collect logs files from the RapidsSE cluster")

    command = f"cd {cluster_cfg.get_dir()} && " \
              f"./{C_CLUSTER_COLLECT_LOGS_CMD} >/dev/null && " \
              f"ls -1 logs-*.tar.gz | sort -r | head -n 1"

    try:
        result = ssh.run_ssh(host, command, C_GENERAL_SSH_TIMEOUT_SEC)

    except subprocess.CalledProcessError as e:
        msg = f"Error while collecting the RapidsSE log files." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Collecting the RapidsSE log files timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    log_file_name = result.stdout.strip()
    src_file = f"{host}:{cluster_cfg.get_dir()}/{log_file_name}"
    LOG.debug(f"About to copy the RapidsSE log file from: {src_file}")

    try:
        ssh.run_scp(src_file, dest_path, C_GENERAL_SSH_TIMEOUT_SEC)

    except subprocess.CalledProcessError as e:
        msg = f"Error while copying RapidsSE log files from the cluster." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Copying of the RapidsSE log files from the cluster timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e


def is_rapidsse_running(host: string, cluster_cfg: ClusterConfig) -> (bool, string):
    """
    Returns a tuple containing a boolean result of whether RapidsSE is running
    and a string with an error message if it is not.
    e.g. (True, None) if SE is running, (False, "some_error_msg") if SE is not running.
    """

    command = f"{cluster_cfg.get_dir()}/{C_CLUSTER_STATUS_CMD}"

    try:
        ssh.run_ssh(host, command, 30)

    except subprocess.CalledProcessError as e:
        msg = f"The RapidsSE cluster is not running." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        return False, msg

    except subprocess.TimeoutExpired as e:
        msg = f"The RapidsSE cluster may not be running as the status check timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        return False, msg

    LOG.debug("RapidsSE is still running!")
    return True, None

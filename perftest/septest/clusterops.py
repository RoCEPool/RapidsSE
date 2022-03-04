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
C_CLUSTER_START_CMD = "startSE.sh"
C_CLUSTER_PROMPT_CMD = "prompt.sh"

C_JDBC_DRIVER_DIR = pathlib.Path("../seth/bin")
C_JDBC_DRIVER_GLOB = "rapidsse-jdbc-*.*.*.jar"

# Cached path to the found JDBC driver
C_JDBC_DRIVER: pathlib.Path = None
C_JDBC_URL_FMT = 'jdbc:rapidsse:{0}/bin/rapidsse?extraargs="--name {1} ' \
                 '--sen ../sen --commands ../commands --appendTrace"'


def build_installer() -> pathlib.Path:
    """
    Compiles RapidsSE and builds the installer. Returns the file path of the installer.
    Assumes we are running from the <proj_root>/perftest directory
    """
    LOG.info("About to invoke the RapidsSE installer packager script to build and package RapidsSE.")
    # TODO: Remove the NO_MOD_CHECK and NO_BUILD below
    command = "NO_MOD_CHECK=1 NO_BUILD=1 ../installer/packaging/package.sh"

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
               "--script", "--debug", "--config", str(cluster_config_path)]

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

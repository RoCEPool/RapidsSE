import os.path
import pathlib
import logging
import subprocess
import tempfile

import pyutils.errors as errors
import pyutils.ssh as ssh
from pyutils.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)

C_GENERAL_SSH_TIMEOUT_SEC = 60

C_JDBC_DRIVER_DIR = pathlib.Path("../../shell")
C_JDBC_DRIVER_GLOB = "sejdbc-*.*.*.jar"

# Cached path to the found JDBC driver
C_JDBC_DRIVER: pathlib.Path = None
C_JDBC_URL_FMT = 'jdbc:se://localhost:{0}'

C_PERF_TESTS_DIR = "perftests"
C_SUPPORT_FILES_DIR = f"{C_PERF_TESTS_DIR}/supportFiles"


def copy_jdbc_driver(host: str, cluster_cfg: ClusterConfig):
    """
    Copy the RapidsSE JDBC driver to the installation directory of the given host.
    """

    src = find_local_jdbc_driver()
    dest = f"{host}:{cluster_cfg.get_dir()}/{src.name}"

    LOG.info(f"About to copy the JDBC driver to the first node of the cluster at: {dest}")

    try:
        ssh.run_scp(str(src), dest, 10)

    except errors.SshError as e:
        msg = f"Error while copying RapidsSE JDBC Driver to the first node."
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Copying of RapidsSE JDBC Driver to the first node timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.info("Copied the JDBC driver successfully")


def copy_support_files(host: str, cluster_cfg: ClusterConfig, src_folder: str):
    """
    Copy the supportFiles directory to the <install_dir>/perftests/supportFiles directory of the given host.
    """
    assert os.path.exists(src_folder), f"src_folder does not exist: {src_folder}"

    dest = f"{host}:{cluster_cfg.get_dir()}/{C_SUPPORT_FILES_DIR}"

    LOG.info(f"About to copy the test supportFiles folder to the first node of the cluster at: {dest}")

    try:
        ssh.run_scp(src_folder, dest, 10, recursive=True)

    except errors.SshError as e:
        msg = f"Error while copying the test supportFiles folder to the first node."
        raise errors.ClusterError(msg) from e

    except subprocess.TimeoutExpired as e:
        msg = f"Copying of the test supportFiles folder to the first node timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.ClusterError(msg) from e

    LOG.info("Copied the test supportFiles folder successfully")


def make_perftests_dir(host: str, cluster_cfg: ClusterConfig):
    """
    Makes the common perftests directory, to hold all tests, support files and their metrics
    """

    dest = f"{cluster_cfg.get_dir()}/{C_PERF_TESTS_DIR}"
    ssh_command = f"mkdir -p {dest}"

    LOG.info(f"About to create the perftests folder on the first node of the cluster at: {host}:{dest}")

    try:
        ssh.run_ssh(host, ssh_command, 10)

    except errors.SshError as e:
        msg = f"Error while creating the perftests folder on the first node."
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


def write_jmeter_property_file(host: str, cluster_cfg: ClusterConfig):
    """
    Generates a JMeter user property file and writes it to the node
    """

    jdbc_url = C_JDBC_URL_FMT.format(cluster_cfg.get_external_port())

    lines = [
        "# Classpath to find the RapidsSE JDBC driver & JMeter plugins",
        f"user.classpath={cluster_cfg.get_dir()}:{cluster_cfg.get_dir()}/{C_SUPPORT_FILES_DIR}",
        "",
        "# JDBC driver URL to use",
        f'JDBC_URL={jdbc_url}',
        "",
        "# Force output files to be flushed (lower performance)",
        "jmeter.save.saveservice.autoflush=true",
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

        except errors.SshError as e:
            msg = f"Error while JMeter user properties file to the first node."
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of JMeter user properties file to the first node timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

    LOG.info("Copied the JMeter user properties file successfully")


def get_remote_jmeter_property_file_path(cluster_cfg: ClusterConfig) -> str:
    """
    Returns the full path and filename of the jmeter user properties file
    to be used when executing jmeter.
    """
    return f"{cluster_cfg.get_dir()}/jmeter.properties"

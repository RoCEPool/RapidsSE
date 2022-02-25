import pathlib
import logging
import subprocess
import tempfile
import string

import septest.errors as errors

LOG = logging.getLogger(__name__)

# How long to wait before killing the RapidsSE installation?
C_INSTALL_TIMEOUT_SEC = 60


def build_installer() -> pathlib.Path:
    """
    Compiles RapidsSE and builds the installer. Returns the file path of the installer.
    Assumes we are running from the <proj_root>/perftest directory
    """
    LOG.info("About to invoke the RapidsSE installer packager script to build and package RapidsSE.")
    # TODO: Remove the NO_MOD_CHECK below
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
        raise errors.CompilationError(msg)

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
        raise errors.InstallationError(msg)

    except subprocess.TimeoutExpired as e:
        msg = f"Installation of RapidsSE timed out after {e.timeout} seconds." \
              f"\nStdout Output:\n{e.stdout}" \
              f"\nStderr Output:\n{e.stderr}"
        raise errors.InstallationError(msg)

    LOG.debug(f"Installation output (stdout):\n{result.stdout}")

    LOG.info("RapidsSE cluster installed successfully")

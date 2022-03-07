"""Runs a test file"""

import string
import logging
import os.path
import pathlib
import subprocess

import clusterops
from septest.results import TestResult
from septest import errors
from septest.configs.clusterconfig import ClusterConfig
from septest import ssh

LOG = logging.getLogger(__name__)

C_JMETER_EXTS = ["jmx", "jtl", "jmeter", "jmetre"]


class TestRunner:
    """
    Runs a test file and collects the results and metrics
    """

    def __init__(self, test_path: string, test_id: int, job_result_id: int, test_seq: int,
                 cluster_config: ClusterConfig, ip_addr: string):

        self.local_test_file = pathlib.Path(test_path)
        self.test_name = self.get_test_name_from_path(test_path)
        self.test_id = test_id
        self.test_seq = test_seq
        self.cluster_config = cluster_config
        self.ip_addr = ip_addr
        self.remote_dir = pathlib.Path(cluster_config.get_dir()).joinpath(
            "perftests",
            "j{:04d}_t{:02d}_{:02d}_{:s}".format(job_result_id, test_id, test_seq, self.test_name)
        )
        self.remote_test_file = None
        self.result = TestResult(self.test_name, test_id, job_result_id, self.remote_dir)

        assert self.local_test_file.exists()
        assert (self.test_name is not None) and (len(self.test_name) > 0)
        assert self.test_id >= 0

    def run_test(self):
        LOG.info(f"Running test:  {self.test_name} (ID: {self.test_id})")

        self.result.set_test_started()
        self.__copy_test_file()

        # Run the test
        splits = str(self.local_test_file).lower().rsplit(".", 1)
        if len(splits) == 2 and splits[1] in C_JMETER_EXTS:
            self.__run_jmeter_test()
        else:
            raise errors.NotImplementedYet("Only JMeter test files (*.[jmx | jtl | jmeter]) are currently supported")

        self.__collect_metrics()
        self.__cleanup_test_file()

        LOG.info(f"Finished test: {self.test_name} (ID: {self.test_id}). "
                 f"Result = {'SUCCESS' if self.result.test_passed else 'FAILURE'}")

    @staticmethod
    def get_test_name_from_path(test_path: string) -> string:
        """Returns the test name given its path"""
        return os.path.basename(test_path).rsplit(sep=".", maxsplit=1)[0]

    def __copy_test_file(self):
        """
        Copy the local test file to the remote node where it will be executed.
        """

        # First, create a directory on the remote node where we will copy the test
        # file to. Any metrics that are produced will be output in this folder.
        self.__create_remote_dir()

        test_filename = os.path.basename(self.local_test_file)
        self.remote_test_file = self.remote_dir / test_filename

        command_timeout = 10  # seconds
        src = str(self.local_test_file)
        dest = f"{self.ip_addr}:{self.remote_test_file}"

        LOG.debug(f"About to copy a test file from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, command_timeout)

        except subprocess.CalledProcessError as e:
            msg = f"Error while copying a test file from {src} to {dest}." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying test file from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.info(f"Successfully copied the test file from {src} to {dest}")

    def __create_remote_dir(self):
        """
        Creates a directory on the remote host to hold the test file and
        associated artifacts.
        """

        # Create a directory to hold the test file and associated metrics
        mkdir_command = f"mkdir -p {str(self.remote_dir)}"

        LOG.debug(f"About to make a directory to hold the test file on host {self.ip_addr}: {mkdir_command}")

        try:
            ssh.run_ssh(self.ip_addr, mkdir_command, 10)

        except subprocess.CalledProcessError as e:
            msg = f"Error while creating the following remote directory: {self.ip_addr}:{str(self.remote_dir)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Creation of remote directory timed out after {e.timeout} seconds: " \
                  f"{self.ip_addr}:{str(self.remote_dir)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.debug(f"Successfully created the remote directory: {self.ip_addr}:{str(self.remote_dir)}")

    def __run_jmeter_test(self):
        """
        Executes the JMeter test file and sets the result and end time of the test.
        """
        # Where did we copy the jmeter properties file to?
        jmeter_properties_path = clusterops.get_remote_jmeter_property_file_path(self.cluster_config)
        cluster_bin_dir = f"{self.cluster_config.get_dir()}/bin"
        log_file_path = self.__remote_log_file_path()

        # We need to run jmeter from the <install>/bin directory so that the JDBC driver
        # can invoke the SE prompt and the SE prompt can find libRapidsSE.so.
        # We also need to set the LD_LIBRARY_PATH too.
        jmeter_command = f"cd {cluster_bin_dir} && " \
                         "LD_LIBRARY_PATH=. jmeter -n " \
                         f"-p {jmeter_properties_path} " \
                         f"-t {self.remote_test_file} " \
                         f"-l {log_file_path}"

        LOG.debug(f"About to remove the JMeter command: {jmeter_command}")

        try:
            ssh.run_ssh(self.ip_addr, jmeter_command)
            LOG.info(f"Successfully ran the JMeter file at {self.ip_addr}:{self.remote_test_file}")

            # Record the test completion result
            self.result.set_test_ended(test_passed=True, test_skipped=False)

        except subprocess.CalledProcessError as e:
            msg = f"Error while trying to execute JMeter file at {self.ip_addr}:{self.remote_test_file}" \
                  f"\n\nStdout Output:\n{e.stdout}" \
                  f"\n\nStderr Output:\n{e.stderr}\n"
            self.result.set_test_ended(test_passed=False, test_skipped=False, stdout=e.stdout, stderr=e.stderr)
            LOG.error(msg)

    def __remote_log_file_path(self):
        """
        Returns the path of the log file that JMeter will produce on the first node.
        """
        remote_path = f"{self.remote_dir}/{self.test_name}.log"
        return remote_path

    def __collect_metrics(self):
        # TODO: Implement this
        # Copy any metrics files from the remote node
        # Parse the metrics
        # Add them to the results.
        pass

    def __cleanup_test_file(self):
        """
        We only delete the test file and not the metrics, because we may end up
        collecting and archiving the metrics. We don't need to archive the test
        file as well.
        """

        rm_command = f"rm -f {self.remote_test_file}"

        LOG.debug(f"About to remove the remote test file at {self.remote_test_file}")

        try:
            ssh.run_ssh(self.ip_addr, rm_command, 10)

        except subprocess.CalledProcessError as e:
            msg = f"Error while remote test file at {self.remote_test_file}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Deletion of remote test file at {self.remote_test_file} timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.debug(f"Successfully deleted the remote test file at {self.remote_test_file}")

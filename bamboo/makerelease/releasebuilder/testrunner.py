""""Runs the test jobs"""
import logging
import pathlib
import string
import threading
import datetime
import time
import subprocess
import multiprocessing

import pyutils.ec2 as ec2
import pyutils.ssh as ssh
import pyutils.errors as errors
from releasebuilder.releaseconfig import ReleaseConfig
from releasebuilder.results import TestResult
from pyutils.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)

C_TEST_NAME = "Test Release"
C_INSTANCE_NAME_PREFIX = "TestRelease"
C_INSTANCE_START_TIMEOUT = 600.0

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_EC2_BOOT_TIMEOUT_SEC = 300
C_SE_TEST_TIMEOUT_SEC = 1200
C_SE_INSTALL_TIMEOUT_SEC = 120
C_TEST_MONITORING_SLEEP_IVL = 10

C_SE_START_CMD = "startSE.sh"
C_SE_START_TIMEOUT_SEC = 90

C_SE_STOP_CMD = "stopSE.sh"
C_SE_STOP_TIMEOUT_SEC = 30

C_SE_COLLECT_LOGS_CMD = "collectLogs.sh"
C_SE_COLLECT_LOGS_TIMEOUT_SEC = 90
C_SE_COLLECT_LOGS_FILESPEC = "logs-*.tar.gz"
C_SE_COPY_LOGS_TIMEOUT_SEC = 90

# Paths are relative to the *repository*
# (because they are used on remote nodes)
C_TESTLIST_PATH = "bamboo/makerelease/configs/release.testlist"
C_SETH_RESULT_PATH = "bamboo/unittest/results/results-*.xml"
C_SETH_TIMEOUT_SEC = 300
C_COLLECT_RESULTS_TIMEOUT_SEC = 90


class TestRunner:

    def __init__(self,
                 artifact_path: pathlib.Path,
                 result_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 config: ReleaseConfig,
                 result_list: [TestResult],
                 installer_path: pathlib.Path):

        self.artifact_path = artifact_path
        self.result_path = result_path
        self.repo_path = repo_path
        self.config = config
        self.result_list = result_list
        self.installer_path = installer_path
        self.instance_runners = []
        self.instance_threads = []

        # Create all of the instance runners, but don't run them yet
        instance_type = self.config.get_build_instance()
        instance_params = list(zip(self.config.get_test_names(),
                                   self.config.get_test_images(),
                                   self.config.get_test_cloudinits_paths(),
                                   self.config.get_test_cluster_config_paths(),
                                   self.config.get_test_cluster_configs()))

        for param in instance_params:
            test_name: string = param[0]
            image_id: string = param[1]
            cloud_init_path: pathlib.Path = param[2]
            cluster_config_path: pathlib.Path = param[3]
            cluster_config: ClusterConfig = param[4]

            instance_runner = InstanceRunner(test_name,
                                             instance_type,
                                             image_id,
                                             pathlib.Path(cloud_init_path),
                                             cluster_config_path,
                                             cluster_config,
                                             self.artifact_path,
                                             self.result_path,
                                             self.repo_path,
                                             self.installer_path)
            self.instance_runners.append(instance_runner)

            # Create, but don't start the threads yet!
            # instance_thread = threading.Thread(target=instance_runner.run(), daemon=True, name=test_name)
            # self.instance_threads.append(instance_thread)

    def run(self):
        """
        Runs the logic that tests SE on a remote node with a particular image.
        """
        LOG.debug(f"About to start {len(self.instance_runners)} parallel processes for testing SE.")
        with multiprocessing.Pool() as pool:
            instance_results = pool.map(InstanceRunner.run, self.instance_runners)

        got_errors: bool = False
        for result in instance_results:
            self.result_list.append(result)
            got_errors = (got_errors and not result.test_passed)

        msg = "Completed testing RapidsSE." if got_errors is True else \
              "Completed testing RapidsSE, with install and run errors."
        LOG.info(msg)

    def __wait_for_tests_to_complete(self, timeout: float) -> bool:
        """
        Waits for all test threads to complete or a given timeout to elapse.
        Returns True if the threads exited before the timeout or False if the timeout was reached.
        """
        current_time = datetime.datetime.now()
        end_time = current_time + datetime.timedelta(seconds=timeout)

        time.sleep(C_TEST_MONITORING_SLEEP_IVL)
        while current_time < end_time:

            tests_running = len(self.instance_threads)

            for instance_thread in self.instance_threads:
                if not instance_thread.is_alive():
                    # test thread has ended normally
                    tests_running -= 1

            if tests_running == 0:
                return True

            # Try again in a little bit.
            time.sleep(C_TEST_MONITORING_SLEEP_IVL)
            current_time = datetime.datetime.now()

        # We have timed out
        return False


class InstanceRunner:
    def __init__(self,
                 test_name: string,
                 instance_type: string,
                 image_id: string,
                 cloudinit_path: pathlib.Path,
                 cluster_config_path: pathlib.Path,
                 cluster_config: ClusterConfig,
                 artifact_path: pathlib.Path,
                 result_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 local_installer_path: pathlib.Path):

        self.test_name: str = test_name
        self.instance_type = instance_type
        self.image_id = image_id
        self.cloudinit_path = cloudinit_path
        self.local_cluster_config_path = cluster_config_path
        self.cluster_config = cluster_config
        self.artifact_path = artifact_path
        self.result_path = result_path
        self.repo_path = repo_path
        self.local_installer_path = local_installer_path

        # Create a new result and add it to the list
        self.test_result = TestResult(f"Install & run ({self.test_name})")

        self.instance_id: string = None
        self.ip_addr: string = None
        self.remote_home_dir: pathlib.Path = None
        self.remote_repo_dir: pathlib.Path = None
        self.remote_installer_path: pathlib.Path = None
        self.remote_cluster_config_path: pathlib.Path = None
        self.can_collect_artifacts = False  # not yet

    def run(self) -> TestResult:
        """"""
        self.test_result.set_test_started()

        # We do the work in a thread so that we can watch for a timeout.
        # The thread doesn't end after a timeout, but we make a daemon thread
        # so that it does exit when the process exits.
        instance_thread = threading.Thread(target=self.__run_thread(), daemon=True)
        instance_thread.start()

        instance_thread.join(C_SE_TEST_TIMEOUT_SEC)
        if instance_thread.is_alive():
            self.force_stop()

        return self.test_result

    def __run_thread(self):
        """"""
        try:
            # Start an EC2 instance and wait for it to become available
            self.__start_test_instance()

            # Copy across the repository
            self.__copy_repo_to_instance()

            # Copy across the installer
            self.__copy_installer_to_instance()

            # Copy across the cluster config file
            self.__copy_cluster_config_to_instance()

            # Run the installer using a config file in the repository
            self.__install_se()

            # Create a symlink for <install_path>/DATA directory on the remote node
            self.__create_data_dir_symlink()

            # Start SE on the remote node
            self.__start_se()
            self.can_collect_artifacts = True

            # Run tests against SE
            self.__run_seth_tests()

            # Stop SE on the remote node
            self.__stop_se()

            # Mark the test as successful
            self.test_result.set_test_ended(True, False, None, None)

            LOG.info(f"Test {self.test_name}: Finished successfully")

        except Exception as e:
            # Record the failed test completion
            self.test_result.set_test_ended(False, False, None, str(e))

            if hasattr(e, 'msg'):
                LOG.exception(f"Test {self.test_name}: " + e.msg)
            else:
                LOG.exception(f"Error occurred during the test phase of test {self.test_name}.")

        finally:
            if self.ip_addr is not None:

                # Collect results from the remote node
                try:
                    self.__collect_seth_results()
                except (Exception,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect seth results")
                    LOG.exception(e)
                    # Otherwise carry on

                # Collect any log files
                try:
                    self.__collect_logs()
                except (Exception,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect SE logs")
                    LOG.exception(e)
                    # Otherwise carry on

                # This bit is REALLY important!
                self.__terminate_instance()

    def __start_test_instance(self) -> None:
        """Starts an EC2 instance to test RapidsSE"""
        # Start the EC2 instance to test RapidsSE.
        LOG.info(f"Test {self.test_name}: About to start an instance to test RapidsSE")

        instance_ids, ip_addrs = ec2.start_instances(self.instance_type,
                                                     self.image_id,
                                                     self.cloudinit_path,
                                                     1,
                                                     C_INSTANCE_NAME_PREFIX,
                                                     C_INSTANCE_START_TIMEOUT,
                                                     False)

        assert len(instance_ids) == 1
        assert len(ip_addrs) == 1
        self.instance_id = instance_ids[0]
        self.ip_addr = ip_addrs[0]

        # Get the absolute path of the home directory on the remote instance
        self.remote_home_dir = self.__get_instance_home_dir()
        self.remote_repo_dir = self.remote_home_dir / "repo"

    def __get_instance_home_dir(self) -> pathlib.Path:
        """Returns the absolute path of the user's home dir when we SSH to the remote node"""
        command = "pwd"
        LOG.debug(f"Test {self.test_name}: About to try to get the home directory of the default user "
                  f"on the remote EC2 instance")

        try:
            result: subprocess.CompletedProcess = ssh.run_ssh(self.ip_addr, command, 10)

        except errors.SshError:
            LOG.error(f"Test {self.test_name}: Error while trying to get the user home directory "
                      f"on host {self.ip_addr}: {command}")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Getting the user's home directory timed out after {e.timeout} seconds: " \
                  f"{command}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        home_dir = result.stdout.strip()
        assert home_dir is not None
        assert len(home_dir) > 0

        LOG.debug(f"Test {self.test_name}: Successfully got the home directory of the default user "
                  f"on the remote EC2 instance: {home_dir}")
        return pathlib.Path(home_dir)

    def __copy_repo_to_instance(self) -> None:
        """Copies the repository from the local machine to the remote instance"""

        command_timeout = 60  # seconds
        src = str(self.repo_path)
        dest = f"{self.ip_addr}:{str(self.remote_repo_dir)}"

        if not src.endswith("/"):
            src = src + "/"

        if not dest.endswith("/"):
            dest = dest + "/"

        LOG.debug(f"Test {self.test_name}: Copying repository from {src} to {dest}")

        # We use rsync here because scp doesn't copy hidden files and folders
        # Ref: https://serverfault.com/questions/21580/how-to-make-scp-copy-hidden-files
        command = ["rsync", "-avz", "-e", "ssh -q -o BatchMode=yes -o StrictHostKeyChecking=no", src, dest]

        LOG.debug(f"Test {self.test_name}: About to copy the repository from {src} to {dest}")

        try:
            subprocess.run(command,
                           capture_output=True,
                           check=True,
                           timeout=command_timeout,
                           text=True)

        except subprocess.CalledProcessError as e:
            msg = f"Test {self.test_name}: Error while copying the repository from {src} to {dest}." \
                  f"\nCommand: {' '.join(command)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Copying the repository from {src} to {dest} " \
                  f"timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: Successfully copied the repository from {src} to {dest}")

    def __copy_installer_to_instance(self) -> None:
        """Copies the installer to the remote instance. Sets remote_installer_path"""

        self.remote_installer_path = self.remote_home_dir / self.local_installer_path.name

        src = str(self.local_installer_path)
        dest = f"{self.ip_addr}:{self.remote_installer_path}"

        LOG.debug(f"Test {self.test_name}: Copying installer from {src} to {dest}")

        ssh.run_scp(src, dest)

    def __copy_cluster_config_to_instance(self) -> None:
        """Copies the cluster config file to the remote instance. Sets remote_cluster_config_path.
           Assumes we are only running SE nodes on one host, otherwise we would need to edit the
           cluster config file and change the hostnames to match the machines that were started."""

        self.remote_cluster_config_path = self.remote_home_dir / self.local_cluster_config_path.name

        src = str(self.local_cluster_config_path)
        dest = f"{self.ip_addr}:{self.remote_cluster_config_path}"

        LOG.debug(f"Test {self.test_name}: Copying cluster config file from {src} to {dest}")

        ssh.run_scp(src, dest)

    def __install_se(self) -> None:
        """Installs SE using the installer on the remote node"""

        LOG.debug(f"Test {self.test_name}: Installing RapidsSE")

        command = f"{str(self.remote_installer_path)} --quiet -- --script " \
                  f"--config {str(self.remote_cluster_config_path)}"

        try:
            ssh.run_ssh(self.ip_addr, command, C_SE_INSTALL_TIMEOUT_SEC)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Installation of SE on {self.ip_addr} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

    def __create_data_dir_symlink(self) -> None:
        """Create a symlink on the remote node from <install_dir>/DATA to <repo_root>/DATA"""
        symlink_path = pathlib.Path(self.cluster_config.get_dir()) / "DATA"
        dest_path = self.remote_repo_dir / "DATA"

        command = f"ln -s {str(dest_path)} {str(symlink_path)}"

        LOG.info(f"Test {self.test_name}: About to setup the data symlink with the following command: {command}")
        ssh.run_ssh(self.ip_addr, command)

    def __start_se(self) -> None:
        """Starts RapidsSE on the remote node"""

        start_command = f"{self.cluster_config.get_dir()}/{C_SE_START_CMD}"

        LOG.info(f"Test {self.test_name}: About to start the RapidsSE cluster with the "
                 f"following command: {start_command}")

        try:
            ssh.run_ssh(self.ip_addr, start_command, C_SE_START_TIMEOUT_SEC)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Starting of the SE cluster on {self.ip_addr} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: RapidsSE cluster started successfully")

    def __run_seth_tests(self) -> None:
        """Invokes SETH against the running cluster"""

        # We will reuse the test.sh script from <repo_root>/bamboo/unittest/scripts.
        # It accepts the following parameters:
        #   ./test.sh release-${TEST_NAME} path/of/cluster.config path/of/testlistFile

        test_sh_path = self.remote_repo_dir / "bamboo" / "unittest" / "scripts" / "test.sh"
        target_name = f"release-{self.test_name.replace(' ', '-')}"
        testlist_path = self.remote_repo_dir / C_TESTLIST_PATH

        test_suffix = "-" + target_name.replace(" ", "-")

        command = f"{test_sh_path} --testsuffix {test_suffix} " \
                  f"{target_name} " \
                  f"{str(self.remote_cluster_config_path)} " \
                  f"{str(testlist_path)}"

        LOG.info(f"Test {self.test_name}: About to invoke SETH on {self.ip_addr} with the following command: {command}")

        try:
            ssh.run_ssh(self.ip_addr, command, C_SETH_TIMEOUT_SEC)
            LOG.info(f"Test {self.test_name}: Completed running SETH on {self.ip_addr}")

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Running of SETH tests on {self.ip_addr} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

    def __stop_se(self) -> None:
        """Stops RapidsSE on the remote node"""

        stop_command = f"{self.cluster_config.get_dir()}/{C_SE_STOP_CMD}"

        LOG.info(f"Test {self.test_name}: About to stop the RapidsSE cluster with "
                 f"the following command: {stop_command}")

        try:
            ssh.run_ssh(self.ip_addr, stop_command, C_SE_STOP_TIMEOUT_SEC)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Stopping of the SE cluster on {self.ip_addr} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: RapidsSE cluster stopped successfully")

    def __collect_seth_results(self) -> None:
        """Collect SETH result files from the remote node and save them in the local results directory"""

        # Results have been written out to <repo_home>/bamboo/unittest/results
        src = f"{self.ip_addr}:{str(self.remote_repo_dir / C_SETH_RESULT_PATH)}"
        dest = str(self.result_path)

        LOG.debug(f"About to collect Seth logs from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, C_COLLECT_RESULTS_TIMEOUT_SEC)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Collecting of SETH results from the SE cluster on {self.ip_addr} " \
                  f"timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

    def __collect_logs(self) -> None:
        """Collect log files from the remote node"""

        collect_logs_command = f"{self.cluster_config.get_dir()}/{C_SE_COLLECT_LOGS_CMD}"

        LOG.info(f"Test {self.test_name}: About to invoke the collection of logs on {self.ip_addr}")

        try:
            ssh.run_ssh(self.ip_addr, collect_logs_command, C_SE_COLLECT_LOGS_TIMEOUT_SEC)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Collecting of logs from the SE cluster on {self.ip_addr} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        # Now we need to copy these logs back to the artifact directory and rename them for the current test.
        # Logs would have been written into the home directory. Let's find the exact name and path

        LOG.debug(f"Test {self.test_name}: About to look for the collected logs on {self.ip_addr}")

        find_logs_command = f"find {self.remote_home_dir} -name \"{C_SE_COLLECT_LOGS_FILESPEC}\""
        result = ssh.run_ssh(self.ip_addr, find_logs_command)

        assert result.stdout is not None
        assert len(result.stdout) > 0

        src_path: pathlib.Path = pathlib.Path(result.stdout.strip())

        # The destination filename is the original filename with the test name inserted
        # just before the suffix.
        # e.g. logs-20220622T12:01:01-CentOS-7.tar.gz
        dest_filename = f"{src_path.name.removesuffix(src_path.suffix)}-" \
                        f"{self.test_name.replace(' ', '-')}{src_path.suffix}"

        src = f"{self.ip_addr}:{result.stdout.strip()}"
        dest = str(self.artifact_path / dest_filename)

        LOG.debug(f"Test {self.test_name}: About to copy the logs from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, C_SE_COPY_LOGS_TIMEOUT_SEC)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Copying of logs from {src} to {dest} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: Successfully collected logs from {self.ip_addr} to {dest}")

    def __terminate_instance(self) -> None:
        """Terminates any previously started EC2 instance"""

        if self.instance_id is None:
            LOG.info(f"Test {self.test_name}: No EC2 instance to stop")
            return

        LOG.info(f"Test {self.test_name}: About to stop EC2 instance with ID: {self.instance_id}")
        ec2.terminate_instances([self.instance_id])

    def force_stop(self) -> None:
        """Forcible terminates an EC2 instance and collects logs if possible"""

        # Record the failed test completion
        self.test_result.set_test_ended(False, False, None,
                                        f"Test operation timed out after {C_SE_TEST_TIMEOUT_SEC} seconds")

        if self.ip_addr is None:
            return

        LOG.error(f"Forcibly stopping SE and trying to collect results and artifacts on {self.ip_addr}")

        # Stop SE on the remote node
        try:
            self.__stop_se()
        except (Exception,) as e:
            # Ignore any failures here
            LOG.error(f"Test {self.test_name}: Could not stop SE")
            LOG.exception(e)

        # Collect results from the remote node
        try:
            self.__collect_seth_results()
        except (Exception,) as e:
            # Ignore any failures here
            LOG.error(f"Test {self.test_name}: Could not collect SETH results")
            LOG.exception(e)

        # Collect any log files
        try:
            self.__collect_logs()
        except (Exception,) as e:
            # Ignore any failures here
            LOG.error(f"Test {self.test_name}: Could not collect SE log artifacts")
            LOG.exception(e)

        # Stop the EC2 instance
        try:
            self.__terminate_instance()

        except (Exception,):
            LOG.error(f"Test {self.test_name}: *** UNABLE TO FORCIBLY STOP EC2 INSTANCE {self.instance_id} ({self.ip_addr}). ***")

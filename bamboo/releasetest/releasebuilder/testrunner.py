""""Runs the test jobs"""
import logging
import os
import pathlib
import threading
import datetime
import time
import subprocess
import multiprocessing
import random

import pyutils.ec2 as ec2
import pyutils.ssh as ssh
import pyutils.errors as errors
from releasebuilder.releaseconfig import ReleaseConfig
from releasebuilder.results import TestResult
from pyutils.clusterconfig import ClusterConfig
import pyutils.clusterops as clusterops

LOG = logging.getLogger(__name__)

C_TEST_NAME: str = "Test Release"
C_INSTANCE_NAME_PREFIX: str = "ReleaseTest"
C_INSTANCE_START_TIMEOUT: float = 600.0

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_SE_TEST_TIMEOUT_SEC: int = 1200
C_SE_INSTALL_TIMEOUT_SEC: int = 120
C_TEST_MONITORING_SLEEP_IVL: int = 10

C_SE_START_CMD: str = "startSE.sh"
C_SE_START_TIMEOUT_SEC: int = 90

C_SE_STOP_CMD: str = "stopSE.sh"
C_SE_STOP_TIMEOUT_SEC: int = 30

C_SE_COLLECT_LOGS_CMD: str = "collectLogs.sh"
C_SE_COLLECT_LOGS_TIMEOUT_SEC: int = 90
C_SE_COLLECT_LOGS_FILESPEC: str = "logs-*.tar.gz"
C_SE_COPY_LOGS_TIMEOUT_SEC: int = 90

# Paths are relative to the *repository*
# (because they are used on remote nodes)
C_TESTLIST_PATH: str = "bamboo/releasetest/configs/release.testlist"
C_SETH_RESULT_PATH: str = "bamboo/unittest/results/*.xml"
C_SETH_LOG_PATH: str = "bamboo/unittest/logs"
C_SETH_TIMEOUT_SEC: int = 300
C_COLLECT_RESULTS_TIMEOUT_SEC: int = 90

C_CLOUD_INIT_LOG_PATH: pathlib.Path = pathlib.Path("/cloud-init-output.log")

# The file that contains the list of running instance IDs
C_EC2_INSTANCE_FILE_BASE: str = "runningInstances"
C_EC2_INSTANCE_FILE_EXT: str = "txt"
C_EC2_INSTANCE_FILE_GLOB: str = f"{C_EC2_INSTANCE_FILE_BASE}*.{C_EC2_INSTANCE_FILE_EXT}"


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
                                   self.config.get_test_iam_role_names(),
                                   self.config.get_test_cluster_config_paths(),
                                   self.config.get_test_cluster_configs()))

        base_cluster_id = random.randint(1, 9223372036854775807) // 1000 * 1000
        if base_cluster_id < 1000:
            base_cluster_id = 1000

        for param in instance_params:
            LOG.debug(f"InstanceRunner params: {str(param)}")

            test_name: str = param[0]
            image_id: str = param[1]
            cloud_init_path: pathlib.Path = param[2]
            iam_role_name: str = param[3]
            cluster_config_path: pathlib.Path = param[4]
            cluster_config: ClusterConfig = param[5]

            base_cluster_id += 1

            instance_runner = InstanceRunner(test_name,
                                             instance_type,
                                             image_id,
                                             pathlib.Path(cloud_init_path),
                                             iam_role_name,
                                             cluster_config_path,
                                             cluster_config,
                                             self.artifact_path,
                                             self.result_path,
                                             self.repo_path,
                                             self.installer_path,
                                             base_cluster_id,
                                             self.config.get_test_terminate())
            self.instance_runners.append(instance_runner)

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

        msg = "Completed testing RapidsSE." if got_errors is False else \
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
                 test_name: str,
                 instance_type: str,
                 image_id: str,
                 cloudinit_path: pathlib.Path,
                 iam_role_name: str,
                 cluster_config_path: pathlib.Path,
                 cluster_config: ClusterConfig,
                 artifact_path: pathlib.Path,
                 result_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 local_installer_path: pathlib.Path,
                 cluster_id: int,
                 terminate: bool):

        self.test_name: str = test_name
        self.instance_type = instance_type
        self.image_id = image_id
        self.cloudinit_path = cloudinit_path
        self.iam_role_name = iam_role_name
        self.local_cluster_config_path = cluster_config_path
        self.cluster_config = cluster_config
        self.artifact_path = artifact_path
        self.result_path = result_path
        self.repo_path = repo_path
        self.local_installer_path = local_installer_path
        self.cluster_id = cluster_id
        self.terminate = terminate

        # Create a new result and add it to the list
        self.test_result = TestResult(f"Install & run ({self.test_name})")

        self.instance_ids: [str] = []
        self.ip_addrs: [str] = []
        self.remote_home_dir: pathlib.Path = None
        self.remote_repo_dir: pathlib.Path = None
        self.remote_installer_path: pathlib.Path = None
        self.remote_cluster_config_path: pathlib.Path = None
        self.can_collect_artifacts = False  # not yet

        self.op_ctx = f"{self.test_name}: "

        self.runningInstancesPath: pathlib.Path = pathlib.Path(C_EC2_INSTANCE_FILE_BASE +
                                                               '_' +
                                                               self.test_name.replace(' ', '_') +
                                                               '.' +
                                                               C_EC2_INSTANCE_FILE_EXT)

    def run(self) -> TestResult:
        """"""
        self.test_result.set_test_started()

        # We do the work in a thread so that we can watch for a timeout.
        # The thread doesn't end after a timeout, but we make a daemon thread
        # so that it does exit when the process exits.
        instance_thread = threading.Thread(target=self.__run_thread(), daemon=True)
        instance_thread.start()

        instance_thread.join(C_SE_TEST_TIMEOUT_SEC)
        if instance_thread.is_alive() and self.terminate:
            self.force_stop()

        return self.test_result

    def __run_thread(self):
        """"""
        try:
            # Start an EC2 instance and wait for it to become available
            self.__start_test_instances()

            # Copy across the repository
            self.__copy_repo_to_all_instances()

            # Copy across the installer
            self.__copy_installer_to_first_instance()

            # Copy across the cluster config file
            self.__copy_cluster_config_to_first_instance()

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
            LOG.exception(f"Error occurred during the test phase of test {self.test_name}.")

        finally:
            if len(self.ip_addrs) > 0:

                # Collect cloud-init logs from the remote nodes
                try:
                    self.__collect_cloudinit_log()
                except (errors.SshError,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect cloud-init logs: {e.stderr}")
                    # Otherwise carry on

                except (Exception,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect cloud-init logs")
                    LOG.exception(e)
                    # Otherwise carry on

                # Collect results from the remote node
                try:
                    self.__collect_seth_results()
                except (errors.SshError,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect seth results: {e.stderr}")
                    # Otherwise carry on

                except (Exception,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect seth results")
                    LOG.exception(e)
                    # Otherwise carry on

                # Collect any log files
                try:
                    self.__collect_logs()

                except (errors.SshError,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect SE logs: {e.stderr}")
                    # Otherwise carry on

                except (Exception,) as e:
                    LOG.error(f"Test {self.test_name}: Unable to collect SE logs")
                    LOG.exception(e)
                    # Otherwise carry on

                # This bit is REALLY important!
                self.__terminate_instances()

    def __start_test_instances(self) -> None:
        """Starts an EC2 instance to test RapidsSE"""
        # Start the EC2 instance to test RapidsSE.
        num_instances = self.cluster_config.get_num_nodes()

        LOG.info(f"Test {self.test_name}: About to start {num_instances} instance(s) to test RapidsSE")

        name_tag = f"{C_INSTANCE_NAME_PREFIX}-{self.test_name.replace(' ', '-')}-"

        suffix = C_CLOUD_INIT_LOG_PATH.suffix
        cloudinit_dest_filename = f"{C_CLOUD_INIT_LOG_PATH.name.removesuffix(suffix)}-" \
                                  f"{self.test_name.replace(' ', '-')}.txt"
        cloudinit_dest = pathlib.Path(self.artifact_path / cloudinit_dest_filename)

        instance_ids, ip_addrs = ec2.start_instances(self.instance_type,
                                                     self.image_id,
                                                     str(self.cloudinit_path),
                                                     self.iam_role_name,
                                                     num_instances,
                                                     name_tag,
                                                     C_INSTANCE_START_TIMEOUT,
                                                     False,
                                                     ctx=self.op_ctx,
                                                     cloudinit_src=C_CLOUD_INIT_LOG_PATH,
                                                     cloudinit_dest=cloudinit_dest)

        # Record the started instances to file so that they can always be terminated by
        # a cleanup script
        ec2.record_started_instances(instance_ids, self.runningInstancesPath)

        assert len(instance_ids) == num_instances
        assert len(ip_addrs) == num_instances
        self.instance_ids = instance_ids
        self.ip_addrs = ip_addrs

        # Get the absolute path of the home directory on the remote instance
        self.remote_home_dir = self.__get_instance_home_dir()
        self.remote_repo_dir = self.remote_home_dir / "repo"

    def __get_instance_home_dir(self) -> pathlib.Path:
        """Returns the absolute path of the user's home dir when we SSH to the remote node"""
        command = "pwd"
        LOG.debug(f"Test {self.test_name}: About to try to get the home directory of the default user "
                  f"on the remote EC2 instance")

        try:
            result: subprocess.CompletedProcess = ssh.run_ssh(self.ip_addrs[0], command, 10, ctx=self.op_ctx)

        except errors.SshError:
            LOG.error(f"Test {self.test_name}: Error while trying to get the user home directory "
                      f"on host {self.ip_addrs[0]}: {command}")
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

    def __copy_repo_to_all_instances(self) -> None:
        """Copies the repository from the local machine to the remote instance"""

        command_timeout = 60  # seconds

        for ip in self.ip_addrs:
            src = str(self.repo_path)
            dest = f"{ip}:{str(self.remote_repo_dir)}"

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

    def __copy_installer_to_first_instance(self) -> None:
        """Copies the installer to the remote instance. Sets remote_installer_path"""

        self.remote_installer_path = self.remote_home_dir / self.local_installer_path.name

        src = str(self.local_installer_path)
        dest = f"{self.ip_addrs[0]}:{self.remote_installer_path}"

        LOG.debug(f"Test {self.test_name}: Copying installer from {src} to {dest}")

        ssh.run_scp(src, dest, ctx=self.op_ctx)

    def __copy_cluster_config_to_first_instance(self) -> None:
        """Copies the cluster config file to the remote instance. Sets remote_cluster_config_path.
           Assumes we are only running SE nodes on one host, otherwise we would need to edit the
           cluster config file and change the hostnames to match the machines that were started."""

        with clusterops.customise_cluster_cfg_hostnames(self.local_cluster_config_path,
                                                        self.ip_addrs,
                                                        new_cluster_id=self.cluster_id,
                                                        ctx=self.op_ctx) as modified_cluster_cfg:

            self.remote_cluster_config_path = self.remote_home_dir / self.local_cluster_config_path.name

            src = modified_cluster_cfg.name
            dest = f"{self.ip_addrs[0]}:{self.remote_cluster_config_path}"

            LOG.debug(f"Test {self.test_name}: Copying cluster config file from {src} to {dest}")

            ssh.run_scp(src, dest, ctx=self.op_ctx)

    def __install_se(self) -> None:
        """Installs SE using the installer on the remote node"""

        LOG.debug(f"Test {self.test_name}: Installing RapidsSE")

        command = f"{str(self.remote_installer_path)} --quiet -- --script " \
                  f"--config {str(self.remote_cluster_config_path)}"

        try:
            ssh.run_ssh(self.ip_addrs[0], command, C_SE_INSTALL_TIMEOUT_SEC, ctx=self.op_ctx)

        except subprocess.TimeoutExpired as e:
            ips = ", ".join(self.ip_addrs)
            msg = f"Test {self.test_name}: Installation of SE on [{ips}] timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

    def __create_data_dir_symlink(self) -> None:
        """Create a symlink on the remote node from <install_dir>/DATA to <repo_root>/DATA"""
        symlink_path = pathlib.Path(self.cluster_config.get_dir()) / "DATA"
        dest_path = self.remote_repo_dir / "DATA"

        command = f"ln -s {str(dest_path)} {str(symlink_path)}"

        LOG.info(f"Test {self.test_name}: About to setup the data symlink with the following command: {command}")
        for ip in self.ip_addrs:
            ssh.run_ssh(ip, command, ctx=self.op_ctx)

    def __start_se(self) -> None:
        """Starts RapidsSE on the remote node"""

        start_command = f"{self.cluster_config.get_dir()}/{C_SE_START_CMD}"

        LOG.info(f"Test {self.test_name}: About to start the RapidsSE cluster with the "
                 f"following command: {start_command}")

        try:
            ssh.run_ssh(self.ip_addrs[0], start_command, C_SE_START_TIMEOUT_SEC, ctx=self.op_ctx)

        except subprocess.TimeoutExpired as e:
            ips = ", ".join(self.ip_addrs)
            msg = f"Test {self.test_name}: Starting of the SE cluster on [{ips}] timed out " \
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
        log_path = self.remote_repo_dir / C_SETH_LOG_PATH
        testlist_path = self.remote_repo_dir / C_TESTLIST_PATH

        test_suffix = "-" + target_name.replace(" ", "-")

        command = f"{test_sh_path} --testsuffix {test_suffix} " \
                  f"{target_name} " \
                  f"{str(self.remote_cluster_config_path)} " \
                  f"{str(log_path)}" \
                  f"{str(testlist_path)}"

        LOG.info(f"Test {self.test_name}: About to invoke SETH on {self.ip_addrs[0]} "
                 f"with the following command: {command}")

        try:
            ssh.run_ssh(self.ip_addrs[0], command, C_SETH_TIMEOUT_SEC, ctx=self.op_ctx)
            LOG.info(f"Test {self.test_name}: Completed running SETH on {self.ip_addrs[0]}")

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Running of SETH tests on {self.ip_addrs[0]} timed out " \
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
            ssh.run_ssh(self.ip_addrs[0], stop_command, C_SE_STOP_TIMEOUT_SEC, ctx=self.op_ctx)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Stopping of the SE cluster on {self.ip_addrs[0]} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: RapidsSE cluster stopped successfully")

    def __collect_cloudinit_log(self) -> None:
        """Collect cloud-init log from each of the remote nodes"""

        last_error = None

        for ip in self.ip_addrs:
            # cloud-init logs have been written out to /cloud-init-output.log
            suffix = C_CLOUD_INIT_LOG_PATH.suffix
            dest_filename = f"{C_CLOUD_INIT_LOG_PATH.name.removesuffix(suffix)}-" \
                            f"{self.test_name.replace(' ', '-')}-{ip}.txt"

            src = f"{ip}:{str(C_CLOUD_INIT_LOG_PATH)}"
            dest = str(self.artifact_path / dest_filename)

            LOG.debug(f"About to collect cloud-init logs from {src} to {dest}")

            try:
                ssh.run_scp(src, dest, C_SE_COPY_LOGS_TIMEOUT_SEC, ctx=self.op_ctx)

            except subprocess.TimeoutExpired as e:
                msg = f"Test {self.test_name}: Collecting of cloud-init logs from the SE cluster on {ip} " \
                      f"timed out after {e.timeout} seconds." \
                      f"\nStdout Output:\n{e.stdout}" \
                      f"\nStderr Output:\n{e.stderr}"

                if last_error is None:
                    last_error = errors.SshError(msg, e.stdout, e.stderr)
                    last_error.__cause__ = e

                LOG.error(last_error)
                # Keep trying other instances in this test

        if last_error is not None:
            raise last_error

    def __collect_seth_results(self) -> None:
        """Collect SETH result files from the remote node and save them in the local results directory"""

        # Results have been written out to <repo_home>/bamboo/unittest/results
        src = f"{self.ip_addrs[0]}:{str(self.remote_repo_dir / C_SETH_RESULT_PATH)}"
        dest = str(self.result_path)

        LOG.debug(f"About to collect Seth logs from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, C_COLLECT_RESULTS_TIMEOUT_SEC, ctx=self.op_ctx)

        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Collecting of SETH results from the SE cluster on {self.ip_addrs[0]} " \
                  f"timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

    def __collect_logs(self) -> None:
        """Collect log files from the remote node"""

        # The logs should each node should already have been copied to the first node, so we
        # just need to get them from there and make sure they go into an appropriate subdirectory.
        dest = str(self.artifact_path / self.test_name.replace(' ', '-'))
        LOG.debug(f"Making directory for output logs: {dest}")
        os.makedirs(dest, exist_ok=True)

        # Copy all the files into this directory
        src = f"{self.ip_addrs[0]}:{str(self.remote_repo_dir / C_SETH_LOG_PATH)}"

        try:
            ssh.run_scp(src, dest, C_SE_COPY_LOGS_TIMEOUT_SEC, ctx=self.op_ctx)
        except subprocess.TimeoutExpired as e:
            msg = f"Test {self.test_name}: Copying of logs from {src} to {dest} timed out " \
                  f"after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: Successfully collected logs from {self.ip_addrs[0]} to {dest}")

#
#
#        collect_logs_command = f"{self.cluster_config.get_dir()}/{C_SE_COLLECT_LOGS_CMD}"
#
#        LOG.info(f"Test {self.test_name}: About to invoke the collection of logs on {self.ip_addrs[0]}")
#
#        try:
#            ssh.run_ssh(self.ip_addrs[0], collect_logs_command, C_SE_COLLECT_LOGS_TIMEOUT_SEC, ctx=self.op_ctx)
#
#        except subprocess.TimeoutExpired as e:
#            msg = f"Test {self.test_name}: Collecting of logs from the SE cluster on {self.ip_addrs[0]} timed out " \
#                  f"after {e.timeout} seconds." \
#                  f"\nStdout Output:\n{e.stdout}" \
#                  f"\nStderr Output:\n{e.stderr}"
#            raise errors.SshError(msg, e.stdout, e.stderr) from e
#
#        # Now we need to copy these logs back to the artifact directory and rename them for the current test.
#        # Logs would have been written into the home directory. Let's find the exact name and path
#
#        LOG.debug(f"Test {self.test_name}: About to look for the collected logs on {self.ip_addrs[0]}")
#
#        find_logs_command = f"find {self.remote_home_dir} -name \"{C_SE_COLLECT_LOGS_FILESPEC}\""
#        result = ssh.run_ssh(self.ip_addrs[0], find_logs_command, ctx=self.op_ctx)
#
#        assert result.stdout is not None
#        assert len(result.stdout) > 0
#
#        src_path: pathlib.Path = pathlib.Path(result.stdout.strip())
#
#        # The destination filename is the original filename with the test name inserted
#        # just before the suffix.
#        # e.g. logs-20220622T12:01:01-CentOS-7.tar.gz
#        suffix = ".tar.gz"
#        dest_filename = f"{src_path.name.removesuffix(suffix)}-" \
#                        f"{self.test_name.replace(' ', '-')}{suffix}"
#
#        src = f"{self.ip_addrs[0]}:{result.stdout.strip()}"
#        dest = str(self.artifact_path / dest_filename)
#
#        LOG.debug(f"Test {self.test_name}: About to copy the logs from {src} to {dest}")
#
#        try:
#            ssh.run_scp(src, dest, C_SE_COPY_LOGS_TIMEOUT_SEC, ctx=self.op_ctx)
#
#        except subprocess.TimeoutExpired as e:
#            msg = f"Test {self.test_name}: Copying of logs from {src} to {dest} timed out " \
#                  f"after {e.timeout} seconds." \
#                  f"\nStdout Output:\n{e.stdout}" \
#                  f"\nStderr Output:\n{e.stderr}"
#            raise errors.SshError(msg, e.stdout, e.stderr) from e
#
#        LOG.info(f"Test {self.test_name}: Successfully collected logs from {self.ip_addrs[0]} to {dest}")

    def __terminate_instances(self) -> None:
        """Terminates any previously started EC2 instance"""

        if self.terminate is False:
            LOG.info(f"Test {self.test_name}: Not stopping instances because of the 'terminate' "
                     f"setting in the test configuration")

            # Remove the file that contains the list of instances that were started
            if os.path.exists(self.runningInstancesPath):
                os.remove(self.runningInstancesPath)
            return

        if self.instance_ids is None:
            LOG.info(f"Test {self.test_name}: No EC2 instance to stop")
            return

        ids = ", ".join(self.instance_ids)
        LOG.info(f"Test {self.test_name}: About to stop EC2 instance(s) with IDs: {ids}")
        ec2.terminate_instances(self.instance_ids, ctx=self.op_ctx)

        # Remove the file that contains the list of instances that were started
        if os.path.exists(self.runningInstancesPath):
            LOG.info(f"Test {self.test_name}: Removing running instance file {str(self.runningInstancesPath)}")
            os.remove(self.runningInstancesPath)

    def force_stop(self) -> None:
        """Forcible terminates an EC2 instance and collects logs if possible"""

        # Record the failed test completion
        self.test_result.set_test_ended(False, False, None,
                                        f"Test operation timed out after {C_SE_TEST_TIMEOUT_SEC} seconds")

        if len(self.ip_addrs) == 0:
            return

        ids = ", ".join(self.instance_ids)
        ips = ", ".join(self.ip_addrs)
        LOG.error(f"Forcibly stopping SE and trying to collect results and artifacts on [{ips}]")

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
            self.__terminate_instances()

        except (Exception,):
            LOG.error(f"Test {self.test_name}: *** UNABLE TO FORCIBLY STOP EC2 INSTANCES {ids} ({ips}). ***")


def terminate_instances_from_file():
    """If an instance file exists, then terminate the instances running in it."""

    files = pathlib.Path('.').glob(C_EC2_INSTANCE_FILE_GLOB)

    count = 0

    for file in files:
        try:
            count += 1
            ec2.terminate_instances_from_file(file)

        finally:
            if os.path.exists(file):
                os.remove(file)

    LOG.info(f"Found {count} files with running instances to be terminated.")

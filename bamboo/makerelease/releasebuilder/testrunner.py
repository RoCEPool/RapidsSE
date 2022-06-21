""""Runs the test jobs"""
import logging
import pathlib
import string
import threading
import datetime
import time
import subprocess

import common.ec2 as ec2
import common.ssh as ssh
import common.errors as errors
from common.releaseconfig import ReleaseConfig
from releasebuilder.results import TestResult

LOG = logging.getLogger(__name__)

C_TEST_NAME = "Test Release"
C_INSTANCE_NAME_PREFIX = "TestRelease"
C_INSTANCE_START_TIMEOUT = 600.0

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_EC2_BOOT_TIMEOUT_SEC = 300
C_SE_TEST_TIMEOUT_SEC = 1200
C_TEST_MONITORING_SLEEP_IVL = 10


class TestRunner:

    def __init__(self,
                 artifact_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 config: ReleaseConfig,
                 result_list: [TestResult],
                 installer_path: pathlib.Path):

        self.artifact_path = artifact_path
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
                                   self.config.get_test_cloudinits()))

        for param in instance_params:
            test_name = param[0]
            image_id = param[1]
            cloud_init_path = param[2]

            instance_runner = InstanceRunner(test_name,
                                             instance_type,
                                             image_id,
                                             pathlib.Path(cloud_init_path),
                                             self.artifact_path,
                                             self.repo_path,
                                             self.result_list,
                                             self.installer_path)
            self.instance_runners.append(instance_runner)

            # Create, but don't start the threads yet!
            instance_thread = threading.Thread(target=instance_runner.run(), daemon=True, name=test_name)
            self.instance_threads.append(instance_thread)

    def run(self):
        """
        Runs the logic that tests SE on a remote node with a particular image.
        """
        # Start each of the threads and wait for them to complete, with a timeout.
        for instance_thread in self.instance_threads:
            instance_thread.start()

        # Wait for all threads to complete
        if not self.__wait_for_tests_to_complete(C_SE_TEST_TIMEOUT_SEC):
            # Handle the timeout
            for instance_runner, instance_thread in zip(self.instance_runners, self.instance_threads):
                if not instance_thread.is_alive():
                    instance_runner.force_stop()

        else:
            LOG.info("Completed testing RapidsSE.")

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
                 artifact_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 result_list: [TestResult],
                 local_installer_path: pathlib.Path):

        self.test_name = test_name
        self.instance_type = instance_type
        self.image_id = image_id
        self.cloudinit_path = cloudinit_path
        self.artifact_path = artifact_path
        self.repo_path = repo_path
        self.local_installer_path = local_installer_path

        # Create a new result and add it to the list
        self.test_result = TestResult(self.test_name)
        result_list.append(self.test_result)

        self.instance_id: string = None
        self.ip_addr: string = None
        self.remote_home_dir: pathlib.Path = None
        self.remote_repo_dir: pathlib.Path = None
        self.remote_installer_path: pathlib.Path = None

    def run(self):
        self.test_result.set_test_started()

        try:
            # Start an EC2 instance and wait for it to become available
            self.__start_test_instance()

            # Copy across the repository
            self.__copy_repo_to_instance()

            # Copy across the installer
            self.__copy_installer_to_instance()

            # TODO: Run the installer using a config file in the repository

            # TODO: Start SE on the remote node

            # TODO: Run tests against SE

            # TODO: Collect results from the remote node

            # TODO: Stop SE on the remote node

            # TODO: Collect any log files

            # Mark the test as successful
            self.test_result.set_test_ended(True, False, None, None)

        except Exception as e:
            # Record the failed test completion
            self.test_result.set_test_ended(False, False, None, str(e))

            if hasattr(e, 'msg'):
                LOG.exception(f"Test {self.test_name}: " + e.msg)
            else:
                LOG.exception(f"Error occurred during the test phase of test {self.test_name}.")

        finally:
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
                                                     C_INSTANCE_START_TIMEOUT)

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
            msg = f"Test {self.test_name}: Copying the repository from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Test {self.test_name}: Successfully copied the repository from {src} to {dest}")

    def __copy_installer_to_instance(self) -> None:
        """Copies the installer to the remote instance. Sets remote_installer_path"""

        self.remote_installer_path = self.remote_home_dir / self.local_installer_path.name

        src = str(self.local_installer_path)
        dest = f"{self.ip_addr}:{self.remote_installer_path}"

        ssh.run_scp(src, dest)

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

        try:
            # TODO: Stop SE on the remote node

            # TODO: Collect any log files
            pass

        except (Exception,):
            pass        # Ignore any exceptions

        finally:
            # Stop the EC2 instance
            try:
                self.__terminate_instance()

            except (Exception,):
                LOG.error(f"*** Unable to forcibly stop EC2 instance {self.instance_id} ({self.ip_addr}). ***")

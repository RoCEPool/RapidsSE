""""Runs the build job"""
import logging
import pathlib
import subprocess
import os
import fnmatch

import pyutils.ec2 as ec2
import pyutils.ssh as ssh
import pyutils.errors as errors
from releasebuilder.releaseconfig import ReleaseConfig
from releasebuilder.results import TestResult

LOG = logging.getLogger(__name__)

C_TEST_NAME = "Build Release"
C_INSTANCE_NAME_PREFIX = "ReleaseBuild"
C_INSTANCE_START_TIMEOUT = 900.0
C_INSTALLER_FILESPEC = "rapidsse-*.run"

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_SE_BUILD_TIMEOUT_SEC = 1200


class BuildRunner:

    def __init__(self,
                 artifact_path: pathlib.Path,
                 repo_path: pathlib.Path,
                 config: ReleaseConfig,
                 result_list: [TestResult]):

        self.artifact_path = artifact_path
        self.repo_path = repo_path
        self.config = config
        self.result_list = result_list
        self.test_result: TestResult = None
        self.instance_id: str = None
        self.ip_addr: str = None
        self.remote_home_dir: pathlib.Path = None
        self.remote_repo_dir: pathlib.Path = None
        self.installer_path: pathlib.Path = None

        self.ctx = f"{self.config.get_build_name()} (build): "

    def run(self):
        """
        Runs the logic that builds SE on a remote node with a particular image.
        """

        # Create a test result and add it to the list
        self.__create_test()

        try:
            # Start the EC2 instance to build SE
            self.__start_build_instance()

            # Copy the repo to this instance
            self.__copy_repo_to_instance()

            # Build SE on this instance
            self.__build_se_remotely()

            # Copy the installer from this instance to the artifacts directory
            self.__collect_installer()

            # Record the successful test completion
            self.test_result.set_test_ended(True, False)

            LOG.info("Successfully built RapidsSE.")

        except Exception as e:
            # Record the failed test completion
            self.test_result.set_test_ended(False, False, None, str(e))
            if hasattr(e, 'msg'):
                LOG.exception(e.msg)
            else:
                LOG.exception("Error occurred during the build phase.")

        finally:
            if self.config.get_build_terminate():
                self.__terminate_build_instance()
            else:
                LOG.info(f"NOT stopping instance {self.instance_id} at {self.ip_addr} "
                         f"as instructed by the config file")

    def __create_test(self) -> None:
        """ Create a test result and add it to the list """
        self.test_result = TestResult(C_TEST_NAME)
        self.test_result.set_test_started()
        self.result_list.append(self.test_result)

    def __start_build_instance(self) -> None:
        """Start the build EC2 instance and save its instance_id and ip_addr"""

        LOG.debug(f"Getting the image name for image_id: {self.config.get_build_image()}")
        image_name = ec2.get_image_desc(self.config.get_build_image(), ctx=self.ctx)

        # Start the EC2 instance to build RapidsSE.
        LOG.info(f"About to start the instance to build RapidsSE on "
                 f"(instance type: {self.config.get_build_instance()} ; "
                 f"image name: {image_name}")

        name_tag = f"{C_INSTANCE_NAME_PREFIX}-{self.config.get_build_name().replace(' ', '-')}-"

        instance_ids, ip_addrs = ec2.start_instances(self.config.get_build_instance(),
                                                     self.config.get_build_image(),
                                                     self.config.get_build_cloudinit(),
                                                     self.config.get_build_iam_role_name(),
                                                     1,
                                                     name_tag,
                                                     C_INSTANCE_START_TIMEOUT,
                                                     False,
                                                     ctx=self.ctx)

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
        LOG.debug("About to try to get the home directory of the default user on the remote EC2 instance")

        try:
            result: subprocess.CompletedProcess = ssh.run_ssh(self.ip_addr, command, 10, ctx=self.ctx)

        except errors.SshError:
            LOG.error(f"Error while trying to get the user home directory on host {self.ip_addr}: {command}")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Getting the user's home directory timed out after {e.timeout} seconds: " \
                  f"{command}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        home_dir = result.stdout.strip()
        assert home_dir is not None
        assert len(home_dir) > 0

        LOG.debug(f"Successfully got the home directory of the default user on the remote EC2 instance: {home_dir}")
        return pathlib.Path(home_dir)

    def __terminate_build_instance(self) -> None:
        """Terminates any previously started EC2 instance"""

        if self.instance_id is None:
            LOG.info("No EC2 instance to stop")
            return

        LOG.info(f"About to stop EC2 instance with ID: {self.instance_id}")
        ec2.terminate_instances([self.instance_id], ctx=self.ctx)

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

        LOG.debug(f"About to copy the repository from {src} to {dest}")

        try:
            subprocess.run(command,
                           capture_output=True,
                           check=True,
                           timeout=command_timeout,
                           text=True)

        except subprocess.CalledProcessError as e:
            msg = f"Error while copying the repository from {src} to {dest}." \
                  f"\nCommand: {' '.join(command)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr, e.returncode) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying the repository from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Successfully copied the repository from {src} to {dest}")

    def __build_se_remotely(self) -> None:
        """Builds SE on the remote EC2 instance"""

        command = "DEBUG=1 " + str(self.remote_repo_dir / "installer" / "packaging" / "package.sh")

        LOG.debug("About to build RapidsSE on the remote EC2 instance")

        try:
            result = ssh.run_ssh(self.ip_addr, command, C_SE_BUILD_TIMEOUT_SEC, ctx=self.ctx)

            # Write build log to the artifact path
            self.__write_build_log_as_artifact(result.stdout, result.stderr)
            self.__collect_cloud_init_output()

        except errors.SshError as e:
            LOG.error(f"Error while trying to build RapidsSE on host {self.ip_addr}: {command}. "
                      f"Refer to the build log artifacts.")

            # Write build log to the artifact path
            self.__write_build_log_as_artifact(e.stdout, e.stderr)

            # Get the cloud-init output
            try:
                self.__collect_cloud_init_output()
            except errors.SshError:
                pass

            # Don't display a giant output message
            e.stdout = None
            e.stderr = None
            raise

        except subprocess.TimeoutExpired as e:
            # Write build log to the artifact path
            self.__write_build_log_as_artifact(e.stdout, e.stderr)

            # Get the cloud-init output
            try:
                self.__collect_cloud_init_output()
            except errors.SshError:
                pass

            msg = f"Building RapidsSE timed out after {e.timeout} seconds: " \
                  f"{command}. Refer to the build log artifacts."
            raise errors.SshError(msg, None, None) from e

        LOG.debug("Successfully built RapidsSE on the remote EC2 instance")

    def __write_build_log_as_artifact(self, stdout: str, stderr: str) -> None:
        """Writes stdout and stderr out to files in the artifacts directory"""
        if stdout is not None and len(stdout) > 0:
            stdout_path: pathlib.Path
            stdout_path = self.artifact_path / "build_stdout.txt"
            with open(stdout_path, "w") as f:
                f.write(stdout)

        if stderr is not None and len(stderr) > 0:
            stderr_path: pathlib.Path
            stderr_path = self.artifact_path / "build_stderr.txt"
            with open(stderr_path, "w") as f:
                f.write(stderr)

    def __collect_cloud_init_output(self) -> None:
        """Copy the /cloud-init-output.log file to the artifacts directory."""

        command_timeout = 60  # seconds

        remote_filespec = "/cloud-init-output.log"
        src = f"{self.ip_addr}:{remote_filespec}"
        dest = f"{str(self.artifact_path)}"

        LOG.debug(f"About to copy the cloud-init-log from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, timeout=command_timeout, ctx=self.ctx)

        except errors.SshError:
            LOG.error(f"Error while copying the cloud-init-log from {src} to {dest}.")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Copying the cloud-init-log from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Successfully copied the cloud-init-log from {src} to {dest}")

        # Copy the tex-live output too

        remote_filespec = "/cloud-init-texlive.log"
        src = f"{self.ip_addr}:{remote_filespec}"
        dest = f"{str(self.artifact_path)}"

        LOG.debug(f"About to copy the texlive log from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, timeout=command_timeout, ctx=self.ctx)

        except errors.SshError:
            LOG.error(f"Error while copying the texlive log from {src} to {dest}.")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Copying the texlive-log from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Successfully copied the texlive log from {src} to {dest}")

    def __collect_installer(self) -> None:
        """Copy the installer from the remote machine to the artifacts directory
           and remember its location.
        """

        command_timeout = 60  # seconds

        remote_filespec = str(self.remote_repo_dir / "installer" / "output" / "*.run")
        src = f"{self.ip_addr}:{remote_filespec}"
        dest = f"{str(self.artifact_path)}"

        LOG.debug(f"About to copy the RapidsSE installer from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, command_timeout, ctx=self.ctx)

        except errors.SshError:
            LOG.error(f"Error while copying the RapidsSE installer from {src} to {dest}.")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Copying the RapidsSE installer from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.info(f"Successfully copied the RapidsSE installer from {src} to {dest}")

        for root, dirs, files in os.walk(str(self.artifact_path)):
            if self.installer_path is not None:
                break

            for name in files:
                if fnmatch.fnmatch(name, C_INSTALLER_FILESPEC):
                    self.installer_path = pathlib.Path(root) / name
                    LOG.info(f"The RapidsSE installer is found at {str(self.installer_path)}")
                    break

        if self.installer_path is None:
            msg = f"Could not find the RapidsSE installer in local directory: {str(self.artifact_path)}"
            raise errors.ClusterError(msg)

        # Now copy the checksums as artifacts too
        remote_filespec = str(self.remote_repo_dir / "installer" / "output" / "checksum*")
        src = f"{self.ip_addr}:{remote_filespec}"
        dest = f"{str(self.artifact_path)}"

        LOG.debug(f"About to copy the installer checksums from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, command_timeout, ctx=self.ctx)

        except errors.SshError:
            LOG.error(f"Error while copying the installer checksums from {src} to {dest}.")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Copying the installer checksums from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.debug(f"Successfully copied the installer checksums from {src} to {dest}")

        # Finally copy the documentation as artifacts too
        remote_filespec = str(self.remote_repo_dir / "installer" / "output" / "*.zip")
        src = f"{self.ip_addr}:{remote_filespec}"
        dest = f"{str(self.artifact_path)}"

        LOG.debug(f"About to copy the documentation from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, command_timeout, ctx=self.ctx)

        except errors.SshError:
            LOG.error(f"Error while copying the documentation from {src} to {dest}.")
            raise

        except subprocess.TimeoutExpired as e:
            msg = f"Copying the documentation from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.SshError(msg, e.stdout, e.stderr) from e

        LOG.debug(f"Successfully copied the documentation from {src} to {dest}")

    def get_installer_path(self) -> pathlib.Path:
        """Returns the path of the local installer file"""
        return self.installer_path

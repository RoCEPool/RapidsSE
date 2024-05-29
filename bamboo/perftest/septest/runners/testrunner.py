"""Runs a test file"""
import datetime
import time
import glob
import string
import logging
import os
import pathlib
import subprocess
import csv
import decimal

from pyutils import clusterops, ssh, errors
from septest import testops
from septest.results import TestResult
from pyutils.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)

C_JMETER_BINARY = "/opt/jmeter/bin/jmeter"
C_JMETER_STOPTEST = "/opt/jmeter/bin/stoptest.sh"
C_JMETER_SHUTDOWN = "/opt/jmeter/bin/shutdown.sh"
C_JMETER_EXTS = ["jmx", "jmeter"]
C_SUPPORT_FILES = "supportFiles"
C_TEST_DIR_FMT = "j{:04d}_t{:02d}_{:02d}_{:s}"
C_SCP_TIMEOUT_SEC = 60
C_CLOUD_INIT_OUTPUT = "/cloud-init-output.log"
C_TEST_FINISH_SLEEP_IVL = 2
C_TEST_FINISH_TIMEOUT = 120

# CSV fields
C_CSV_LABEL = "label"
C_CSV_SUCCESS = "success"
C_CSV_LATENCY = "Latency"
C_CSV_TIMESTAMP = "timeStamp"
C_CSV_FAILUREMSG = "failureMessage"
C_CSV_RESPONSEMSG = "responseMessage"
C_CSV_CONNECT = "Connect"

C_MANDATORY_CSV_FIELDS = [
    C_CSV_LABEL,
    C_CSV_SUCCESS,
    C_CSV_LATENCY
]

C_OPTIONAL_CSV_FIELDS = [
    C_CSV_TIMESTAMP,
    C_CSV_FAILUREMSG,
    C_CSV_RESPONSEMSG,
    C_CSV_CONNECT
]

# TSV fields
C_TSV_LABEL = "label"
C_TSV_VALUE = "value"

C_MANDATORY_TSV_FIELDS = [
    C_TSV_LABEL,
    C_TSV_VALUE
]


class TestRunner:
    """
    Runs a test file and collects the results and metrics
    """

    def __init__(self, test_path: string, test_id: int, job_result_id: int, test_seq: int,
                 cluster_config: ClusterConfig, ip_addr: string, artifact_dir: string):

        self.local_test_file = pathlib.Path(test_path)
        self.test_name = self.get_test_name_from_path(test_path)
        self.job_result_id = job_result_id
        self.test_id = test_id
        self.test_seq = test_seq
        self.cluster_config = cluster_config
        self.ip_addr = ip_addr
        self.artifact_dir = artifact_dir
        self.remote_test_dir = pathlib.Path(cluster_config.get_dir()).joinpath(
            testops.C_PERF_TESTS_DIR,
            C_TEST_DIR_FMT.format(job_result_id, test_id, test_seq, self.test_name)
        )
        self.remote_test_file = None
        self.result = TestResult(test_path, self.test_name, test_id, job_result_id, self.remote_test_dir)
        self.test_finished = False

        assert self.local_test_file.exists()
        assert (self.test_name is not None) and (len(self.test_name) > 0)
        assert self.test_id >= 0

    def run_test(self):
        LOG.info(f"Running test:  {self.test_name} (ID: {self.test_id})")

        self.result.set_test_started()

        # First, create a directory on the remote node where we will copy the test
        # file to. Any metrics that are produced will be output in this folder.
        self.__create_remote_dir()
        self.__copy_test_file_to_remote_node()
        self.__create_support_files_symlink()

        # Run the test
        splits = str(self.local_test_file).lower().rsplit(".", 1)
        if len(splits) == 2 and splits[1] in C_JMETER_EXTS:
            self.__run_jmeter_test()
        else:
            raise errors.NotImplementedYet("Only JMeter test files (*.[jmx | jtl | jmeter]) are currently supported")

        # The test may have ended with an error, or possibly because the test hung or ran out
        # of time and the machines got shut down. In either case we want to try to collect
        # the test artifacts, though this may produce additional errors.
        try:
            self.collect_test_artifacts()
        except errors.ClusterError as e:
            msg = f"Error detected while trying to clean up test and collect metrics:\n" \
                  f"{str(e)}"
            LOG.exception(msg, e)

        LOG.info(f"Finished test: {self.test_name} (ID: {self.test_id}). "
                 f"Result = {'SUCCESS' if self.result.test_passed else 'FAILURE'}")
        self.test_finished = True

    def notify_stop_jmeter_test(self):
        """
        Attempt to notify and force exit a running JMeter test.
        We do this by calling a JMeter script on the remote node which sends
        a UDP message to the listening JMeter process. We will wait a little
        bit of time for this to complete.
        """

        # First we try a shutdown command. This will stop the test after the current
        # command has finished executing. This should hopefully not take too long
        # (unless it is a load command running).
        LOG.info(f"Attempting to gracefully notify the running JMeter process that it should exit "
                 f"with the command: {C_JMETER_SHUTDOWN}")

        timeout = 10
        success = False

        try:
            ssh.run_ssh(self.ip_addr, C_JMETER_SHUTDOWN, timeout=timeout)
            success = True

        except errors.SshError as e:
            LOG.exception("Encountered an error while trying to notify the JMeter process to shutdown.", e)

        except subprocess.TimeoutExpired:
            LOG.error(f"Timed out while trying to notify the JMeter process to shutdown ({timeout} sec).")

        if success and self.__wait_for_test_finish(C_TEST_FINISH_TIMEOUT):
            return

        # The above didn't work so we need to be more forceful. When we run
        # a stop command then JMeter will try to exit immediately.
        self.hard_stop_jmeter_test()

    def hard_stop_jmeter_test(self):
        """
        Attempt to hard stop and force exit a running JMeter test.
        We do this by calling a JMeter script on the remote node which sends
        a UDP message to the listening JMeter process. We will wait a little
        bit of time for this to complete.
        """

        timeout = 10
        success = False

        LOG.info(f"Attempting to forcefully stop the running JMeter process "
                 f"with the command: {C_JMETER_STOPTEST}")

        try:
            ssh.run_ssh(self.ip_addr, C_JMETER_STOPTEST, timeout=timeout)
            success = True

        except errors.SshError as e:
            LOG.exception("Encountered an error while trying to forcefully stop the JMeter process.", e)

        except subprocess.TimeoutExpired:
            LOG.error(f"Timed out while trying to forcefully stop the JMeter process ({timeout} sec).")

        if success:
            self.__wait_for_test_finish(C_TEST_FINISH_TIMEOUT)

    def collect_test_artifacts(self):
        """Copies logs and artifacts back to artifact directory"""
        self.__cleanup_test()
        self.__collect_metrics()

    @staticmethod
    def get_test_name_from_path(test_path: string) -> string:
        """Returns the test name given its path"""
        return os.path.basename(test_path).rsplit(sep=".", maxsplit=1)[0]

    def __copy_test_file_to_remote_node(self):
        """
        Copy the local test file to the remote node where it will be executed.
        """
        test_filename = os.path.basename(self.local_test_file)
        self.remote_test_file = self.remote_test_dir / test_filename

        command_timeout = 10  # seconds
        src = str(self.local_test_file)
        dest = f"{self.ip_addr}:{self.remote_test_file}"

        LOG.info(f"About to copy a test file from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, command_timeout)

        except errors.SshError as e:
            msg = f"Error while copying a test file from {src} to {dest}."
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying test file from {src} to {dest} timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.info(f"Successfully copied the test file from {src} to {dest}")

    def __create_support_files_symlink(self):
        """
        Creates a symbolic link in the remote test dir for the supportFiles folder.
        """
        from_dir = f"{self.remote_test_dir}/{C_SUPPORT_FILES}"
        target_dir = f"{self.cluster_config.get_dir()}/{testops.C_SUPPORT_FILES_DIR}"
        command = f"ln -s {target_dir} {from_dir}"

        LOG.info(f"About to create a symbolic link for the test supportFiles directory on host "
                 f"{self.ip_addr}: {command}")

        try:
            ssh.run_ssh(self.ip_addr, command, 10)

        except errors.SshError as e:
            msg = f"Error while creating the symbolic link on host {self.ip_addr}: {command}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Creation of supportFiles symbolic link timed out after {e.timeout} seconds: " \
                  f"{command}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.info(f"Successfully created the symbolic link: {from_dir} --> {target_dir}")

    def __create_remote_dir(self):
        """
        Creates a directory on the remote host to hold the test file and
        associated artifacts.
        """

        # Create a directory to hold the test file and associated metrics
        mkdir_command = f"mkdir -p {str(self.remote_test_dir)}"

        LOG.info(f"About to make a directory to hold the test file on host {self.ip_addr}: {mkdir_command}")

        try:
            ssh.run_ssh(self.ip_addr, mkdir_command, 10)

        except errors.SshError as e:
            msg = f"Error while creating the following remote directory: {self.ip_addr}:{str(self.remote_test_dir)}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Creation of remote directory timed out after {e.timeout} seconds: " \
                  f"{self.ip_addr}:{str(self.remote_test_dir)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.info(f"Successfully created the remote directory: {self.ip_addr}:{str(self.remote_test_dir)}")

    def __run_jmeter_test(self):
        """
        Executes the JMeter test file and sets the result and end time of the test.
        """
        # Where did we copy the jmeter properties file to?
        jmeter_properties_path = testops.get_remote_jmeter_property_file_path(self.cluster_config)
        log_file_path = self.__remote_test_log_file_path()

        # The "cd" is important here because JMeter will spit out a jmeter.log file in this directory,
        # which we will pick up afterwards.
        jmeter_command = f"cd {self.remote_test_dir} && " \
                         f"export JVM_ARGS=\"-Dnashorn.args=--no-deprecation-warning\" && " \
                         f"{C_JMETER_BINARY} -n " \
                         f"-p {jmeter_properties_path} " \
                         f"-t {self.remote_test_file} " \
                         f"-l {log_file_path}"

        LOG.info(f"About to execute the JMeter command: {jmeter_command}")

        try:
            ssh.run_ssh(self.ip_addr, jmeter_command)
            LOG.info(f"Finished running the JMeter file at {self.ip_addr}:{self.remote_test_file}")

            # Record the test completion result
            self.result.set_test_ended(test_passed=True, test_skipped=False)

        except errors.SshError as e:
            stdout = ("  " + e.stdout.replace("\n", "\n    ")) if e.stdout is not None else ""
            stderr = ("  " + e.stderr.replace("\n", "\n    ")) if e.stderr is not None else ""

            msg = f"Error while trying to remotely execute JMeter file at {self.ip_addr}:{self.remote_test_file}" \
                  f"\n  Exit code:\n{e.exit_code}" \
                  f"\n  Stdout Output:\n{stdout}" \
                  f"\n  Stderr Output:\n{stderr}\n"
            self.result.set_test_ended(test_passed=False, test_skipped=False, stdout=e.stdout, stderr=e.stderr)
            LOG.error(msg)

    def __remote_test_log_file_path(self):
        """
        Returns the path of the log file that JMeter will produce on the first node.
        """
        remote_path = f"{self.remote_test_dir}/{self.test_name}.log"
        return remote_path

    def __remote_jmeter_log_file_path(self):
        """
        Returns the path of the log file that JMeter will produce on the first node.
        """
        remote_path = f"{self.remote_test_dir}/jmeter.log"
        return remote_path

    def __collect_metrics(self):

        assert os.path.exists(self.artifact_dir), f"Artifact directory does not exist: {self.artifact_dir}"

        # Copy the metrics to this directory
        self.__copy_remote_metrics_locally()

        # Copy the jmeter logs to this directory
        self.copy_jmeter_logs()

        self.copy_cloud_init_output()

        # Parse the metrics and add them to the results.
        # We look for both *.csv and *.tsv files.
        csv_list = glob.glob(f"{self.artifact_dir}/*.csv")
        tsv_list = glob.glob(f"{self.artifact_dir}/*.tsv")

        if len(csv_list) == 0 and len(tsv_list) == 0:
            LOG.info(f"No metric files were produced by test {self.test_name}")

        for csv_metric in csv_list:
            assert os.path.exists(csv_metric)
            self.__parse_csv_metrics(csv_metric)

        for tsv_metric in tsv_list:
            assert os.path.exists(tsv_metric)
            self.__parse_tsv_metric(tsv_metric)

        LOG.info(f"Extracted a total of {str(len(self.result.metrics))} metrics")

    def __copy_remote_metrics_locally(self):
        """
        Copies metric files from a remote host to a local temporary directory
        """

        LOG.debug("About to copy metrics files from remote node to local directory")
        src_spec = f"{self.ip_addr}:{self.remote_test_dir}/*"

        try:
            ssh.run_scp(src=src_spec, dest=self.artifact_dir, timeout=C_SCP_TIMEOUT_SEC)

        except errors.SshError as e:
            msg = f"Error while copying metrics from remote path: {src_spec}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of metrics from remote path at {src_spec} timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        file_list = os.listdir(self.artifact_dir)
        LOG.debug("Copied the following files from the remote node:\n%s", '\n  '.join(file_list))

    def copy_jmeter_logs(self):
        """
        Copies test and jmeter log files from a remote host to the artifact directory
        """

        LOG.debug("About to copy test log file from remote node to local directory")
        src = f"{self.ip_addr}:{self.__remote_test_log_file_path()}"

        try:
            ssh.run_scp(src=src, dest=self.artifact_dir, timeout=C_SCP_TIMEOUT_SEC)

        except errors.SshError as e:
            msg = f"Error while copying jmeter log file from remote path: {src}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of jmeter log file from remote path at {src} timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        # Now for the jmeter.log file
        LOG.debug("About to copy jmeter log file from remote node to local directory")
        src = f"{self.ip_addr}:{self.__remote_jmeter_log_file_path()}"

        try:
            ssh.run_scp(src=src, dest=self.artifact_dir, timeout=C_SCP_TIMEOUT_SEC)

        except errors.SshError as e:
            msg = f"Error while copying jmeter log file from remote path: {src}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of jmeter log file from remote path at {src} timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

    def copy_cloud_init_output(self):
        """
        Copies cloud-init output file from a remote host to the artifact directory
        """

        LOG.debug("About to copy cloud-init output file from remote node to local directory")
        src = f"{self.ip_addr}:{C_CLOUD_INIT_OUTPUT}"

        try:
            ssh.run_scp(src=src, dest=self.artifact_dir, timeout=C_SCP_TIMEOUT_SEC)

        except errors.SshError as e:
            msg = f"Error while copying cloud-init output file from remote path: {src}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of cloud-init output file from remote path at {src} timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

    def __parse_csv_metrics(self, metric_file: string) -> bool:
        """
        Parses a single CSV file containing test metrics and saves the metrics into the result.
        Returns false if an error is encountered.
        """

        LOG.debug(f"Parsing CSV metrics file: {os.path.basename(metric_file)}")
        metrics_added = 0

        if os.stat(metric_file).st_size == 0:
            return False

        with open(metric_file, newline='') as csvfile:
            reader = csv.DictReader(csvfile, dialect='excel')
            fieldnames = reader.fieldnames

            if fieldnames is None:
                return False

            return_status = True

            # Must have these fields minimum: label, success, Latency
            # Optional fields that can be used: timestamp, failureMessage, responseMessage, Connect

            # Check that the mandatory fields are in the file
            for mandatory_field in C_MANDATORY_CSV_FIELDS:
                if mandatory_field not in fieldnames:
                    errmsg = f"Could not process metrics file {os.path.basename(metric_file)} because " \
                             f"it does not have the mandatory field \"{mandatory_field}\""
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

            has_timestamp_field = (C_CSV_TIMESTAMP in fieldnames)
            has_failuremsg_field = (C_CSV_FAILUREMSG in fieldnames)
            has_responsemsg_field = (C_CSV_RESPONSEMSG in fieldnames)
            has_connect_field = (C_CSV_CONNECT in fieldnames)

            for row in reader:
                label = row[C_CSV_LABEL]
                latency_str = row[C_CSV_LATENCY]
                success = row[C_CSV_SUCCESS].lower() == "true"

                # Check if label is null or an empty string (it shouldn't ever be)
                if (label is None) or (len(label) == 0):
                    errmsg = f"Value for field \"{C_CSV_LABEL}\" in test {os.path.basename(metric_file)} " \
                             f"must not be a null or empty string value"
                    LOG.error(errmsg)

                    if return_status:
                        return_status = False
                        # Mark the test as having failed
                        self.result.test_passed = False
                        self.result.stderr = errmsg
                    continue

                if not success:
                    failuremsg = None

                    # The error message could be in either failureMessage or responseMessage fields
                    if has_failuremsg_field and not failuremsg:
                        failuremsg = row[C_CSV_FAILUREMSG]

                    if has_responsemsg_field and not failuremsg:
                        failuremsg = row[C_CSV_RESPONSEMSG]

                    if not failuremsg:
                        # Is RapidsSE still running?
                        (is_running, error_msg) = clusterops.is_rapidsse_running(self.ip_addr, self.cluster_config)
                        if is_running is False:
                            failuremsg = "RapidsSE is not running and may have crashed"

                    if failuremsg:
                        errmsg = f"Statement with label \"{label}\" in test {self.test_name} " \
                                 f"failed with error: {failuremsg}"
                    else:
                        errmsg = f"Statement with label \"{label}\" in test {self.test_name} " \
                                 f"failed with an error"
                    LOG.error(errmsg)

                    if return_status:
                        return_status = False
                        # Mark the test as having failed
                        self.result.test_passed = False
                        self.result.stderr = errmsg
                    continue

                # Check that latency_str is not null or an empty string
                if (latency_str is None) or (len(latency_str) == 0):
                    errmsg = f"Value for field \"{C_CSV_LATENCY}\" in metric file {os.path.basename(metric_file)} " \
                             f"for test {self.test_name} must not be a null or empty string value"
                    LOG.error(errmsg)

                    if return_status:
                        return_status = False
                        # Mark the test as having failed
                        self.result.test_passed = False
                        self.result.stderr = errmsg
                    continue

                # Convert the latency string into a numeric
                try:
                    latency = decimal.Decimal(latency_str)

                except ValueError:
                    errmsg = f"Statement with label \"{label}\" failed because the " \
                             f"\"{C_CSV_LATENCY}\" field is not numeric in metric file " \
                             f"{os.path.basename(metric_file)} for test {self.test_name}"
                    LOG.error(errmsg)

                    if return_status:
                        return_status = False
                        # Mark the test as having failed
                        self.result.test_passed = False
                        self.result.stderr = errmsg
                    continue

                # Subtract the connect field from the latency field, if it exists
                if has_connect_field:
                    try:
                        connect = decimal.Decimal(row[C_CSV_CONNECT])
                        latency -= connect

                        if latency < 0:
                            latency = 0

                    except ValueError:
                        errmsg = f"Statement with label \"{label}\" failed because the " \
                                 f"\"{C_CSV_CONNECT}\" field is not numeric in metric file " \
                                 f"{os.path.basename(metric_file)} for test {self.test_name}"
                        LOG.error(errmsg)

                        if return_status:
                            return_status = False
                            # Mark the test as having failed
                            self.result.test_passed = False
                            self.result.stderr = errmsg
                        continue

                # Is there a timestamp field we can use?
                timestamp = None
                if has_timestamp_field:
                    timestamp_str = row[C_CSV_TIMESTAMP]

                    # Convert the offset from epoch to a datetime
                    try:
                        timestamp = datetime.datetime.fromtimestamp(float(timestamp_str) / 1000)

                    except ValueError:
                        errmsg = f"Statement with label \"{label}\" failed because the " \
                                 f"\"{C_CSV_TIMESTAMP}\" field couldn't be converted to datetime object " \
                                 f"in metric file {os.path.basename(metric_file)} for test {self.test_name}"
                        LOG.error(errmsg)

                        if return_status:
                            return_status = False
                            # Mark the test as having failed
                            self.result.test_passed = False
                            self.result.stderr = errmsg
                        continue

                # Record the metric
                self.result.add_metric(label, latency, timestamp)
                metrics_added += 1

        if return_status:
            LOG.info(f"Found {metrics_added} metrics from file {metric_file}")
        else:
            LOG.error(f"Found {metrics_added} metrics from file {metric_file}, with errors.")

        return return_status

    def __parse_tsv_metric(self, metric_file: string) -> bool:
        """
        Parses a single TSV file containing test metrics and saves the metrics into the result.
        Returns false if an error is encountered.
        """

        LOG.debug(f"Parsing TSV metrics file: {os.path.basename(metric_file)}")

        with open(metric_file, newline='') as csvfile:
            reader = csv.DictReader(csvfile, dialect='excel-tab')
            fieldnames = reader.fieldnames

            for mandatory_field in C_MANDATORY_TSV_FIELDS:
                if (mandatory_field not in fieldnames) and (mandatory_field.lower() not in fieldnames):
                    errmsg = f"Could not process metrics file {os.path.basename(metric_file)} because " \
                             f"it does not have the mandatory field \"{mandatory_field}\""
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

            for row in reader:
                label = row[C_TSV_LABEL]

                # Check if label is null or an empty string
                if (label is None) or (len(label) == 0):
                    errmsg = f"Value for field \"{C_TSV_LABEL}\" in test {os.path.basename(metric_file)} " \
                             f"must not be a null or empty string value"
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

                # Read the value and convert the value string into a numeric
                value_str = row[C_TSV_VALUE]

                if (value_str is None) or (len(value_str) == 0):
                    errmsg = f"Value for field \"{C_TSV_VALUE}\" in test {os.path.basename(metric_file)} " \
                             f"must not be a null or empty string value"
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

                try:
                    value = decimal.Decimal(value_str)

                except ValueError:
                    errmsg = f"Statement with label \"{label}\" failed because the " \
                             f"\"{C_TSV_VALUE}\" field is not numeric in test {os.path.basename(metric_file)}"
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

                self.result.add_metric(label, value)

        return True

    def __wait_for_test_finish(self, timeout: float) -> bool:
        current_time = datetime.datetime.now()
        end_time = current_time + datetime.timedelta(seconds=timeout)

        LOG.info(f"Waiting for JMeter test to complete ({timeout} sec)")

        while current_time < end_time:
            if self.test_finished:
                return True

            time.sleep(C_TEST_FINISH_SLEEP_IVL)
            current_time = datetime.datetime.now()

        return self.test_finished

    def __cleanup_test(self):
        self.__cleanup_test_file()
        self.__cleanup_support_files_link()

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

        except errors.SshError as e:
            msg = f"Error while removing remote test file at {self.remote_test_file}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Deletion of removing remote test file at {self.remote_test_file} " \
                  f"timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.debug(f"Successfully deleted the remote test file at {self.remote_test_file}")

    def __cleanup_support_files_link(self):
        """
        We only delete the test file and not the metrics, because we may end up
        collecting and archiving the metrics. We don't need to archive the test
        file as well.
        """

        link = f"{self.remote_test_dir}/{C_SUPPORT_FILES}"
        rm_command = f"rm -f {link}"

        LOG.debug(f"About to remove the supportFiles symbolic link at {link}")

        try:
            ssh.run_ssh(self.ip_addr, rm_command, 10)

        except errors.SshError as e:
            msg = f"Error while removing remote supportFiles symbolic link at {link}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Deletion of removing remote supportFiles symbolic link at {link} " \
                  f"timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.debug(f"Successfully deleted the remote supportFiles symbolic link at {link}")

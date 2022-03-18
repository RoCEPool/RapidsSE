"""Runs a test file"""
import datetime
import glob
import string
import logging
import os.path
import pathlib
import subprocess
import csv
import decimal

from septest import clusterops
from septest.results import TestResult
from septest import errors
from septest.configs.clusterconfig import ClusterConfig
from septest import ssh

LOG = logging.getLogger(__name__)

C_JMETER_BINARY = "/opt/jmeter/bin/jmeter"
C_JMETER_EXTS = ["jmx", "jmeter"]
C_TEST_DIR_FMT = "j{:04d}_t{:02d}_{:02d}_{:s}"
C_SCP_TIMEOUT_SEC = 60

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
            "perftests",
            C_TEST_DIR_FMT.format(job_result_id, test_id, test_seq, self.test_name)
        )
        self.remote_test_file = None
        self.result = TestResult(test_path, self.test_name, test_id, job_result_id, self.remote_test_dir)

        assert self.local_test_file.exists()
        assert (self.test_name is not None) and (len(self.test_name) > 0)
        assert self.test_id >= 0

    def run_test(self):
        LOG.info(f"Running test:  {self.test_name} (ID: {self.test_id})")

        self.result.set_test_started()
        self.__copy_test_file_to_remote_node()

        # Run the test
        splits = str(self.local_test_file).lower().rsplit(".", 1)
        if len(splits) == 2 and splits[1] in C_JMETER_EXTS:
            self.__run_jmeter_test()
        else:
            raise errors.NotImplementedYet("Only JMeter test files (*.[jmx | jtl | jmeter]) are currently supported")

        self.__cleanup_test_file()
        self.__collect_metrics()

        LOG.info(f"Finished test: {self.test_name} (ID: {self.test_id}). "
                 f"Result = {'SUCCESS' if self.result.test_passed else 'FAILURE'}")

    @staticmethod
    def get_test_name_from_path(test_path: string) -> string:
        """Returns the test name given its path"""
        return os.path.basename(test_path).rsplit(sep=".", maxsplit=1)[0]

    def __copy_test_file_to_remote_node(self):
        """
        Copy the local test file to the remote node where it will be executed.
        """

        # First, create a directory on the remote node where we will copy the test
        # file to. Any metrics that are produced will be output in this folder.
        self.__create_remote_dir()

        test_filename = os.path.basename(self.local_test_file)
        self.remote_test_file = self.remote_test_dir / test_filename

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
        mkdir_command = f"mkdir -p {str(self.remote_test_dir)}"

        LOG.debug(f"About to make a directory to hold the test file on host {self.ip_addr}: {mkdir_command}")

        try:
            ssh.run_ssh(self.ip_addr, mkdir_command, 10)

        except subprocess.CalledProcessError as e:
            msg = f"Error while creating the following remote directory: {self.ip_addr}:{str(self.remote_test_dir)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Creation of remote directory timed out after {e.timeout} seconds: " \
                  f"{self.ip_addr}:{str(self.remote_test_dir)}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.debug(f"Successfully created the remote directory: {self.ip_addr}:{str(self.remote_test_dir)}")

    def __run_jmeter_test(self):
        """
        Executes the JMeter test file and sets the result and end time of the test.
        """
        # Where did we copy the jmeter properties file to?
        jmeter_properties_path = clusterops.get_remote_jmeter_property_file_path(self.cluster_config)
        log_file_path = self.__remote_log_file_path()
        rapidsse_bin_dir = f"{self.cluster_config.get_dir()}/bin"

        # We need to run jmeter from the <install>/bin directory so that the JDBC driver
        # can invoke the SE prompt and the SE prompt can find libRapidsSE.so.
        # We need to set the LD_LIBRARY_PATH so that the rapidsse binary can find the libRapidsSE.so.
        # If we tried to run this on MacOS then we would need to run jmeter from the
        # <install_dir>/bin directory because the rapidsse executable has a dependency on
        # the file "../bin/libRapidsse.dylib"
        jmeter_command = f"cd {self.remote_test_dir} && " \
                         f"LD_LIBRARY_PATH={rapidsse_bin_dir} " \
                         f"{C_JMETER_BINARY} -n " \
                         f"-p {jmeter_properties_path} " \
                         f"-t {self.remote_test_file} " \
                         f"-l {log_file_path}"

        LOG.debug(f"About to execute the JMeter command: {jmeter_command}")

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
        remote_path = f"{self.remote_test_dir}/{self.test_name}.log"
        return remote_path

    def __collect_metrics(self):

        assert os.path.exists(self.artifact_dir), f"Artifact directory does not exist: {self.artifact_dir}"

        # Copy the metrics to this directory
        self.__copy_remote_metrics_locally()

        # Parse the metrics and add them to the results.
        # We look for both *.csv and *.tsv files.
        csv_list = glob.glob(f"{self.artifact_dir}/*.csv")
        tsv_list = glob.glob(f"{self.artifact_dir}/*.tsv")

        if len(csv_list) == 0 and len(tsv_list) == 0:
            LOG.info(f"No metric files were produced by test {self.test_name}")

        for csv_metric in csv_list:
            assert os.path.exists(csv_metric)
            if not self.__parse_csv_metrics(csv_metric):
                return

        for tsv_metric in tsv_list:
            assert os.path.exists(tsv_metric)
            if not self.__parse_tsv_metric(tsv_metric):
                return

        LOG.info(f"Extracted a total of {str(len(self.result.metrics))} metrics")

    def __copy_remote_metrics_locally(self):
        """
        Copies metric files from a remote host to a local temporary directory
        """

        LOG.debug("About to copy metrics files from remote node to local directory")
        src_spec = f"{self.ip_addr}:{self.remote_test_dir}/*"

        try:
            ssh.run_scp(src=src_spec, dest=self.artifact_dir, timeout=C_SCP_TIMEOUT_SEC)

        except subprocess.CalledProcessError as e:
            msg = f"Error while copying metrics from remote path: {src_spec}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Copying of metrics from remote path at {src_spec} timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        file_list = os.listdir(self.artifact_dir)
        LOG.debug("Copied the following files from the remote node:\n%s", '\n  '.join(file_list))

    def __parse_csv_metrics(self, metric_file: string) -> bool:
        """
        Parses a single CSV file containing test metrics and saves the metrics into the result.
        Returns false if an error is encountered.
        """

        LOG.debug(f"Parsing CSV metrics file: {os.path.basename(metric_file)}")
        metrics_added = 0

        with open(metric_file, newline='') as csvfile:
            reader = csv.DictReader(csvfile, dialect='excel')
            fieldnames = reader.fieldnames

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
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

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

                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

                # Check that latency_str is not null or an empty string
                if (latency_str is None) or (len(latency_str) == 0):
                    errmsg = f"Value for field \"{C_CSV_LATENCY}\" in metric file {os.path.basename(metric_file)} " \
                             f"for test {self.test_name} must not be a null or empty string value"
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

                # Convert the latency string into a numeric
                try:
                    latency = decimal.Decimal(latency_str)

                except ValueError:
                    errmsg = f"Statement with label \"{label}\" failed because the " \
                             f"\"{C_CSV_LATENCY}\" field is not numeric in metric file " \
                             f"{os.path.basename(metric_file)} for test {self.test_name}"
                    LOG.error(errmsg)
                    # Mark the test as having failed
                    self.result.test_passed = False
                    self.result.stderr = errmsg
                    return False

                # Subtract the connect field from the latency field, if it exists
                if has_connect_field:
                    try:
                        connect = decimal.Decimal(row[C_CSV_CONNECT])
                        latency -= connect

                    except ValueError:
                        errmsg = f"Statement with label \"{label}\" failed because the " \
                                 f"\"{C_CSV_CONNECT}\" field is not numeric in metric file " \
                                 f"{os.path.basename(metric_file)} for test {self.test_name}"
                        LOG.error(errmsg)
                        # Mark the test as having failed
                        self.result.test_passed = False
                        self.result.stderr = errmsg
                        return False

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
                        # Mark the test as having failed
                        self.result.test_passed = False
                        self.result.stderr = errmsg
                        return False

                # Record the metric
                self.result.add_metric(label, latency, timestamp)
                metrics_added += 1

        LOG.info(f"Found {metrics_added} metrics from file {metric_file}")
        return True

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
            msg = f"Error while removing remote test file at {self.remote_test_file}" \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        except subprocess.TimeoutExpired as e:
            msg = f"Deletion of removing remote test file at {self.remote_test_file} " \
                  f"timed out after {e.timeout} seconds: " \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            raise errors.ClusterError(msg) from e

        LOG.debug(f"Successfully deleted the remote test file at {self.remote_test_file}")

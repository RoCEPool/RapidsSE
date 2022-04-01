""""Runs a job"""
import datetime
import time
import os
import shutil
import string
import logging
import threading
import pathlib
import tempfile

import septest.ec2 as ec2
import septest.errors as errors
import septest.db.jobstart
import septest.db.jobend
import septest.clusterops as clusterops
from septest.configs.allconfigs import AllConfigs
from septest.runners.testrunner import TestRunner
from septest.results import TestResult

LOG = logging.getLogger(__name__)

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_EC2_BOOT_TIMEOUT_SEC = 300

# The format specification for the artifact directory for each test result
C_TEST_ARTIFACT_FMT = "tid{:02d}-{}"

C_JUNIT_FILENAME = "results.xml"

# The supportFiles directory, containing supporting scripts for tests
C_SUPPORT_FILES_SRC = pathlib.Path("configs/tests/supportFiles").resolve()
assert C_SUPPORT_FILES_SRC.exists(), f"Cannot find the supportFiles folder: {str(C_SUPPORT_FILES_SRC)}"

# How long to sleep between checking the completion of tests / SE status
C_TEST_MONITORING_SLEEP_IVL = 2


class JobRunner:
    """Coordinates and runs a job, specified by a job config file"""

    def __init__(self,
                 job_path: string,
                 bamboo_plan_key: string,
                 bamboo_build_num: int,
                 git_hash: string,
                 git_branch: string,
                 invoking_user: string,
                 archive_metrics: bool,
                 result_path: string,
                 tags: dict):

        # Parse the job file and the hardware and cluster configs that it references
        self.all_configs = AllConfigs(job_path)

        # The plan key string used by bamboo when this was invoked. e.g. "SE-PERF"
        self.bamboo_plan_key: string = bamboo_plan_key

        # The bamboo build number for the above plan key. e.g. 1234
        self.bamboo_build_num: int = bamboo_build_num

        # The git hash of the software being tested
        self.git_hash: string = git_hash

        # The git branch being tested
        self.git_branch: string = git_branch

        # The user who invoked this performance build. May be 'SYSTEM'
        # if it was invoked on a schedule, otherwise it contains a bamboo
        # username. e.g. "craig.mcintyre"
        self.invoking_user: string = invoking_user

        # Do we want to collect and save the metrics in bamboo afterwards?
        self.archive_metrics = archive_metrics

        # Where to save the overall results and archived files?
        self.result_path = result_path

        # A dictionary of key/value pairs that are stored with the job results
        self.tags: {} = tags

        self.job_start_ts = datetime.datetime.now()  # timestamp of when the job began
        self.test_start_ts = None   # timestamp of when the tests were first executed
        self.test_end_ts = None     # timestamp of when the tests completed being executed
        self.job_end_ts = None      # timestamp of when the job completed

        self.job_result_id = -1     # ID of this result instance running a given job
        self.job_id = -1            # ID of the job definition
        self.test_tuples: [(string, int)] = None  # tuple of resolved test path and associated test_id
        self.test_results: [TestResult] = []

        self.instance_ids = []  # EC2 instance IDs
        self.ip_addresses = []  # EC2 instance private IP addresses

        # cluster config customised for dynamic AWS hostnames.
        # This file will be closed and deleted when this class is destructed.
        self.custom_cluster_config: tempfile.NamedTemporaryFile = None

        # The current test that is running, and a lock to protect the changing of this variable
        self.current_test_runner = None
        self.test_runner_lock = threading.RLock()

        # Is the test ending early due to an abnormality (timeout, crash, etc)?
        self.abnormal_exit = False

    def run_job(self):
        """Runs the job and all of its tests"""

        job_timed_out = False
        se_crashed = False
        last_error = None

        # Catch errors in the job setup and ensure that machines get terminated
        try:
            # (1) Setup operations and get the job result ID that is created in the database
            self.__setup_job()

            # (2) Run the tests

            # We run them in a thread so we can set a timeout if the tests take too long or
            # the system hangs. We note that threading in Python is not truely concurrently
            # due to the Global Interpreter Lock, but that is ok for this situation as we
            # simply want to wait and detect timeout situations.

            test_thread = threading.Thread(target=self.__run_tests, name="testThread", daemon=True)
            timeout = self.all_configs.get_job().get_max_duration() * 60  # convert minutes to seconds

            self.test_start_ts = datetime.datetime.now()
            test_thread.start()

            (job_timed_out, se_crashed) = self.__wait_for_test_to_complete(test_thread, timeout)
            # test_thread.join(timeout)

            self.test_end_ts = datetime.datetime.now()
            # job_timed_out = test_thread.is_alive()

            errmsg = None
            if job_timed_out:
                LOG.error(f"Job timeout of {self.all_configs.get_job().get_max_duration()} "
                          f"minutes has been exceeded. Commencing shutdown hooks.")
                errmsg = f"Job has timed out after {self.all_configs.get_job().get_max_duration()} minutes."

            elif se_crashed:
                LOG.error("RapidsSE has crashed!! Commencing shutdown hooks.")
                errmsg = "RapidsSE has crashed. Please check the log artifact files for details."

            # If we have had an abnormal exit and there is a test in progress then we need to
            # mark it as failed and add its result.
            if errmsg is not None:
                with self.test_runner_lock:
                    if self.current_test_runner is not None:
                        errmsg = "" if job_timed_out else ""
                        self.current_test_runner.result.set_test_ended(False, False, stderr=errmsg)
                        self.test_results.append(self.current_test_runner.result)

            # (3) Gather results, logs and artifacts
            self.__gather_artifacts()

        except errors.Ec2InstanceTimeoutError as e:
            LOG.error(e)
            last_error = e
            # Extract out the Instance IDs so they can be terminated in the finally() section
            self.instance_ids = e.instance_ids
            raise

        except Exception as e:
            LOG.error(e)
            last_error = e
            raise

        finally:
            # (4) Cleanup
            self.__cleanup_job()

            # (5) Record results
            self.__record_results(self.abnormal_exit)

            self.job_end_ts = datetime.datetime.now()

            job_duration: datetime.timedelta = self.job_end_ts - self.job_start_ts

            if last_error is not None:
                LOG.info(f"Job aborted with error in {str(job_duration)}")
            elif job_timed_out:
                LOG.info(f"Job timed out after {str(job_duration)}")
            elif se_crashed:
                LOG.info("Job ended early due to RapidsSE crashing.")
            else:
                LOG.info(f"Job completed in {str(job_duration)}")

    def __setup_job(self):
        """Do all the pre-requisites before we can start to run the tests"""

        (job_result_id, job_id, test_tuples) = \
            septest.db.jobstart.record_job_start(self.all_configs,
                                                 self.bamboo_plan_key,
                                                 self.bamboo_build_num,
                                                 self.git_hash,
                                                 self.git_branch,
                                                 self.invoking_user,
                                                 self.tags)

        self.job_result_id = job_result_id
        self.job_id = job_id
        self.test_tuples = test_tuples

        # Clear out any contents of the result and artifacts directory
        self.__delete_results_dir_contents()

        # Compile RapidsSE and build the installer
        installer_path = clusterops.build_installer()

        # Start up the necessary AWS hosts
        (self.instance_ids, self.ip_addresses) = ec2.start_instances(self.all_configs.get_hardware(),
                                                                     C_EC2_BOOT_TIMEOUT_SEC,
                                                                     self.job_result_id,
                                                                     self.job_id)

        assert len(self.instance_ids) > 0
        assert len(self.ip_addresses) > 0

        # Copy and modify the cluster config file for the AWS hosts that were started.
        orig_cluster_cfg = pathlib.Path(self.all_configs.get_job().get_resolved_cluster_config_path())
        self.custom_cluster_config = clusterops.customise_cluster_cfg_hostnames(orig_cluster_cfg,
                                                                                self.ip_addresses)

        # Install the cluster using the cluster config
        clusterops.install_rapidsse(installer_path, pathlib.Path(self.custom_cluster_config.name))

        # Start the cluster on the remote nodes
        clusterops.start_rapidsse(self.ip_addresses[0], self.all_configs.cluster_config)

        # Run a dummy command on the prompt to help validate the cluster
        clusterops.run_dummy_statement(self.ip_addresses[0], self.all_configs.cluster_config)

        # Copy the JDBC driver to the installation dir of the first host
        clusterops.copy_jdbc_driver(self.ip_addresses[0], self.all_configs.cluster_config)

        # Write/Copy a jmeter property file to the installation dir of the first host to
        # set the classpath to find the above JDBC driver.
        clusterops.write_jmeter_property_file(self.ip_addresses[0], self.all_configs.cluster_config)

        # Make the common perftests directory, to hold all tests, support files and their metrics
        clusterops.make_perftests_dir(self.ip_addresses[0], self.all_configs.cluster_config)

        # Copy the supportFiles directory to the first host
        clusterops.copy_support_files(self.ip_addresses[0],
                                      self.all_configs.cluster_config,
                                      str(C_SUPPORT_FILES_SRC))

    def __run_tests(self):
        """Run each of the tests specified in this job file
            This should be run in a thread with a suitable timeout period.
        """
        LOG.info("Beginning execution of tests...")

        continue_on_test_failure = self.all_configs.get_job().get_continue_on_test_failure()
        skip_remaining_tests = False
        test_seq = 0
        got_failures = False

        for (test_path, test_id) in self.test_tuples:
            test_seq += 1

            if skip_remaining_tests:
                self.__skip_test(test_path, test_id)
                LOG.debug(f"Test {TestRunner.get_test_name_from_path(test_path)} is being skipped.")
                continue

            # Make the directory where this test will store artifacts
            artifact_dir = self.__make_test_artifact_dir(test_id, test_path)

            with self.test_runner_lock:
                self.current_test_runner = TestRunner(test_path,
                                                      test_id,
                                                      self.job_result_id,
                                                      test_seq,
                                                      self.all_configs.get_cluster(),
                                                      self.ip_addresses[0],
                                                      artifact_dir)
            self.current_test_runner.run_test()

            with self.test_runner_lock:
                test_result = self.current_test_runner.result
                # No tests currently running
                self.current_test_runner = None

            #  Check if we are aborting abnormally due to a crash, timeout, etc.
            if not self.abnormal_exit:
                self.test_results.append(test_result)

            if test_result.test_passed is False:
                LOG.debug(f"Test {test_result.test_name} failed")
                got_failures = True

                if continue_on_test_failure is False or self.abnormal_exit:
                    skip_remaining_tests = True

                # If RapidsSE is no longer running then we also need to skip all remaining tests
                # and log this.
                if not self.abnormal_exit:
                    (se_running, error_msg) = clusterops.is_rapidsse_running(self.ip_addresses[0],
                                                                             self.all_configs.cluster_config)
                    if se_running is False:
                        # We can't run any more tests
                        skip_remaining_tests = True
                        LOG.error(error_msg)
            else:
                LOG.debug(f"Test {test_result.test_name} passed")

        if got_failures:
            LOG.info("Completed execution of tests, with failures.")
        else:
            LOG.info("Completed execution of all tests successfully.")

    def __skip_test(self, test_path: string, test_id: int):
        # Create a test_result for this skipped test and add it to the result list
        test_result = TestResult(TestRunner.get_test_name_from_path(test_path),
                                 test_id,
                                 self.job_result_id)
        test_result.test_passed = None
        test_result.test_skipped = True
        self.test_results.append(test_result)

    def __delete_results_dir_contents(self):
        """
        Remove the contents of the results directory.
        """
        # Simplest way is to delete the entire directory and then recreate it.
        LOG.debug(f"Clearing the contents of results & artifacts directory: {self.result_path}")

        shutil.rmtree(self.result_path, ignore_errors=True)
        os.makedirs(self.result_path, exist_ok=True)

    def __make_test_artifact_dir(self, test_id: int, test_path: string) -> string:
        """
        Make a directory for storing test artifacts and logs.
        """
        artifact_name = C_TEST_ARTIFACT_FMT.format(test_id, TestRunner.get_test_name_from_path(test_path))
        artifact_dir = pathlib.Path(self.result_path) / artifact_name

        LOG.debug(f"Making artifact dir: {str(artifact_dir)}")
        os.makedirs(str(artifact_dir), exist_ok=True)
        return str(artifact_dir)

    def __delete_test_artifact_dir(self, test_id: int, test_path: string):
        """
        Delete the directory (and all content) that stores test artifacts and logs.
        """
        artifact_name = C_TEST_ARTIFACT_FMT.format(test_id, TestRunner.get_test_name_from_path(test_path))
        artifact_dir = pathlib.Path(self.result_path) / artifact_name

        LOG.debug(f"Deleting artifact dir: {str(artifact_dir)}")
        shutil.rmtree(str(artifact_dir), ignore_errors=True)

    def __gather_artifacts(self):
        """Collect all results, logs and artifacts from the machines used to run the tests"""
        # We gather test artifacts by default. Need to delete test artifacts if
        # self.archive_metrics is false and a given test is successful.
        # If a test fails, or if self.archive_metrics is true then we keep those artifacts.

        collect_logs = False

        for test_result in self.test_results:
            if test_result.test_passed is False:
                collect_logs = True

            elif self.archive_metrics is False:     # test must have passed
                self.__delete_test_artifact_dir(test_result.test_id, test_result.test_path)

        # Need to gather SE logs if any test fails.
        if collect_logs:
            clusterops.get_rapidsse_logs(self.ip_addresses[0], self.all_configs.cluster_config, self.result_path)

    def __cleanup_job(self):
        """Common cleanup operations after a job has completed, successfully or not"""
        # Terminate all of the EC2 instances that we have previously started
        if self.all_configs.get_hardware().get_terminate_on_finish():
            ec2.terminate_instances(self.instance_ids)

        else:
            msg = f"NOT shutting down the following instances as indicated in the hardware config file: " \
                  f"{str(self.instance_ids)}"
            LOG.warning(msg)

        pass

    def __record_results(self, abnormal_exit: bool):
        """Record the results of the test into the database and prepare the result files for Bamboo"""
        septest.db.jobend.record_job_results(self.job_result_id, abnormal_exit, self.test_results)

        # Write JUnit result files for Bamboo
        result_path = pathlib.Path(self.result_path) / C_JUNIT_FILENAME
        septest.results.summarise_to_junit(self.test_results, result_path)

        # Write out a textual summary too
        summary_text = septest.results.summarise_to_string(self.test_results)
        LOG.info(f"\n{summary_text}\n")

    def __wait_for_test_to_complete(self, test_thread: threading.Thread, timeout: float) -> (bool, bool):
        """
        Waits for the given test thread to complete, a given timeout to elapse, or for SE to crash.
        Returns a tuple of booleans (timed_out, se_crashed) that indicate the end result.
        """

        timed_out = False
        se_crashed = False

        current_time = datetime.datetime.now()
        end_time = current_time + datetime.timedelta(seconds=timeout)

        while current_time < end_time:

            if not test_thread.is_alive():
                # test thread has ended normally
                return timed_out, se_crashed

            if not clusterops.is_rapidsse_running(self.ip_addresses[0], self.all_configs.cluster_config):
                LOG.error("*** Detected that RapidsSE has crashed ***")
                se_crashed = True
                self.abnormal_exit = True
                return timed_out, se_crashed

            # Test thread still running and SE not crashed. Try again in a little bit.
            time.sleep(C_TEST_MONITORING_SLEEP_IVL)
            current_time = datetime.datetime.now()

        # We have timed out and SE hasn't crashed
        timed_out = True
        self.abnormal_exit = True
        return timed_out, se_crashed

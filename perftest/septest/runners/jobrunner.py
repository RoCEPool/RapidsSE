""""Runs a job"""
import datetime
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

        # A dictionary of key/value pairs that are stored with the job results
        self.tags: {} = tags

        self.job_start_ts = datetime.datetime.now()  # timestamp of when the job began
        self.test_start_ts = None   # timestamp of when the tests were first executed
        self.test_end_ts = None     # timestamp of when the tests completed being executed
        self.job_end_ts = None      # timestamp of when the job completed

        self.job_result_id = -1     # ID of this result instance running a given job
        self.job_id = -1            # ID of the job definition
        self.test_tuples: [(string, int)] = None  # tuple of resolved test path and associated test_id
        self.test_results = []

        self.instance_ids = []  # EC2 instance IDs
        self.ip_addresses = []  # EC2 instance private IP addresses

        # cluster config customised for dynamic AWS hostnames.
        # This file will be closed and deleted when this class is destructed.
        self.custom_cluster_config: tempfile.NamedTemporaryFile = None

    def run_job(self):
        """Runs the job and all of its tests"""

        job_timed_out = False
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
            test_thread.join(timeout)
            self.test_end_ts = datetime.datetime.now()
            job_timed_out = test_thread.is_alive()

            # (3) Gather results, logs and artifacts
            self.__gather_results()

        except errors.Ec2InstanceTimeoutError as e:
            LOG.error(e)
            last_error = e
            # Extract out the Instance IDs so they can be terminated in the finally() section
            self.instance_ids = e.instance_ids
            raise e

        except Exception as e:
            LOG.error(e)
            last_error = e
            raise e

        finally:
            # (4) Cleanup
            self.__cleanup_job()

            # (5) Record results
            self.__record_results(job_timed_out)

            self.job_end_ts = datetime.datetime.now()

            job_duration: datetime.timedelta = self.job_end_ts - self.job_start_ts

            if last_error is not None:
                LOG.info(f"Job aborted with error in {str(job_duration)}")
            elif job_timed_out:
                LOG.info(f"Job timed out after {str(job_duration)}")
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

    def __run_tests(self):
        """Run each of the tests specified in this job file
            This should be run in a thread with a suitable timeout period.
        """
        LOG.info("Beginning execution of tests...")

        continue_on_test_failure = self.all_configs.get_job().get_continue_on_test_failure()
        skip_remaining_tests = False
        test_seq = 0

        for (test_path, test_id) in self.test_tuples:
            test_seq += 1

            if skip_remaining_tests:
                self.__skip_test(test_path, test_id)
                continue

            test_runner = TestRunner(test_path, test_id, self.job_result_id, test_seq,
                                     self.all_configs.get_cluster(), self.ip_addresses[0])
            test_runner.run_test()
            test_result = test_runner.result
            self.test_results.append(test_result)

            if not continue_on_test_failure and test_result.test_passed is False:
                skip_remaining_tests = True

        LOG.info("Completed execution of all tests.")

    def __skip_test(self, test_path: string, test_id: int):
        # Create a test_result for this skipped test and add it to the result list
        test_result = TestResult(TestRunner.get_test_name_from_path(test_path),
                                 test_id,
                                 self.job_result_id)
        test_result.test_passed = None
        test_result.test_skipped = True
        self.test_results.append(test_result)

    def __gather_results(self):
        """Collect all results, logs and artifacts from the machines used to run the tests"""
        pass
        # TODO: Gather metrics if self.archive_metrics is true

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

    def __record_results(self, job_timed_out: bool):
        """Record the results of the test into the database and prepare the result files for Bamboo"""
        septest.db.jobend.record_job_results(self.job_result_id, job_timed_out, self.test_results)

        # TODO: prepare result files for Bamboo

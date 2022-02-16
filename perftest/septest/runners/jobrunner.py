""""Runs a job"""
import datetime
import string
import logging
import threading
import septest.db.jobstart

from septest.configs.allconfigs import AllConfigs
from septest.configs.jobconfig import JobConfig
from septest.configs.hardwareconfig import HardwareConfig
from septest.configs.clusterconfig import ClusterConfig

LOG = logging.getLogger(__name__)


class JobRunner:
    """Coordinates and runs a job, specified by a job config file"""

    def __init__(self, job_path: string, bamboo_build_str: string, git_hash: string, tags: dict):
        # Parse the job file and the hardware and cluster configs that it references
        self.all_configs = AllConfigs(job_path)

        # The plan and build string used by bamboo when this was invoked
        self.bamboo_build = bamboo_build_str

        # The git hash of the software being tested
        self.git_hash = git_hash

        # A dictionary of key/value pairs that are stored with the job results
        self.tags = tags

        self.job_start_ts = datetime.datetime.now()  # timestamp of when the job began
        self.test_start_ts = None   # timestamp of when the tests were first executed
        self.test_end_ts = None     # timestamp of when the tests completed being executed
        self.job_end_ts = None      # timestamp of when the job completed

    def run_job(self):
        """Runs the job and all of its tests"""

        # Setup operations
        self.__setup_job()

        # Run tests. We run them in a thread so we can set a timeout
        # if the tests take too long or the system hangs. We note that
        # threading in Python is not truely concurrently due to the Global
        # Interpreter Lock, but that is ok for this situation as we simply
        # want to wait and detect timeout situations.

        test_thread = threading.Thread(target=self.__run_tests, name="testThread", daemon=True)
        timeout = self.all_configs.get_job().get_max_duration() * 60  # convert minutes to seconds

        self.test_start_ts = datetime.datetime.now()
        test_thread.start()
        test_thread.join(timeout)
        self.test_end_ts = datetime.datetime.now()
        job_timed_out = test_thread.is_alive()

        # Gather results, logs and artifacts
        self.__gather_results()

        # Cleanup
        self.__cleanup_job()

        # Record results
        self.__record_results(job_timed_out)

        self.job_end_ts = datetime.datetime.now()
        job_duration: datetime.timedelta = self.job_end_ts - self.job_start_ts

        if job_timed_out:
            LOG.info(f"Job timed out after {str(job_duration)}")
        else:
            LOG.info(f"Job completed in {str(job_duration)}")

    def __setup_job(self):
        """Do all of the pre-requisites before we can start to run the tests"""
        # TODO: Also need to pass in the name of the user invoking this run.
        job_result_id = septest.db.jobstart.record_job_start(self.all_configs,
                                                             self.bamboo_build,
                                                             self.git_hash,
                                                             None,  # TODO: invoking user
                                                             self.tags)
        pass

    def __run_tests(self):
        """Run each of the tests specified in this job file
            This should be run in a thread with a suitable timeout period.
        """
        LOG.info("Beginning execution of tests...")
        # TODO: Execute the job here
        LOG.info("Completed execution of all tests.")

    def __gather_results(self):
        """Collect all results, logs and artifacts from the machines used to run the tests"""
        pass

    def __cleanup_job(self):
        """Common cleanup operations after a job has completed, successfully or not"""
        pass

    def __record_results(self, test_timed_out: bool):
        """Record the results of the test into the database and prepare the result files for Bamboo"""
        pass

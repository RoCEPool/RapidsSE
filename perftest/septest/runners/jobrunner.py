""""Runs a job"""
import datetime
import string
import logging
import threading
import septest.db.jobstart
import septest.db.jobend

from septest.configs.allconfigs import AllConfigs
from septest.runners.results import TestResult

LOG = logging.getLogger(__name__)


class JobRunner:
    """Coordinates and runs a job, specified by a job config file"""

    def __init__(self,
                 job_path: string,
                 bamboo_plan_key: string,
                 bamboo_build_num: int,
                 git_hash: string,
                 git_branch: string,
                 invoking_user: string,
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

        # A dictionary of key/value pairs that are stored with the job results
        self.tags: {} = tags

        self.job_start_ts = datetime.datetime.now()  # timestamp of when the job began
        self.test_start_ts = None   # timestamp of when the tests were first executed
        self.test_end_ts = None     # timestamp of when the tests completed being executed
        self.job_end_ts = None      # timestamp of when the job completed

        self.test_results = []

    def run_job(self):
        """Runs the job and all of its tests"""

        # (1) Setup operations
        job_result_id = self.__setup_job()

        # Run tests. We run them in a thread so we can set a timeout
        # if the tests take too long or the system hangs. We note that
        # threading in Python is not truely concurrently due to the Global
        # Interpreter Lock, but that is ok for this situation as we simply
        # want to wait and detect timeout situations.

        test_thread = threading.Thread(target=self.__run_tests, name="testThread", daemon=True)
        timeout = self.all_configs.get_job().get_max_duration() * 60  # convert minutes to seconds

        # (2) Run the tests
        self.test_start_ts = datetime.datetime.now()
        test_thread.start()
        test_thread.join(timeout)
        self.test_end_ts = datetime.datetime.now()
        job_timed_out = test_thread.is_alive()

        # (3) Gather results, logs and artifacts
        self.__gather_results()

        # (4) Cleanup
        self.__cleanup_job()

        # (5) Record results
        self.__record_results(job_result_id, job_timed_out)

        self.job_end_ts = datetime.datetime.now()
        job_duration: datetime.timedelta = self.job_end_ts - self.job_start_ts

        if job_timed_out:
            LOG.info(f"Job timed out after {str(job_duration)}")
        else:
            LOG.info(f"Job completed in {str(job_duration)}")

    def __setup_job(self):
        """Do all the pre-requisites before we can start to run the tests"""
        job_result_id = septest.db.jobstart.record_job_start(self.all_configs,
                                                             self.bamboo_plan_key,
                                                             self.bamboo_build_num,
                                                             self.git_hash,
                                                             self.git_branch,
                                                             self.invoking_user,
                                                             self.tags)
        return job_result_id

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

    def __record_results(self, job_result_id: int, job_timed_out: bool):
        """Record the results of the test into the database and prepare the result files for Bamboo"""
        septest.db.jobend.record_job_results(job_result_id, job_timed_out, self.test_results)

        # TODO: prepare result files for Bamboo

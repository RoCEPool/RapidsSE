"""Runs a test file"""

import string
import logging
import os.path
from septest.runners.results import TestResult

LOG = logging.getLogger(__name__)


class TestRunner:
    """
    Runs a test file and collects the results and metrics
    """

    def __init__(self, test_path: string, test_id: int, job_result_id: int):
        self.test_name = self.get_test_name_from_path(test_path)
        self.test_id = test_id
        self.result = TestResult(self.test_name, test_id, job_result_id)

    def run_test(self):
        LOG.info(f"Running test {self.test_name} (ID: {self.test_id})")

        self.result.set_test_started()
        test_passed = True
        test_skipped = False

        # TODO: Run test here

        self.result.set_test_ended(test_passed, test_skipped)
        self.__collect_metrics()

        LOG.info(f"Finished test {self.test_name} (ID: {self.test_id}). "
                 f"Result = {'SUCCESS' if test_passed else 'FAILURE'}")

    @staticmethod
    def get_test_name_from_path(test_path: string) -> string:
        """Returns the test name given its path"""
        return os.path.basename(test_path).rsplit(sep=".", maxsplit=1)[0]

    def __collect_metrics(self):
        # TODO: Implement this
        pass

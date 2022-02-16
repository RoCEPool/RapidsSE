
import datetime
import string
import decimal
import logging

LOG = logging.getLogger(__name__)


class TestResult:
    """
    Holds result information about running a test.
    """

    def __init__(self, test_name: string, test_id: int, job_result_id: int):
        self.test_name = test_name
        self.test_id: int = test_id
        self.job_result_id: int = job_result_id
        self.start_time: datetime.datetime = None
        self.end_time: datetime.datetime = None
        self.test_passed: bool = False
        self.test_skipped: bool = True
        self.metrics: [(string, decimal)] = []

    def set_start(self):
        self.start_time = datetime.datetime.now()

    def set_end(self, test_passed: bool, test_skipped: bool):
        self.end_time = datetime.datetime.now()
        self.test_passed = test_passed
        self.test_skipped = test_skipped

    def add_metric(self, name: string, value: decimal):
        self.metrics.append((name, value))

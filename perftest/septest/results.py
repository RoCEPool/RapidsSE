
import datetime
import pathlib
import string
from decimal import Decimal
import logging

LOG = logging.getLogger(__name__)


class TestResult:
    """
    Holds result information about running a test.
    """

    def __init__(self, test_name: string, test_id: int, job_result_id: int, remote_dir: pathlib.Path):
        self.test_name = test_name
        self.test_id: int = test_id
        self.job_result_id: int = job_result_id
        self.start_time: datetime.datetime = None
        self.end_time: datetime.datetime = None
        self.test_passed: bool = False
        self.test_skipped: bool = True
        self.metrics: [(string, Decimal)] = []
        self.remote_dir = remote_dir   # where the raw metrics are saved
        self.stdout: string = None
        self.stderr: string = None

    def set_test_started(self):
        """
        Record when the test has started.
        """
        self.start_time = datetime.datetime.now()

    def set_test_ended(self, test_passed: bool, test_skipped: bool, stdout: string = None, stderr: string = None):
        """
        Record when the test has stopped and the result of the test.
        """
        self.end_time = datetime.datetime.now()
        self.test_passed = test_passed
        self.test_skipped = test_skipped
        self.stdout = stdout
        self.stderr = stderr

    def add_metric(self, name: string, value: Decimal, ts: datetime.datetime = None):
        """
        Add a metric to this test result.
        """
        assert value >= 0, f"Got a negative metric value for metric \"{name}\""

        if ts is None:
            ts = datetime.datetime.now()

        self.metrics.append((name, value, ts))

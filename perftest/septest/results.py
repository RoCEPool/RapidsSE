
import datetime
import pathlib
import string
from decimal import Decimal
import logging

import junit_xml
from junit_xml import TestSuite, TestCase

LOG = logging.getLogger(__name__)


class TestResult:
    """
    Holds result information about running a test.
    """

    def __init__(self, test_path: string, test_name: string, test_id: int,
                 job_result_id: int, remote_dir: pathlib.Path):
        self.test_path = test_path
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


def summarise_to_junit(self, results: [TestResult], dest: pathlib.Path):
    """
    Writes a summary of the tests as a JUnit XML report, so that Bamboo
    can pick it up and intepret it.
    """
    assert dest.exists() is False, f"JUnit file already exists: {str(dest)}"

    test_cases = []
    for result in results:
        duration: datetime.timedelta = result.end_time - result.start_time
        test_case = TestCase(result.test_name,
                             result.test_name,
                             duration.total_seconds(),
                             result.stdout,
                             result.stderr)

        if result.test_skipped:
            test_case.add_skipped_info()

        elif result.test_passed is False:
            test_case.add_error_info()

        test_cases.append(test_case)

    test_suite = TestSuite('Performance Test', test_cases)

    with open(dest, 'w') as f:
        junit_xml.to_xml_report_file(f, [test_suite])


def summarise_to_string(results: [TestResult]) -> string:
    """
    Writes a textual summary of all results and returns it in a string.
    """
    tests_passed = 0
    tests_failed = 0
    tests_skipped = 0
    failed_test_names = []

    for test_result in results:
        if test_result.test_skipped:
            tests_skipped += 1

        if test_result.test_passed:
            tests_passed += 1
        else:
            tests_failed += 1
            failed_test_names.append(test_result.test_name)

    failed_tests_str = "\n  ".join(failed_test_names)

    summary = f"\n" \
              f"*******************************************\n" \
              f"  Job Summary\n" \
              f"*******************************************\n" \
              f"\n" \
              f"List of failed test names:\n" \
              f"  {failed_tests_str}" \
              f"\n" \
              f"Tests Passed:   {tests_passed}\n" \
              f"Tests Failed:   {tests_failed}\n" \
              f"Tests Skipped:  {tests_skipped}\n\n" \
              f"=============   =========\n" \
              f"Total Tests:    {str(len(results))}"

    return summary

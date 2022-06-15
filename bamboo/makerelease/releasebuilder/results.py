
import datetime
import pathlib
import string
import logging

import junit_xml
from junit_xml import TestSuite, TestCase

LOG = logging.getLogger(__name__)


class TestResult:
    """
    Holds result information about running a test.
    """

    def __init__(self, test_name: string):
        self.test_name = test_name
        self.start_time: datetime.datetime = None
        self.end_time: datetime.datetime = None
        self.test_passed: bool = False
        self.test_skipped: bool = True
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


def summarise_to_junit(results: [TestResult], dest: pathlib.Path, test_suite: string):
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
            error_msg = result.stderr if result.stderr else "No failure information available"
            test_case.add_failure_info(message=error_msg)

        test_cases.append(test_case)

    test_suite = TestSuite(test_suite, test_cases)

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

    failed_test_joiner = "\n  - "
    failed_tests_str = failed_test_joiner.join(failed_test_names)
    failed_test_desc = ""

    if tests_failed > 0:
        failed_test_desc = \
            f"List of failed test names:" \
            f"{failed_test_joiner}{failed_tests_str}\n" \
            f"\n" \

    summary = f"\n" \
              f"***********************\n" \
              f"  Job Summary\n" \
              f"***********************\n" \
              f"\n" \
              f"{failed_test_desc}" \
              f"Tests Passed:     {tests_passed:>3}\n" \
              f"Tests Failed:     {tests_failed:>3}\n" \
              f"Tests Skipped:    {tests_skipped:>3}\n" \
              f"\n" \
              f"=============   =======\n" \
              f"Total Tests:      {len(results):>3}\n" \
              f"=============   =======\n"

    return summary

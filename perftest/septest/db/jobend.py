import string
import decimal
import typing
import logging

import psycopg

import septest.errors as errors
import septest.db.dbhelper
from septest.runners.results import TestResult

# Create a main logger
LOG = logging.getLogger(__name__)


def record_job_results(job_result_id: int,
                       job_timed_out: bool,
                       test_results: [TestResult]) -> None:
    """
    Record records in the database about ending the job and all of the tests.
    """

    token = septest.db.dbhelper.get_auth_token()
    connect_params: {} = septest.db.dbhelper.get_connect_params()

    # Note: not autocommit transactions
    with psycopg.connect(password=token,
                         autocommit=False,
                         **connect_params) as conn:
        cur = conn.cursor()

        __update_job_result_row(cur, job_result_id, job_timed_out, test_results)

        for test_result in test_results:
            test_result_id = __create_test_result_row(cur, test_result)
            __create_metric_result_rows(cur, test_result_id, test_result)

        # Commit the above transactions
        conn.commit()

    # Database connection will be automatically closed at this point


def __update_job_result_row(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                            job_result_id: int,
                            job_timed_out: bool,
                            test_results: [TestResult]) -> None:
    """
    Updates a record in the jobResults table regarding end times, tests completed, etc.
    """

    (tests_executed, tests_passed, tests_failed, tests_skipped) = __calculate_test_summary(test_results)

    sql = "UPDATE jobResults SET " \
          "endTime = NOW(), " \
          "timedOut = (%s), " \
          "testsExecuted = (%s), " \
          "testsPassed = (%s), " \
          "testsFailed = (%s), " \
          "testsSkipped = (%s) " \
          "WHERE jobResultId = (%s)"
    data = (job_timed_out,  # timedOut
            tests_executed, # testsExecuted
            tests_passed,   # testsPassed
            tests_failed,   # testsFailed
            tests_skipped,  # testsSkipped
            job_result_id)  # jobResultId

    LOG.debug(f"About to record the job results in the jobResults table for jobResultsId {job_result_id}")
    cur.execute(sql, data)


def __calculate_test_summary(test_results: [TestResult]) -> (int, int, int, int):
    """
    Calculates and returns a tuple of (tests_executed, tests_passed, tests_failed, tests_skipped)
    """

    tests_executed: int = 0
    tests_passed: int = 0
    tests_failed: int = 0
    tests_skipped: int = 0

    for test_result in test_results:
        if test_result.test_skipped:
            tests_skipped += 1
        else:
            tests_executed += 1
            if test_result.test_passed:
                tests_passed += 1
            else:
                tests_failed += 1

    return tests_executed, tests_passed, tests_failed, tests_skipped


def __create_test_result_row(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                             test_result: TestResult) -> int:
    """
    Creates a row in the testResults table.
    Returns the testResultId of the row created.
    """

    sql = "INSERT INTO testResults " \
          "(testId, jobResultId, startTime, endTime, testPassed, testSkipped) " \
          "VALUES (%s, %s, %s, %s, %s, %s) " \
          "RETURNING testResultId"
    data = (test_result.test_id,            # testId
            test_result.job_result_id,      # jobResultId
            test_result.start_time,         # startTime
            test_result.end_time,           # endTime
            test_result.test_passed,        # testPassed
            test_result.test_skipped)       # testSkipped

    LOG.debug(f"Creating a new entry in the testResults table for test {test_result.test_name}, "
              f"testId {test_result.test_id}")

    cur.execute(sql, data)
    row = cur.fetchone()

    if row is None:
        #  Should never happen
        msg = f"Could not retrieve the testResultId when inserting a new row into the testResults table"
        raise errors.DatabaseError(msg)

    test_result_id = row[0]
    return test_result_id


def __create_metric_result_rows(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                                test_result_id: int,
                                test_result: TestResult) -> None:
    """
    Creates rows in the metricResults table.
    """

    for (metric_name, metric_value) in test_result.metrics:
        __create_metric_result_row(cur, test_result_id, metric_name, metric_value)


def __create_metric_result_row(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                               test_result_id: int,
                               metric_name: string,
                               metric_value: decimal):
    """
    Creates a row in the metricResults table.
    """

    sql = "INSERT INTO metricResults " \
          "(testResultId, metricName, metricValue) " \
          "VALUES (%s, %s, %s)"
    data = (test_result_id,  # testResultId
            metric_name,     # metricName
            metric_value)    # metricValue

    LOG.debug(f"Creating a new entry in the metricResults table for metric_name {metric_name}, "
              f"testResultId {test_result_id}")

    cur.execute(sql, data)

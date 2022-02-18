import os.path
import string
import typing
import logging

import psycopg

from septest.configs.jobconfig import JobConfig
from septest.configs.hardwareconfig import HardwareConfig
from septest.configs.clusterconfig import ClusterConfig
from septest.configs.allconfigs import AllConfigs
import septest.errors as errors
import septest.db.dbhelper


# Create a main logger
LOG = logging.getLogger(__name__)


def record_job_start(all_configs: AllConfigs,
                     bamboo_plan_key: string,
                     bamboo_build_num: int,
                     git_hash: string,
                     git_branch: string,
                     invoking_user: string,
                     tags: {}) -> (int, [(string, int)]):
    """
    Record records in the database about starting the job.
    Returns a tuple containing the jobResultId of the partial row created in the
    jobResults table, as well as a list of tuples containing test_paths and
    their corresponding test_ids.
    """

    token = septest.db.dbhelper.get_auth_token()
    connect_params: {} = septest.db.dbhelper.get_connect_params()

    with psycopg.connect(password=token,
                         autocommit=True,
                         **connect_params) as conn:
        cur = conn.cursor()

        # We need to check to see if there are config records in the jobs, hardware,
        # cluster and tests tables. If not then we need to create a record. If a record
        # exists then we need to check that the configuration hasn't changed in the
        # config files.
        hw_id      = __create_hardware_record(cur, all_configs.get_hardware())
        cluster_id = __create_cluster_record(cur, all_configs.get_cluster())
        job_id     = __create_jobs_record(cur, all_configs, hw_id, cluster_id)
        test_ids   = __create_test_records(cur, all_configs.get_job())

        # Now create some records for beginning the job
        job_result_id = __create_jobresult_record(cur,
                                                  job_id,
                                                  bamboo_plan_key,
                                                  bamboo_build_num,
                                                  git_hash,
                                                  git_branch,
                                                  invoking_user)

        # add any tags to the tags table for this job_result_id
        __create_tags(cur, job_result_id, tags)
        return (job_result_id, test_ids)

    # Database connection will be automatically closed at this point


def __create_hardware_record(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                             hw_config: HardwareConfig) -> int:
    """Creates a record for this hardware configuration if one does not already exist.
       If a record does exist then it validates that the fields haven't changed.
       Returns the id of the hardware row created or the matching hardware row found.
    """

    sql = "SELECT hardwareId, hardwareName, numMachines, instanceType, isSpotInstance " \
          "FROM hardware WHERE hardwareName = (%s)"
    data = (hw_config.get_hardware_name(),)

    cur.execute(sql, data)
    rows: [] = cur.fetchall()
    row_id: int

    if len(rows) == 0:
        LOG.debug(f"Creating a new entry in the hardware table for hardwareName {hw_config.get_hardware_name()}")

        # This hardware configuration is new. We need to insert a row into the database
        sql = "INSERT INTO hardware (hardwareName, numMachines, instanceType, isSpotInstance) " \
              "VALUES (%s, %s, %s, %s) RETURNING hardwareId"
        data = (hw_config.get_hardware_name(),
                hw_config.get_num_machines(),
                hw_config.get_instance_type(),
                hw_config.get_use_spot_instances())

        cur.execute(sql, data)
        row = cur.fetchone()

        if row is None:
            #  Should never happen
            msg = "Could not retrieve the hardware_id when inserting a new row into the hardware table"
            raise errors.DatabaseError(msg)

        row_id = row[0]

    elif len(rows) == 1:
        # This hardware configuration already exists.

        row = rows[0]
        row_id = row[0]

        # Validate that the fields haven't changed
        (hw_id, hw_name, num_machines, instance_type, spot_instance) = row

        common_msg = "The contents of the hardware config file has been changed from what has " \
                     "previously been used. The configuration files should largely be considered " \
                     "immutable so that the meaning of past results is not altered. Please create " \
                     "a new config file if you wish to change the configuration.\n\n"

        if num_machines != hw_config.get_num_machines():
            msg = f"{common_msg}The {HardwareConfig.C_NUMMACHINES} field has changed. " \
                  f"Expected: {num_machines}, Actual: {hw_config.get_num_machines()}"
            raise errors.ConfigurationError(msg)

        if instance_type != hw_config.get_instance_type():
            msg = f"{common_msg}The {HardwareConfig.C_INSTANCETYPE} field has changed. " \
                  f"Expected: {instance_type}, Actual: {hw_config.get_instance_type()}"
            raise errors.ConfigurationError(msg)

        if spot_instance != hw_config.get_use_spot_instances():
            msg = f"{common_msg}The {HardwareConfig.C_USESPOTINSTANCES} field has changed. " \
                  f"Expected: {spot_instance}, Actual: {hw_config.get_use_spot_instances()}"
            raise errors.ConfigurationError(msg)

        LOG.debug(f"Hardware configuration already exists for hardwareName {hw_config.get_hardware_name()}")

    else:
        # Error, we've got > 1 rows returned.
        msg = f"There are multiple rows returned from the hardware table for " \
              f"hardwareName = '{hw_config.get_hardware_name()}' : {str(rows)}"
        raise errors.DatabaseConsistencyError(msg)

    return row_id


def __create_cluster_record(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                            cluster_config: ClusterConfig) -> int:
    """Creates a record for this cluster configuration if one does not already exist.
       If a record does exist then it validates that the fields haven't changed.
       Returns the id of the cluster row created or the matching cluster row found.
    """

    sql = "SELECT clusterId, clusterName, numNodes, ptnsPerNode, threadsPerNode, sharedMem " \
          "FROM cluster WHERE clusterName = (%s)"
    data = (cluster_config.get_cluster_name(),)

    cur.execute(sql, data)
    rows: [] = cur.fetchall()
    row_id: int

    if len(rows) == 0:
        LOG.debug(f"Creating a new entry in the cluster table for clusterName {cluster_config.get_cluster_name()}")

        # This cluster configuration is new. We need to insert a row into the database
        sql = "INSERT INTO cluster (clusterName, numNodes, ptnsPerNode, threadsPerNode, sharedMem) " \
              "VALUES (%s, %s, %s, %s, %s) RETURNING clusterId"
        data = (cluster_config.get_cluster_name(),
                cluster_config.get_num_nodes(),
                cluster_config.get_partitions(),
                cluster_config.get_threads(),
                cluster_config.get_shared_mem())

        cur.execute(sql, data)
        row = cur.fetchone()

        if row is None:
            #  Should never happen
            msg = "Could not retrieve the cluster_id when inserting a new row into the cluster table"
            raise errors.DatabaseError(msg)

        row_id = row[0]

    elif len(rows) == 1:
        # This cluster configuration already exists.
        row = rows[0]
        row_id = row[0]

        # Validate that the fields haven't changed
        (cluster_id, cluster_name, num_nodes, partitions_per_node, threads_per_node, shared_mem) = row

        common_msg = "The contents of the cluster config file has been changed from what has " \
                     "previously been used. The configuration files should largely be considered " \
                     "immutable so that the meaning of past results is not altered. Please create " \
                     "a new config file if you wish to change the configuration.\n\n"

        if num_nodes != cluster_config.get_num_nodes():
            msg = f"{common_msg}The number of nodes in the cluster has changed. " \
                  f"Expected: {num_nodes}, Actual: {cluster_config.get_num_nodes()}"
            raise errors.ConfigurationError(msg)

        if partitions_per_node != cluster_config.get_partitions():
            msg = f"{common_msg}The partitions per node field has changed. " \
                  f"Expected: {partitions_per_node}, Actual: {cluster_config.get_partitions()}"
            raise errors.ConfigurationError(msg)

        if threads_per_node != cluster_config.get_threads():
            msg = f"{common_msg}The threads per node field has changed. " \
                  f"Expected: {threads_per_node}, Actual: {cluster_config.get_threads()}"
            raise errors.ConfigurationError(msg)

        if shared_mem != cluster_config.get_shared_mem():
            msg = f"{common_msg}The shared memory field has changed. " \
                  f"Expected: {shared_mem}, Actual: {cluster_config.get_shared_mem()}"
            raise errors.ConfigurationError(msg)

        LOG.debug(f"Cluster configuration already exists for clusterName {cluster_config.get_cluster_name()}")

    else:
        # Error, we've got > 1 rows returned.
        msg = f"There are multiple rows returned from the cluster table for " \
              f"clusterName = '{cluster_config.get_cluster_name()}' : {str(rows)}"
        raise errors.DatabaseConsistencyError(msg)

    return row_id


def __create_test_records(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                          job_config: JobConfig) -> [int]:
    """Creates records for the test files if one does not already exist.
       If a record does exist then it validates that the fields haven't changed.
       Returns a list of (test_path, test_id) tuples of the test row created or the matching test row found.
    """

    test_ids: [(string, int)] = []

    for test_path in job_config.get_test_paths():
        test_id = __create_test_record(cur, test_path)
        test_ids.append((test_path, test_id))

    return test_ids


def __create_test_record(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                         test_path: string) -> [int]:
    test_name = os.path.basename(test_path).rsplit(sep=".", maxsplit=1)[0]
    ext_name = os.path.basename(test_path).rsplit(sep=".", maxsplit=1)[1]

    sql = "SELECT testId, testName, extName " \
          "FROM tests WHERE testName = (%s)"
    data = (test_name,)

    cur.execute(sql, data)
    rows: [] = cur.fetchall()
    row_id: int

    if len(rows) == 0:
        LOG.debug(f"Creating a new entry in the tests table for testName {test_name}")

        # This test configuration is new. We need to insert a row into the database
        sql = "INSERT INTO tests (testName, extName) " \
              "VALUES (%s, %s) RETURNING testId"
        data = (test_name, ext_name)

        cur.execute(sql, data)
        row = cur.fetchone()

        if row is None:
            #  Should never happen
            msg = "Could not retrieve the test_id when inserting a new row into the tests table"
            raise errors.DatabaseError(msg)

        row_id = row[0]

    elif len(rows) == 1:
        # This test configuration already exists.
        row = rows[0]
        row_id = row[0]

        # Validate that the fields haven't changed
        (test_id, ret_test_name, ret_ext_name) = row

        common_msg = "The contents of the job config file has been changed from what has " \
                     "previously been used. The configuration files should largely be considered " \
                     "immutable so that the meaning of past results is not altered. Please create " \
                     "a new config file if you wish to change the configuration.\n\n"

        if ret_ext_name != ext_name:
            msg = f"{common_msg}The file extension for the test {test_name} has changed. " \
                  f"Expected: {ext_name}, Actual: {ret_ext_name}"
            raise errors.ConfigurationError(msg)

        LOG.debug(f"Test configuration already exists for testName {test_name}")

    else:
        # Error, we've got > 1 rows returned.
        msg = f"There are multiple rows returned from the tests table for " \
              f"testName = '{test_name}' : {str(rows)}"
        raise errors.DatabaseConsistencyError(msg)

    return row_id


def __create_jobs_record(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                         all_configs: AllConfigs,
                         hw_id: int,
                         cluster_id: int) -> int:
    """Creates a record for this job configuration if one does not already exist.
       If a record does exist then it validates that the fields haven't changed.
       Returns the id of the job created or the matching job found.
    """

    job: JobConfig = all_configs.get_job()

    sql = "SELECT jobId, jobName, hardwareId, clusterId FROM jobs WHERE jobName = (%s)"
    data = (job.get_job_name(),)

    cur.execute(sql, data)
    rows: [] = cur.fetchall()
    row_id: int

    if len(rows) == 0:
        LOG.debug(f"Creating a new entry in the jobs table for jobName {job.get_job_name()}")

        # This job configuration is new. We need to insert a row into the database
        sql = "INSERT INTO jobs (jobName, hardwareId, clusterId) VALUES (%s, %s, %s) RETURNING jobId"
        data = (job.get_job_name(), hw_id, cluster_id)

        cur.execute(sql, data)
        row = cur.fetchone()

        if row is None:
            #  Should never happen
            msg = "Could not retrieve the job_id when inserting a new row into the jobs table"
            raise errors.DatabaseError(msg)

        row_id = row[0]

    elif len(rows) == 1:
        # This job configuration already exists.
        # Validate that the other config files still match on file name
        row = rows[0]
        row_id = row[0]
        (job_id, job_name, hardware_id, cluster_id) = row

        sql = "SELECT hardwareName, clusterName FROM hardware, cluster " \
              "WHERE hardwareId = (%s) " \
              "AND clusterId = (%s)"
        data = (hardware_id, cluster_id)

        cur.execute(sql, data)
        row = cur.fetchone()

        if row is None:
            #  Should never happen
            msg = f"Could not retrieve the matching rows for hardware_id = {hardware_id} " \
                  f"and cluster_id = {cluster_id} from the hardware and cluster tables."
            raise errors.DatabaseError(msg)

        (hardware_name, cluster_name) = row

        common_msg = "The contents of the job config file has been changed from what has " \
                     "previously been used. The configuration files should largely be considered " \
                     "immutable so that the meaning of past results is not altered. Please create " \
                     "a new config file if you wish to change the configuration.\n\n"

        if hardware_name != all_configs.get_hardware().get_hardware_name():
            msg = f"{common_msg}The name of the hardware config file referenced in the job file has changed. " \
                  f"Expected: {all_configs.get_hardware().get_hardware_name()}, Actual: {hardware_name}"
            raise errors.ConfigurationError(msg)

        if cluster_name != all_configs.get_cluster().get_cluster_name():
            msg = f"{common_msg}The name of the cluster config file referenced in the job file has changed. " \
                  f"Expected: {all_configs.get_cluster().get_cluster_name()}, Actual: {cluster_name}"
            raise errors.ConfigurationError(msg)

        LOG.debug(f"Job configuration already exists for jobName {job.get_job_name()}")

    else:
        # Error, we've got > 1 rows returned.
        msg = f"There are multiple rows returned from the jobs table for jobName = '{job.get_job_name()}' : {str(rows)}"
        raise errors.DatabaseConsistencyError(msg)

    return row_id


def __create_jobresult_record(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                              job_id: string,
                              bamboo_plan_key: string,
                              bamboo_build_num: int,
                              git_hash: string,
                              git_branch: string,
                              invoking_user: string = None) -> int:
    """
    Create a record in the jobs table to indicate that a job has started.
    Return the jobResultId for this row so it can be updated later.
    """

    sql = "INSERT INTO jobResults " \
          "(jobId, bambooPlan, bambooBuild, gitHash, gitBranch, invokingUser, " \
          "timedOut, startTime, testsExecuted, testsPassed, testsFailed)" \
          "VALUES (%s, %s, %s, %s, %s, %s, %s, NOW(), %s, %s, %s) " \
          "RETURNING jobResultId"
    data = (job_id,  # jobId
            bamboo_plan_key,    # bambooPlan
            bamboo_build_num,   # bambooBuild
            git_hash,           # gitHash
            git_branch,         # gitBranch
            "SYSTEM" if invoking_user is None else invoking_user,  # invokingUser
            False,              # timedOut
            0,                  # testsExecuted
            0,                  # testsPassed
            0)                                                              # testsFailed

    LOG.debug(f"Creating a new entry in the jobResults table for job_id {job_id}")
    cur.execute(sql, data)

    row = cur.fetchone()
    assert row is not None

    job_result_id = row[0]
    LOG.debug(f"Got jobResultId of {job_result_id}")
    return job_result_id


def __create_tags(cur: typing.Union[psycopg.Cursor[typing.Any], psycopg.ServerCursor[typing.Any]],
                  job_result_id: int,
                  tags: {}):
    """
    Create records in the tags table for any tags being passed in here
    """

    if tags is None or len(tags) == 0:
        return

    sql = "INSERT INTO tags (jobResultId, tagKey, tagValue) VALUES (%s, %s, %s)"

    for key in tags:
        val = tags[key]
        data = (job_result_id, key, val)

        LOG.debug(f"Creating a new entry in the tags table for {key} = {val}")

        cur.execute(sql, data)

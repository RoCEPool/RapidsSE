import string

import boto3
import pathlib

# Various constants needed
C_ENDPOINT  = "postgres-database.cgkt3r1odnaa.us-east-1.rds.amazonaws.com"
C_PORT      = "5432"
C_USER      = "septest"
C_REGION    = "us-east-1"
C_DATABASE  = "septest"
C_SSLCERT_FILENAME = "us-east-1-bundle.pem"
C_SSLCERT_PATH     = str((pathlib.Path(__file__).parent / C_SSLCERT_FILENAME).resolve())

C_COMMON_CONNECT_PARAMS = {"host": C_ENDPOINT,
                           "port": C_PORT,
                           "database": C_DATABASE,
                           "user": C_USER,
                           "sslmode": "require",
                           "sslrootcert": C_SSLCERT_PATH
                           }


def get_auth_token() -> string:
    """Returns the authentication token required when making a new connection"""
    assert pathlib.Path(C_SSLCERT_PATH).exists()

    # Sessions are not thread safe
    # https://boto3.amazonaws.com/v1/documentation/api/latest/guide/session.html
    session = boto3.Session
    client = session.client(service_name='rds')

    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/rds.html#RDS.Client.generate_db_auth_token
    token = client.generate_db_auth_token(DBHostname=C_ENDPOINT,
                                          Port=C_PORT,
                                          DBUsername=C_USER,
                                          Region=C_REGION)
    return token


def get_connect_params() -> {}:
    """
    Returns the common database connection parameters, except for the password token
    and any other settings (e.g. autocommit)
    """
    return C_COMMON_CONNECT_PARAMS

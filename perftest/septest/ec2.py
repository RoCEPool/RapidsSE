import pathlib
import string
import pprint
import logging
import time
import subprocess

import botocore
import botocore.exceptions
import boto3
import base64

import septest.errors as errors
from septest.configs.hardwareconfig import HardwareConfig

# ---------- AWS Constants -------------
C_REGION = "us-east-1"
C_VPC_ID = "vpc-01293432312a06b76"

C_IAM_ROLE_ARN = "arn:aws:iam::076505786432:instance-profile/PerfTest"  # The IAM role to assume

C_SUBNET_A = "subnet-041ee117f205bc727"     # us-east-1a ; 192.168.1.0/24
C_SUBNET_B = "subnet-0e776deff59214e7f"     # us-east-1b ; 192.168.2.0/24
C_SUBNET_C = "subnet-08a8902a8805eac14"     # us-east-1c ; 192.168.3.0/24
C_SUBNET_D = "subnet-05ba2d55c547c3dea"     # us-east-1d ; 192.168.4.0/24
C_SUBNET_E = "subnet-0f309ade9393090a0"     # us-east-1e ; 192.168.5.0/24
C_SUBNET_F = "subnet-04a49a5add701b003"     # us-east-1f ; 192.168.6.0/24

C_AVAIL_1A = "us-east-1a"
C_AVAIL_1B = "us-east-1b"
C_AVAIL_1C = "us-east-1c"
C_AVAIL_1D = "us-east-1d"
C_AVAIL_1E = "us-east-1e"
C_AVAIL_1F = "us-east-1f"

C_SUBNET_MAP = {
    C_AVAIL_1A: "subnet-041ee117f205bc727",
    C_AVAIL_1B: "subnet-0e776deff59214e7f",
    C_AVAIL_1C: "subnet-08a8902a8805eac14",
    C_AVAIL_1D: "subnet-05ba2d55c547c3dea",
    C_AVAIL_1E: "subnet-0f309ade9393090a0",
    C_AVAIL_1F: "subnet-04a49a5add701b003"
}

C_SUBNET_RMAP = {
    "subnet-041ee117f205bc727": C_AVAIL_1A,
    "subnet-0e776deff59214e7f": C_AVAIL_1B,
    "subnet-08a8902a8805eac14": C_AVAIL_1C,
    "subnet-05ba2d55c547c3dea": C_AVAIL_1D,
    "subnet-0f309ade9393090a0": C_AVAIL_1E,
    "subnet-04a49a5add701b003": C_AVAIL_1F
}

C_SUBNET_ID_PREFERENCE = [
    C_SUBNET_MAP[C_AVAIL_1C],
    C_SUBNET_MAP[C_AVAIL_1B],
    C_SUBNET_MAP[C_AVAIL_1F],
    C_SUBNET_MAP[C_AVAIL_1A],
    C_SUBNET_MAP[C_AVAIL_1D],
    C_SUBNET_MAP[C_AVAIL_1E]
                         ]

C_SECURITY_GROUP_ID = "sg-0af380e49a6c6b7eb"    # Private security group (all in/outbound traffic allowed)
C_KEYPAIR_ID = "key-0b5ebe74924256d94"
C_KEYPAIR_NAME = "aws_rapidsse"
# --------------------------------------

# Notes:
# boto3 EC2 API:
#   https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html
#
# botocore exceptions defined here:
#   https://github.com/boto/botocore/blob/develop/botocore/exceptions.py
#
# boto3 Error Handling Guide:
#   https://boto3.amazonaws.com/v1/documentation/api/latest/guide/error-handling.html
#
# Permissions required for dynamically starting other EC2 instances:
#   https://aws.amazon.com/blogs/security/granting-permission-to-launch-ec2-instances-with-iam-roles-passrole-permission/
#

# ---------- Other Constants -------------
C_PROJ_ROOT = ".."
C_CLOUD_INIT_SCRIPT = f"{C_PROJ_ROOT}/seth/bamboo/aws/cloud-init-downloader.sh"
C_CLOUD_INIT_COMPLETE = "/tmp/cloud-init-complete"
C_SSH_CONNECT_TIMEOUT = 5  # seconds
C_MSG_PERIOD = 5  # Print updates every 5 seconds

LOG = logging.getLogger(__name__)

# ----------------------------------------


def describe_instances():
    session, client = __get_session_client()
    response = client.describe_instances()
    pprint.pprint(response)


def start_instances(cfg: HardwareConfig, timeout_sec: float) -> ([string], [string]):
    """
    Starts up AWS EC2 instances according to the hardware configuration
    and waits until they have booted and are ready to be used.
    Returns a tuple containing a list of EC2 instance-ids and a list
    of private ip addresses, in that order.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    session, client = __get_session_client()

    __check_instance_type(client, cfg.get_instance_type())
    __check_image_id(client, cfg.get_image_id())

    if cfg.get_use_spot_instances():
        msg = "EC2 spot instances are not supported in septest just yet"
        raise errors.Ec2Error(msg)

    # API Reference:
    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html#EC2.Client.run_instances
    kwargs = {
        "InstanceType": cfg.get_instance_type(),
        "MinCount": cfg.get_num_machines(),
        "MaxCount": cfg.get_num_machines(),
        "InstanceInitiatedShutdownBehavior": "terminate",
        "ImageId": cfg.get_image_id(),
        "KeyName": C_KEYPAIR_NAME,
        "Monitoring": {
            "Enabled": False
        },
        "SecurityGroupIds": [C_SECURITY_GROUP_ID],
        "SubnetId": C_SUBNET_F,
        "UserData": __get_boot_script(),
        "DisableApiTermination": False,
        "IamInstanceProfile": {
          "Arn": C_IAM_ROLE_ARN
        },
        "TagSpecifications": [{
            "ResourceType": "instance",
            "Tags": [{
                "Key": "PerfTest",
                "Value": ""
            }]
        }]}

    if LOG.isEnabledFor(logging.DEBUG):
        pp_args = pprint.pformat(kwargs)
        LOG.debug("About to dry-run start an instance with parameters:\n%s", pp_args)

    # Try a dry run to check for privileges
    try:
        client.run_instances(DryRun=True, **kwargs)
        LOG.debug("Dry-run start an instance was successful")

    except botocore.exceptions.ClientError as e:
        if e.response["Error"]["Code"] != "DryRunOperation":
            raise e

    # Try each subnet in order of preference
    last_error = None
    instance_ids = []
    ip_addresses = []

    for subnet_id in C_SUBNET_ID_PREFERENCE:
        kwargs["SubnetId"] = subnet_id

        try:
            LOG.debug("About to start an instance in subnet %s", C_SUBNET_RMAP[subnet_id])
            response = client.run_instances(DryRun=False, **kwargs)

            LOG.debug(f"Response of run_instances command:\n{pprint.pformat(response)}")

            last_error = None   # reset

            assert len(response["Instances"]) > 0

            for instance in response["Instances"]:
                instance_id = response["Instances"][0]["InstanceId"]
                ip_address = response["Instances"][0]["PrivateIpAddress"]
                instance_ids.append(instance_id)
                ip_addresses.append(ip_address)
            break

        except botocore.exceptions.CapacityNotAvailableError as e:
            # Save the exception and try the next subnet, just in case
            last_error = e

    if last_error is not None:
        raise last_error

    LOG.info("Started %d machines with the following instance-ids: %s", len(instance_ids), str(instance_ids))
    LOG.info("Private IP addresses assigned: %s", str(ip_addresses))

    # Wait for all of the instances to be usable
    __wait_for_instances_to_be_ready(instance_ids, ip_addresses, timeout_sec)

    return instance_ids, ip_addresses


def terminate_instances(instance_ids: [string]):
    """
    Terminates the EC2 instances given by instance_ids.
    """

    if instance_ids is None or len(instance_ids) == 0:
        LOG.info("No EC2 instances to terminate")
        return

    session, client = __get_session_client()

    # API Reference:
    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html#EC2.Client.terminate_instances
    kwargs = {
        "InstanceIds": instance_ids
    }

    if LOG.isEnabledFor(logging.DEBUG):
        pp_args = pprint.pformat(kwargs)
        LOG.debug("About to dry-run stop an instance with parameters:\n%s", pp_args)

    # Try a dry run to check for privileges
    try:
        client.terminate_instances(DryRun=True, **kwargs)
    except botocore.exceptions.ClientError as e:
        if e.response["Error"]["Code"] != "DryRunOperation":
            raise e

    LOG.info(f"About to terminate {len(instance_ids)} instance(s): {str(instance_ids)}")
    response = client.terminate_instances(DryRun=False, **kwargs)

    LOG.debug(f"Response of terminate_instances command:\n{pprint.pformat(response)}")

    assert len(response["TerminatingInstances"]) == len(instance_ids)
    for instance in response["TerminatingInstances"]:
        assert instance["CurrentState"]["Name"] == "shutting-down" or \
               instance["CurrentState"]["Name"] == "terminated" or \
               instance["CurrentState"]["Name"] == "stopping" or \
               instance["CurrentState"]["Name"] == "stopped", \
               f"Instance ID {instance['InstanceId']} is not in a terminating state: {instance['CurrentState']['Name']}"

    LOG.info(f"All instance IDs have been terminated: {str(instance_ids)}")


def __get_session_client():
    """
    Returns a new session and client object for interaction with EC2 programmatically.
    """
    session = boto3.Session()
    client = session.client(service_name='ec2', region_name=C_REGION)
    return session, client


def __check_instance_type(client, instance_type: string):
    """
    Check that the instance type is valid.
    If not, then a botocore.exceptions.ClientError is thrown with a message
    that the instance type does not exist.
    """
    LOG.info("Validating the instance type: %s", instance_type)
    response = client.describe_instance_types(InstanceTypes=[instance_type])
    LOG.debug(f"Response of describe_instances command:\n{pprint.pformat(response)}")


def __check_image_id(client, image_id: string):
    """Check that the image ID is valid."""
    kwargs = {
        "ImageIds": [image_id],
        "DryRun": False
    }

    LOG.info("Validating the imageId: %s", image_id)
    response = client.describe_images(**kwargs)
    LOG.debug(f"Response of describe_images command:\n{pprint.pformat(response)}")

    image_name = response["Images"][0]["Name"]
    LOG.info(f"Using AMI: {image_name} (image-id: {image_id})")
    # pprint.pp(response)


def __get_boot_script() -> string:
    """
    Returns a base-64 encoded script to run when the machine is first created.
    This reads the file, encodes it and returns the encoded bytes as a string.
    """
    cloud_init_path = pathlib.Path(C_CLOUD_INIT_SCRIPT)
    if not cloud_init_path.exists():
        msg = f"Could not find the local cloud-init script file at: {cloud_init_path.resolve()}"
        raise errors.Ec2Error(msg)

    txt = cloud_init_path.read_text()

    encoded_bytes = base64.b64encode(txt.encode("utf-8"))
    encoded_str = encoded_bytes.decode()

    assert len(encoded_str) > 0
    assert len(encoded_str) < 16384     # 16 KB AWS limit

    return encoded_str


def __wait_for_instances_to_be_ready(instance_ids: [string], ip_addresses: [string], timeout_sec: float):
    """
    Waits for the set of instances to be runnable, SSH-able, and finished booting.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    LOG.info("Waiting for %d instance(s) to start up...", len(instance_ids))

    current_time = time.time()
    end_time = current_time + timeout_sec
    time_left = timeout_sec

    # First we need the EC2 instances to change from the pending to the runnable state.
    # This means that they have been created and are either booting or running.
    op_start = current_time
    __wait_for_instances_to_be_running(instance_ids, time_left)
    current_time = time.time()
    LOG.info("All EC2 instances have transitioned from pending to running in %.1f seconds", current_time - op_start)

    # Now we need to wait for the machines to listen for SSH connections
    time_left = end_time - current_time
    op_start = current_time
    __wait_for_instances_to_accept_ssh(instance_ids, ip_addresses, time_left)
    current_time = time.time()
    LOG.info("All EC2 instances have booted enough to accept SSH connections in %.1f seconds", current_time - op_start)

    # Now we need to wait for the machines to have completed the cloud-init script.
    time_left = end_time - current_time
    op_start = current_time
    __wait_for_instances_to_finish_booting(instance_ids, ip_addresses, time_left)
    current_time = time.time()
    time_left = end_time - current_time
    LOG.info("All EC2 instances have completed running their cloud-init script in %.1f seconds",
             current_time - op_start)

    # Now the machines are ready.
    LOG.info("All instances are now ready to be used. Total startup time: %.1f seconds", timeout_sec - time_left)


def __wait_for_instances_to_be_running(instance_ids: [string], timeout_sec: float):
    """
    Waits for the newly started instances to transition to a running state.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    assert timeout_sec > 1

    session, client = __get_session_client()

    LOG.info("Waiting for %d VM instance(s) to be created...", len(instance_ids))

    # API Reference:
    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html#EC2.Client.describe_instances
    kwargs = {
        "InstanceIds": instance_ids,
    }

    current_time = time.time()
    end_time = current_time + timeout_sec

    # Wait 1 seconds to start with before issuing the first request
    time.sleep(1.0)
    logged_response = False
    attempts = 0

    while current_time < end_time:
        attempts += 1
        response = client.describe_instances(**kwargs)

        if not logged_response:
            logged_response = True
            LOG.debug(f"Response of describe_instances command:\n{pprint.pformat(response)}")

        if attempts % C_MSG_PERIOD == 0:
            LOG.debug("Time remaining (VM creation): %.1f seconds", end_time - current_time)

        assert len(response["Reservations"]) == 1
        instances = response["Reservations"][0]["Instances"]
        assert len(instances) == len(instance_ids)

        all_ready = True
        for instance in instances:
            if instance["State"]["Name"] != "running":
                all_ready = False
                break

        if all_ready:
            LOG.info("All VM instances have been created are in the 'running' state")
            return

        time.sleep(1.0)
        current_time = time.time()

    # We timed out
    msg = f"Timed out waiting for {len(instance_ids)} VM instance(s) to be created and enter the 'running' state"
    LOG.error(msg)
    raise errors.Ec2InstanceTimeoutError(msg, instance_ids)


def __wait_for_instances_to_accept_ssh(instance_ids: [string], ip_addresses: [string], time_left: float):
    """
    Waits for the newly started instances to accept SSH connections.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    LOG.info("Waiting for %d VM instance(s) to accept SSH connections with IPs: %s",
             len(instance_ids),
             str(ip_addresses))

    current_time = time.time()
    end_time = current_time + time_left

    for ip_addr in ip_addresses:
        cmd = ["ssh", "-q",
               "-o", "BatchMode=yes",
               "-o", "StrictHostKeyChecking=no",
               "-o", f"ConnectTimeout={C_SSH_CONNECT_TIMEOUT}",
               ip_addr,
               "echo \"Connected!\""]

        last_error = None
        attempts = 0

        current_time = time.time()
        while current_time < end_time:
            attempts += 1

            if attempts % C_MSG_PERIOD == 0:
                LOG.debug("Time remaining (accept SSH): %.1f seconds", end_time - current_time)

            try:
                # subprocess.run(cmd,
                subprocess.run(cmd,
                               capture_output=True,
                               check=True,
                               text=True)
                LOG.info(f"Instance at {ip_addr} is now accepting SSH connections")
                break

            except subprocess.CalledProcessError as e:
                if attempts % C_MSG_PERIOD == 0:
                    LOG.debug(f"Instance at {ip_addr} not yet accepting SSH connections: "
                              f"Exit code = {e.returncode}, stdout = \"{e.stdout}\", stderr = \"{e.stderr}\"")
                last_error = e.stderr
                time.sleep(1.0)
                current_time = time.time()
                continue

        if current_time >= end_time:
            # We timed out
            msg = f"Timed out waiting for instance at {ip_addr} to accept SSH connections. " + \
                  f"Last error message: {last_error}" if last_error is not None else ""
            LOG.error(msg)
            raise errors.Ec2InstanceTimeoutError(msg, instance_ids)

        # Go on to the next host

    # All hosts are reachable over SSH!


def __wait_for_instances_to_finish_booting(instance_ids: [string], ip_addresses: [string], time_left: float):
    """
    Waits for the newly started instances to complete their boot-up cloud-init script.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    assert len(instance_ids) == len(ip_addresses)

    LOG.info("Waiting for %d VM instance(s) to complete executing the cloud-init script...", len(instance_ids))

    current_time = time.time()
    end_time = current_time + time_left

    for (instance_id, ip_addr) in zip(instance_ids, ip_addresses):

        cmd = ["ssh", "-q",
               "-o", "BatchMode=yes",
               "-o", "StrictHostKeyChecking=no",
               "-o", f"ConnectTimeout={C_SSH_CONNECT_TIMEOUT}",
               ip_addr,
               f"cat {C_CLOUD_INIT_COMPLETE}"]

        last_error = None
        attempts = 0

        current_time = time.time()
        while current_time < end_time:
            attempts += 1

            if attempts % C_MSG_PERIOD == 0:
                LOG.debug("Time remaining (cloud-init): %.1f seconds", end_time - current_time)

            try:
                result = subprocess.run(cmd,
                                        capture_output=True,
                                        check=True,
                                        text=True)

                LOG.info(f"Instance {instance_id} at {ip_addr} has completed it's cloud-init script:\n{result.stdout}")
                break

            except subprocess.CalledProcessError as e:
                if attempts % C_MSG_PERIOD == 0:
                    LOG.debug(f"Instance at {ip_addr} has not yet completed its cloud-init script")
                last_error = e.stderr
                time.sleep(1.0)
                current_time = time.time()
                continue

        if current_time >= end_time:
            # We timed out
            msg = f"Timed out waiting for instance at {ip_addr} to complete executing its cloud-init script. " + \
                  f"Last error message: {last_error}" if last_error is not None else ""
            LOG.error(msg)
            raise errors.Ec2InstanceTimeoutError(msg, instance_ids)

        # Go on to the next host

    # All hosts are reachable over SSH!

import pathlib
import pprint
import logging
import time
import subprocess

import botocore
import botocore.exceptions
import boto3

import pyutils.errors as errors
import pyutils.ssh as ssh

# ---------- AWS Constants -------------
C_REGION = "us-east-1"
C_VPC_ID = "vpc-01293432312a06b76"

C_DEFAULT_IAM_ROLE_ARN = "arn:aws:iam::076505786432:instance-profile/PerfTest"  # The default IAM role to assume

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

C_VOLUME_SIZE_GB = 20
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
C_CLOUD_INIT_COMPLETE = "/tmp/cloud-init-complete"
C_SSH_CONNECT_TIMEOUT = 5  # seconds
C_MSG_PERIOD = 20  # Print updates every 20 seconds
C_RETRY_INTERVAL = 10  # seconds
C_MAX_RETRIES = 20     # before giving up

LOG = logging.getLogger(__name__)

# ----------------------------------------


def describe_instances():
    session, client = __get_session_client()
    response = client.describe_instances()
    # pprint.pprint(response)


def get_image_desc(image_id: str, ctx: str = "") -> str:
    session, client = __get_session_client()

    kwargs = {
        "ImageIds": [image_id],
        "DryRun": False
    }

    LOG.info(f"{ctx}Getting the OS for the imageId: {image_id}")
    response = client.describe_images(**kwargs)
    # LOG.debug(f"{ctx} Response of describe_images command:\n{pprint.pformat(response)}")

    image_name = response["Images"][0]["Description"]
    return image_name


def start_instances(instance_type: str,
                    image_id: str,
                    cloudinit_path: str,
                    iam_role_name: str,
                    count: int,
                    name_prefix: str,
                    timeout_sec: float,
                    use_spot_instances: bool,
                    tags: {} = None,
                    ctx: str = "",
                    cloudinit_src: pathlib.Path = None,
                    cloudinit_dest: pathlib.Path = None) -> ([str], [str]):
    """
    Starts up AWS EC2 instances according to the hardware configuration
    and waits until they have booted and are ready to be used.
    Returns a tuple containing a list of EC2 instance-ids and a list
    of private ip addresses, in that order.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    session, client = __get_session_client()

    __check_instance_type(client, instance_type, ctx)
    __check_image_id(client, image_id, ctx)

    if use_spot_instances:
        msg = f"{ctx}EC2 spot instances are not supported just yet"
        raise errors.Ec2Error(msg)

    # API Reference:
    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html#EC2.Client.run_instances
    kwargs = {
        "InstanceType": instance_type,
        "MinCount": count,
        "MaxCount": count,
        "InstanceInitiatedShutdownBehavior": "terminate",
        "ImageId": image_id,
        "KeyName": C_KEYPAIR_NAME,
        "BlockDeviceMappings": [
            {
                "DeviceName": __get_root_device_name(client, image_id),
                "Ebs": {
                    "DeleteOnTermination": True,
                    "VolumeSize": C_VOLUME_SIZE_GB
                }
            }
        ],
        "Monitoring": {
            "Enabled": False
        },
        "SecurityGroupIds": [C_SECURITY_GROUP_ID],
        "SubnetId": C_SUBNET_F,
        "UserData": __get_boot_script(cloudinit_path, ctx),
        "DisableApiTermination": False,
        "IamInstanceProfile": { },
        "TagSpecifications": [{
            "ResourceType": "instance",
            "Tags": [
                {
                    "Key": name_prefix,
                    "Value": ""
                }]
        }]}

    if iam_role_name is not None and len(iam_role_name) != 0:
        # Use the supplied value
        kwargs["IamInstanceProfile"]["Name"] = iam_role_name

    elif C_DEFAULT_IAM_ROLE_ARN is not None and len(C_DEFAULT_IAM_ROLE_ARN):
        # Use the default value
        kwargs["IamInstanceProfile"]["Arn"] = C_DEFAULT_IAM_ROLE_ARN

    else:
        del kwargs["IamInstanceProfile"]

    if tags is not None:
        for tag_key in tags:
            tag_val = tags[tag_key]
            kwargs["TagSpecifications"][0]["Tags"].append({"Key": tag_key, "Value": tag_val})

    if LOG.isEnabledFor(logging.DEBUG):
        # pp_args = pprint.pformat(kwargs)
        LOG.debug(f"{ctx}About to dry-run start an instance to check for privileges")

    # Try a dry run to check for privileges
    try:
        client.run_instances(DryRun=True, **kwargs)
        LOG.debug(f"{ctx}Dry-run start an instance was successful")

    except botocore.exceptions.ClientError as e:
        if e.response["Error"]["Code"] != "DryRunOperation":
            raise

    # Try each subnet in order of preference
    last_error = None
    instance_ids = []
    ip_addresses = []

    retry_count = 0
    i = -1
    while i < len(C_SUBNET_ID_PREFERENCE) - 1:
        i += 1
        subnet_id = C_SUBNET_ID_PREFERENCE[i]
        kwargs["SubnetId"] = subnet_id

        try:
            LOG.debug(f"{ctx}About to start an instance in subnet %s", C_SUBNET_RMAP[subnet_id])
            response = client.run_instances(DryRun=False, **kwargs)

            LOG.debug(f"{ctx}Response of run_instances command:\n{pprint.pformat(response)}")

            last_error = None   # reset

            assert len(response["Instances"]) > 0

            for instance in response["Instances"]:
                instance_id = instance["InstanceId"]
                ip_address = instance["PrivateIpAddress"]
                instance_ids.append(instance_id)
                ip_addresses.append(ip_address)
            break

        except botocore.exceptions.CapacityNotAvailableError as e:
            # Save the exception and try the next subnet, just in case
            last_error = errors.Ec2InsufficientCapacity(getattr(e, 'message', repr(e)))

        except botocore.exceptions.ClientError as e:
            if e.response["Error"]["Code"] == "InsufficientInstanceCapacity":
                # Save the exception and try the next subnet
                last_error = errors.Ec2InsufficientCapacity(getattr(e, 'message', repr(e)))
                LOG.warning(f"{ctx}Got error response InsufficientInstanceCapacity in region "
                            f"{C_SUBNET_RMAP[subnet_id]}.")
                
                if (i+1) < len(C_SUBNET_ID_PREFERENCE):
                    LOG.info("Trying another region.")
                else:
                    LOG.error("No more regions left to try.")
                continue

            elif e.response["Error"]["Code"] == "RequestLimitExceeded":
                retry_count += 1

                if retry_count >= C_MAX_RETRIES:
                    raise

                i -= 1
                LOG.warning(f"{ctx}Got temporary error RequestLimitExceeded while trying to create {count} "
                            f"instance(s). Sleeping for {C_RETRY_INTERVAL} seconds before trying again")
                time.sleep(C_RETRY_INTERVAL)
                continue

            elif e.response["Error"]["Code"] == "Unsupported":
                # Ignore this exception and try the next subnet
                LOG.warning(f"{ctx}Got error response {e.response['Error']['Code']} for instance "
                            f"type {instance_type} in region {C_SUBNET_RMAP[subnet_id]}.")

                if (i + 1) < len(C_SUBNET_ID_PREFERENCE):
                    LOG.info("Trying another region.")
                else:
                    # Only because we have to.
                    last_error = errors.Ec2InsufficientCapacity(getattr(e, 'message', repr(e)))
                    LOG.error("No more regions left to try.")
                continue

            else:
                # Save the exception and try the next subnet
                last_error = e
                LOG.warning(f"{ctx}Got error response {e.response['Error']['Code']} in region "
                            f"{C_SUBNET_RMAP[subnet_id]}. Full error message:\n{str(e)}")

                if (i + 1) < len(C_SUBNET_ID_PREFERENCE):
                    LOG.info("Trying another region.")
                else:
                    LOG.error("No more regions left to try.")
                continue

    if last_error is not None:
        raise last_error

    LOG.info(f"{ctx}Started {len(instance_ids)} machines with the following instance-ids: {str(instance_ids)}")
    LOG.info(f"{ctx}Private IP addresses assigned: {str(ip_addresses)}")

    # Wait for all of the instances to be usable
    try:
        __wait_for_instances_to_be_ready(instance_ids, ip_addresses, timeout_sec, name_prefix, client, ctx,
                                         cloudinit_src, cloudinit_dest)

    except (Exception,) as e:
        # Clean up, but try to ignore any errors in the process
        try:
            terminate_instances(instance_ids, ctx)
        except botocore.exceptions.ClientError:
            pass
        except AssertionError:
            pass

        raise e

    return instance_ids, ip_addresses


def terminate_instances(instance_ids: [str], ctx: str = ""):
    """
    Terminates the EC2 instances given by instance_ids.
    """

    if instance_ids is None or len(instance_ids) == 0:
        LOG.info(f"{ctx}No EC2 instances to terminate")
        return

    session, client = __get_session_client()

    # API Reference:
    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html#EC2.Client.terminate_instances
    kwargs = {
        "InstanceIds": instance_ids
    }

    if LOG.isEnabledFor(logging.DEBUG):
        pp_args = pprint.pformat(kwargs)
        LOG.debug(f"{ctx}About to dry-run stop an instance with parameters:\n%s", pp_args)

    # Try a dry run to check for privileges
    try:
        client.terminate_instances(DryRun=True, **kwargs)
    except botocore.exceptions.ClientError as e:
        if e.response["Error"]["Code"] != "DryRunOperation":
            raise

    LOG.info(f"{ctx}About to terminate {len(instance_ids)} instance(s): {str(instance_ids)}")
    response = client.terminate_instances(DryRun=False, **kwargs)

    LOG.debug(f"{ctx}Response of terminate_instances command:\n{pprint.pformat(response)}")

    assert len(response["TerminatingInstances"]) == len(instance_ids)
    for instance in response["TerminatingInstances"]:
        assert instance["CurrentState"]["Name"] == "shutting-down" or \
               instance["CurrentState"]["Name"] == "terminated" or \
               instance["CurrentState"]["Name"] == "stopping" or \
               instance["CurrentState"]["Name"] == "stopped", \
               f"{ctx}Instance ID {instance['InstanceId']} is not in a terminating state: " \
               f"{instance['CurrentState']['Name']}"

    LOG.info(f"{ctx}All instance IDs have been terminated: {str(instance_ids)}")


def __get_session_client(ctx: str = ""):
    """
    Returns a new session and client object for interaction with EC2 programmatically.
    """
    session = boto3.Session()
    client = session.client(service_name='ec2', region_name=C_REGION)
    return session, client


def is_valid_instance_type(instance_type: str, ctx: str = "") -> bool:
    (session, client) = __get_session_client()
    try:
        __check_instance_type(client, instance_type, ctx)
        return True
    except botocore.exceptions.ClientError:
        return False


def __check_instance_type(client, instance_type: str, ctx: str = ""):
    """
    Check that the instance type is valid.
    If not, then a botocore.exceptions.ClientError is thrown with a message
    that the instance type does not exist.
    """
    # LOG.info(f"{ctx}Validating the instance type: %s", instance_type)
    response = client.describe_instance_types(InstanceTypes=[instance_type])
    # LOG.debug(f"{ctx}Response of describe_instances command:\n{pprint.pformat(response)}")

    assert response["InstanceTypes"][0]["InstanceType"] == instance_type


def is_valid_image_id(image_id: str, ctx: str = "") -> bool:
    (session, client) = __get_session_client()
    try:
        __check_image_id(client, image_id, ctx)
        return True
    except botocore.exceptions.ClientError:
        return False


def __check_image_id(client, image_id: str, ctx: str = ""):
    """Check that the image ID is valid."""
    kwargs = {
        "ImageIds": [image_id],
        "DryRun": False
    }

    LOG.info(f"{ctx}Validating the imageId: %s", image_id)
    response = client.describe_images(**kwargs)
    # LOG.debug(f"{ctx}Response of describe_images command:\n{pprint.pformat(response)}")

    if len(response['Images']) != 1:
        msg = f"{ctx}Image ID does not exist: {image_id}"
        raise errors.Ec2Error(msg)

    image_desc = response["Images"][0]["Description"]
    LOG.info(f"{ctx}Using AMI: {image_desc} (image-id: {image_id})")
    # pprint.pp(response)


def __get_root_device_name(client, image_id: str, ctx: str = "") -> str:
    """Returns the name of the root device, given an image_id"""
    kwargs = {
        "ImageIds": [image_id],
        "DryRun": False
    }

    response = client.describe_images(**kwargs)

    assert len(response['Images']) == 1

    for mapping in response['Images'][0]['BlockDeviceMappings']:
        if mapping.get('Ebs') is not None:
            return mapping['DeviceName']

    assert False, "Could not determine the root EBS device name"


def __get_boot_script(path_str: str, ctx: str = "") -> str:
    """
    Returns contents of a script to run when the machine is first created.
    """
    cloud_init_path = pathlib.Path(path_str)
    assert cloud_init_path.exists()

    LOG.info(f"{ctx}Reading cloud-init script from path: {path_str}")
    txt = cloud_init_path.read_text()
    LOG.info(f"{ctx}Cloud-init script is {len(txt)} bytes long")

    assert len(txt) > 0
    assert len(txt) < 16384     # 16 KB AWS limit

    return txt


def __set_instance_names(instance_ids: [str], name_prefix: str, client, ctx: str = "") -> None:
    """
    Sets the 'name' tag of each instance using the prefix and a unique incrementing number.
    """
    # Set a Name tag for each instance
    for n, instance_id in enumerate(instance_ids, start=1):
        kwargs = {
            "DryRun": False,
            "Resources": [instance_id],
            "Tags": [
                {
                    "Key": "Name",
                    "Value": f"{name_prefix}#{n:02}"
                }
            ]
        }
        LOG.debug(f"{ctx}About to create a Name tag for instance %s", instance_id)
        response = client.create_tags(**kwargs)
        LOG.debug(f"{ctx}Response of create_tags command:\n{pprint.pformat(response)}")


def __wait_for_instances_to_be_ready(instance_ids: [str], ip_addresses: [str], timeout_sec: float,
                                     name_prefix: str = None, client=None, ctx: str = "",
                                     cloudinit_src: pathlib.Path = None, cloudinit_dest: pathlib.Path = None):
    """
    Waits for the set of instances to be runnable, SSH-able, and finished booting.
    If name_prefix and client are set then these instances are named when they are ready.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    LOG.info(f"{ctx}Waiting for %d instance(s) to start up...", len(instance_ids))

    current_time = time.time()
    end_time = current_time + timeout_sec
    time_left = timeout_sec

    # First we need the EC2 instances to change from the pending to the runnable state.
    # This means that they have been created and are either booting or running.
    op_start = current_time
    __wait_for_instances_to_be_running(instance_ids, time_left, timeout_sec, ctx)
    current_time = time.time()
    LOG.info(f"{ctx}All EC2 instances have transitioned from pending to running in %.1f seconds",
             current_time - op_start)

    if name_prefix is not None and client is not None:
        __set_instance_names(instance_ids, name_prefix, client, ctx)

    # Now we need to wait for the machines to listen for SSH connections
    time_left = end_time - current_time
    op_start = current_time
    __wait_for_instances_to_accept_ssh(instance_ids, ip_addresses, time_left, timeout_sec, ctx)
    current_time = time.time()
    LOG.info(f"{ctx}All EC2 instances have booted enough to accept SSH connections in %.1f seconds",
             current_time - op_start)

    # Now we need to wait for the machines to have completed the cloud-init script.
    time_left = end_time - current_time
    op_start = current_time
    __wait_for_instances_to_finish_booting(instance_ids, ip_addresses, time_left, timeout_sec, ctx,
                                           cloudinit_src, cloudinit_dest)
    current_time = time.time()
    time_left = end_time - current_time
    LOG.info(f"{ctx}All EC2 instances have completed running their cloud-init script in %.1f seconds",
             current_time - op_start)

    # Now the machines are ready.
    LOG.info(f"{ctx}All instances are now ready to be used. Total startup time: %.1f seconds", timeout_sec - time_left)


def __wait_for_instances_to_be_running(instance_ids: [str], timeout_sec: float, total_time: float, ctx: str = ""):
    """
    Waits for the newly started instances to transition to a running state.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    assert timeout_sec > 1

    session, client = __get_session_client()

    LOG.info(f"{ctx}Waiting for %d VM instance(s) to be created...", len(instance_ids))

    # API Reference:
    # https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/ec2.html#EC2.Client.describe_instances
    kwargs = {
        "InstanceIds": instance_ids,
    }

    current_time = time.time()
    end_time = current_time + timeout_sec

    time.sleep(1.0)
    attempts = 0

    while current_time < end_time:
        time.sleep(1.0)
        current_time = time.time()

        attempts += 1

        if attempts % C_MSG_PERIOD == 0:
            LOG.debug(f"{ctx}Time remaining (VM creation): %.1f seconds", end_time - current_time)

        try:
            response = client.describe_instances(**kwargs)
        except botocore.exceptions.ClientError as e:
            # https://boto3.amazonaws.com/v1/documentation/api/latest/guide/error-handling.html
            if e.response['Error']['Code'] == 'InvalidInstanceID.NotFound':
                # This can occur because AWS internals are eventually consistent.
                # So the API endpoint might not know about the instance just yet.
                # Reference: https://docs.aws.amazon.com/AWSEC2/latest/APIReference/
                #            query-api-troubleshooting.html#eventual-consistency
                continue
            else:
                raise

        assert len(response["Reservations"]) == 1
        instances = response["Reservations"][0]["Instances"]
        assert len(instances) == len(instance_ids)

        all_ready = True
        for instance in instances:
            if instance["State"]["Name"] != "running":
                all_ready = False
                break

        if all_ready:
            LOG.info(f"{ctx}All VM instances have been created are in the 'running' state")
            return

    # We timed out
    msg = f"{ctx}Timed out waiting for {len(instance_ids)} VM instance(s) to be created and enter the " \
          f"'running' state. Total time waited = {str(total_time)} seconds."
    LOG.error(msg)

    # Terminate the machines here as we haven't passed the instance_ids back to the caller yet.
    try:
        terminate_instances(instance_ids, ctx)
    except botocore.exceptions.ClientError:
        pass
    except AssertionError:
        pass

    raise errors.Ec2InstanceTimeoutError(msg, instance_ids)


def __wait_for_instances_to_accept_ssh(instance_ids: [str], ip_addresses: [str], time_left: float,
                                       total_time: float, ctx: str = ""):
    """
    Waits for the newly started instances to accept SSH connections.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    LOG.info(f"{ctx}Waiting for %d VM instance(s) to accept SSH connections with IPs: %s",
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
                LOG.debug(f"{ctx}Time remaining (accept SSH): %.1f seconds", end_time - current_time)

            try:
                # subprocess.run(cmd,
                subprocess.run(cmd,
                               capture_output=True,
                               check=True,
                               text=True)
                LOG.info(f"{ctx}Instance at {ip_addr} is now accepting SSH connections")
                break

            except subprocess.CalledProcessError as e:
                if attempts % C_MSG_PERIOD == 0:
                    LOG.info(f"{ctx}Instance at {ip_addr} not yet accepting SSH connections: "
                              f"Exit code = {e.returncode}, stdout = \"{e.stdout}\", stderr = \"{e.stderr}\"")
                last_error = e.stderr
                time.sleep(1.0)
                current_time = time.time()
                continue

        if current_time >= end_time:
            # We timed out
            msg = f"{ctx}Timed out waiting for instance at {ip_addr} to accept SSH connections. " + \
                  f"Total time waited = {str(total_time)} seconds. " + \
                  f"Last error message: {last_error}" if last_error is not None else ""
            LOG.error(msg)

            # Terminate the machines here as we haven't passed the instance_ids back to the caller yet.
            try:
                terminate_instances(instance_ids, ctx)
            except botocore.exceptions.ClientError:
                pass
            except AssertionError:
                pass

            raise errors.Ec2InstanceTimeoutError(msg, instance_ids)

        # Go on to the next host

    # All hosts are reachable over SSH!


def __wait_for_instances_to_finish_booting(instance_ids: [str], ip_addresses: [str], time_left: float,
                                           total_time: float, ctx: str = "",
                                           cloudinit_src: pathlib.Path = None, cloudinit_dest: pathlib.Path = None):
    """
    Waits for the newly started instances to complete their boot-up cloud-init script.
    If cloudinit_src and cloudinit_dest are set then the cloud-init log file is copied
    if the startup times out.
    Throws an errors.Ec2InstanceTimeoutError if the timeout is exceeded.
    """

    assert len(instance_ids) == len(ip_addresses)

    LOG.info(f"{ctx}Waiting for %d VM instance(s) to complete executing the cloud-init script...", len(instance_ids))

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
                LOG.info(f"{ctx}Time remaining (cloud-init): %.1f seconds", end_time - current_time)

            try:
                result = subprocess.run(cmd,
                                        capture_output=True,
                                        check=True,
                                        text=True)

                LOG.info(f"{ctx}Instance {instance_id} at {ip_addr} has completed it's cloud-init script:"
                         f"\n{result.stdout}")
                break

            except subprocess.CalledProcessError as e:
                if attempts % C_MSG_PERIOD == 0:
                    LOG.info(f"{ctx}Instance at {ip_addr} has not yet completed its cloud-init script")
                last_error = e.stderr
                time.sleep(1.0)
                current_time = time.time()
                continue

        if current_time >= end_time:
            # We timed out
            msg = f"{ctx}Timed out waiting for instance at {ip_addr} to complete executing its cloud-init script. " + \
                  f"Total time waited = {str(total_time)} seconds. " + \
                  f"Last error message: {last_error}" if last_error is not None else ""
            LOG.error(msg)

            if cloudinit_src is not None and cloudinit_dest is not None:
                __copy_cloudinit_logs(ip_addresses, cloudinit_src, cloudinit_dest, ctx)

            # Terminate the machines here as we haven't passed the instance_ids back to the caller yet.
            try:
                terminate_instances(instance_ids, ctx)
            except botocore.exceptions.ClientError:
                pass
            except AssertionError:
                pass

            raise errors.Ec2InstanceTimeoutError(msg, instance_ids)

        # Go on to the next host

    # All hosts are reachable over SSH!


def __copy_cloudinit_logs(ip_addresses: [str], cloudinit_src: pathlib.Path, cloudinit_dest: pathlib.Path,
                          ctx: str = ""):

    LOG.info(f"{ctx} Trying to copy cloud-init scripts from the failed instances at {ip_addresses}")

    for ip in ip_addresses:
        # Add "-<ip_address>" into the filename so it doesn't conflict
        suffix = cloudinit_dest.suffix
        dest_filename = f"{cloudinit_dest.name.removesuffix(suffix)}" \
                        f"-{ip}{suffix}"

        src = f"{ip}:{str(cloudinit_src)}"
        dest = str(cloudinit_dest.parent / dest_filename)

        LOG.debug(f"About to collect cloud-init logs from {src} to {dest}")

        try:
            ssh.run_scp(src, dest, 30, ctx=ctx)

        except subprocess.TimeoutExpired as e:
            msg = f"{ctx}: Collecting of cloud-init logs from the machine at {ip} " \
                  f"timed out after {e.timeout} seconds." \
                  f"\nStdout Output:\n{e.stdout}" \
                  f"\nStderr Output:\n{e.stderr}"
            LOG.error(msg)
            # Keep trying other instances in this test

        except errors.SshError as e:
            LOG.error(e)
            # Keep trying other instances in this test

def record_started_instances(instance_ids: [str], instance_file: pathlib.Path):
    """
    Records the list of running instances to file.
    :param instance_ids: list of the instances IDs that were started
    :param instance_file: path of the file to write to
    :return: None
    """

    if instance_ids is None or len(instance_ids) == 0:
        LOG.info("No running instance IDs to record to file for later cleanup.")
        return

    with open(instance_file, 'w') as f:
        for instance_id in instance_ids:
            f.write(instance_id)
            f.write('\n')

    LOG.info(f"Recorded {len(instance_ids)} instance IDs to file for later cleanup: {str(instance_file)}")

def terminate_instances_from_file(instance_file: pathlib.Path):
    """
    Terminates the instance IDs found in the file
    :param instance_file: the path of the file containing the instance IDs
    :return: None
    """
    if not instance_file.exists():
        LOG.info(f"No running instance file exists. No instances to stop: {str(instance_file)}")
        return

    instance_ids: [str] = []

    with open(instance_file, 'r') as f:
        lines = f.readlines()

        for line in lines:
            line = line.strip()

            if len(line) == 0:
                continue

            if line.startswith('#'):
                continue

            instance_ids.append(line)

    if len(instance_ids) == 0:
        LOG.info(f"There were no instance IDs to be terminated in the file: {str(instance_file)}")
        return

    LOG.info(f"Found {len(instance_ids)} instance(s) to be terminated in file: {str(instance_file)}")
    try:
        terminate_instances(instance_ids)

    except (Exception,):
        LOG.error(f"Error encountered while terminating the following instance IDs: {', '.join(instance_ids)}")
        raise

    LOG.info(f"{len(instance_ids)} instances terminated.")

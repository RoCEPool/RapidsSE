import string
import pprint
import logging
import boto3

import errors
from septest.configs.hardwareconfig import HardwareConfig

C_REGION    = "us-east-1"

LOG = logging.getLogger(__name__)


def describe_instances():
    session, client = __get_session_client()
    response = client.describe_instances()
    pprint.pprint(response)


def start_instances(cfg: HardwareConfig):
    session, client = __get_session_client()

    __check_instance_type(client, cfg.get_instance_type())
    __check_image_id(client, cfg.get_image_id())

    if cfg.get_use_spot_instances():
        msg = "EC2 spot instances are not supported in septest just yet"
        raise errors.Ec2Error(msg)

    kwargs = {
        "InstanceType": cfg.get_instance_type(),
        "MinCount": cfg.get_num_machines(),
        "MaxCount": cfg.get_num_machines(),
        "InstanceInitiatedShutdownBehavior": "terminate",
        "DisableApiTermination": False,
        "DryRun": True,
        "ImageId": cfg.get_image_id(),
        "KeyName": "XXXXXXXXXX",
        "SubnetId": "XXXXXXXXXX",
        "TagSpecifications": [{
            "ResourceType": "instance",
            "Tags": [{
                "Key": "PerfTest",
                "Value": ""
            }]
        }],
        "UserData": "XXXXXX"   # Must be base64 encoded text. Max 16KB

    }
    response = client.run_instances(**kwargs)

    pass

def stop_instances():
    session, client = __get_session_client()
    pass


def __get_session_client():
    session = boto3.Session()
    client = session.client(service_name='ec2', region_name=C_REGION)
    return session, client


def __check_instance_type(client, instance_type: string):
    """
    Check that the instance type is valid.
    If not, then a botocore.exceptions.ClientError is thrown with a message
    that the instance type does not exist.
    """
    response = client.describe_instance_types(InstanceTypes=[instance_type])
    # pprint.pp(response)

def __check_image_id(client, image_id: string):
    """Check that the image ID is valid."""
    kwargs = {
        "ImageIds": [image_id]
        "DryRun": False
    }
    response = client.describe_images(**kwargs)

    image_name = response["Images"][0]["Name"]
    LOG.info(f"Using AMI: {image_name} (image-id: {image_id})")
    # pprint.pp(response)

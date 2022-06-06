""""Runs the build job"""
import datetime
import time
import os
import shutil
import string
import logging
import threading
import pathlib
import tempfile

import releasecommon.ec2 as ec2
import releasecommon.errors as errors
from releasecommon.releaseconfig import ReleaseConfig

LOG = logging.getLogger(__name__)

# How long should we wait for all of the EC2 instances to be started
# and be ready to use?
C_EC2_BOOT_TIMEOUT_SEC = 300
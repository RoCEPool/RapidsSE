#!/bin/bash

# This script is intended to work around a limitation of the cloud-init scripts
# being constrained to under 16KB. This script will download the real
# cloud-init script from s3 and then run it.
#
# NOTE: The ec2 instance will need have an appropriate IAM profile that gives
#       it access to read from S3.

###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Print an error message to stderr and exit,
## Params:
##   $1 : Error message
##   $2..$n : (optional) other arguments to print.

function errorExit() {
  >&2 echo -e "ERROR: $*"
  exit 1
}

###############################################################################
### Main Script                                                             ###
###############################################################################

BUCKET="s3://rapids-cloud-init"
SCRIPT="cloud-init.sh"


# Set up any pre-requisites
if ! which aws; then
  # AWS CLI needs curl and unzip
  yum install -y curl unzip

  curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
  unzip awscliv2.zip
  ./aws/install
fi

# Download the script
echo "Downloading the real cloud-init script"
aws s3 cp "${BUCKET}/${SCRIPT}" /tmp/"${SCRIPT}" || errorExit "Could not download the cloud-init script"
chmod +x /tmp/"${SCRIPT}"

# Run it!
echo "Running the real cloud-init script"
/tmp/"${SCRIPT}"

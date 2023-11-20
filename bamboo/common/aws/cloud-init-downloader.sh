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

RELEASE="unknown"
if [ -f "/etc/system-release" ]; then
  SYSREL="$(cat "/etc/system-release")"

  case "${SYSREL}" in
    "Amazon Linux release 2023 "*)
      RELEASE="AL2023"
      ;;

    "Amazon Linux release 2 "*)
      RELEASE="AL2"
      ;;

    *)
      RELEASE="AL2"
      ;;
  esac
fi

BUCKET="s3://rapids-cloud-init"
SCRIPT="cloud-init.${RELEASE}.sh"

# Set the package manager
PKGMAN="yum"
if ! which yum &>/dev/null; then
  PKGMAN="dnf"
  if ! which dnf &>/dev/null; then
    PKGMAN="apt"
    if ! which apt &>/dev/null; then
      errorExit "Could not find a supported package manager. Tried yum, dnf and apt."
    fi
  fi
fi

# Set up any pre-requisites
# Note that we retry any network operations as these can fail or timeout for
# various reasons. A timeout means that we ultimately cannot connect to this
# machine.

if ! which aws; then
  # AWS CLI needs curl and unzip
  while ! "${PKGMAN}" install -y curl unzip; do
    sleep 1
  done

  while ! curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"; do
    sleep 1
  done

  unzip awscliv2.zip
  ./aws/install
fi

# Download the script
echo "Downloading the real cloud-init script"

while ! aws s3 cp "${BUCKET}/${SCRIPT}" /tmp/"${SCRIPT}"; do
  sleep 1
done

chmod +x /tmp/"${SCRIPT}"

# Run it!
echo "Running the real cloud-init script"
/tmp/"${SCRIPT}"

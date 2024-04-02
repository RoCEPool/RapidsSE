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

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# A function that retries a command until it returns a successful exit code.
# Each subsequent attempt is separated by a 1 second delay.
# Params:
#  $1 : maximum number of retries
#  $@ : The command to execute, as an array
function retryCommand() {
  local maxRetries="$1"
  shift
  local cmd=("$@")
  local retries=1

  # check that maxRetries has been provided
  re='^[0-9]+$'
  if ! [[ "${maxRetries}" =~ $re ]] ; then
     errorExit "First parameter to function retryCommand() is not a number"
  fi

  while [ "${retries}" -le "${maxRetries}" ]; do
    if [ "${retries}" -gt 1 ]; then
      echo "...retry attempt #${retries}"
    fi

    ((retries++))

    if "${cmd[@]}"; then
      true
      return
    fi

    sleep 1
  done

  false
  return
}
# ----------------------------------------------------------------------


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

    "Amazon Linux release 2023."*)
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

MAX_NETWORK_RETRIES=10
MAX_YUM_RETRIES=3

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
  declare -a cmd
  cmd=("${PKGMAN}" "install" "-y" "curl" "unzip")
  if ! retryCommand "${MAX_YUM_RETRIES}" "${cmd[@]}"; then
    errorExit "Could not download and install curl and unzip - the ${PKGMAN} repository may be"\
              "unavailable or a network error could be preventing access."
  fi

  cmd=("curl" "-L" "--fail" "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" "-o" "awscliv2.zip")
  if retryCommand "${MAX_NETWORK_RETRIES}" "${cmd[@]}"; then
    unzip awscliv2.zip
    ./aws/install
  else
    errorExit "Could not download AWS CLIv2 - the version might be old and may need to be updated"\
              "or a network error could be preventing access."
  fi
fi

# Download the script
echo "Downloading the real cloud-init script"

cmd=("aws" "s3" "cp" "${BUCKET}/${SCRIPT}" "/tmp/${SCRIPT}")
if ! retryCommand "${MAX_NETWORK_RETRIES}" "${cmd[@]}"; then
  errorExit "Could not copy the cloud-init script from s3"
fi

chmod +x /tmp/"${SCRIPT}"

# Run it!
echo "Running the real cloud-init script"
/tmp/"${SCRIPT}"

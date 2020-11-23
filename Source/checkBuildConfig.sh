#!/bin/bash

# Checks whether a makefile's target binary was last built with the same 
# target configuration (debug/release). If it wasn't, then the binary is
# removed so it can be rebuilt.

# Parameters: 
#  $1 : path of the binary
#  $2 : path of the file containing the configuration it was built with.
#  $3 : the configuration being built now.


BINARY_PATH="$1"
LAST_CONFIG_PATH="$2"
CURRENT_BUILD_CONFIG="$3"

if [ "$#" -ne 3 ]; then
  >&2 echo "Usage: ./$0 <binaryPath> <lastConfigPath> <currentBuildConfig>"
  exit 1
fi

# If the binary doesn't exist then there's nothing to do.
if [ ! -f "${BINARY_PATH}" ]; then
  exit 0
fi

# If the file with the last build config doesn't exist then 
# let us assume that we need to rebuild the binary.
if [ ! -f "${LAST_CONFIG_PATH}" ]; then
  echo "Removing the binary ${BINARY_PATH} because it's last build config is unknown."
  rm -f "${BINARY_PATH}"
fi

# Get the configuration that the binary was last built with.
LAST_CONFIG="$(cat "${LAST_CONFIG_PATH}")"

if [ "${LAST_CONFIG}" != "${CURRENT_BUILD_CONFIG}" ]; then
  echo "Removing the binary ${BINARY_PATH} because it's last build config (${LAST_CONFIG}) does not match the current build config (${CURRENT_BUILD_CONFIG})."
  rm -f "${BINARY_PATH}"
fi

exit 0

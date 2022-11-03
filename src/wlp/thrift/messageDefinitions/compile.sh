#!/bin/bash

# This file compiles all *.thrift files found in subdirectories of here
# and writes them out into the typical maven source directory.
# It is only necessary to run this when the thrift message definition files
# are changed.
# This tool depends on thrift being installed and available on the system PATH.

# Path to the directory that this script is in.
DIR=$(dirname "$0")

# Find the latest version directory that matches "v<num>".
LATEST_THRIFT_DIR="$(find -E "${DIR}" -type d -regex ".*/v[0-9]+" -maxdepth 1 | sort -n -k 2 -t v | tail -n 1)"
echo "Latest thrift dir is: ${LATEST_THRIFT_DIR}"
COMPAT_THRIFT_DIR="${DIR}/compatibility"

THRIFT_FILES=$(find "${LATEST_THRIFT_DIR}" -type f -name "*.thrift" -print0  | xargs -0)
THRIFT_FILES+=$(find "${COMPAT_THRIFT_DIR}" -type f -name "*.thrift" -print0  | xargs -0)
OUTPUT_DIR="${DIR}/../generated"

# Check for thrift support
if ! which thrift >/dev/null 2>&1
then
  echo "Cannot find the thrift executable. Check that it is installed and is in the system path." 1>&2
  exit 1
fi

# Print the thrift version
echo "Found $(thrift -version)"

# Exit on any error return code encountered
set -o errexit

mkdir -p "${OUTPUT_DIR}"

COUNTER=0

for f in ${THRIFT_FILES}
do
  echo -n "Compiling ${f}... "
  #thrift -r --out "${OUTPUT_DIR}" --gen cpp "${f}"
  echo "Done."

  COUNTER="$((COUNTER + 1))"
done

echo "All done. ${COUNTER} schema files processed and written to ${OUTPUT_DIR}."

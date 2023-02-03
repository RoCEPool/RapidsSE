#!/bin/bash

DIR="$(dirname "$0")"

EX_SCRIPT="${DIR}/rrFragGen.ex"

# Check for ex
which ex &>/dev/null || { echo "This tool requires 'ex' to be installed."; exit 1; }

[ "$#" -eq 2 ] || { echo "Usage: $0 <file_to_convert> <output_file>"; exit 1; }
SRC_FILE="$1"
OUTPUT_FILE="$2"

[ -f "${SRC_FILE}" ] || { echo "Error: ${SRC_FILE} does not exist or is not a file."; exit 1; }

# Now apply it to the file
ex -s -E "${SRC_FILE}" -S "${EX_SCRIPT}" -c "% w! ${OUTPUT_FILE}" -c "q!"
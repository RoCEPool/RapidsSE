#!/bin/bash
# This file produces a HTML fragment file (a partial HTML file that can be
# #included into another file) of the syntax tree SVG railroad diagrams.
# It takes the xhtml SVG file and turns it into a regular html file and
# strips out any unnecessary stuff.

DIR="$(dirname "$0")"

EX_SCRIPT="${DIR}/rrHtmlFragGen.ex"

# Check for ex
which ex &>/dev/null || { echo "This tool requires 'ex' to be installed."; exit 1; }

[ "$#" -eq 2 ] || { echo "Usage: $0 <file_to_convert> <output_file>"; exit 1; }
SRC_FILE="$1"
OUTPUT_FILE="$2"

[ -f "${SRC_FILE}" ] || { echo "Error: ${SRC_FILE} does not exist or is not a file."; exit 1; }

# Now apply it to the file
ex -s -E "${SRC_FILE}" -S "${EX_SCRIPT}" -c "% w! ${OUTPUT_FILE}" -c "q!"
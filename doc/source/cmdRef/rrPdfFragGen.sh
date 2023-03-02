#!/bin/bash
# This file produces a partial rST file that can be included in another rST
# page to show the syntax tree railroad diagrams. It does this by walking the
# grammar file, finding the rules and creating links to the png diagrams that
# exist in the PNG/HTML railroad diagram output.

DIR="$(dirname "$0")"

INDENT="   "
AWK_SCRIPT=""
read -r -d '' AWK_SCRIPT <<EOF

BEGIN {
  inComment = 0;
}

END {
#  print "${INDENT}"
}

# Whole line comment
/^ *\/\*[^*]\*\/ *$/   {
  next
}

# Block quote start
\$1 == "/*" && \$0 !~ /\*\// {
  inComment = 1
  next
}

# Block quote end
\$1 == "*/" && \$0 !~ /\/\*/ {
  inComment = 0
  next
}

/^[a-zA-Z0-9]+.*/ && inComment == 0 {
  # Some rules don't have a space between their names and "::="
  split(\$1, arr, "::=")

  ruleName = arr["1"]
  imageFile = "/rr/diagram/" ruleName ".png"

  print ".. rubric:: " ruleName ":"
  print ""
  print ".. _" ruleName "Syntax:"
  print ""
  print ".. image:: " imageFile
  print "${INDENT}" ":alt: ``" ruleName "`` rule syntax"
  print "${INDENT}" ":align: left"

  if (imageFile == "/rr/diagram/expr.png") {
    print "${INDENT}" ":height: 23.5cm"
  }

  print ""
  print ""
}

EOF

# Check for awk
which awk &>/dev/null || { echo "This tool requires 'awk' to be installed."; exit 1; }

[ "$#" -eq 2 ] || { echo "Usage: $0 <grammar_file> <output_file>"; exit 1; }
SRC_FILE="$1"
OUTPUT_FILE="$2"

[ -f "${SRC_FILE}" ] || { echo "Error: ${SRC_FILE} does not exist or is not a file."; exit 1; }

# Now apply it to the file
awk "${AWK_SCRIPT}" "${SRC_FILE}" > "${OUTPUT_FILE}"
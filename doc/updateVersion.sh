#!/bin/bash

readonly DIR="$(dirname "${BASH_SOURCE[0]}")"
readonly SPHINX_CONFIG_FILE="${DIR}/source/conf.py"
readonly VERSION_SH="${DIR}/../src/version.sh"
readonly TEMPFILE="${DIR}/tempfile"

# Get the latest version information
"${DIR}/../src/generateVersion.sh" --bash
source "${VERSION_SH}"

# Update the conf.py file with this information
SED_EXPR1="s/^version = .*$/version = '${MAJOR_VERSION}.${MINOR_VERSION}'/"
SED_EXPR2="s/^release = .*$/release = '${VERSION_STRING}'/"
sed -e "${SED_EXPR1}" -e "${SED_EXPR2}" "${SPHINX_CONFIG_FILE}" > "${TEMPFILE}" && mv "${TEMPFILE}" "${SPHINX_CONFIG_FILE}"

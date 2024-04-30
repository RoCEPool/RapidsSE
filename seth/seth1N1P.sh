#!/bin/bash

# Convenience shortcut to run the single node single partition unit tests (1N1P)
DIR=$(dirname "$0")
REPO_ROOT="${DIR}/.."
TESTLIST_FILE="${REPO_ROOT}/bamboo/unittest/testlists/1N1P.testlist"

args=("-f" "${TESTLIST_FILE}")

if [ $# -gt 0 ]; then
  args+=("$@")
fi

"${DIR}/sethTest.sh" "${args[@]}"

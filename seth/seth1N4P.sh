#!/bin/bash

# Convenience shortcut to run the single node multi, partition unit tests (1N4P).
# To run in parallel, pass the arguments "-p <num_parallel_tasks>"
# where <num_parallel_tasks> is the number of tests to be run concurrently.

DIR=$(dirname "$0")
REPO_ROOT="${DIR}/.."
TESTLIST_FILE="${REPO_ROOT}/bamboo/unittest/testlists/1N4P.testlist"

args=("-f" "${TESTLIST_FILE}")

if [ $# -gt 0 ]; then
  args+=("$@")
fi

"${DIR}/sethTest.sh" "${args[@]}"

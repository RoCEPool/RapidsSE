#!/bin/bash

# Convenience shortcut to run the single node multi, partition unit tests (1N4P)
DIR=$(dirname "$0")
REPO_ROOT="${DIR}/.."
TESTLIST_FILE="${REPO_ROOT}/bamboo/unittest/testlists/1N4P.testlist"

"${DIR}/sethTest.sh" -f "${TESTLIST_FILE}" $@

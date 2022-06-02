#!/bin/bash

# Convenience shortcut to run the multinode unit tests (2N4P)
DIR=$(dirname "$0")
REPO_ROOT="${DIR}/.."
TESTLIST_FILE="${REPO_ROOT}/bamboo/unittest/testlists/2N4P.testlist"

"${DIR}/sethTest.sh" -f "${TESTLIST_FILE}" $@

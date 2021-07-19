#!/bin/bash

# Convenience shortcut to run the single node single partition unit tests
DIR=$(dirname "$0")
TESTLIST_FILE="${DIR}/bamboo/testlists/1N1P.testlist"

"${DIR}/sethTest.sh" -f "${TESTLIST_FILE}"
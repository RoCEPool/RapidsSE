#!/bin/bash

# Convenience shortcut to run the single node multi, partition unit tests (1N4P)
DIR=$(dirname "$0")
TESTLIST_FILE="${DIR}/bamboo/testlists/1N4P.testlist"

"${DIR}/sethTest.sh" -f "${TESTLIST_FILE}" $@

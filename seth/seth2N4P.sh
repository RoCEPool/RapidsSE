#!/bin/bash

# Convenience shortcut to run the multinode unit tests (2N4P)
DIR=$(dirname "$0")
TESTLIST_FILE="${DIR}/bamboo/testlists/2N4P.testlist"

"${DIR}/sethTest.sh" -f "${TESTLIST_FILE}" $@

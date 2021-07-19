#!/bin/bash

# Convenience shortcut to run the multinode unit tests
DIR=$(dirname "$0")
TESTLIST_FILE="${DIR}/bamboo/testlists/MN.testlist"

"${DIR}/sethTest.sh" -f "${TESTLIST_FILE}"
#!/bin/bash

DIR=$(dirname "$0")

# Read the file ${DIR}/tests/testlist for the list of all tests to run.
${DIR}/seth.sh -f ${DIR}/tests/testList

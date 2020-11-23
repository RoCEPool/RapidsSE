#!/bin/bash

# The intent of this script file is to build and start RapidsSE and run all
# of the unit tests against it. Bamboo itself should have very little knowledge
# of how to do any of this. That way we can change the layout/nature of tests
# and it remains consistent to a given checkin. i.e., we should be able to 
# check out an older version and re-run the Bamboo tests against it.

# This script file is called by Bamboo after it has checked out the repository.

###############################################################################
### Environment Variables                                                   ###
###############################################################################

# If these environment variables are defined and are non-null then they will
# alter how this script executes.
#
# NO_BUILD              Do not build the software.
# TARGETS               The array of targets to build and test. Currently only
#                       supporting "debug" and "release".
# NO_SINGLE_NODE_TESTS  Do not run the single node tests.
# NO_MULTI_NODE_TESTS   Do not run the multi-node tests.
# NO_COMPRESS           Do not compress the large binary artifacts afterwards.

# e.g. NO_BUILD=1 NO_COMPRESS=1 ./runBamboo.sh
#
# e.g. TARGET="debug" ./runBamboo.sh


###############################################################################
### Constants                                                               ###
###############################################################################

readonly DIR="$(dirname "$0")"
readonly PROJ_ROOT="${DIR}/../.."
readonly BIN_DIR="${PROJ_ROOT}/bin"
readonly SRC_DIR="${PROJ_ROOT}/src"

readonly RELATIVE_BAMBOO_DIR="Seth/bamboo"  # Relative to the project root

# Set to 1 to print debug messages in Bamboo.
readonly DEBUG=0

readonly NUM_CPUS="$(nproc)"

# The default build targets to compile and test.
readonly DEFAULT_TARGETS=("debug" "release")

###############################################################################
### Significant Global Variables                                            ###
###############################################################################


###############################################################################
### Functions                                                               ###
###############################################################################

## An exit hook that runs whether the script is exiting cleanly or in error.
## Params: None

function onExit() {
  # kill any rogue SE and SETH instances owned by the current user.
  killRogueInstances 

  # Compress any large files before they get collected by Bamboo
  compressLargeFiles
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Print an error message to stderr and exit,
## Params: 
##   $1 : Error message
##   $2..$n : (optional) other arguments to print.

function errorExit() {
  >&2 echo -e "ERROR: $*"
  exit 1
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Prints a given message if debug mode is turned on (i.e., $DEBUG == 1).
## Params:
##  $@ : The arguments to print

function debugPrint() {
  if [ -n "${DEBUG}" ] && [ "${DEBUG}" -eq 1 ]; then
    echo -e "$*"
  fi
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Kills any rogue instances of rapidsse and seth processes.
## Params: None

function killRogueInstances() {
  # Kill all rapidsse instances
  pkill -u $UID -x rapidsse &>/dev/null || true

  # Kill SE Prompt children of SETH
  { jps | grep Seth | cut -d ' ' -f 1 | xargs -r -n 1 ps -o pid= --ppid | xargs -I{} kill -9 {}; } &>/dev/null  || true

  # Kill all SETH instances
  { jps | grep Seth | cut -d ' ' -f 1 | xargs -r -I{}  kill -9 {}; } &>/dev/null || true
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Compresses any large artifact files that were created by building or
## running the tests.
## Params: None

function compressLargeFiles() {

  if [ "${NO_COMPRESS:+x}" == x ]; then
    echo "Skipping compression of large files."
    return
  fi

  # Run in a subshell so that we can change directory temporarily
  (
    # Compress the binaries
    cd "${BIN_DIR}" 
    local sharedObj

    sharedObj="$(ls lib*.so 2>/dev/null)"

    if [ -f "${sharedObj}" ]; then
      tar cvzf "${sharedObj}.tar.gz" "${sharedObj}" &>/dev/null
      rm "${sharedObj}" &>/dev/null
    fi

    local seBinary="rapidsse"
    if [ -f "${seBinary}" ]; then
      tar cvzf "${seBinary}.tar.gz" "${seBinary}" &>/dev/null
      rm "${seBinary}" &>/dev/null
    fi
  )
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Builds RapidsSE for a given target.
## Params:
##  $1 : the target to be build ("debug" or "release")

function buildSE() {

  local target="$1"

  if [[ "${target}" != "debug" && "${target}" != "release" ]]; then
    errorExit "Invalid target for building RapidsSE. Must be \"debug\" or \"release\" : ${target}"
  fi

  if [ "${NO_BUILD:+x}" == x ]; then
    echo "Skipping building RapidsSE"
    return
  fi

  echo ""
  echo "********************************************************************************"
  echo "*** Building RapidsSE for ${target}"
  echo "********************************************************************************"
  echo ""

  # Run in a subshell so we can change directories without any consequence.
  (
    cd "${SRC_DIR}"

    # Enable gcc version 5. Required for antlr.
    local gccVersion
    local hasOldGcc

    gccVersion="$(gcc -dumpversion | cut -f1-2 -d.)"
    hasOldGcc="$(echo "${gccVersion} < 5" | bc)"

    if [ "${hasOldGcc}" -eq 1 ]; then
      scl enable devtoolset-7 "make clean && time make ${target} -j${NUM_CPUS}"

    else
      make clean && time make "${target}" "-j${NUM_CPUS}"
    fi
  )
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Starts SE in a 1 node cluster and runs the SETH tests against it.
## Params:
##  $1 : the target being tested ("debug" or "release")
function testSE1Node() {

  local target="$1"

  if [[ "${target}" != "debug" && "${target}" != "release" ]]; then
    errorExit "Invalid target for building RapidsSE. Must be \"debug\" or \"release\" : ${target}"
  fi

  if [ "${NO_SINGLE_NODE_TESTS:+x}" == x ]; then
    echo "Skipping single node tests."
    return
  fi

  echo ""
  echo "********************************************************************************"
  echo "*** Testing RapidsSE - Single Node (${target})"
  echo "********************************************************************************"
  echo ""

  startSE1Node "${target}"
  runSeth "${target}"
  stopSE1Node "${target}"
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Starts up a single node instance of RapidsSE.
## Params: 
##  $1 : the target being tested ("debug" or "release")

function startSE1Node() {

  local target="$1"

  echo ""
  echo "Running single node RapidsSE for ${target}."
  echo ""

  "${DIR}"/startSE.sh
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Runs SETH against a currently running cluster of SE.
## Params:
##  $1 : the target being tested ("debug" or "release")

function runSeth() {
  local target="$1"

  echo ""
  echo "Running SETH tests again RapidsSE for ${target}."
  echo ""

  # Start from the project root directory so that SE can change to ./Commands successfully.
  (
    cd "${PROJ_ROOT}"
    ./"${RELATIVE_BAMBOO_DIR}"/seth.sh --resultname "results-${target}.xml" --testsuffix " (${target})"
  )
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Stops a currently running single node cluster of RapidsSE.
## Params:
##  $1 : the target being tested ("debug" or "release")

function stopSE1Node() {
  local target="$1"

  echo ""
  echo "Stopping single node RapidsSE for ${target}."
  echo ""
  
  "${DIR}"/stopSE.sh
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Starts SE in a 2 node cluster and runs the SETH tests against it.
## Params:
##  $1 : the target being tested ("debug" or "release")
function testSE2Nodes() {

  local target="$1"

  if [[ "${target}" != "debug" && "${target}" != "release" ]]; then
    errorExit "Invalid target for building RapidsSE. Must be \"debug\" or \"release\" : ${target}"
  fi

  if [ "${NO_MULTI_NODE_TESTS:+x}" == x ]; then
    echo "Skipping multi-node tests."
    return
  fi

  echo ""
  echo "********************************************************************************"
  echo "*** Testing RapidsSE - Two Nodes (${target})"
  echo "********************************************************************************"
  echo ""

  startSE2Nodes "${target}"
  runSeth2Nodes "multinode-${target}"
  stopSE2Nodes "${target}"
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Starts up a two node instance of RapidsSE.
## Params: 
##  $1 : the target being tested ("debug" or "release")

function startSE2Nodes() {

  local target="$1"
  
  echo ""
  echo "Running two node RapidsSE for ${target}."
  echo ""
  
  "${DIR}"/startSE_2nodes.sh
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Runs SETH against a currently running cluster of SE.
## Params:
##  $1 : the target being tested ("debug" or "release")

function runSeth2Nodes() {
  local target="$1"
  
  echo ""
  echo "Running SETH tests again RapidsSE for ${target}."
  echo ""

  # Start from the project root directory so that SE can change to ./Commands successfully.
  (
    cd "${PROJ_ROOT}"
    ./"${RELATIVE_BAMBOO_DIR}"/seth_2nodes.sh --resultname "results-${target}.xml" --testsuffix " (${target})"
  )
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Stops a currently running two node cluster of RapidsSE.
## Params:
##  $1 : the target being tested ("debug" or "release")

function stopSE2Nodes() {
  local target="$1"

  echo ""
  echo "Stopping two node RapidsSE for ${target}."
  echo ""

  "${DIR}"/stopSE_2nodes.sh
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Pretty prints a given number of seconds as days/hours/minutes/seconds instead.
## Params:
##  $1 : the total number of seconds

function secondsToTime() {

  local T="$1"
  local D="$((T/60/60/24))"
  local H="$((T/60/60%24))"
  local M="$((T/60%60))"
  local S="$((T%60))"
  (( D > 0 )) && printf '%d days ' "${D}"
  (( H > 0 )) && printf '%d hours ' "${H}"
  (( M > 0 )) && printf '%d minutes ' "${M}"
  (( D > 0 || H > 0 || M > 0 )) && printf 'and '
  printf '%d seconds' "${S}"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

###############################################################################
### Main Script                                                             ###
###############################################################################

# Cause Bash to exit on command errors, unset variables and pipe errors.
set -eEuo pipefail

# Record the start time so we can work out the elapsed time.
STARTTIME="$(date +%s)"

echo ""
echo "********************************************************************************"
echo "*** Beginning the Continuous Integration Script                              ***"
echo "********************************************************************************"
echo ""
echo "Started at: $(date)"
echo ""


# Call the exit hook, onExit(), whenever the script actually exits.
trap onExit EXIT

# First, kill any rogue SE and SETH instances owned by the current user.
killRogueInstances

#shellcheck disable=2206
TEST_TARGETS=(${TARGETS:-})

if [ "${#TEST_TARGETS[@]}" -eq 0 ]; then
  # Use the default targets
  TEST_TARGETS=("${DEFAULT_TARGETS[@]}")
fi

for target in "${TEST_TARGETS[@]}"
do
  # Build RapidsSE for a given target type
  buildSE "${target}"

  # Test SE (single node)
  testSE1Node "${target}"

  # Test SE (two nodes)
  testSE2Nodes "${target}"
done

ENDTIME="$(date +%s)"
ELAPSEDTIME="$(secondsToTime $((ENDTIME - STARTTIME)))"

echo ""
echo "********************************************************************************"
echo "*** Continuous Integration Script Complete                                   ***"
echo "********************************************************************************"
echo ""
echo "Elapsed time: ${ELAPSEDTIME}"
echo ""


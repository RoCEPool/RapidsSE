#!/bin/bash
# This is called by Bamboo. Bamboo will populate the environment with various
# variables, some of which are used below.

#BASE="$(basename $0)"
DIR="$(dirname $0)"

# Which python to use?
PYTHON="python3"

# Pyenv stuff
PYENV_EXE="${HOME}/.pyenv/bin/pyenv"

# Bamboo system variables
PLAN_KEY="${bamboo_planKey}"
BUILD_NUM="${bamboo_buildNumber}"
INVOKING_USER="${bamboo_ManualBuildTriggerReason_userName}"
GIT_BRANCH="${bamboo_planRepository_branch}"

# Plan variables (defined in Bamboo)
JOB_FILE="${bamboo_jobFile}"
ARCHIVE_METRICS="${bamboo_archiveMetrics}"

# Where results and archived files are written out
RESULT_DIR="${DIR}/results"


if [ -z "${JOB_FILE}" ]; then
  >&2 echo "Error: environment variable \${bamboo_jobFile} is not set with the job file to run."
  exit 1
fi

if [ -z "${PLAN_KEY}" ]; then
  >&2 echo "Error: environment variable \${bamboo_planKey} is not set with the Bamboo plan key (e.g. \"SE-PERF\")."
  exit 1
fi

if [ -z "${BUILD_NUM}" ]; then
  >&2 echo "Error: environment variable \${bamboo_buildNumber} is not set with the Bamboo build number (e.g. 123)."
  exit 1
fi


# Build the mandatory command line parameters to the python septest script
PARAMS="--job ${JOB_FILE} --planKey ${PLAN_KEY} --buildNum ${BUILD_NUM} --resultDir ${RESULT_DIR}"

# Add in any optional command line parameters:
  # Git hash
if [ -n "${GIT_BRANCH}" ]; then
  PARAMS="${PARAMS} --branch ${GIT_BRANCH}"
fi

  # Was this invoked by a user (manual build) or a periodic system build?
if [ -n "${INVOKING_USER}" ]; then
  PARAMS="${PARAMS} --user ${INVOKING_USER}"
fi

if [ -n "${ARCHIVE_METRICS}" ] && [ "${ARCHIVE_METRICS}" -ne 0 ] && [ "${ARCHIVE_METRICS,,}" != "false" ]; then
  PARAMS="${PARAMS} --archiveMetrics"
fi



# Check the environment:
  # Do we have pyenv setup to virtualise python?
if [ -f "${PYENV_EXE}" ]; then

  # Is Pyenv setup?
  if [ -z "${PYENV_ROOT}" ] || ! which pyenv &>/dev/null; then
    echo "Setting up pyenv environment"
    echo
    export PYENV_ROOT="${HOME}/.pyenv"
    export PATH="${PYENV_ROOT}/bin:${PATH}"
    eval "$(pyenv init --path)"
    export PATH
  fi

  PYTHON="python"
fi

if ! which "${PYTHON}" &>/dev/null; then
  >&2 echo "${PYTHON} must be installed."
  exit 1
fi

readarray -t PYTHON_VER < <(${PYTHON} -V | cut -f2 -d ' ' | tr "." "\n")
echo "Found Python version ${PYTHON_VER[0]}.${PYTHON_VER[1]}.${PYTHON_VER[2]}"

if [ "${PYTHON_VER[0]}" -lt 3 ] || [[ "${PYTHON_VER[0]}" -eq 3 && "${PYTHON_VER[1]}" -lt 5 ]]; then
  >&2 echo "Python 3.5 or later is required. You have Python ${PYTHON_VER[0]}.${PYTHON_VER[1]}.${PYTHON_VER[2]}"
  exit 1
fi

if ! which pipenv &>/dev/null; then
  >&2 echo "pipenv must be installed."
  exit 1
fi

if [ "${PWD}" != "${DIR}" ]; then
  cd "${DIR}"
fi

if ! [ -f "Pipfile" ]; then
  >&2 echo "No Pipfile found in directory ${PWD}."
  exit 1
fi

# Install dependencies
echo
pipenv sync --bare || exit 1

# Make the directory for results if it doesn't exist
mkdir -p "${RESULT_DIR}"

echo
echo "Invoking septest with PARAMS = ${PARAMS}"
echo

# Run the system
# shellcheck disable=SC2086
pipenv run ${PYTHON} -m septest ${PARAMS} "$@"


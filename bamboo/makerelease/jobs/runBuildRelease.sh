#!/bin/bash
# This is called by Bamboo. Bamboo will populate the environment with various
# variables, some of which are used below.

DIR="$(dirname $0)"
PLAN_DIR="$(realpath "${DIR}/..")"            # Directory of <repoRoot>/bamboo/makerelease
REPO_ROOT="$(realpath "${PLAN_DIR}/../..")"   # Directory of the repository root

MODULE="buildrelease"

# Bamboo system variables
# PLAN_KEY="${bamboo_planKey}"                    # e.g. SE-REL
# BUILD_NUM="${bamboo_buildNumber}"               # e.g. 123
# GIT_BRANCH="${bamboo_planRepository_branch}"    # e.g. master

# Plan variables (defined in Bamboo)
#   None used

# Where results and archived files are written out
RESULT_DIR="${PLAN_DIR}/results"

# Directory where artifacts are to be copied to
ARTIFACT_DIR="${PLAN_DIR}/artifacts"

# Make the directory for results and artifacts if they don't exist
mkdir -p "${RESULT_DIR}"
mkdir -p "${ARTIFACT_DIR}"

# Build the mandatory command line parameters to the python septest script
PARAMS=("--resultDir" "${RESULT_DIR}" "--artifactDir" "${ARTIFACT_DIR}" "--repoDir" "${REPO_ROOT}")

# Which python to use?
PYTHON="python3"

# Pyenv stuff
PYENV_EXE="${HOME}/.pyenv/bin/pyenv"

### Change into the root of this plan directory so we can execute the python module ###
if ! cd "${PLAN_DIR}"; then
  >&2 echo "Could not change into directory: ${PLAN_DIR}"
  exit 1
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

if ! [ -f "Pipfile" ]; then
  >&2 echo "No Pipfile found in directory ${PWD}."
  exit 1
fi

# Install dependencies
echo
pipenv sync --bare || exit 1


echo
echo "Invoking ${MODULE} with PARAMS = ${PARAMS[*]}"
echo

# Run the system
# shellcheck disable=SC2086
pipenv run ${PYTHON} -m "${MODULE}" "${PARAMS[@]}" "$@"

#!/bin/bash
# Ensures that all AWS instances have been stopped, even in the case of a
# Bamboo manual forced stop.
#
# This is called by Bamboo. Bamboo will populate the environment with various
# variables, some of which are used below.

DIR="$(dirname "$0")"
PLAN_DIR="$(realpath "${DIR}/..")"            # Directory of <repoRoot>/bamboo/releasetest

MODULE="releasebuilder"

# Build the mandatory command line parameters to the python script
PARAMS=( "--cleanup" )

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
pipenv run ${PYTHON} -m "${MODULE}" "${PARAMS[@]}"

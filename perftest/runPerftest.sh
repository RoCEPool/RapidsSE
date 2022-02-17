#!/bin/bash

# The default job to run
#DEFAULT_JOB="configs/jobs/exampleJob.config"

# Bamboo system variables
PLAN_KEY="${bamboo_planKey}"
BUILD_NUM="${bamboo_buildNumber}"
INVOKING_USER="${bamboo_ManualBuildTriggerReason_userName}"
GIT_BRANCH="${bamboo_planRepository_branch}"

# Plan variables (defined in Bamboo)
JOB_FILE="${bamboo_jobFile}"


# Command line parameters to the python septest script
PARAMS="--job ${JOB_FILE} --planKey ${PLAN_KEY} --buildNum ${BUILD_NUM} --branch ${GIT_BRANCH}"

# Was this invoked by a user (manual build) or a periodic system build?
if [ -n "${INVOKING_USER}" ]; then
  PARAMS="${PARAMS} --user ${INVOKING_USER}"
fi

if ! which pipenv &>/dev/null; then
  >&2 echo "pipenv must be installed."
  exit 1
fi


pipenv sync || exit 1

pipenv run python -m septest ${PARAMS} $*


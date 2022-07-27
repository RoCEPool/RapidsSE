#!/bin/bash

# A script used to invoke the performance test manually on AWS for debugging purposes.
DIR="$(dirname "$0")"

export bamboo_jobFile="configs/jobs/sf1-2N-01.config"
export bamboo_planRepository_branch="$(git branch --show-current)"
export bamboo_planKey="debug"
export bamboo_buildNumber="-1"
export bamboo_ManualBuildTriggerReason_userName="craig.mcintyre"
export bamboo_archiveMetrics="0"

"${DIR}/runPerftest.sh" --nobuild --archiveMetrics

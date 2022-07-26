# A script used to invoke the performance test manually on AWS for debugging purposes.
DIR="$(dirname "$0")"

bamboo_jobFile="configs/jobs/tpch-sf1-2D2F.config"
bamboo_planRepository_branch="$(git branch --show-current)"
bamboo_planKey="debug"
bamboo_buildNumber="-1"
bamboo_ManualBuildTriggerReason_userName="craig.mcintyre"
bamboo_archiveMetrics="0"

"${DIR}/runPerftest.sh" --nobuild
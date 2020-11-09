#!/bin/bash

if ! which shellcheck &>/dev/null; then
  echo "Cannot check scripts: shellcheck not installed."
  exit 1
fi

# Files to be checked
declare -a FILES
FILES+=("../../Bootstrap/setup.sh")
FILES+=("../../Bootstrap/setup.fn")
FILES+=("../../Bootstrap/envCheck.fn")
FILES+=("../../Bootstrap/common.fn")
FILES+=("../../Bootstrap/boot.fn")
FILES+=("../../Bootstrap/startSE.sh")
FILES+=("../../Bootstrap/stopSE.sh")
FILES+=("../../Bootstrap/prompt.sh")
FILES+=("../../Bootstrap/generateData.sh")
FILES+=("../../Bootstrap/collectLogs.sh")

shellcheck -s bash -P ../../Bootstrap "${FILES[@]}"

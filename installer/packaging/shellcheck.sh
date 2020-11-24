#!/bin/bash

if ! which shellcheck &>/dev/null; then
  echo "Cannot check scripts: shellcheck not installed."
  exit 1
fi

# Files to be checked
declare -a FILES
FILES+=("../../bootstrap/setup.sh")
FILES+=("../../bootstrap/setup.fn")
FILES+=("../../bootstrap/envCheck.fn")
FILES+=("../../bootstrap/common.fn")
FILES+=("../../bootstrap/boot.fn")
FILES+=("../../bootstrap/startSE.sh")
FILES+=("../../bootstrap/stopSE.sh")
FILES+=("../../bootstrap/prompt.sh")
FILES+=("../../bootstrap/generateData.sh")
FILES+=("../../bootstrap/collectLogs.sh")

shellcheck -s bash -P ../../bootstrap "${FILES[@]}"

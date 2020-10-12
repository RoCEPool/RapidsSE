#!/bin/bash

if ! which shellcheck &>/dev/null; then
  echo "Cannot check scripts: shellcheck not installed."
  exit 1
fi

shellcheck -P ../../Bootstrap ../../Bootstrap/setup.sh ../../Bootstrap/setup.fn

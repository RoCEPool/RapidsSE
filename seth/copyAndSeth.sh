#!/usr/bin/env bash

# A utility that copies the repository, starts SE
# and runs a SETH test against it. The intent is
# that it allows a test to be run in the background
# while further changes are being made.

# Uncomment this line for debugging
#set -x

if ((BASH_VERSINFO[0] < 4)); then 
  echo -e "Sorry, you need at least bash version 4.0 or later to run "\
          "this script.\n"\
          "Try installing a newer version of bash through Homebrew "\
          "and setting homebrew at the start of your \${PATH}.\n"\
          "(Don't forget to restart your terminal too)" >&2
  exit 1 
fi

if ! which realpath &>/dev/null; then
  echo "The utility \"realpath\" is required." >&2
  exit 1
fi

readonly DIR="$(realpath "$(dirname "$0")")"
readonly SRC_ROOT="$(realpath "${DIR}/..")"
readonly TGT_ROOT="$(realpath "${SRC_ROOT}/..")/copyAndSeth"

readonly SETH_TEST="${TGT_ROOT}/seth/seth1N4P.sh"
readonly SE_ARGS=("--node" "NODE1" "127.0.0.1" "5541" "9123"
                  "--name" "NODE1" 
                  "--start" 
                  "--size" "8GB"
                  "--metadata" 
                  "--developer")

if ! which rsync &>/dev/null; then
  echo "The utility \"rsync\" is required." >&2
  exit 1
fi


# Copy the repo
echo "Copying the repo to ${TGT_ROOT}"
mkdir -p "${TGT_ROOT}"
rsync -aqv --delete --exclude "/build/" "${SRC_ROOT}/" "${TGT_ROOT}/"


# Kill any prior instances of rapidsse
if pgrep -q rapidsse; then
  echo -en "There are running instances of rapidsse.\n"\
           "Shall I kill them? [Y/n]: "

  IFS=$'\n' read -r line
  
  if [ "${line:-x}" = "x" ] || [[ "${line}" =~ [yY] ]]; then
    echo "Killing these processes:"
    pgrep -l rapidsse
    pkill -9 rapidsse
  
  elif [[ "${line}" =~ [nN] ]]; then
    echo "Exiting"
    exit 0

  else
    echo "Input was not understood. Please enter 'y' or 'n'." >&2
    exit 1
  fi
fi

# Setup the exit hook
function onExit() {
  echo
  if [ "${SE_PID:-x}" = "x" ] || ! ps -p "${SE_PID}" &>/dev/null; then
    echo "No SE PID to stop, or SE has already exited"
  else
    kill -9 "${SE_PID}"
    echo "Stopped SE (pid ${SE_PID})"
  fi

  popd &>/dev/null || exit 1
}

trap onExit EXIT
SE_PID=""
pushd "${PWD}" >/dev/null || exit 1


# Start SE in the background
echo "Starting SE"
SE_READY="${TGT_ROOT}/se.ready"

rm -f "${SE_READY}"
cd "${TGT_ROOT}/bin" || exit 1
./rapidsse "${SE_ARGS[@]}" >/dev/null &
SE_PID="$!"


# Wait for SE to start up
BOOT_TIMEOUT_SEC=15
echo -n "Waiting up to ${BOOT_TIMEOUT_SEC} sec for SE to start up"

START_TIME="$(date +%s)"
TARGET_TIME=$((START_TIME + BOOT_TIMEOUT_SEC))

while true; do
  sleep 1
  echo -n " ."
  CURRENT_TIME="$(date +%s)"

  if [ "${CURRENT_TIME}" -ge "${TARGET_TIME}" ]; then
    echo
    echo "RapidsSE boot timed out"
    exit 1
  fi

  if ! ps -p "${SE_PID}" &>/dev/null; then
    echo
    echo "RapidsSE has failed to start up"
    exit 1
  fi

  if [ -f "${SE_READY}" ]; then
    echo
    echo "SE has finished booting"
    break
  fi

done


# Start SETH
echo "Running SETH"
"${SETH_TEST}"


# Stop SE. This will happen in the exit hook



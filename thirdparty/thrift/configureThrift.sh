#!/bin/bash

readonly DIR="$(dirname "$0")"
readonly BOOSTDIR="$(realpath "${DIR}/boost_1_80_0")"

MACOS_OPENSSL_BASE_DIR="/usr/local/opt/openssl"

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Print an error message to stderr and exit,
## Params:
##   $1 : Error message
##   $2..$n : (optional) other arguments to print.

function errorExit() {
  >&2 echo -e "ERROR: $*"
  exit 1
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Prints a given message if debug mode is turned on (i.e., $DEBUG == 1).
## Params:
##  $@ : The arguments to print

function debugPrint() {
  if [ -n "${DEBUG:-}" ] && [ "${DEBUG}" -eq 1 ]; then
    echo -e "$*"
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that the necessary dependencies are installed 
function checkDependencies() {
  echo "Checking for Thrift dependencies"

  if   [ "${IS_MACOS}" -eq 1 ];        then checkDependenciesMacOs;
  elif [ "${IS_CENTOS_7}" -eq 1 ];     then checkDependenciesCentOS7;
  elif [ "${IS_CENTOS_OTHER}" -eq 1 ]; then checkDependenciesCentOSOther;
  elif [ "${IS_UBUNTU}" -eq 1 ];       then checkDependenciesUbuntu;
  elif [ "${IS_AMZNLINUX}" -eq 1 ];    then checkDependenciesAmazonLinux;
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that the necessary dependencies are installed on MacOS
function checkDependenciesMacOs() {
  # 1. Check for homebrew
  if ! which brew &>/dev/null; then
    errorExit "Homebrew is required to install other Thrift pre-requisites"
  fi

  # 2. Check for Bison >= 2.5
  if ! which bison &>/dev/null; then
    errorExit "The Thrift pre-requisite Bison does not appear to be installed.\n"\
              "Install with the command:  brew install bison"
  fi

  # If we have a homebrew version of bison installed then we will put that at the start
  # of the PATH so the thrift configure script will find it.
  # Note that the bison check seems to only be needed for the configure script
  # and not the actual compilation.
  if brew list --versions bison &>/dev/null; then
    PATH="$(brew --prefix bison)/bin:${PATH}"
  fi

  local bisonVerStr
  local bisonMajor
  local bisonMinor
  bisonVerStr="$(bison -V | head -n 1 | cut -f 4 -d ' ')" || errorExit "Could not extract the bison version number"
  bisonMajor="$(echo "${bisonVerStr}" | cut -d '.' -f 1)"
  bisonMinor="$(echo "${bisonVerStr}" | cut -d '.' -f 2)"

  local bisonErr=0
  if [[ "${bisonMajor}" -lt 2 ]]; then
    bisonErr=1
  elif [[ "${bisonMajor}" -eq 2 && "${bisonMinor}" -lt 5 ]];
    then bisonErr=1
  fi

  if [ "${bisonErr}" -eq 1 ]; then
    errorExit "The Thrift pre-requisite Bison must be version 2.5 or greater.\n"\
              "Installed version of bison is ${bisonVerStr}.\n"\
              "An updated version of bison can be installed via homebrew with the command:\n"\
              "  brew install bison\n"
  fi

  # 3. Check for OpenSSL headers and library
  if [ -d /usr/local/opt/openssl ]; then
    MACOS_OPENSSL_BASE_DIR="/usr/local/opt/openssl"

  elif [ -d /opt/homebrew/opt/openssl ]; then
    MACOS_OPENSSL_BASE_DIR="/opt/homebrew/opt/openssl"

  else
    errorExit "The Thrift pre-requisite OpenSSL does not appear to be installed.\n"\
              "It can be installed with the command:\n"\
              "  brew install openssl\n"
  fi

  if [ ! -d "${MACOS_OPENSSL_BASE_DIR}/include/openssl" ]; then
    errorExit "The Thrift pre-requisite OpenSSL headers could not be found. They were \n"\
              "expecting to be found at: ${MACOS_OPENSSL_BASE_DIR}/include/openssl"
  fi

  if [ ! -d "${MACOS_OPENSSL_BASE_DIR}/lib" ]; then
    errorExit "The Thrift pre-requisite OpenSSL libraries could not be found. They were \n"\
              "expecting to be found at: ${MACOS_OPENSSL_BASE_DIR}/lib"
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that the necessary dependencies are installed on CentOS 7
function checkDependenciesCentOS7() {
  # 1. Check for OpenSSL headers
  if [ ! -d /usr/include/openssl ]; then
    errorExit "The Thrift pre-requisite OpenSSL headers do not appear to be installed. " \
              "Install with the command \"sudo yum install openssl-devel\"."
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that the necessary dependencies are installed on CentOS other than v7
function checkDependenciesCentOSOther() {
  # 1. Check for OpenSSL headers
  if [ ! -d /usr/include/openssl ]; then
    errorExit "The Thrift pre-requisite OpenSSL headers do not appear to be installed. " \
              "Install with the command \"sudo yum install openssl-devel\"."
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that the necessary dependencies are installed on Ubuntu
function checkDependenciesUbuntu() {
  # 1. Check for OpenSSL headers
  if [ ! -d /usr/include/openssl ]; then
    errorExit "The Thrift pre-requisite OpenSSL headers do not appear to be installed. " \
              "Install with the command \"sudo apt-get install libssl-dev\"."
  fi
}


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Checks that the necessary dependencies are installed on AmazonLinux
function checkDependenciesAmazonLinux() {
  # 1. Check for OpenSSL headers
  if [ ! -d /usr/include/openssl ]; then
    errorExit "The Thrift pre-requisite OpenSSL headers do not appear to be installed. " \
              "Install with the command \"sudo yum install openssl-devel\"."
  fi
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Configure thrift as per the OS we are on
function runConfigure() {
  local command
  local commonOptions=("--prefix=$(realpath .)"
                       "--enable-libs"
                       "--disable-tests"
                       "--disable-tutorial"
                       "--disable-coverage"
                       "--disable-shared"
                       "--enable-static"
                       "--with-cpp"
                       "--with-boost=${BOOSTDIR}"
                       "--without-java"
                       "--without-kotlin"
                       "--without-erlang"
                       "--without-nodejs"
                       "--without-nodets"
                       "--without-lua"
                       "--without-python"
                       "--without-py3"
                       "--without-perl"
                       "--without-php"
                       "--without-php_extension"
                       "--without-dart"
                       "--without-ruby"
                       "--without-go"
                       "--without-swift"
                       "--without-rs"
                       "--without-cl"
                       "--without-haxe"
                       "--without-netstd"
                       "--without-d"
                       "--without-libevent"
                       "--without-qt5")

  if   [ "${IS_MACOS}" -eq 1 ]; then
    local arch=""
    if [ -n "${ARCH_FLAG_FORCE}"  ]; then
      arch="-arch ${ARCH_FLAG_FORCE}"
      echo "Compiling thrift for x86_64 on Arm Mac"
    fi
    command=("env"
             "LDFLAGS=-L${MACOS_OPENSSL_BASE_DIR}/lib ${arch}"
             "CPPFLAGS=-I${MACOS_OPENSSL_BASE_DIR}/include ${arch}"
             "./configure"
             "${commonOptions[@]}" )

  else
    command=("./configure" "${commonOptions[@]}")
  fi

  # Run in a subshell so we only change the directory temporarily
  (
    echo "Configuring Thrift"

    cd "${SRC_DIR}" || exit 1

    local output
    readonly configureLog="../configure.log"

    debugPrint "Configure command is:\n${command[*]}"

    if ! "${command[@]}" | tee "${configureLog}"; then
      errorExit "Unable to configure Thrift. Refer to the output in the logfile $(realpath ${configureLog})"
    fi

    output=$(grep "Building C++ Library" "${configureLog}")

    # Check if the c++ library is actually being configured
    [[ "${output}" =~ "yes" ]] || errorExit "Thrift did not configure the c++ library to be built. This is likely due to a missing dependency."

  ) || exit 1

  echo "Configured Thrift successfully!"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Main Script follows

# Part 0 - Arguments
if [ "$#" -ne 1 ]; then
  errorExit "Script $0 must be called with 1 argument being the path of the untarred thrift source"
fi

SRC_DIR="$1"
[ -d "${SRC_DIR}" ] || errorExit "Thrift source directory does not exist: ${SRC_DIR}"


# Part 1 - Determine what OS and version we are on.

IS_MACOS=0
IS_CENTOS_7=0
IS_CENTOS_OTHER=0
IS_UBUNTU=0
IS_AMZNLINUX=0

# Get the OS name. This should be "Darwin" or "Linux"
OS_NAME="$(uname -s)"

if [ "${OS_NAME}" = "Darwin" ]; then
  IS_MACOS=1

elif [ "${OS_NAME}" = "Linux" ]; then
  readonly osRelease="/etc/os-release"

  if [ ! -f "${osRelease}" ]; then
    errorExit "Cannot determine the OS type because the file ${osRelease} cannot be found."
  fi

  #shellcheck disable=SC1090
  source "${osRelease}"

  if [ "${ID}" = "centos" ]; then
    if [ "${VERSION_ID}" -eq 7 ]; then
      IS_CENTOS_7=1
    else
      IS_CENTOS_OTHER=1
    fi

  elif [ "${ID}" = "ubuntu" ]; then
    IS_UBUNTU=1

  elif [ "${ID}" = "amzn" ]; then
    IS_AMZNLINUX=1
  fi

else
  errorExit "Operating system not supported: ${OS_NAME}"
fi


# Part 2 - Check for the necessary dependencies on this machine
checkDependencies

# Part 3 - Configure thrift as per the OS we are on
runConfigure

#!/bin/bash

###############################################################################
### Constants                                                               ###
###############################################################################

# Set to 1 for extra debugging prints.
readonly DEBUG=0


###############################################################################
### Functions                                                               ###
###############################################################################

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## Prints a given message if debug mode is turned on (i.e., $DEBUG == 1).
## Params:
##  $@ : The arguments to print

function debugPrint() {
  if [ -n "${DEBUG}" ] && [ "${DEBUG}" -eq 1 ]; then
    echo -e "$*"
  fi
}


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
## Checks to see if we need to do a clean build and if so, do it.
## Params: None

function checkForMakeClean() {
  # Rather than being a good IDE-citizen, XCode decided not to pass on a
  # build-clean event. Instead they simply clean up their own temporary
  # files and don't call the external build system. So we have to work
  # around this in a not-so-pretty way.
  #
  # The gist of it is this: When XCode builds (even with an external build
  # system), it creates some temporary directories in various locations. One of
  # those locations is set in the ${TEMP_ROOT} variable that XCode sets
  # in the environment. We will create a file within this directory. If this 
  # file is missing then that is because the user has executed the "Clean 
  # Build Folder". If this file exists then the user hasn't done a "Clean 
  # Build Folder".
  #
  # Here's the ugly part: before we build anything, we will check for the
  # presence of this file. If it does not exist, we will run a
  # "make clean" first. When the product is cleaned this temporary file 
  # will be created again, so a subsequent build (without clean) will 
  # detect this file and then *not* do a make clean.

  if [ -z "${TEMP_ROOT}" ]; then
    errorExit "\$TEMP_ROOT has not been passed from XCode to this script."   
  fi

  debugPrint "\$TEMP_ROOT = ${TEMP_ROOT}"

  # The all-important file to check for.
  local quickBuildFile="${TEMP_ROOT}/RapidsSE_QuickBuild"

  # Look for it!
  if [ -f "${quickBuildFile}" ]; then
    echo "Doing a QUICK build (\$quickBuildFile exists)"
    debugPrint "$(cat "${quickBuildFile}")"

  else
    echo "Doing a CLEAN build (\$quickBuildFile does not exist)"
    make clean

    # Re-create the quick build file so we can do a quick build next time
    mkdir -p "${TEMP_ROOT}"
    touch "${quickBuildFile}"
    echo "# Last cleaned on: $(date)" > "${quickBuildFile}"
  fi
}

###############################################################################
### Main Script                                                             ###
###############################################################################

# Do a "make clean" if necessary.
checkForMakeClean

# Set the MacOS minimum deployment target. This should be set via the 
# CPPFLAGS set in the makefile.darwin.in file, so it is probably
# unnecessary here.
#export MACOSX_DEPLOYMENT_TARGET="10.12"

# Spawn the same number of concurrent build jobs as there are processors 
# in the system.
concurrentJobs="$(sysctl -n hw.ncpu)"

# XCode will set an environment variable, $CONFIGURATION, to either Debug
# or Release (both capitalised). If this is set then use it appropriately.
if [ "${CONFIGURATION}" == "Debug" ]; then
  buildTarget="debug"
elif [ "${CONFIGURATION}" == "Release" ]; then
  buildTarget="release"
else
  buildTarget="release"
fi

debugPrint "Environment variables : $(env)"
echo "Building ${buildTarget} target with ${concurrentJobs} concurrent jobs."

# Run the build
debugPrint "Build output starts now\n"
make "-j${concurrentJobs}" "${buildTarget}"


###############################################################################
### Notes                                                                   ###
###############################################################################
# These are some of the other notable environment variables that XCode will set.
# To see the rest of them, set DEBUG=1 at the top of this file and re-run. It
# will print out all environment variables to stdout.

# CONFIGURATION=Debug
# PROJECT_FILE_PATH=/Users/craig/repos/rapidsse/RapidsSE/RapidsSE.xcodeproj
# TARGET_NAME=RapidsSE
# TEMP_DIR=/Users/craig/Library/Developer/Xcode/DerivedData/RapidsSE-fbvgqxkvvkihvnasiyilzkfftzer/Build/Intermediates.noindex/RapidsSE.build/Debug/RapidsSE.build
# BUILT_PRODUCTS_DIR=/Users/craig/Library/Developer/Xcode/DerivedData/RapidsSE-fbvgqxkvvkihvnasiyilzkfftzer/Build/Products/Debug
# BUILD_ROOT=/Users/craig/Library/Developer/Xcode/DerivedData/RapidsSE-fbvgqxkvvkihvnasiyilzkfftzer/Build/Products
# TEMP_ROOT=/Users/craig/Library/Developer/Xcode/DerivedData/RapidsSE-fbvgqxkvvkihvnasiyilzkfftzer/Build/Intermediates.noindex
# OBJECT_FILE_DIR=/Users/craig/Library/Developer/Xcode/DerivedData/RapidsSE-fbvgqxkvvkihvnasiyilzkfftzer/Build/Intermediates.noindex/RapidsSE.build/Debug/RapidsSE.build/ObjectsA
# PROJECT_TEMP_DIR=/Users/craig/Library/Developer/Xcode/DerivedData/RapidsSE-fbvgqxkvvkihvnasiyilzkfftzer/Build/Intermediates.noindex/RapidsSE.build


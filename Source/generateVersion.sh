#!/bin/bash

VERSION_FILE="Version.h"
BUILD_GIT_HASH=$(git rev-parse --short=16 HEAD)
BUILD_DATE=$(date)
BUILD_HOSTNAME=$(hostname -f)
BUILD_USERNAME=$(whoami)
BUILD_BRANCH=$(git rev-parse --abbrev-ref HEAD)

rm -f ${VERSION_FILE}

{
  echo "/*============================================================================*/"
  echo "/* Version.h -- Dynamically generated version information                     */"
  echo "/*============================================================================*/"
  echo ""
  echo "static const Stz* C_BUILD_GIT_HASH = \"${BUILD_GIT_HASH}\"_Stz;"
  echo "static const Stz* C_BUILD_DATE     = \"${BUILD_DATE}\"_Stz;"
  echo "static const Stz* C_BUILD_HOSTNAME = \"${BUILD_HOSTNAME}\"_Stz;"
  echo "static const Stz* C_BUILD_USERNAME = \"${BUILD_USERNAME}\"_Stz;"
  echo "static const Stz* C_BUILD_BRANCH   = \"${BUILD_BRANCH}\"_Stz;"
  echo ""
  echo "/*============================================================================*/"
  echo "/* end                                                                        */"
  echo "/*============================================================================*/"
} >> ${VERSION_FILE}

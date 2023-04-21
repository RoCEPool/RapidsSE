README
======

The main file in this directory is package.sh. It compiles RapidsSE
and the documentation and then packages them into an installer,
ready for release.

There are a number of support files, as described below:

  ./package.sh
    The main script for building the installer

  ./repackage.sh
    A script that does not compile RapidsSE but simply rebuilds
    the installer. This is useful when developing/testing changes
    to the installation, which doesn't need to be recompiled.

  ./package-nodocs.sh
    A script that compiles RapidsSE and builds the installer but
    does not build or include the documentation. This is useful when
    you wish to avoid the dependencies that the documentation requires.

  ./shellcheck.sh
    A supporting script file, called by package.sh, to check all
    other script files against ShellCheck (a bash linter). Any new
    bash script files that are added to the project should also be
    added here.

  ./createlayout.sh
    A supporting script file, called by package.sh, to gather up all
    the files needed for installation and lay them out as they should
    appear post-installation. Any new files that need to be added to
    the installation should have the appropriate instruction added here.


Environment Variables
=====================

There are a number of environment variables that can be set to alter
how the package.sh script runs. If an environment variable is to be
enabled, its value should be set to 1. Here's an example of how to
call package.sh with an environment variable set:

  MY_ENV_VAR=1 ./package.sh

Here are the supported environment variables:

  DEBUG         - print extra debugging information
  NO_MOD_CHECK  - don't check for modified files that haven't been committed.
  NO_BUILD      - don't compile the software
  NO_SHELLCHECK - don't run shellcheck on the scripts beforehand.
  KEEP_OBJECTS  - don't remove build objects afterwards. Implied with NO_BUILD.
  NO_DOCS       - don't build or package documentation.
  NO_DOC_BUILD  - don't build the documentation but continue to package it.
  DEBUG_BUILD   - build RapidsSE in debug mode instead of release (the default)


.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _compilationGuide:

Compilation Guide
=================
This section describes how to compile RapidsSE from source.

Supported Operating Systems
---------------------------
RapidsSE can be compiled on the following operating systems:

#. CentOS 7
#. CentOS 8
#. Ubuntu 20 or above

RapidsSE can probably be compiled on other operating systems, however the
installation of pre-requisites may vary. Setup instructions vary slightly
between Linux distributions (and even between versions of the same Linux
distribution) because they can name their software packages differently.

While RapidsSE can also be compiled on MacOS, the installer can only be built
on Linux due to MacOS only including a very old version of bash (v3) and
other necessary utilities. Details for compiling RapidsSE on MacOS are not
included in this document.


Machine Setup
-------------
This section has details on how to install and setup the necessary pre-requisites
for compiling RapidsSE on each supported operating system.

CentOS 7
````````
.. important::

   CentOS 7 comes with an old version of gcc that will not compile a 3rd party
   library (ANTLR) used by RapidsSE. This is resolved by installing a newer
   version of gcc **without replacing the default gcc installation** as well as
   updating the version of ``cmake``. The steps below cater for this.

First install git, the default version of gcc and other necessary libraries & tools:

.. code:: bash

   sudo yum install git gcc-c++ unzip libuuid-devel nmap-ncat iproute bc ncurses parallel gawk vim-enhanced

Git Large File Support (``git-lfs``) needs to be installed manually as it is not
included in the CentOS7 packages:

.. code:: bash

   curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.rpm.sh | sudo bash
   sudo yum install git-lfs
   git lfs install

Install an updated version of ``gcc`` for this version of CentOS:

.. code:: bash

   sudo yum install libuuid-devel openssl-devel centos-release-scl
   sudo yum install devtoolset-10-gcc* devtoolset-10-libatomic-devel

Install an updated version of cmake (>= v3.15):

.. code:: bash

   CMAKE_VER="3.29.0"
   CMAKE_PLATFORM="linux-x86_64"
   CMAKE_FILE="cmake-${CMAKE_VER}-${CMAKE_PLATFORM}.sh"
   CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VER}/${CMAKE_FILE}"

   cd /opt
   sudo mkdir -p /opt/cmake
   sudo curl -L --fail ${CMAKE_URL} -o ${CMAKE_FILE}
   sudo sh "${CMAKE_FILE}" --prefix="${CMAKE_INSTALL_DIR}" --skip-license --exclude-subdir

   # Ensure that this copy of cmake appears in the PATH first
   echo -e 'export PATH="/opt/cmake/bin:${PATH}"\n' >> sudo tee -a /etc/profile.d/sh.local

Install the necessary tools for generating the documentation:

.. code:: bash

	 sudo yum install python3 java-11-openjdk
	 python3 -m pip install --user -U sphinx

**Optional:** install the necessary libraries for generating documentation in
PDF format. This installs Tex Live, a distribution of latex and associated
tools and libraries. This requires a download of approximately **4GB**.

.. code:: bash

   sudo yum install perl-Digest-MD5 perl-Env wget tar
   cd /tmp
   wget https://mirror.ctan.org/systems/texlive/tlnet/install-tl-unx.tar.gz
   tar xvzf install-tl-unx.tar.gz
   cd install-tl-*
   sudo perl ./install-tl --no-interaction --no-doc-install --no-src-install -portable

CentOS 7 should now be ready to compile RapidsSE. Other setup steps may be
required in order to install and run RapidsSE. Please refer to the
:ref:`RapidsSE Installation and Management Guide <instMgmt>` for details on this.


CentOS 8
````````
CentOS 8 can be easily set up to compile RapidsSE with the following commands:

.. code:: bash

   sudo yum install git git-lfs gcc-c++ libatomic-static cmake openssl-devel unzip libuuid-devel nmap-ncat iproute bc ncurses parallel gawk vim-enhanced
   git lfs install

Install the necessary tools for generating the documentation:

.. code:: bash

   sudo yum install python3 java-11-openjdk
   python3 -m pip install --user -U sphinx

**Optional:** install the necessary libraries for generating documentation in
PDF format. This installs Tex Live, a distribution of latex and associated
tools and libraries. This requires a download of approximately **4GB**.

.. code:: bash

   sudo yum install perl-Digest-MD5 wget tar
   cd /tmp
   wget https://mirror.ctan.org/systems/texlive/tlnet/install-tl-unx.tar.gz
   tar xvzf install-tl-unx.tar.gz
   cd install-tl-*
   sudo perl ./install-tl --no-interaction --no-doc-install --no-src-install -portable

CentOS 8 should now be ready to compile RapidsSE. Other setup steps may be
required in order to install and run RapidsSE. lease refer to the
:ref:`RapidsSE Installation and Management Guide <instMgmt>` for details on this.


Ubuntu 22
`````````
Ubuntu 22 can be easily set up to compile RapidsSE with the following three commands:

.. code:: bash

   sudo apt update
   sudo apt install g++ git git-lfs libatomic1 cmake unzip libuuid1 uuid-dev netcat iproute2 bc parallel gawk vim libssl-dev
   git lfs install

Install the necessary tools for generating the documentation:

.. code:: bash

   sudo apt install python3 openjdk-11-jdk-headless
   python3 -m pip install --user -U sphinx

**Optional:** install the necessary libraries for generating documentation in
PDF format. This installs Tex Live, a distribution of latex and associated
tools and libraries. This requires a download of approximately **4GB**.

.. code:: bash

   sudo apt install libdigest-perl-md5-perl wget tar
   cd /tmp
   wget https://mirror.ctan.org/systems/texlive/tlnet/install-tl-unx.tar.gz
   tar xvzf install-tl-unx.tar.gz
   cd install-tl-*
   sudo perl ./install-tl --no-interaction --no-doc-install --no-src-install -portable

Ubuntu 22 should now be ready to compile RapidsSE. Other setup steps may be
required in order to install and run RapidsSE. lease refer to the
:ref:`RapidsSE Installation and Management Guide <instMgmt>` for details on this.


.. _compilingSEAndBuildingInstaller:

Compiling SE and Building the Installer
---------------------------------------
These steps will compile RapidsSE and package the compiled binaries into a
self-executing installer. If you only want to build the SE binary only then
please refer to the :ref:`section below <compilingSEBinaryOnly>` for this.

#. First, please follow the steps in the above sections for setting
   up the pre-requisite tools and libraries.

#. Enable git password caching to prevent being asked for your git password
   multiple times while cloning the git repository:

   .. code:: bash

      git config --global credential.helper cache

#. On the command line, checkout the RapidsSE git repository with the command:

   .. code:: bash

      git clone https://<username>@bitbucket.org/rapidsdataproject/rapidsse.git

   Ensure you replace ``<username>`` with the username of your Bitbucket account.
   You will be prompted for your app password.

   Please note that this app password is not your Bitbucket account password.
   Bitbucket made changes in early 2022 to use a separate password for your
   account and a separate “app” password for accessing repositories. See the
   link below for more information on this.

   - `Clone a git repository <https://support.atlassian.com/bitbucket-cloud/docs/clone-a-git-repository/>`_

   - `Bitbucket app passwords <https://support.atlassian.com/bitbucket-cloud/docs/app-passwords>`_

#. Change to the ``installer/packaging`` directory of the cloned repository:

   .. code:: bash

      cd rapidsse/installer/packaging

#. Run the script to create the installer. This will compile RapidsSE and then
   package everything up:

   .. code:: bash

      ./package.sh

#. If it compiles and packages the installer successfully then it will show the
   path to the installer file that was created as well as the file checksums.

   .. code:: bash

      Self-extractable archive "/home/craig/repos/rapidsse/installer/output/rapidsse-1.1.0-dev0.run" successfully created.
      Cleaning up
      Size of installer: 107,400 kB

      Installer checksums:
      md5:   f6152ac9bb6c4b98c7e29dbd224edaba  rapidsse-1.1.0-dev0.run
      sha1:  a7d6e2b61b8ed51927de48635ac0e5c792ad30b5  rapidsse-1.1.0-dev0.run

#. Change to the directory containing the self-executing installer that was created:

   .. code:: bash

      cd ../output

#. The installation file will be named something like this:

   .. code:: bash

      rapidsse-1.1.0.run

Note: An installer file with ``-SNAPSHOT`` in its name indicates that it is
not a formal release and hasn’t been tagged as such in the repository.

For instructions on installing and running RapidsSE, please refer to the 
:ref:`RapidsSE Installation and Management Guide <instMgmt>`.


.. _compilingSEBinaryOnly:

Compiling the SE Binary Only
----------------------------
These steps will checkout RapidsSE from the git repository and build the binary
only. If you wish to build the full installer then please refer to the
:ref:`section above <compilingSEAndBuildingInstaller>`.

#. First, please follow the steps in the above sections for setting
   up the pre-requisite tools and libraries.

#. Enable git password caching to prevent being asked for your git password
   multiple times while cloning the git repository:

   .. code:: bash

      git config --global credential.helper cache

#. On the command line, checkout the RapidsSE git repository with the command:

   .. code:: bash

      git clone https://<username>@bitbucket.org/rapidsdataproject/rapidsse.git

   Ensure you replace ``<username>`` with the username of your Bitbucket account.
   You will be prompted for your app password.

   Please note that this app password is not your Bitbucket account password.
   Bitbucket made changes in early 2022 to use a separate password for your
   account and a separate “app” password for accessing repositories. See the
   link below for more information on this.

   - `Clone a git repository <https://support.atlassian.com/bitbucket-cloud/docs/clone-a-git-repository/>`_

   - `Bitbucket app passwords <https://support.atlassian.com/bitbucket-cloud/docs/app-passwords>`_

#. Change to the ``src`` directory of the cloned repository:

   .. code:: bash

      cd src

#. To build the binary in *release* mode run this command, replacing ``$numberOfCpus``
   with the numeric value of the number of CPU cores on your machine:

   .. code:: bash

      make -j $numberOfCpus

#. To build the binary in *debug* mode run this command, replacing ``$numberOfCpus``
   with the numeric value of the number of CPU cores on your machine:

   .. code:: bash

      make -j $numberOfCpus debug

#. The build binary will be in the ``bin`` directory from the repository root.

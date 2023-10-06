.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: RapidsSE; installation
   single: installation

.. _installation:

Installation Instructions
=========================

Installation of RapidsSE involves first configuring the environment of each
machine in the cluster, defining the cluster configuration and the major
RapidsSE settings, and then installing the RapidsSE software. The following
subsections will guide you through this.


Environment Setup
-----------------
Firstly, ensure that you are using a supported operating system as detailed in
the :ref:`supportedOperatingSystems` section. Setting up the cluster environment involves
the following steps:

#.	Setting up password-less SSH access to each node.
#.	Configuring the amount of shared memory on each node.
#.	Configuring the system firewall.
#.	Ensuring the installation directory is writable.

.. index::
   pair: SSH; configuration

Configuring SSH
```````````````
RapidsSE requires the ability to SSH to each node in the cluster without
prompting the user for a password. This is typically achieved with the use
of public and private keys. There are several different ways of achieving
this. One simple way is as follows:

#. Generate a public and private key for the desired user account that
   RapidsSE will be operating under.

   This can be done by executing the command ``ssh-keygen``. Do not enter a
   passphrase when prompted as this cause the user to be prompted for the
   password when installing and starting RapidsSE.

   The ssh-keygen utility will produce two files. These are often named
   ``id_rsa`` and ``id_rsa.pub`` and will reside in the ``~/.ssh`` directory.

#. Use the ``ssh-copy-id`` system utility to authorise logins on remote
   computers using these generated keys. This utility copies the ``id_rsa.pub``
   file on the current node and appends it into the ``~/.ssh/authorized_keys``
   file on the remote node. It can be invoked with the command: ::

     ssh-copy-id -i id.rsa.pub <remote_host>

   where <remote_host> is the name of the remote computer that you wish to
   allow authentication to. When this is done for the first time you will be
   prompted to enter the password for the current user on the remote machine.
   After this command completes, you should be able to SSH to that machine
   without needing to enter a password.

#. Copy the ``~/.ssh/id_rsa`` file to the same location on each node in the
   cluster, and ensure it has a permission mask of 600. E.g.: ::

     scp ~/.ssh/id_rsa <remoteHost>:~/.ssh
     ssh <remoteHost> chmod 600 ~/.ssh/id_rsa

#. On each node in the cluster, invoke the ``ssh-copy-id`` utility to allow
   for SSH access to itself: ::

     ssh-copy-id -i id.rsa.pub localhost


.. index::
   single: shared memory; configuration (host)
   single: configuration; shared memory (host)

Configuring Shared Memory
`````````````````````````
RapidsSE uses shared memory to store all application and internal working data.
On Linux, shared memory is represented as a device, ``/dev/shm``, that
typically uses the in-memory filesystem ``tmpfs``. You can read and write to
files in shared memory just like any other filesystem.

``/dev/shm`` appears like any other mounted filesystem, so it’s size and
available space can be observed by running the command:	``df -h /dev/shm``

By default, many Linux distributions configure ``/dev/shm`` with a maximum
size of half of the available RAM. If this is not changed then RapidsSE will
not be able to use all of the memory in the system. The steps to permanently
change this setting are:

#. As root, open up the file ``/etc/fstab`` and add the following line: ::

     tmpfs     /dev/shm     tmpfs     defaults,size=500g     0 0

   Replace the ``size=500g`` with a value (in GB) that is approximately 95% of
   the system’s RAM. The system will use this when it next restarts.

#. As root, execute the command: ::

     mount -o remount /dev/shm

   This will remount ``/dev/shm`` using the settings we just added to the the
   ``/etc/fstab`` file.

#. Confirm that ``/dev/shm`` is now using the desired size by executing the
   command: ::

     df -h /dev/shm

#. Repeat the previous steps for each node in the cluster.


.. index::
   pair: firewall; installation
   single: configuration; firewall

Configuring the Firewall
````````````````````````
Each RapidsSE node communicates with other RapidsSE nodes via TCP/IP and
UDP/IP sockets. RapidsSE uses one TCP/IP socket to accept connections from
clients (e.g. via the :ref:`JDBC driver <jdbcDriverInterface>`) and one TCP/IP socket to accept connections
from other RapidsSE nodes in the cluster. It also uses one UDP/IP socket to
send and receive periodic heartbeat messages that convey status and health
information about the cluster.

If a firewall is being used on any of the nodes in the RapidsSE cluster then
an exception must be added to the firewall to allow for connections to be made
on the default RapidsSE ports of TCP/9123, TCP/5541 and UDP/8888.

On **Centos 7** and similar systems, the steps for adding a port to the firewall
are as follows:

#. Execute the following command: ::

     sudo firewall-cmd --zone=public \
       --add-port=9123/tcp \
       --add-port=5541/tcp \
       --add-port=8888/udp \
       --add-port=22/tcp \
       --permanent

#. As root, tell the firewall to reload from its saved configuration with the command: ::

     sudo firewall-cmd --reload

#. Repeat the above commands on each node in the cluster.

On **Ubuntu 20** and similar systems, the steps for adding a port to the firewall
are as follows:

#. Execute the following commands: ::

     sudo ufw allow 9123/tcp
     sudo ufw allow 5541/tcp
     sudo ufw allow 8888/udp
     sudo ufw allow ssh

#. As root, enable the firewall if it is not already enabled: ::

     sudo ufw enable

#. As root, tell the firewall to reload from its saved configuration with the
   command: ::

     sudo ufw reload

#. Repeat the above commands on each node in the cluster.


.. index::
   single: installation; directory permissions

Installation Directory Writability
``````````````````````````````````
By default, RapidsSE will be installed to ``/opt/rapidsse``. If this default
value is used then the parent directory, ``/opt``, must be writable on every
node so that it can create the ``rapidsse`` subdirectory.


.. index::
   single: installation; media validation

Validating the Installation Media
---------------------------------
RapidsSE is supplied as a runnable installation file. Separate to this are two
checksum files that can be used to validate that the installation file is not
corrupted. The two checksums use different algorithms, md5 and sha1, to minimise
the possibility of a false positive result.

To validate the installation file, open a terminal and change to the directory
containing the installation file and checksum files. Then simply execute: ::

  md5sum -c checksum.md5
  sha1sum -c checksum.sha1

If they are successful then these commands will print the name of the file being
checked followed by the word ``OK``.



Installing RapidsSE
-------------------
The RapidsSE installation software allows for both interactive configuration and
installation as well as scripted installations from an existing cluster
configuration. Both of these approaches are described in the following sections.

The RapidsSE installer is a self-contained tar-archive. It requires the ``tar``
utility to be present on the machine in order to decompress and run the
subsequent stages of the installation.


.. index::
   single: installation; installer

Interactive Installation & Configuration
````````````````````````````````````````
When you run the RapidsSE installer without any arguments it will unpack itself
to a temporary directory before launching an interactive installation wizard to
guide you through the configuration of the cluster. The installation wizard will
ask a small number of questions to build the cluster configuration, using smart
default values to fill in the remaining settings.

The wizard will write out a ``cluster.config`` file in the current directory
before installing the software to each node. In the process the ``cluster.config``
file is also copied to each node so that each node is then ready to run.

Running the installation wizard is as simple as executing the installation file: ::

  ./rapidsse-<version>.run

The installation wizard can also be started in an “advanced” mode where it will
prompt the user for more settings during the installation process. This mode can
be activated by supplying a command line argument to indicate that the advanced
interactive wizard is to be used, as follows: ::

  ./rapidsse-<version>.run -- --advanced

.. important::
   Note that the first ``--`` before the ``--advanced`` is necessary to separate
   arguments for the installation wizard from arguments that may be given to the
   self-extracting run file itself.

.. index::
   single: installation; modes compared

A summary of the information that is required in either the default (basic) or
advanced wizard modes is presented below:

.. csv-table:: Installation Modes Compared
   :header: "Item", "Basic Mode (default)", "Advanced Mode", "Description"
   :name: installation-modes
   :widths: 15, 15, 15, 55

   "Node names", "", "✅", "The prefix to be given to uniquely name each node. A
   sequential incrementing number is appended after this name.

   Default: ""``NODE``"""
   "Hostnames", "✅", "✅", "The hostnames or IP addresses of each node in the cluster."
   "Client Port Number", "✅", "✅", "The external TCP port number that RapidsSE will
   use to communicate with clients and drivers.

   Default: ``9123``"
   "Internal Port Number", "", "✅", "The internal TCP port number that RapidsSE will
   use to communicate with each node in the cluster.

   Default: ``5541``"
   "Heartbeat Network Address", "", "✅", "The network address (hostname or IP address)
   used to send heartbeat messages to. This is either the keyword ``BROADCAST`` or a
   valid multicast address.

   When set to ``BROADCAST`` then each node sends UDP heartbeat messages out on the
   network broadcast address ``255.255.255.255``. If network broadcasting is not
   desired or allowed then a multicast address can be used if the network has appropriate
   multicast-aware switches and routers. RapidsSE supports multicast group management
   via IGMP. Any multicast addresses specified should be in the ``239.255.0.0/16`` subnet
   in accordance with RFC 2365.

   Default: ``BROADCAST``"
   "Heartbeat Port Number", "", "✅", "The UDP port number that each node uses to send
   and receive heartbeat messages on.

   Default: ``8888``"
   "Installation Path", "✅", "✅", "The path on each node where RapidsSE will be installed.

   Default: ``/opt/rapidsse``"
   "Disk Paths", "✅", "✅", "Where to write persistent data (e.g. an unload command).
   One or more paths can be specified.

   Default: ``<installation_path>/data``"
   "Shared memory reservation", "✅", "✅", "How much shared memory that RapidsSE will
   reserve on each node in the cluster.

   Default: Limited to the size of ``/dev/shm`` or 95% of system RAM, whichever is
   smaller, on the smallest node in the cluster."
   "Number of partitions", "", "✅", "How many partitions on each node the data will
   be sharded across.

   Default: approximately half the number of CPU cores on the smallest node in the cluster."
   "Number of threads", "", "✅", "The number of parallel tasks that can be executed concurrently.

   Default: approximately set to the number of cores on the smallest machine in the cluster."
   "Additional startup parameters", "", "✅", "Any additional command line
   parameters that will be given to RapidsSE when it starts up.

   Default: """" (empty string)"


Refer to the section :ref:`considerationsInstaller` below for details and limitations
on how to set the shared memory, partitions and threads appropriately for your
cluster.

.. index::
   single: installation; scripted

Scripted Installation
`````````````````````
If you already have a ``cluster.config`` file and you wish to reinstall or
upgrade RapidsSE then this can be done without having to re-enter all of the
configuration settings. This method also allows for the installation of
RapidsSE to be automated.

To do this, simply run the installation file with the following arguments: ::

  ./rapidsse-<version>.run -- --script --config cluster.config

In this example, the ``--script`` argument indicates that this is not an
interactive installation (therefore do not prompt the user), and the
``--config`` argument specifies the path to the ``cluster.config`` file
to be used.

The installer will load and validate the installation, validate the
environment being installed into, and then install the software onto each
node. The ``cluster.config`` file specified will also be copied to each node
so they are ready to run.


.. index::
   single: installation; arguments

Installation Arguments
``````````````````````
The RapidsSE installation file consists of a self-extracting archive and a
(contained) cluster installation setup script. When arguments are specified
to the installation file we need to separate any arguments for the
self-extracting archive from the arguments for the setup script. This is done
by first specifying the argument ``--``. Every argument after this will be
passed to the cluster installation setup script.

The RapidsSE installation file accepts the following arguments:

.. csv-table:: Installation Arguments
   :header: "Argument", "Description"
   :widths: 20, 80

   "``--``", "Indicates that all arguments that follow are for the cluster
   installation setup script. This must be specified before any of the
   arguments below."
   "``--script``", "Indicates that the installation wizard is not to
   prompt the user for input as this would break the ability to easily
   script the installation. Instead the installation will proceed or fail
   without user intervention.

   Requires a cluster configuration file to be specified with ``--config``.
   Must not be used with ``--interactive``."
   "``--config <file>``", "Allows for a pre-defined cluster configuration
   file to be specified, given by ``<file>``. The installation wizard will
   use this cluster configuration as a starting point, instead of prompting
   the user to enter values manually."
   "``--advanced``

   ``--adv``", "Allows the user to enter all cluster configuration
   settings manually. The installation wizard will still set the default
   values based on the capabilities of each node (CPUs, memory, etc) but
   the user is able to override these settings."
   "``--help``", "Prints out the command line arguments that are accepted."

For example, to run the installation wizard in advanced mode, specify it as follows: ::

  ./rapids-<version>.run -- --advanced


.. index::
   single: installation; sizing considerations
   single: RapidsSE; sizing considerations

.. _considerationsInstaller:

Considerations For Overriding Installer Settings
````````````````````````````````````````````````
Unless an existing cluster configuration file is passed to the installer, the
installation wizard will try to automatically configure RapidsSE for the size
of the nodes in the cluster. There are three main parameters to be considered:

#.	How much shared memory to reserve?
#.	How many partitions to use on each node?
#.	How many threads to use on each node?

After the hostnames of each RapidsSE instance have been entered, the
installation wizard will connect to each host via SSH and determine the amount
of memory and number of CPU cores on each node. It will then find the smallest
value from all nodes in the cluster and create recommendations for each of the
above three parameters based on that smallest value. These values can be
overridden, either by running the installation wizard in advanced mode (using
the ``--advanced`` command line parameter) or by manually altering the
``cluster.config`` file that is generated in each installation directory
afterwards.

Let’s consider how to size each of these parameters manually.

.. index::
   single: shared memory; configuration (RapidsSE)
   single: configuration; shared memory (RapidsSE)

.. rubric:: Shared Memory
   :name: sizing-shared-memory

In general, RapidsSE should be given access to as much of the shared memory as
possible. RapidsSE will make a reservation on that memory but not allocate it
until it is required. This does not mean that other processes cannot use that
memory, instead it expands the virtual memory size of the rapidsse process in
preparation for potentially using this memory. This is visible when observing
the output of the top command when RapidsSE is running as it will show this
memory in the virtual memory column (“``VIRT``”) for the rapidsse process but
the resident memory (“``RES``”), being the size of the memory actually being
used, will typically be much smaller.

Sizing the shared memory reservation for RapidsSE needs to take into account
two factors for any given node:

1.	How big is the space allocated to ``/dev/shm``?
2.	How much memory is available on the system.

RapidsSE cannot be given a shared memory reservation that is bigger than the
size of ``/dev/shm`` on any node in the cluster. The size of ``/dev/shm`` can
be checked by running the command ``df -h /dev/shm``.

However it is possible to configure ``/dev/shm`` to be larger than the
available RAM on the machine. Additionally, we don’t want RapidsSE to use all
of the memory on the machine. So RapidsSE cannot be given a shared memory
reservation that is bigger than 95% of the RAM in the machine.

*Example 1*: if a machine has 64GB of RAM and the size of ``/dev/shm`` is
32GB then RapidsSE can only be allocated a shared memory reservation of 32GB.

*Example 2*: if a machine has 64GB of RAM and the size of ``/dev/shm`` is
64GB then RapidsSE can only be allocated a shared memory reservation of 60GB,
being 95% of the system’s RAM.

*Example 3*: if a machine has 64GB of RAM and the size of ``/dev/shm`` is 96GB
then RapidsSE can only be allocated a shared memory reservation of 60GB,
being 95% of the system’s RAM.


.. index::
   single: partitions; configuration
   single: configuration; partitions

.. rubric:: Partitions
   :name: sizing-partitions

Partitions are how RapidsSE shards data internally. They also allow for
operations on that data to occur in parallel (e.g. fetching, aggregating, etc).
Increasing the number of partitions therefore increases the concurrency in the
system and (with enough CPUs) can give better performance.

In the current version of RapidsSE, each node must have at least 1 partition
and a maximum of 32 partitions. This upper limit on partitions will be raised
in future versions.

Since operations on partitions run concurrently, there is no point specifying
a value that is greater than the number of CPU cores on a node as the system
would not be able to process all partitions at the same time anyway. Instead,
the current recommendation is to specify the number of partitions as half the
number of CPU cores on a given system, keeping within the minimum and maximum
limits above.

*Example 1*: If a machine has 16 cores then the recommendation is to use 8
partitions.

*Example 2*: If a machine has 128 cores then the recommendation is to use
32 partitions, being the current upper limit.

*Example 3*: If a machine has 1 core then the recommendation is to use 1
partition, being the lower limit.


.. index::
   single: threads; configuration
   single: configuration; threads

.. rubric:: Threads
   :name: sizing-threads

Internally, RapidsSE uses tasks to perform work within the system. A task
is a small and discrete unit of work to be done. This could be inserting
a row into an index, aggregating a column value from a set of rows, etc.

Tasks are scheduled and executed on threads. The number of threads that is
set in the system determines how many tasks can be executed in parallel. Set
this number too low and not all the CPUs in the system will be used. Set this
number too high and the system can spent too much time context switching
between threads. Typically this number is set to the number of cores in the
system. However RapidsSE does have a small number of tasks that run on
dedicated threads for common operations. On systems with a small number of
CPU cores this will mean running more threads than there are cores available.
The recommendation is to set the number of threads to the number of cores
available, or on smaller machines it can be set to the number of partitions +
6, or the minimum value of 8, whichever value is greater.

*Example 1*: If a machine has 64 cores and 32 partitions then the
recommendation is to use 64 threads.

*Example 2*: If a machine has 4 cores and 2 partitions then the
recommendation is to use 8 threads being the minimum value.

*Example 3*: If a machine has 8 cores and 4 partitions then the
recommendation is to use 10 threads, being the number of partitions + 6.


.. rubric:: Real World Examples
   :name: sizing-real-world-examples

The following example configurations have been tried successfully. It is
likely that a higher scale factor can be run on these configurations,
however these configurations have not been stressed to their upper limits.

**Note**: These examples do not include significant indexes.

.. csv-table:: Scale Factor 100

  "Nodes", "3"
  "CPU Cores (per node)", "16"
  "RAM (per node)", "64GB"
  "RapidsSE Partitions", "8"
  "RapidsSE Threads", "16"
  "RapidsSE Memory", "58GB"

.. csv-table:: Scale Factor 400

  "Nodes", "3"
  "CPU Cores (per node)", "32"
  "RAM (per node)", "256GB"
  "RapidsSE Partitions", "16"
  "RapidsSE Threads", "32"
  "RapidsSE Memory", "236GB"

.. csv-table:: Scale Factor 1000

  "Nodes", "4"
  "CPU Cores (per node)", "64"
  "RAM (per node)", "512GB"
  "RapidsSE Partitions", "32"
  "RapidsSE Threads", "64"
  "RapidsSE Memory", "473GB"
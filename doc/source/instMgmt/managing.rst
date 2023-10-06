.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. index::
   single: RapidsSE; managing

.. _managing:

Managing RapidsSE
=================

.. index::
   single: installation; directory contents
   single: managing RapidsSE; directory contents

.. _installationDir:

Installation Directory Contents
-------------------------------
RapidsSE is typically installed into the directory ``/opt/rapidsse``.
This directory contains the RapidsSE binary, management scripts and other
files that are used by RapidsSE at runtime.

Important files and folders in the installation directory are descibed
below.

.. table::
   :name: install-dir-desc
   :widths: 25, 75

   ========================== ===========================================================
   Path                       Description
   ========================== ===========================================================
   ``bin/``                   The directory that contains the compiled binaries for
                              RapidsSE.
   ``bin/rapidsse``           The main RapidsSE application binary.
   ``cluster.config``         The configuration file that contains the settings for all
                              RapidsSE nodes in the cluster. This exists on each node
                              and it is used to provide command line parameters to the
                              RapidsSE binary.
   ``collectLogs.sh``         A management tool that collects log and environmental data
                              from each node in the cluster.
   ``commands/``              A directory containing internal RapidsSE script files
                              that is used at runtime.
   ``dbgen/``                 A directory containing the TPC-H utility ``dbgen`` for
                              generating TPC-H data files.
   ``node.name``              Contains the node name of the current instance. This file
                              is created when the node is started.
   ``res/``                   A directory containing various resources that can be used
                              by RapidsSE at runtime.
   ``se.log``                 The main logfile used by RapidsSE.
   ``se.pid``                 A file containing the process ID (pid) of the instance that
                              was started.
   ``seshell.sh``             A script file to invoke SE Shell.
   ``setup.sh``               A script file that is automatically invoked after the
                              installer has unpacked itself to a temporary directory and
                              before RapidsSE is installed in the installation directory.
   ``shell/``                 A directory containing the necessary files for running
                              SE Shell locally or remotely. This directory can be copied
                              to remote machines for remote shell execution.
   ``shell/sejdbc-*.jar``     The RapidsSE JDBC driver.
   ``shell/seshell-*.jar``    The SE Shell binary.
   ``shell/seshell.[sh|bat]`` Script files for executing SE Shell.
   ``startSE.sh``             The script file that starts the RapidsSE cluster.
   ``status.sh``              A script file that checks whether each node in the cluster
                              is running or not.
   ``stopSE.sh``              The script file that stops the RapidsSE cluster.
   ``thirdparty/``            A directory containing libraries and other external files
                              used by RapidsSE at runtime.
   ``tmp/``                   A directory used by RapidsSE for any temporary files.
   ``version``                A file containing the build and version information of
                              this installation of RapidsSE.
   ========================== ===========================================================


.. index::
   single: cluster status
   single: managing RapidsSE; cluster status

.. _clusterStatus:

Checking The Cluster Status
---------------------------

The status of the cluster can be checked at any time by
navigating to the installation directory and executing: ::

  ./status.sh

This script will use SSH to login to each node, change to the installation
directory, read the pidfile (``se.pid``) and confirm if that process is
running. It does not execute any commands through the node. A summary of
the status of each node in the cluster is then printed to the screen.

An example of a running cluster looks like this:

.. code:: console

   craig@hubble:rapidsse$ ./status.sh
   Cluster status:
     MACHINE1 (host hubble) :        running (pid 6662)
     MACHINE2 (host copernicus) :    running (pid 4073)
   All nodes are running.

An example of a stopped cluster looks like this:

.. code:: console

   craig@hubble:rapidsse$ ./status.sh
   Cluster status:
     NODE1 (host hubble) :        NOT RUNNING
     NODE2 (host copernicus) :    NOT RUNNING
   0 nodes out of 2 are running. 2 nodes are stopped.



.. index::
   single: cluster configuration
   single: managing RapidsSE; cluster configuration

.. _clusterConfig:

Cluster Config
--------------
When RapidsSE is first installed, the installer prompts the user to enter
various bits of information such as the installation path where RapidsSE
should be installed. The installer also collects information from each of
the nodes (such as number of CPU cores, memory size, etc) and uses this to
help set the important RapidsSE parameters such as number of partitions,
threads and shared memory size. All of this information is written to a
config file called ``cluster.config``.

The ``cluster.config`` file is written out into the root installation
directory of each node that is installed. The ``cluster.config`` file
contains the settings for each node in the cluster, not just the current node.

When RapidsSE is started via the ``startSE.sh`` script, the settings for
the current node are extracted and passed to the ``rapidsse`` binary as
command line arguments. Each node is started independently this way.

.. caution::

  The ``cluster.config`` file can be modified by hand, however care must be
  taken when doing so.

  * Changes are not reflected until the cluster is stopped and started again;
  * Entering invalid settings may cause RapidsSE nodes not to start;
  * Changes to a ``cluster.config`` file on one node should manually be
    copied to the ``cluster.config`` files on every other node so that each
    node is aware of the changes.

The ``cluster.config`` file can be saved and reused to automate the
re-installation of the cluster without user interaction.


.. index::
   single: log files
   single: managing RapidsSE; log files

.. _logfiles:

Logfiles
--------
If a problem or failure occurs when running RapidsSE, there may be information
about the failure produced in any of two places:

#. On the stdout of the :ref:`SE Shell <seShell>` window, if a command was
   being executed.
#. In the RapidsSE log file.

The RapidsSE log file is located at ``se.log`` in the installation directory
of each node. A failure in the cluster could be caused by a failure in any one
node, so all log files should be checked.

The ``se.log`` file will grow to a maximum of 20MB before it is rolled over to
the file ``se_0.log``. If the file ``se_0.log`` already exists then it is
rolled over to ``se_1.log``. This continues up to a maximum of 4 log files at
which point the oldest file is deleted. The ``se.log`` file will always contain
the newest log entries and ``se_3.log`` will contain the oldest log entries.

The logging behaviour (log file size, number of log files, etc) is not current
configurable.

Log messages will continue to append to the log files when the RapidsSE cluster
is started and stopped, as per the file size and rollover rules above. If you
do not want historical log information then simply delete all log files on each
node of the cluster prior to starting the cluster.

SE Shell does not write to the logfile as it is a separate application and can
be executed from remote machines on the network.

The log files and operating environment each each node can be collected into one
package by running the following command from the installation directory of any
of the nodes.

.. code:: console

  ./collectLogs.sh

This command creates a compressed tarball in the current directory of all of the
logfiles from each node as well as environmental information from each node as
well. This information should be collected and provided when requesting help with
a problem as it will aid in diagnosing the issue.


.. _unloadreload:

Memory Unload & Reload
----------------------

RapidsSE version 1.1.0 includes a fast memory unload to disk and reload from
disk feature. This allows an administrator to take a memory snapshot of the
RapidsSE cluster at a point in time and later reload that snapshot in a timely
manner. This can be significantly faster than starting a cluster and reloading
all the schema and data as there is no parsing and statement execution involved.


.. index::
   single: unload, memory
   single: memory unload
   single: managing RapidsSE; memory unload

.. _unload:

Unload
``````
A RapidsSE unload operation writes out its memory segment that contains
persistent data such as tables, indexes and rows to disk on each node in the
cluster. This occurs concurrently on each node and it is typically limited by
the I/O bandwidth of the disk devices in the system. Unloading to multiple disks
on each device improves both the unload and reload performance.

The data that is written out during an unload operation is not human readable
as it is a raw memory dump. The written data is also only relevant to the
current version of RapidsSE as newer versions of RapidsSE may change the
internal memory layout. For this reason, the unload/reload capability should
not be considered an export or a backup utility.

An unload operation can only occur when the cluster is running. It is invoked
by running the unload command on the SE Shell. Please refer to the RapidsSE
Command Reference section for details. Additionally, the cluster must be
quiesced at the time of the unload command (that is, no user activity on the
system) otherwise the memory snapshot taken may be inconsistent and RapidsSE
may fail upon reload.

During this unload operation the memory snapshot will be written out to
multiple files, one per RapidsSE partition, spread over one or more
directories. This will occur on each node in the cluster concurrently. That
is, each node will write its memory footprint to its own local disk(s). The
paths where these files are written are specified during installation as
Storage Paths. Each node will write to the same path locally, so the mounting
of disks in the filesystem on each node in the cluster must be configured
similarly. For best performance each of these paths specified during installation
should be mounted on separate disks.

There must also be sufficient disk space on these filesystem paths to hold
the memory segment. The amount of disk space required is approximately similar
to the output reported from the space command for any given node, plus some
minor overhead when the data size is small.

Multiple snapshots can be written out to disk (assuming there is sufficient
disk space) by specifying a different snapshot name with the unload command.
Data will be written to a subdirectory with this snapshot name underneath the
filesystem paths that were specified during installation.

Obsolete snapshots can simply be deleted from disk on each node in the cluster
when they are no longer required.

.. index::
   single: reload, memory
   single: memory reload
   single: managing RapidsSE; memory reload

.. _reload:

Reload
``````
A reload operation can only occur when the cluster is being started. It is
not possible to reload a snapshot while the system is running. This is done
by specifying additional command line parameters to the ``startSE.sh`` script
to tell it which unloaded memory segment is to be reloaded immediately after
the cluster boots.

The command syntax to be used is: ::

	./startSE.sh --reload <name_of_snapshot>

Example: ::

	./startSE.sh --reload mySnapshot

If the cluster was configured with a single data storage path at ``/mnt/disk1``
then this reload command would read the data files located at
``/mnt/disk1/mySnapshot`` on each node in the cluster.

Reloading a snapshot does not modify or remove the snapshot. The cluster can be
reloaded from it multiple times.

Constraints
```````````
The major constraints around the unload and reload feature can be summarised
as follows:

#. The cluster must be quiesced before and during the entire unload operation.
#. The disk paths where data will be written to must be available and writable
   on each node in the cluster.
#. The unload snapshot is only applicable to the version of RapidsSE that it
   was created on.
#. The unloaded snapshot should not be considered a data backup for the above
   reasons.


Example
```````
Assume that RapidsSE has been installed in a 2 node, 4 partition cluster, and
the following storage paths were specified during installation: ::

	/mnt/disk1
	/mnt/disk2

A snapshot of the persistent data heap can be taken by running the following
command in the SE Shell: ::

  unload '<name_of_snapshot>';

Here is what it looks like when it is run:

.. code:: console

   se> unload 'snapshot-2022-12-08';
   success: 0 rows fetched

This will create the following directories on each node: ::

	/mnt/disk1/snapshot-2022-12-08
	/mnt/disk2/snapshot-2022-12-08

These directories will contain the memory dump of the local partitions,
spread over these two paths. In the directory listings below, partitions 0 and
2 are written under ``/mnt/disk1`` and partitions 1 and 3 are written under
``/mnt/disk2``.

.. code:: console

   craig@ubuntu22:disk1$ ls -l /mnt/disk1/snapshot-2022-12-08/
   total 30720
   -rw-rw-r-- 1 craig craig 18874368 Dec  8 11:59 saveFile_NODE1_0
   -rw-rw-r-- 1 craig craig 12582912 Dec  8 11:59 saveFile_NODE1_2
   craig@ubuntu22:disk1$
   craig@ubuntu22:disk1$ ls -l /mnt/disk2/snapshot-2022-12-08/
   total 24576
   -rw-rw-r-- 1 craig craig 13631488 Dec  8 11:59 saveFile_NODE1_1
   -rw-rw-r-- 1 craig craig 11534336 Dec  8 11:59 saveFile_NODE1_3

At this point the cluster can be stopped before it is restarted with this
snapshot data.

.. code:: console

   craig@ubuntu22:disk1$ ./stopSE.sh
   Stopping the RapidsSE cluster.
   Stopping all 1 nodes in the cluster. Waiting for them to shutdown.
   Stopped all nodes in the cluster.

Now the system can be restarted with this snapshot specified on the command
line: ::

  ./startSE.sh --reload snapshot-2022-12-08

Here is what it looks like when it is run:

.. code:: console

   craig@ubuntu22: rapidsse$ ./startSE.sh --reload snapshot-2022-12-08
   Checking node versions...All nodes are running the same version: 1.1.0-dev0-g576e8eb590
   Starting the RapidsSE cluster.
   Started 1 node in the cluster. Waiting for it to boot
   Waiting for the added nodes to boot: . . . . . . . . . . . .
   Cluster status:
     NODE1 (host ubuntu22) :     running (pid 108344)
   All nodes are running.
   Waiting for the cluster to cross-connect...Done
   Cluster startup complete.

The tables, indexes and data that were present at the time of the unload should
now exist within RapidsSE.

.. index::
   single: security
   single: managing RapidsSE; security

.. _security:

Security
--------
RapidsSE does not currently have any authentication or authorization
capabilities, though this is an important feature for a future release.


.. index::
   single: memory usage
   single: managing RapidsSE; memory usage

.. _monitoringMemoryUsage:

Monitoring Memory Usage
-----------------------
As RapidsSE is an in-memory system, it is important for administrators to
understand the usage of memory. RapidsSE stores all temporary and persistent
data in a shared memory segment, which is typically exposed by the operating
system through the virtual device ``/dev/shm`` on Linux. This device can be
inspected with the ``ls`` command to show the shared memory segments that
have been created and their size, as per the example below:

.. code:: console

  craig@hubble:rapidsse$ ls -l /dev/shm
  total 2762696
  -rw------- 1 postgres postgres      16192 Dec 14 12:49 PostgreSQL.472656321
  -rw------- 1 bamboo   bamboo   8589934592 Jan  4 17:18 SE_NODE1

In this example we can see that there is a shared memory 'file' called
``SE_NODE1`` that consumes about 8.5GB of memory. This is a first and *very
rough* indicator of memory usage as RapidsSE may not be using all of that
8.5GB of space at present. There may be plenty of empty blocks that were
deallocated by RapidsSE, ready to be reused again.

Using commands like ``top`` and ``ps`` will also show a similar picture
that RapidsSE is likely using a good amount of *virtual* memory corresponding
to the size of the configured shared memory segment. The amount of *resident*
will indicate how much memory has been requested from RapidsSE from the
operating system, but it doesn't include memory that RapidsSE has deallocated
but that still belongs to the process.

To get a true understanding of how much memory RapidsSE is using and how
much is spare we must ask RapidsSE itself. This is done by executing the
following command through the SE Shell: ::

  space;

The ``space`` command will inspect the shared memory segments of each node
and calculate how much has been used and how much is free. This is a fast
operation. The results will be returned as one row per node. This is seen
in the example below:

.. code:: console

   se> space;
   metadata: (nodeName VARCHAR,totalSpace INTEGER,totalSpaceStr VARCHAR,inUseSpace INTEGER,inUseSpaceStr VARCHAR,freeSpace INTEGER,freeSpaceStr VARCHAR)

   row: ('NODE1',15032385536,'14GB',842393600,'803MB',14189991936,'13GB')
   row: ('NODE2',15032385536,'14GB',819503104,'782MB',14212882432,'13GB')
   success: 2 rows fetched

In this example we have a 2 node cluster and ``NODE1`` is using 803MB of its
14GB shared memory segment. ``NODE2`` is using a similar amount and both have
approximately 13GB of memory free.

To help drill down and identify where some of this memory is being used,
the commands sztbl and szidx can be helpful. They will show a similar
output with one row per node showing the size of the data or the size of
an index respectively.

.. code:: console

   se> sztbl lineitem;

   metadata: (NodeName VARCHAR,size INTEGER,sizeStr VARCHAR)

   row: ('NODE1',2097152,'2MB')
   row: ('NODE2',2097152,'2MB')
   success: 2 rows fetched


.. code:: console

   se> szidx lineitem lineitem_idx1;

   metadata: (NodeName VARCHAR,size INTEGER,sizeStr VARCHAR)

   row: ('NODE1',2097152,'2MB')
   row: ('NODE2',2097152,'2MB')
   success: 2 rows fetched

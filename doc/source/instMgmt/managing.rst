.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _managing:

Managing RapidsSE
=================

Logfiles
--------
If a problem or failure occurs when running RapidsSE, there may be information
about the failure produced in any of two places:

#. On the stdout of the :ref:`SE Prompt <sePrompt>` window, if a command was
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

The SE prompt does not write to the logfile.

The log files and operating environment each each node can be collected into one
package by running the command ``./collectLogs.sh`` from the installation
directory of any of the nodes. This information should be collected and provided
when requesting help with a problem as it will aid in diagnosing the issue.


Memory Unload & Reload
----------------------

RapidsSE version 1.1.0 includes a fast memory unload to disk and reload from
disk feature. This allows an administrator to take a memory snapshot of the
RapidsSE cluster at a point in time and later reload that snapshot in a timely
manner. This can be significantly faster than starting a cluster and reloading
all the schema and data as there is no parsing and statement execution involved.

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
by running the unload command on the SE prompt. Please refer to the RapidsSE
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
command in the SE prompt: ::

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



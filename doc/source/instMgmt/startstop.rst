.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _startstop:

Starting and Stopping RapidsSE
==============================

.. index::
   single: RapidsSE; starting
   single: starting RapidsSE
   single: managing RapidsSE; starting

Starting RapidsSE
-----------------
After installing and configuring RapidsSE the cluster can be started. Each node
will have a ``cluster.config`` file in their installation directory with the
configuration settings for the entire cluster.

To start the RapidsSE cluster, simply navigate to the installation directory on
one of the nodes and execute the command: ::

  ./startSE.sh

This script file will first SSH to each node in the cluster and confirm that all
nodes are running the same version of software before launching them. When they
have all been given time to start up the status of each node will be printed.

After each node has started successfully, the installer waits for each node in
the cluster to cross connect. This is where each node in the cluster discovers
one another via the cluster heartbeat UDP broadcast, at which point they all
connect to each other on the internal port number. A simple message that the
cluster initialisation is complete indicates that this operation was successful.
However if this cluster initialisation fails or takes longer than 15 seconds
(or 45 seconds in the case of a multicast heartbeat) to complete then an error
is printed to the screen indicating that this has failed. The log files for
each node in the cluster should be consulted (``se.log``).

If all of the nodes start up successfully then the SE Prompt can be launched.

A successful startup of RapidsSE looks like the following:

.. code:: console

   craig@hubble:rapidsse$ ./startSE.sh
   Checking node versions...All nodes are running the same version: 1.0.0-g3225a7b5bb
   Starting the RapidsSE cluster.
   Started 2 nodes in the cluster. Waiting for them to boot
   Waiting for the added nodes to boot: . . .
   Cluster status:
     NODE1 (host hubble) :        running (pid 16620)
     NODE2 (host copernicus) :    running (pid 3871)
   All nodes are running.
   Waiting for the cluster to cross-connect...Done
   Cluster startup complete.

.. index::
   single: RapidsSE; stopping
   single: stopping RapidsSE
   single: managing RapidsSE; stopping

Stopping RapidsSE
-----------------
Stopping the RapidsSE cluster is also a simple operation. Simply navigate to
the installation directory on one of the nodes and execute: ::

  ./stopSE.sh

When this occurs, the script file will SSH to each node, read the process ID
out of a file (``se.pid``) and send a signal to that process. If the pidfile
does not exist then it will try to kill all processes owned by the current
user with the name ``rapidsse``.

A successful stop of the cluster looks like this:

.. code:: console

   craig@hubble:rapidsse$ ./stopSE.sh
   Stopping the RapidsSE cluster.
   Stopping all 2 nodes in the cluster. Waiting for them to shutdown.
   Stopped all nodes in the cluster.


.. index::
   single: SE Prompt; starting & stopping
   single: managing RapidsSE; SE Prompt

.. _sePrompt:

SE Prompt
---------

The SE prompt is the current mechanism for interactively submitting commands
into RapidsSE. The SE Prompt must be executed on one of the nodes in the
cluster as it communicates with the node via a shared memory interface.

The SE Prompt can be launched by navigating to the installation directory on
any of the nodes and executing: ``./prompt.sh``

SE prompt does not support up-arrow command histories, cursor word/line jumping,
or text replacement (other than simple backspacing). The SE Prompt is intended
to be replaced by the :ref:`SE Shell <seShellInterface>`.

When it starts up, the SE prompt will print some internal version information
before printing the console input marker. An example of this is shown below:

.. code:: console

   craig@hubble:rapidsse$ ./prompt.sh
   RapidsSE 1.0.0 (release version, pid 3980, git 3225a7b5bb386300)
   (c) 2020 Rapids Data Technology Co. Ltd
   se>

Commands that are submitted to the SE Prompt must be terminated with a
semicolon.

Sequences of commands can also be put into a file in the ``Commands``
subdirectory and that entire file executed by simply typing its file name.
e.g. the file ``Commands/_demoSetup`` contains various commands for setting
up a TPC-H demo. It can be executed from the SE Prompt by simply typing ``_demoSetup;``.

To quit the SE Prompt, either enter the command ``bye;`` or simply press ``Control-C``.

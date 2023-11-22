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

If all of the nodes start up successfully then the SE Shell can be launched.

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
   single: SE Shell; starting & stopping
   single: managing RapidsSE; SE Shell

.. _seShell:

SE Shell
---------

The SE Shell is the current mechanism for interactively submitting commands
into RapidsSE. SE Shell can be executed on any machine on the network, however
if it is not executed on a machine running a RapidsSE node or if that node is
not listening on the default port then SE Shell must be given command line
arguments telling it which host/IP and port to connect to.

SE Shell can be launched by navigating to the installation directory on
any of the nodes and executing: ``./seshell.sh``

Commands that are submitted through SE Shell must be terminated with a
semicolon.

To quit SE Shell, either enter the commands ``exit;``, ``bye;`` or ``quit;``,
or simply press ``Control-C``.

For more detailed information, refer to the :ref:`SE Shell documentation <seShellInterface>`.


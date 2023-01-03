.. contents:: Contents of this section:
   :depth: 3
   :backlinks: none

.. _startstop:

Starting and Stopping RapidsSE
==============================

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


Checking Cluster Status
-----------------------

The status of the cluster can be checked at any time by
navigating to the installation directory and executing: ::

  ./status.sh

This script will use SSH to login to each node, change to the installation
directory, read the pidfile (``se.pid``) and confirm if that process is
running. A summary of the status of each node in the cluster is then printed
to the screen.

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

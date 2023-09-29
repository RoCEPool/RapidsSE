.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. index::
   single: SE Shell; introduction

.. _seShellIntro:

Introduction
============

About the SE Shell
------------------

The SE Shell is a command line application for executing SQL statements against
a RapidsSE cluster and viewing the results. It is designed for ad-hoc custom
queries being entered directly by a user and it formats results for on-screen
readability. SE Shell can connect to any RapidsSE node in the cluster.

SE Shell is not designed for being used programmatically. It is possible to
script SE Shell for simple purposes such as collecting results to a file or
executing a command from the stdout of a UNIX pipeline, but more elaborate
programmatic intentions should use the :ref:`JDBC driver <jdbcDriverInterface>`
or write their own interface to RapidsSE using the
:ref:`wireline protocol <wirelineProtocolInterface>`.

SE Shell is included in the installation of each RapidsSE node.

SE Shell is based off the Rapids Shell that is included with RapidsDB. Both
applications work in the same way however there are some minor differences as
each shell has some feature customisations for each server they are connecting
to.


.. index::
   single: SE Shell; architecture

Architecture
------------

SE Shell is a Java application that is built on top of a JDBC driver. Being
Java based, it can be run on a variety of platforms. It is also largely agnostic
of the JDBC driver that is being used, so it is possible to use SE Shell against
other SQL systems.

SE Shell comes coupled with the RapidsSE JDBC driver. Because this JDBC driver
is TCP socket based, SE Shell does not need to be executed on the same machine as
the RapidsSE node that it wishes to connect to. SE Shell can be given a hostname
(or IP address) and port to use to connect to a remote node.

The JDBC driver that SE Shell uses is built upon the
:ref:`RapidsSE wireline protocol <wirelineProtocolInterface>`, which defines both
the way messages are formatted as well as the sequences in which those messages
are sent and received.

:numref:`seShellArchitecture` shows the architecture of SE Shell communicating
with a RapidsSE node.

.. _seShellArchitecture:

.. figure:: seShellArchitecture.*
   :align: center
   The architecture of the SE Shell client communicating with RapidsSE.



Limits & Constraints
--------------------

- SE Shell is designed for communicating with RapidsSE using the RapidsSE JDBC
  driver. As such, while it is possible to use SE Shell against other JDBC
  data systems, SE Shell may not work perfectly against them. For example,
  SE Shell is built to support the same data types that RapidsSE supports and
  may not correctly render additional data types from a 3rd party product.

- SE Shell does not have all of the same internal commands as Rapids Shell
  because those commands different are built for the different functionality
  of RapidsDB.



.. index::
   single: SE Shell; requirements

Requirements and Dependencies
-----------------------------

SE Shell has been tested under Java runtime 11. It has not certified for
use under prior versions of Java, even though it may appear to run fine.

SE Shell requires a JDBC driver and a matching JDBC URL to work. By default
it is bundled with the RapidsSE JDBC driver, though it is possible to use
SE Shell against other products by putting the 3rd party JDBC driver in the
Java classpath of SE Shell and using a different JDBC URL that will invoke
that 3rd party JDBC driver.

Otherwise SE Shell has no other external dependencies.


.. index::
   single: SE Shell; installation

Installation
------------

SE Shell is distributed as part of the RapidsSE installation. SE Shell is
installed with the installation of each node in the cluster. It can be found
in the ``<installation_dir>/shell`` path on each node.

SE Shell can be copied and executed on other machines by simply copying the
``shell`` directory from one of the RapidsSE node installations to a remote
machine. This directory contains the necessary shell scripts and JDBC drivers
that SE Shell requires. There is nothing else to be installed.
.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. index::
   single: JDBC driver; introduction

.. _jdbcDriverIntro:

Introduction
============

About the JDBC Driver
---------------------

The RapidsSE JDBC driver is the primary supported method for accessing RapidsSE
from a client application. It is a type 4 JDBC driver that provides a programmatic
interface for Java applications to RapidsSE. A type 4 JDBC driver is written
entirely in Java and communicates with the database system using the database
system’s own network protocol. Because of this, the driver is platform independent;
once compiled, the driver can be used on any system.

The RapidsSE JDBC driver is based on the similar RapidsDB JDBC driver, however
while the two are very similar and share a lot of code, the two are not
compatible. This is not only because of differences in the version of the
underlying wireline protocol that they both use, but also with how the JDBC driver
implements certain functions such as metadata retrieval. The RapidsDB JDBC driver
also has support for federated data sources which RapidsSE and its JDBC driver
does not need to handle.

This section of the documentation describes how to configure and use the RapidsSE
JDBC driver. It is not intended to be a guide for programming with JDBC. For more
information on JDBC the user should refer to the standard JDBC API documentation.


.. index::
   single: JDBC driver; architecture

Architecture
------------

The JDBC driver communicates with a node in the RapidsSE cluster using a
Thrift-based messaging protocol referred to as the
:ref:`RapidsSE Wireline Protocol <wirelineProtocolInterface>`. This is shown
in :numref:`jdbcDriverArchitecture`. This wireline protocol can support
various different transport mechanisms, though the currently supported transport
is TCP/IP sockets.

.. _jdbcDriverArchitecture:

.. figure:: jdbcDriverArchitecture.*
   :align: center

   The architecture of a JDBC client communicating with RapidsSE.

The client can configure the JDBC Driver to connect to any of the nodes in the
RapidsSE cluster - all nodes are considered equal. There can also be multiple
JDBC drivers communicating with different nodes in the RapidsSE cluster at any
time. This can be via multiple connections from a single client application or
through multiple clients all communicating with RapidsSE.

The JDBC standard only allows a single query at a time on a single JDBC
connection. Therefore, if a client application wishes to execute concurrent
queries it must create multiple JDBC connections and execute single queries
over each of the connections. Each connection will have its own TCP/IP connection
to RapidsSE.

Within RapidsSE a Socket Handler will accept incoming client connections and read
data payloads off the socket. These are passed on to a Wireline Adaptor which
decodes the payload for a given version of the wireline protocol and determines
how to process it. This will typically involve extracting the query to be executed
and passing this off to the Execution Engine to be executed. The Execution Engine
will return metadata and data (e.g. rows or error messages) back to the Wireline
Adaptor which will then serialise this information into Wireline Thrift messages
before passing it back to the Socket Handler which will send it back to the
client application.


Limits & Constraints
--------------------

- The RapidsSE JDBC driver is not compatible with RapidsDB. Similarly, the
  RapidsDB JDBC driver is not compatible with RapidsSE. Both drivers have
  different JDBC URLs to protect against accidentally using one driver against
  the other server.


.. index::
   single: JDBC driver; requirements

Requirements and Dependencies
-----------------------------

The driver has been tested under Java runtime 11. It has not certified for
use under prior versions of Java.

The driver uses the Java package SLF4J to integrate with client logging
frameworks. The driver will emit a warning message from SLF4J if there is no
logging framework supplied by the client application. The JDBC driver complies
with the SLF4J logging API, but it is up to the end-user to provide a logging
framework compatible to SLF4J.

The client port that the JDBC driver connects to on a RapidsSE server must be
opened on any firewalls that exist between the client computer and the RapidsSE
server. This is typically port ``TCP/9123``, though it can be customised when
RapidsSE is installed.


.. index::
   single: JDBC driver; installation

Installation
------------

The JDBC driver does not need any specific installation instructions. It already
comes bundled in the installation of each RapidsSE node, for use with
:ref:`SE Shell <seShellInterface>`.

To use the JDBC driver in a separate client application, simply copy the JDBC
driver JAR file to your project and ensure it is included in the Java class path
when your client is executed. Java will preload the driver JAR file and register
it as an available JDBC driver. If the client application then uses the a valid
JDBC URL then this driver will be used.

.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. index::
   single: JDBC driver; usage

.. _jdbcDriverUsage:

Using the JDBC Driver
=====================

.. index::
   single: JDBC driver; configuration

Configuration
-------------

There are two aspects to configuring the Java client to use the JDBC driver
for connecting to RapidsSE: understanding the TCP/IP port number that RapidsSE
is listening on, and forming a valid JDBC connection URL. These are explained
below.

.. index::
   single: JDBC driver; port number

.. _clientPort:

RapidsSE Client Port
````````````````````

The RapidsSE JDBC driver can connect to and communicate with any node in a
RapidsSE cluster. By default each node in the cluster listens on port
``TCP/9123`` for client connections, though this can be customised during the
RapidsSE installation.

To verify the port number that is being used open the RapidsSE ``cluster.config``
file, found in the root installation directory of any node (default:
``/opt/rapidsse``) in a text editor. This is a Bash script file that defines
the cluster properties as Bash variables. Each node in the cluster has an array
variable that holds all of its properties. Find the array variable that
corresponds to the RapidsSE node that the JDBC driver will connect to and look
for the key named ``client_port``. This will be set with the port number that
that node will be listening on for incoming TCP/IP client connections.

A portion of an example ``cluster.config`` file showing the RapidsSE node
listening on port ``9123`` is shown below.

.. code:: bash

  declare -ga NODE_LIST
  NODE_LIST=( "NODE1" )

  declare -gA NODE_CFG_NODE1
  NODE_CFG_NODE1["params"]=""
  NODE_CFG_NODE1["threads"]="10"
  NODE_CFG_NODE1["host"]="se1.local"
  NODE_CFG_NODE1["client_port"]="9123"
  NODE_CFG_NODE1["name"]="NODE1"
  NODE_CFG_NODE1["partitions"]="4"
  NODE_CFG_NODE1["heartbeat_address"]="BROADCAST"
  NODE_CFG_NODE1["mem"]="8"
  NODE_CFG_NODE1["disk_paths"]="/mnt/disk1/rapidsse/NODE1/data"
  NODE_CFG_NODE1["dir"]="/mnt/disk1/rapidsse/NODE1"
  NODE_CFG_NODE1["clusterid"]="1234"
  NODE_CFG_NODE1["internal_port"]="5541"
  NODE_CFG_NODE1["heartbeat_port"]="8888"

In this example it is the line ``NODE_CFG_NODE1["client_port"]="9123"``
that defines that this node (installed to the machine ``se1.local``) is listening
on port ``9123``.


.. index::
   single: JDBC driver; url

.. _basicJdbcUrl:

Basic JDBC Connection URL
`````````````````````````

Every JDBC driver requires a connection URL that not only identifies it as a
connection to a RapidsSE system, but can also configure that connection and
set any default values it may use. All RapidsSE JDBC drivers must use a
connection URL that begins with ``jdbc:se:``

Connection URLs are case sensitive. The ``se`` part of the URL indicates that
this is a connection to a RapidsSE database. It will be recognised by the
RapidsSE JDBC Driver and ignored by JDBC drivers to other systems (e.g., the
RapidsDB JDBC driver). When used, the above URL will tell the RapidsSE JDBC
Driver to connect to a RapidsSE node on ``localhost:9123``.

The JDBC driver has a number of optional fields that can be specified to
configure how the JDBC driver connects and interacts with the RapidsSE server.
These include:

- Setting a list of one or more host and port numbers of the nodes in the
  RapidsSE cluster that the JDBC Driver can connect to.

- Setting any default session properties.

These fields need to be set in a specific order and with specific syntax. This
is documented below.


Connection URL Grammar
``````````````````````

The full railroad grammar diagrams for the JDBC URL are shown below.

.. _jdbcDriverURLRailroad:

.. rubric:: Rule: connectionURL

.. image:: connectionURL/connectionURL.png
   :align: center


.. rubric:: Rule: hostAndPort

.. image:: connectionURL/hostAndPort.png
   :align: center


.. rubric:: Rule: host

.. image:: connectionURL/host.png
   :align: center

..  TODO: add this in when supported
.. .. rubric:: Rule: defaultLocation

.. .. image:: connectionURL/defaultLocation.png
..    :align: center


.. rubric:: Rule: properties

.. image:: connectionURL/properties.png
   :align: center


In addition, the following rules apply to the connection URL:

#. The URL must not contain any backslashes or escaped characters.

#. The URL must not end with any of the following characters: ``/``, ``=``, ``?` or ``&``.


Specifying the Host and Port Numbers
````````````````````````````````````

The RapidsSE JDBC Driver can connect to any node on the port number configured
by the ``client_port`` parameter for that RapidsSE node (see :ref:`above <clientPort>`).
When not specified, the JDBC driver will connect to localhost:9123 by default.
The user can provide a comma-separated list of host and port numbers, and the
JDBC Driver will randomly choose a host from the set when a connection is being
established. The use of multiple nodes allows for connection balancing across the
specified set of hosts.

Specifying the list of host and port numbers must come immediately after the
:ref:`basic URL <basicJdbcUrl>` given above. The host and port list is delineated
by a double forwardslash (``//``) before the hostname.

The hostname can be a DNS-resolvable name of a host, or it may also be an IP
address. Both IPv4 and IPv6 addresses are valid, however an IPv6 address must be
enclosed within square brackets because it uses colons to delimit octets and the
square brackets prevent the IPv6 address from being confused with the port number.

The port number is separated from the hostname by a colon (``:``). Specifying the
port number is optional.

If multiple hostnames or IP addresses are specified then they are separated
by commas (``,``).

Some valid examples of setting the hostname and port include:

.. code::

  # Using loopback on port 9123
  jdbc:rdp://localhost
  jdbc:rdp://localhost:9123
  jdbc:rdp://127.0.0.1:9123

  # Using IPv6 loopback on port 9123
  jdbc:rdp://[::1]
  jdbc:rdp://[::1]:9123

  # Using a specific IPv6 address
  jdbc:rdp://[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:9123

  # Using connection balancing over 3 hosts
  jdbc:rdp://192.168.10.10:9123,192.168.10.11:9123,192.168.10.12:9123


Specifying the Default Catalog and Schema
`````````````````````````````````````````

TODO: Fill in, but comment out until supported

Supported Properties
````````````````````

TODO: on the connection URL
Properties may be key only.
Property values may be quoted strings with single or double quotes.

Check if these are usable:
continuebatchonerror
maxrowstoreturn
sotimeout
sokeepalive
tcpnodelay
rowsperrowdataresponse
logintimeout

rdpkrbprincipal
clientkrbprincipal
catalogname

are these case sensitive?

Authentication
--------------


Supported ResultSet Retrieval Data Types
----------------------------------------

Supported SQL to Java Data Type Conversions
-------------------------------------------

Debugging JDBC Applications
---------------------------

Sample Application
------------------



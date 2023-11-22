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
   single: JDBC driver; RapidsSE configuration

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


.. index::
   single: JDBC driver; url

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



.. index::
   single: JDBC driver; host
   single: JDBC driver; port number

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


..
  **** NOTE: This is commented out until it is supported. ****

  Specifying the Default Catalog and Schema
  `````````````````````````````````````````

  When using the JDBC driver, RapidsSE supports database sessions and therefore
  some degree of statefulness. This allows a user to set the default catalog and/or
  schema, which will then change the behavior of the system when subsequent queries
  are planned and executed. Setting the default catalog and schema can be achieved
  either by specifying them as part of the JDBC connection URL, or by sending the
  following explicit commands to RapidsSE:

  - ``SET CATALOG catalogName;``

  - ``SET SCHEMA [ catalogName . ] schemaName;``

  When setting the schema, the catalog must either have been previously set, or
  the catalog name must be specified as part of the ``SET SCHEMA`` command.

  The value of the current catalog and schema can be retrieved by sending the
  following query via JDBC:

  .. code:: sql

    SELECT CURRENT_CATALOG, CURRENT_SCHEMA FROM RAPIDS.SYSTEM.TABLES LIMIT 1;

  Setting a default catalog or schema will change the way that RapidsSE resolves
  unqualified tables (table names that do not have the catalog and schema
  specified). Without a default catalog or schema set, RapidsSE will search every
  catalog and schema for a table matching the table name in the query. However
  with a default catalog and/or schema set, any unqualified table name will be
  converted into a qualified table name with the default catalog and schema
  prepended before it. For example, if the default catalog was ``RAPIDS`` and
  the default schema was ``SYSTEM`` and the query was ``SELECT * FROM TABLES;``
  then the unqualified table name ``TABLES`` would first be translated into the
  qualified name ``RAPIDS.SYSTEM.TABLES``.

  After a default catalog or schema has been set, it is possible to go back to the
  original behavior of the system by setting the current catalog and/or schema to
  null. e.g., ``SET CATALOG NULL;``.

  Instead of executing these ``SET`` commands individually, one can specify the
  default catalog and schema in the JDBC connection URL, and the JDBC driver will
  set these defaults for you automatically when the connection is established.

  Setting the default catalog/schema is specified in a hierarchical fashion using
  a single forward slash (``/``) to delineate these parameters. This grouping of
  parameters occurs after the hostname/port group (if set).

  In this field, it is possible to set the following combinations of defaults:

  #.	The default catalog.

  #.	The default catalog and schema

  The ordering of fields in the above list is significant.

  Some examples of setting these defaults can be seen below:

  .. code::

    # sets the default catalog to RAPIDS
    jdbc:se://localhost:4333/RAPIDS

    # sets the default catalog to RAPIDS and schema to SYSTEM
    jdbc:se://localhost:4333/RAPIDS/SYSTEM

    # set default catalog and schema with multiple hosts
    jdbc:se://192.168.10.10, 192.168.10.11/RAPIDS/SYSTEM

    # sets the default catalog to RAPIDS without specifying a host/port
    jdbc:se:/RAPIDS

    # sets the default catalog to RAPIDS and schema to SYSTEM without specifying a host/port
    jdbc:se:/RAPIDS/SYSTEM


.. index::
   single: JDBC driver; properties

.. _jdbcSupportedProperties:

Supported Properties
````````````````````

The JDBC driver supports the use of properties which can change how the driver
works. It can also change the behaviour of the associated session on a RapidsSE
node it is connected to.

Properties have a key name and an optional value. Both the key and any value are
specified as strings. **Key names are case sensitive**.

The RapidsSE JDBC driver supports the following properties:

.. table::
  :name: jdbc-properties
  :widths: 20, 15, 65

  =========================== ============= ==============================================================
  Key                         Default Value Description
  =========================== ============= ==============================================================
  ``sokeepalive``             ``false``     If set to ``true``, this sets the keepalive option on the TCP
                                            socket. If there is no data transferred over the socket for
                                            2 hours and this option is set to ``true``, the TCP/IP stack
                                            on the client will send a keepalive probe to the server. This
                                            tests whether the server is still alive and operating. If the
                                            server has stopped or is no longer running then an error will
                                            be propagated back to the client.

                                            It is also useful if the connection between the JDBC driver
                                            and the server go through firewalls or proxies. Setting the
                                            keepalive flag to ``true`` will help stop an intermediary
                                            proxy from thinking this connection is idle and stale and
                                            therefore closing this connection to free up resources.

                                            Please note that setting this option to ``true`` does
                                            generate extra network traffic, though it is likely to be
                                            negligible.
  ``tcpnodelay``              ``true``      If set to ``true`` the Nagle algorithm (TCP_NODELAY) for
                                            this connection will be disabled. Data that is written to
                                            the socket is no longer buffered pending acknowledgements
                                            of previous writes. Instead the data is sent immediately.
                                            This can help improve latency of small socket writes at the
                                            expense of overall throughput.
  ``logintimeout``            ``0``         The maximum number of seconds to wait while the JDBC driver
                                            establishes a TCP/IP connection to the server and logs in.
                                            If this time is exceeded then the driver will close the
                                            socket and an error will be returned to the client.

                                            If this is set to ``0`` then no timeout will apply to the
                                            login sequence.
  ``continuebatchonerror``    ``false``     Controls whether RapidsSE will continue executing additional
                                            statements in the batch if the current statement has returned
                                            an error. If set to ``true`` then any further statements in
                                            the batch will be executed. If set to ``false`` then the
                                            execution of the batch ends.
  ``maxrowstoreturn``         ``-1``        Sets an upper limit on the maximum number of rows to return
                                            when any given statement is executed. After this limit is
                                            reached RapidsSE will not send any additional rows in that
                                            may be in the result set.

                                            Set to ``-1`` for all rows to be returned. Setting this to
                                            a zero or positive value will limit the number of rows for
                                            each statement executed.
  ``rowsperrowdataresponse``  ``10000``     Controls the maximum number of rows that are sent from the
                                            RapidsSE server to the JDBC driver in each RowDataResponse
                                            message. This can be used to help control the size of
                                            these individual messages as they are sent over the wireline
                                            protocol. Typically RapidsSE will also internally limit these
                                            messages to no larger than 1MB. This property can be used to
                                            limit the size of these messages based on the number of rows
                                            that they contain.
  =========================== ============= ==============================================================

..
  TODO: Don't forget to add these properties in when Kerberos authentication is supported
  rdpkrbprincipal
  clientkrbprincipal
  keytab


Properties may be specified in two ways: in the JDBC URL, or programmatically in
a Java ``Properties`` instance prior that is passed to the driver when the
connection is established. An example of using the Java ``Properties`` approach
is shown below.

.. code:: java

  // The JDBC URL to be used with host and port. No properties in the URL.
  String url = "jdbc:se://host1:9123";

  // Define the properties to use. Values are always strings.
  Properties info = new Properties();
  info.setProperty("key1", "value1");
  info.setProperty("key2", "value2");
  info.setProperty("key3", null);

  // Now establish the connection with these properties
  Connection connection = DriverManager.connect(url, info);

Alternatively, to specify the properties in the URL we must first separate the
properties from the earlier structure of the URL with a ``?`` character. The
properties can then be specified separated by ``&`` characters, like a standard
HTTP URL parameter encoding. Property keys and values can be quoted with either
single or double quotes if they contain the ``?``, ``&`` or ``=`` characters.
Some examples are shown below:

.. code::

  # An empty parameter list
  jdbc:se://host1:9123?

  # An empty parameter list with the default host and port
  jdbc:se:?

  # Setting a simple unquoted property
  jdbc:se://host1:9123?key1=value1

  # Setting 3 simple unquoted properties
  jdbc:se://host1:9123?key1=val1&key2=val2&key3=val3

  # Setting a key with no value
  jdbc:se://host1:9123?key1

  # Setting a quoted key and value
  jdbc:se://host1:9123?"key1"="val1"

  # Setting two quoted values but unquoted keys
  jdbc:se://host1:9123?key1="a=b"&key2="c=d"



.. index::
   single: JDBC driver; authentication

Authentication
--------------

Currently the JDBC driver does not support user authentication because the
RapidsSE server does not implement this yet.


.. index::
   single: JDBC driver; prepared statements

Prepared Statement Support
--------------------------

The RapidsSE JDBC Driver supports creating prepared statements from the JDBC
connection using the `standard Prepared Statement interface
<https://docs.oracle.com/en/java/javase/20/docs/api/java.sql/java/sql/PreparedStatement.html>`_.
For example:

.. code:: java

  String cmd = "SELECT * FROM t WHERE col1 = ?;"
  PreparedStatement ps = connection.prepareStatement(cmd);
  ps.setInt(1, 123);
  ResultSet rs = ps.executeQuery();


The Driver supports all methods of the PreparedStatement interface except for the following:

.. hlist::
   :columns: 2

   * ``getParameterMetaData()``
   * ``setBytes(int, byte[])``
   * ``setBinaryStream(int, InputStream, int)``
   * ``setBinaryStream(int, InputStream)``
   * ``setBinaryStream(int, InputStream, long)``
   * ``setBlob(int, Blob)``
   * ``setBlob(int, InputStream)``
   * ``setBlob(int, InputStream, long)``
   * ``setNCharacterStream(int, Reader)``
   * ``setNCharacterStream(int, Reader, long)``
   * ``setNClob(int, NClob)``
   * ``setNClob(int, Reader)``
   * ``setNClob(int, Reader, long)``
   * ``setNString(int, String)``
   * ``setObject(in, Object, SQLType)``
   * ``setObject(in, Object, SQLType, int)``
   * ``setRowId(int, RowId)``
   * ``setSQLXML(int, SQLXML)``
   * ``setTime(int, Time)``
   * ``setTime(int, Time, Calendar)``
   * ``setUnicodeStream(int, InputStream, int)``
   * ``closeInCompletion()``
   * ``isCloseOnCompletion()``
   * ``getLargeUpdateCount()``
   * ``setLargeMaxRows(long)``
   * ``getLargeMaxRows()``
   * ``executeLargeBatch()``
   * ``executeLargeUpdate(String)``
   * ``executeLargeUpdate(String, int)``
   * ``executeLargeUpdate(String, int[])``
   * ``executeLargeUpdate(String, String[])``


.. index::
   single: JDBC driver; resultsets

ResultSet Support
-----------------

The JDBC driver currently only supports ``ResultSet`` objects that are not
updatable. In addition the ``ResultSet`` can only be iterated in a forward
direction only. This means that the client cannot move the ``ResultSet``
iterator backward to look at a row it previously went past.

By using a forward-only iterating ``ResultSet``, the client will not incur a
large overhead of reading all row data from the server in one go (for a statement
that returns a large number of rows). Instead the client will read a single
buffer of rows (typically up to 1MB in size) and only when the client has
iterated through these rows will the next buffer be read from the socket. Due
to TCP/IP flow control, the server will be blocked from sending additional
buffers of rows to the client until the client is ready to read them.


.. index::
   single: JDBC driver; batched statements

Batched Query and Multiple ResultSets
-------------------------------------

Both RapidsSE and the JDBC driver support the concept of batched queries.
That is, a single command that contain one or more separately logical statements.
When this is sent to RapidsSE, the server will parse the string into individual
statements and then execute then sequentially, returning results from each of
them back to the JDBC driver. This means that a single batched query being
executed may return multiple ResultSets and update counts.

An example of a batched query being executed through a JDBC driver is shown below:

.. code:: java

  String query = "SELECT * FROM t1; SELECT * FROM t2;";

  ResultSet rs = statement.executeQuery(query);

  // Process the first result set
  // ...

  // Get the subsequent result sets
  while (statement.getMoreResults()) {
    rs = statement.getResultSet();

    // Process the subsequent result sets
    // ...
  }


The above example shows a single query being executed that contains two logical
``SELECT`` statements. The two statements are each properly terminated by
semicolons and the statements are concatenated together into a single string that
is then sent to the server to be executed as a single query.

The server will parse this string into two separate commands to be executed. The
first command will be executed and its results will be generated before the second
command is executed. Each of these individual ``SELECT`` statement will return a
different ResultSet. This means that the client needs to handle this using the
methods available through the JDBC specification.

In the above specific example we can see that after the first ResultSet is
returned and processed by the client, the call to ``statement.getMoreResults()``
will initially return ``true`` because a second ResultSet exists. This can then
be retrieved by calling ``statement.getResultSet()``. Please note that as per
the JDBC specification, calling ``statement.getMoreResults()`` will close the
first ResultSet that was retrieved.

.. note::

  Please refer to the JDBC specification and Java documentation on how to correctly
  handle ResultSets, update counts and errors when multiple results are being
  returned.

A batched query does not necessarily need to contain commands that generate
ResultSets. A batched query can contain any valid commands. As per the JDBC
specification, a command that doesn't generate a ResultSet will represent their
result as an update count instead. The client application should use the JDBC
specification with care to determine whether there is a next result and whether
that next result is a ResultSet, an update count or an error.

By default, if an individual command in a batched query causes an error to be
generated in RapidsSE then the remaining commands in that batch will not be
executed. Consider the following batched query:

.. code:: sql

  SELECT * FROM t1; SELECT * FROM badTable; CREATE TABLE t2 (col1 INTEGER);

Initially the first command (``SELECT * FROM t1;``) will be executed and this
will return a ResultSet. Then the second command executes (``SELECT * FROM badTable;``)
but this generates an error because ``badTable`` does not exist. Because of this
the third command in the batch (``CREATE TABLE t2 (col1 INTEGER);``) does not
get executed and the batched query is now ended.

This behaviour can be controlled by setting a JDBC property ``continuebatchonerror``.
Setting this property to ``true`` will cause the JDBC driver to instruct RapidsSE
that it wishes for execution of subsequent commands in a batched query to be
executed after a previous command has generated an error. If this setting was
applied to the above example then the third command (``CREATE TABLE t2 (col1 INTEGER);``)
would have been executed and table ``t2`` would have been created.

This property must be set at the time the JDBC connection is established to the
server. It cannot be set on an individual JDBC Statement object. Refer to
:ref:`this section <jdbcSupportedProperties>` for how to specify the property
when establishing a new JDBC connection.

.. note::

  The JDBC specification includes the methods ``Statement.addBatch(String)`` and
  ``Statement.executeBatch()``. These methods only support commands that do not
  generate a ResultSet. As such, it is preferable and more flexible to specify a
  batched query as a single string (as per the previous example) rather than use
  these JDBC methods.


.. index::
   single: JDBC driver; warnings

Warnings
--------

RapidsSE and the JDBC driver do support the concept of SQL warnings. A warning
can occur at any time of a connection or query, and it may contain pertinent
information that the user may want to know about. It is up to the client
application to check for warnings and to display or handle them appropriately.

The execution of a statement may produce one or more warnings. A client should
call ``statement.getWarnings()`` after executing a statement as well as before that
``statement`` object is closed. This is because RapidsSE does not return all of
the ResultSet data in one hit. Instead it sends additional chunks of rows as
the client iterates the ResultSet. This can mean that SQL warnings that were
generated as later row results were being created may not have been sent to
the client. So it is important to check for warnings both immediately after the
statement has been executed as well as after all rows in the ResultSet have
been iterated over.

Calling ``statement.getWarnings()`` will return the first warning in a chain.
The client should then check for additional warnings in the chain by calling
``warning.getNextWarning()`` until it returns ``null``. This is shown in the
example below:

.. code::java

  public void someFunction() {
    ...
    ResultSet rs = statement.executeQuery("SELECT * FROM t;");
    checkForWarnings();
    ...
    // iterate through all rows in result set
    ...
    checkForWarnings();
  }

  public void checkForWarnings() {
    Warning warning = statement.getWarnings();

    while (warning != null) {
      // handle warning in some way
      ...
      warning = warning.getNextWarning();
    }
  }


.. index::
   single: JDBC driver; data types

.. _jdbcResultSetDataTypes:

Supported ResultSet Data Types
------------------------------

The table below shows the data types that are supported in the JDBC driver
when calling methods in the JDBC class ``ResultSet``.

.. table::
  :name: jdbc-resultset-data-types
  :class: widetable
  :widths: 40, 10, 50

  ========================= =========== ========================================================================
  ResultSet Method          Supported?  Notes
  ========================= =========== ========================================================================
  ``getArray()``            No          Not supported in RapidsSE
  ``getAsciiStream()``      Yes
  ``getBigDecimal()``       Yes
  ``getBinaryStream()``     Yes         Binary types are not supported in RapidsSE
  ``getBlob()``             Yes         Binary types are not supported in RapidsSE
  ``getBoolean()``          Yes
  ``getByte()``             Yes         RapidsSE returns all integers with 64 bit precision
  ``getBytes()``            No          Binary types are not supported in RapidsSE
  ``getCharacterStream()``  Yes
  ``getClob()``             No
  ``getDate()``             Yes
  ``getDouble()``           Yes
  ``getFloat()``            Yes         RapidsSE returns all floating point numbers with double length precision
  ``getInt()``              Yes         RapidsSE returns all integers with 64 bit precision
  ``getLong()``             Yes
  ``getNCharacterStream()`` No          RapidsSE does not support the ``NVARCHAR`` or ``NCHAR`` data types
  ``getNClob()``            No          RapidsSE does not support the ``NVARCHAR`` or ``NCHAR`` data types
  ``getNString()``          No          RapidsSE does not support the ``NVARCHAR`` or ``NCHAR`` data types
  ``getObject()``           Yes
  ``getRef()``              No          RapidsSE does not support the REFs
  ``getRowId()``            No          RapidsSE does not support the RowIds
  ``getShort()``            Yes
  ``getSQLXML()``           No
  ``getString()``           Yes
  ``getTime()``             Yes
  ``getTimestamp()``        Yes         RapidsSE supports timestamps with microsecond precision, however the
                                        JDBC interface only supports precision to the millisecond level.
  ``getUnicodeStream()``    No
  ``getURL()``              No
  ========================= =========== ========================================================================


.. index::
   single: JDBC driver; type conversions

.. _jdbcTypeConversions:

Supported SQL to JDBC Data Type Conversions
-------------------------------------------

JDBC drivers can support automatic type conversions when the client requests a
column value through the ``ResultSet`` class. For example, the client may
execute a query where the data type of the first column returned is a ``TIMESTAMP``,
however the client could then request to get this first column from the ResultSet
as a ``TIME`` value instead. The JDBC driver will then attempt to convert the
``TIMESTAMP`` value to a ``TIME`` value. Not all conversions are supported, or
may be valid. The table below shows the conversions that are supported by the
JDBC driver from a SQL data type when a JDBC ``ResultSet`` method is invoked.

.. raw:: latex

  \begin{landscape}

.. table:: SQL to JDBC Data Type Conversions
   :name: sql-to-jdbc-data-type-conversions
   :class: widetable longtable
   :widths: 20, 5,5,5,5,5,5,5,5,5,5,5, 25

   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | JDBC Method               | ``BOOLEAN`` | ``CHAR`` | ``DATE`` | ``DECIMAL`` | ``FLOAT`` | ``INTEGER`` | ``INTERVAL D-T`` | ``INTERVAL Y-M`` | ``TIME`` | ``TIMESTAMP`` | ``VARCHAR`` |                       Notes                                   |
   +===========================+=============+==========+==========+=============+===========+=============+==================+==================+==========+===============+=============+===============================================================+
   | ``getArray()``            |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getAsciiStream()``      |     Yes     |   Yes    |   Yes    |    Yes      |    Yes    |     Yes     |     Yes          |     Yes          |   Yes    |      Yes      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getBigDecimal()``       |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getBinaryStream()``     |     ---     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getBlob()``             |     ---     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getBoolean()``          |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Numerics: A value of 0 is converted to FALSE.                 |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | Everything else is TRUE.                                      |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             |                                                               |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | ``VARCHAR``: Conversion is supported if the string is a case  |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | insensitive form of "true" or "false".                        |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getByte()``             |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getBytes()``            |     ---     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getCharacterStream()``  |     Yes     |   Yes    |   Yes    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   Yes    |      Yes      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getClob()``             |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getDate()``             |     ---     |   Yes    |   Yes    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      Yes      |     Yes     | Conversion from VARCHAR support if it only contains a valid   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | date string                                                   |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getDouble()``           |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getFloat()``            |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getInt()``              |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getLong()``             |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getNCharacterStream()`` |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getNClob()``            |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getNString()``          |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getObject()``           |     Yes     |   Yes    |   Yes    |    Yes      |    Yes    |     Yes     |     Yes          |     Yes          |   Yes    |      Yes      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getRef()``              |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getRowId()``            |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getShort()``            |     Yes     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     | Conversion from a ``CHAR``/``VARCHAR`` supported only if it   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | contains a numeric value                                      |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getSQLXML()``           |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getString()``           |     Yes     |   Yes    |   Yes    |    Yes      |    Yes    |     Yes     |     Yes          |     Yes          |   Yes    |      Yes      |     Yes     |                                                               |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getTime()``             |     ---     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   Yes    |      Yes      |     Yes     | Conversion from VARCHAR support if it only contains a valid   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | time string                                                   |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getTimestamp()``        |     ---     |   Yes    |   Yes    |    ---      |    ---    |     ---     |     ---          |     ---          |   Yes    |      Yes      |     Yes     | Conversion from VARCHAR support if it only contains a valid   |
   |                           |             |          |          |             |           |             |                  |                  |          |               |             | timestamp string                                              |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getURL()``              |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+
   | ``getUnicode()``          |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     | Data type not supported                                       |
   +---------------------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+---------------------------------------------------------------+


.. raw:: latex

  \end{landscape}



.. index::
   single: JDBC driver; unsupported features

Unsupported JDBC Features
-------------------------

The following features are not supported in the RapidsSE JDBC Driver:

.. table::
  :name: jdbc-unsupported-features
  :class: widetable longtable
  :widths: 10, 20, 20, 50

  === ================= ================= =============================================================
  Num Interface         Topic             Description
  === ================= ================= =============================================================
  1   General           Error Codes       Exceptions thrown by the JDBC driver may not have SQLState
                                          error codes.
  2   Connection        Auto-commit       Non-auto-commit transactions are not supported since RapidsSE
                        transactions      does not support them.
  3   Connection        Callable          Callable statements are not supported since RapidsSE
                        Statements        does not support them.
  4   Connection        Rollbacks         Transaction rollbacks are not supported since RapidsSE does
                                          not support non-auto-commit transactions.
  5   Connection        Savepoints        Transaction savepoints are not supported since RapidsSE does
                                          not support them.
  6   Connection        Network           Setting a network timeout with
                                          ``connection.setNetworkTimeout()`` is not yet supported.
                        Timeouts
  7   Connection        Connection        Connection pooling within the JDBC driver is not supported.
                        pooling
  8   Statement         closeOnCompletion ``Statement.setCloseOnCompletion()`` is not supported.
  9   Statement         Pooling           Statement pooling via ``statement.setPoolable()`` is not
                        statements        supported.
  10  Statement         Auto-generated    Execution and retrieval of auto-generated keys is not
                        keys              supported (e.g., via
                                          ``statement.execute(String sql, String[] columns)``, since
                                          RapidsSE does not yet support retrieval of auto-generated
                                          keys.
  11  Statement         Query Timeout     Setting a query timeout via Statement.setQueryTimeout() is
                                          not yet supported.
  12  Statement         Escape Processing Escape processing within the JDBC driver is generally not
                                          supported.
  13  Statement         Fetch Direction   The JDBC driver only supports a forward fetch direction of
                                          ResultSets.
  14  Statement         Fetch Size        The fetch size set for a JDBC statement is not currently
                                          used by the driver.
  15  Statement         Max Field Size    The driver currently does not adhere to any maximum field
                                          size set via ``Statement.setMaxFieldSize()``.
  16  ResultSet         ResultSet         ResultSets only support read-only operations and are not
                        Updatability      updatable. Operations such as ``insertRow()``,
                                          ``deleteRow()``, ``updateRow()`` and ``update<datatype>()``
                                          related operations are not supported.
  17  ResultSet         ResultSet         By default, ResultSets are kept open over commits. They
                        Holdability       do not support "close at commit" semantics since RapidsSE
                                          does not support this.
  18  ResultSet         ResultSet Type    ResultSets currently only support forward-only iterating,
                                          even though ``Statement.getResultSetType()`` returns
                                          ``TYPE_SCROLL_INSENSITIVE``. This is because this type of
                                          ResultSet is required for JMeter, however ResultSets
                                          do not yet support backwards scrolling, or absolute or
                                          relative cursor positioning via methods like:

                                          - ``absolute()``

                                          - ``afterLast()``

                                          - ``beforeFirst()``

                                          - ``relative()``

                                          - ``first()``

                                          - ``last()``

                                          - ``previous()``
  19  ResultSet         ResultSet         The JDBC driver only supports ResultSets that are insensitive
                        Sensitivity       to database changes. As such, related operations such as
                                          ``refreshRow()`` are not supported.
  20  ResultSet         Data Types        The JDBC driver does not support all possible data types
                                          defined in the ResultSet interface definition. Refer to
                                          `this section <jdbcResultSetDataTypes>` for supported and
                                          unsupported data types.
  21  ResultSet         Data Type         The JDBC driver does not support all possible conversions
                        Conversions       from SQL data types to Java data types. Refer to `this
                                          section <jdbcTypeConversions>` for supported and unsupported
                                          data type conversions.
  22  DatabaseMetaData  Unsupported       The following methods are not supported:
                        Methods
                                          -	``getBestRowIdentifier()``

                                          - ``getColumnPrivileges()``

                                          - ``getCrossReference()``

                                          - ``getExportKeys()``

                                          - ``getFunctionColumns()``

                                          - ``getFunctions()``

                                          - ``getImportedKeys()``

                                          - ``getIndexInfo()``

                                          - ``getProcedureColumns()``

                                          - ``getProcedures()``

                                          - ``getSQLKeywords()``


  23  DatabaseMetaData  Methods Returning The following methods are not fully implemented and return
                        Empty Results or  empty ResultSets:
                        Empty ResultSet
                                          - ``getPrimaryKeys()``

                                          - ``getStringFunctions()``

                                          - ``getSuperTables()``

                                          - ``getSuperTypes()``

                                          - ``getSystemFunctions()``

                                          - ``getTablePrivileges()``

                                          - ``getTimeDateFunctions()``

                                          - ``getUDTs()``

                                          - ``getVersionColumns()``

  24  ResultSetMetaData Column Name And   RapidsSE and the JDBC driver currently do not distinguish
                        Label             between column names and column labels. If an alias is given
                                          for a column then that alias will be used as both the column
                                          name and the column label.
  25  ResultSetMetaData Originating       RapidsSE and the JDBC driver currently do not provide the
                        Table, Schema And catalog name, schema name or table name that each column of
                        Catalog           a ResultSet came from. Instead, an empty string is returned.

  === ================= ================= =============================================================




.. index::
   single: JDBC driver; debugging applications

Debugging JDBC Applications
---------------------------

The JDBC driver does not contain a logging framework, as this would impose a
constraint on any client that uses it. Instead, the driver uses the SLF4J
logging API, which has bindings with many popular logging frameworks. So it is
up to the client application that is using the JDBC driver to provide a logging
framework that can bind to SLF4J. Failure to do so will result in some warning
messages appearing on the console about the lack of a logging framework being
supplied:

.. code::

  SLF4J: Failed to load class "org.slf4j.impl.StaticLoggerBinder".
  SLF4J: Defaulting to no-operation (NOP) logger implementation
  SLF4J: See http://www.slf4j.org/codes.html#StaticLoggerBinder for further details.

These messages will not appear if a client application provides a jar file for
binding SLF4J to Log4J (for example), as well as a jar file for the implementation
of Log4J. This enables all of the logging that occurs within the JDBC driver to be
directed through the Log4J framework.

Once a logging framework is in place, the driver can be debugged by setting the log
level to ``TRACE`` or ``DEBUG`` (where ``TRACE`` is an even more verbose level than
``DEBUG``) for all the classes in the ``com.rapidsdata.jdbcdriver`` namespace. How
this is done will vary greatly with the logging framework used, as well as the user's
preferred way of configuring the logging framework.

As one possible example, if the user is using Log4J then the JDBC driver can be
debugged by providing a file, ``log4j.properties``, on the application's classpath
with the following contents:

.. code:: properties

  # By default, log INFO or higher to console
  log4j.rootLogger=INFO,consoleLogger

  # Output ALL logging messages from the RapidsDB JDBC driver
  log4j.logger.com.rapidsdata.jdbcdriver=TRACE

  # A logger for sending messages to the console
  log4j.appender.consoleLogger=org.apache.log4j.ConsoleAppender
  log4j.appender.consoleLogger.layout.ConversionPattern=%d{ISO8601} [%-22c{2}] %-5p: %m%n
  log4j.appender.consoleLogger.layout=org.apache.log4j.EnhancedPatternLayout



.. index::
   single: JDBC driver; sample application

Sample Application
------------------

Below is a simple Java application that uses the RapidsSE JDBC driver to query
a table that already exists in the system. It shows how a query that generates
a ResultSet can be executed and how that ResultSet can be iterated.

.. code:: java

  // Copyright (c) 2019 Boray Data Co. Ltd.  All rights reserved.

  // Instructions:
  // * Make the directories [some_dir]/com/rapidsdata
  // * Copy this content into a file at [some_dir]/com/rapidsdata/JdbcSample.java
  //
  // From [some_dir]:
  // To compile:  javac com/rapidsdata/JdbcSample.java
  // To execute:  java -cp .:/path/to/rapidsse-jdbc-<version>.jar JdbcSample
  //
  // This example assumes an table exists called T with
  // columns (A VARCHAR(128), B INTEGER) and that table contains
  // some rows. It also assumes that the JDBC driver can connect to a
  // RapidsSE node listening on localhost:9123.
  //
  // The app will read all rows in the table T and then print them
  // to the screen.
  //
  //
  // To avoid a warning from SLF4J about logging defaulting to a no-operation,
  // simply add a compatible logging framework to the class path when running
  // the sample (e.g. any one of slf4j-nop.jar, slf4j-simple.jar, slf4j-log4j12.jar,
  // slf4j-jdk14.jar or logback-classic.jar).


  package com.rapidsdata;

  import java.sql.*;

  public class JdbcSample
  {
    public static boolean doSample()
    {
      boolean bRet = false;
      String url = "jdbc:se://localhost:9123";
      String userName = null;
      String password = null;
      Connection conn = null;
      Statement stmt = null;
      try {
        // Connect to Rapids
        conn = DriverManager.getConnection(url, userName, password);

        // Get a Statement object from the connection
        stmt = conn.createStatement();

        // Execute an SQL statement "Select...".
        ResultSet rs = stmt.executeQuery("SELECT A, B FROM T;");

        ResultSetMetaData metaData = rs.getMetaData();
        int numberOfColumns = metaData.getColumnCount();

        while (rs.next()) {
          for (int i = 1; i <= numberOfColumns; i++) {
            System.out.print(String.format("%s=", metaData.getColumnName(i)));

            switch(metaData.getColumnType(i)) {
              case java.sql.Types.VARCHAR:
                System.out.print(String.format("%s, ", rs.getString(i)));
                break;

              case java.sql.Types.TINYINT:
              case java.sql.Types.SMALLINT:
              case java.sql.Types.INTEGER:
              case java.sql.Types.BIGINT:
                System.out.print(String.format("%d, ", rs.getLong(i)));
                break;

              default:
                System.err.print(String.format("Unhandled type: %d",
                                               metaData.getColumnType(i)));
            }
          } // end for-loop

          System.out.println(" ");

        } // end while-loop

        rs.close();
        stmt.close();
        conn.close();
        bRet = true;

      } catch (SQLException se) {
        System.out.println(se.toString());
      }
      return bRet;
    }

    public static void main(String[] args)
    {
      doSample();
    }
  }


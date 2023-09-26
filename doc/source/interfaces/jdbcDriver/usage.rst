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



Authentication
--------------

Currently the JDBC driver does not support user authentication because the
RapidsSE server does not implement this yet.


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



ResultSet Support
-----------------

The JDBC driver currently only supports ``ResultSet`` objects that are not
updatable. In addition the ``ResultSet`` can only be iterated in a forward
direction only. This means that the client cannot move the ``ResultSet``
iterator backward to look at a row it previously went past.


Batched Statements and Multiple ResultSets
------------------------------------------

Describe batched statements in a single string as well as Statement.addBatch()
and statement.executeBatch().

Warnings
--------


.. _jdbcResultSetDataTypes:

Supported ResultSet Data Types
------------------------------

.. _jdbcTypeConversions:

Supported SQL to Java Data Type Conversions
-------------------------------------------

Debugging JDBC Applications
---------------------------

Unsupported JDBC Features
-------------------------

The following features are not supported in the RapidsSE JDBC Driver:

.. table::
  :name: jdbc-unsupported-features
  :widths: 5, 20, 20, 60

  === ================= ================= =============================================================
  Num Interface         Topic             Description
  === ================= ================= =============================================================
  1.  General           Error Codes       Exceptions thrown by the JDBC driver may not have SQLState
                                          error codes.
  2.  Connection        Auto-commit       Non-auto-commit transactions are not supported since RapidsSE
                        transactions      does not support them.
  3.  Connection        Callable          Callable statements are not supported since RapidsSE
                        Statements        does not support them.
  4.  Connection        Rollbacks         Transaction rollbacks are not supported since RapidsSE does
                                          not support non-auto-commit transactions.
  5.  Connection        Savepoints        Transaction savepoints are not supported since RapidsSE does
                                          not support them.
  6.  Connection        Network           Setting a network timeout with
                                          ``connection.setNetworkTimeout()`` is not yet supported.
                        Timeouts
  7.  Connection        Connection        Connection pooling within the JDBC driver is not supported.
                        pooling
  8.  Statement         closeOnCompletion ``Statement.setCloseOnCompletion()`` is not supported.
  9.  Statement         Pooling           Statement pooling via ``statement.setPoolable()`` is not
                        statements        supported.
  10. Statement         Auto-generated    Execution and retrieval of auto-generated keys is not
                        keys              supported (e.g., via
                                          ``statement.execute(String sql, String[] columns)``, since
                                          RapidsSE does not yet support retrieval of auto-generated
                                          keys.
  11. Statement         Query Timeout     Setting a query timeout via Statement.setQueryTimeout() is
                                          not yet supported.
  12. Statement         Escape Processing Escape processing within the JDBC driver is generally not
                                          supported.
  13. Statement         Fetch Direction   The JDBC driver only supports a forward fetch direction of
                                          ResultSets.
  14. Statement         Fetch Size        The fetch size set for a JDBC statement is not currently
                                          used by the driver.
  15. Statement         Max Field Size    The driver currently does not adhere to any maximum field
                                          size set via ``Statement.setMaxFieldSize()``.
  16. ResultSet         ResultSet         ResultSets only support read-only operations and are not
                        Updatability      updatable. Operations such as ``insertRow()``,
                                          ``deleteRow()``, ``updateRow()`` and ``update<datatype>()``
                                          related operations are not supported.
  17. ResultSet         ResultSet         By default, ResultSets are kept open over commits. They
                        Holdability       do not support "close at commit" semantics since RapidsSE
                                          does not support this.
  18. ResultSet         ResultSet Type    ResultSets currently only support forward-only iterating,
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
  19. ResultSet         ResultSet         The JDBC driver only supports ResultSets that are insensitive
                        Sensitivity       to database changes. As such, related operations such as
                                          ``refreshRow()`` are not supported.
  20. ResultSet         Data Types        The JDBC driver does not support all possible data types
                                          defined in the ResultSet interface definition. Refer to
                                          `this section <jdbcResultSetDataTypes>` for supported and
                                          unsupported data types.
  21. ResultSet         Data Type         The JDBC driver does not support all possible conversions
                        Conversions       from SQL data types to Java data types. Refer to `this
                                          section <jdbcTypeConversions>` for supported and unsupported
                                          data type conversions.
  22. DatabaseMetaData  Unsupported       The following methods are not supported:
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


  23. DatabaseMetaData  Methods Returning The following methods are not fully implemented and return
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

  24. ResultSetMetaData Column Name And   RapidsSE and the JDBC driver currently do not distinguish
                        Label             between column names and column labels. If an alias is given
                                          for a column then that alias will be used as both the column
                                          name and the column label.
  25. ResultSetMetaData Originating       RapidsSE and the JDBC driver currently do not provide the
                        Table, Schema And catalog name, schema name or table name that each column of
                        Catalog           a ResultSet came from. Instead, an empty string is returned.

  === ================= ================= =============================================================

Sample Application
------------------



.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. index::
   single: SE Shell; usage

.. _seShellUsage:

Using SE Shell
==============

.. index::
   single: SE Shell; starting

Starting
--------

SE Shell is started up by running the ``seshell.sh`` script file on a Linux
platform or the ``seshell.bat`` file on a Windows platform. If you are running
SE Shell from the installation directory of one of the RapidsSE nodes, then the
``seshell.sh`` file can be found in the root of the installation directory
(typically ``/opt/rapidsse``).

Running this script file will, by default, attempt to connect to a RapidsSE
node running on the localhost and listening on the default port (``9123``).
An error will be displayed if SE Shell cannot connect to this RapidsSE node.

SE Shell will also accept the following command line arguments:

.. list-table:: SE Shell Command Line Arguments
  :widths: 20, 15, 65
  :header-rows: 1

  * - Option
    - Default
    - Description
  * - ``-h <host/ip_address>``
    - ``localhost``
    - The hostname or IP address of the node in the RapidsSE cluster that
      SE Shell should connect to.
  * - ``-p <port_number>``
    - ``9123``
    - The port number of the node in the RapidsSE cluster that SE Shell
      should connect to.
  * - ``-s``
    -
    - Runs SE Shell in :ref:`scripting mode <seShellScriptingMode>`
  * - ``-t``
    -
    - Specifies that when a ResultSet is being displayed the data types for
      each column should be printed on the next line after printing the column headings


.. rubric:: Examples

.. code:: bash

  cd /opt/rapidsse

  # RapidsSE running on localhost:9123
  ./seshell.sh

  # RapidsSE running on a host resolvable as "node1" with port 9123
  ./seshell.sh -h node1

  # RapidsSE running on a host resolvable as "node1" with port 12345
  ./seshell.sh -h node1 -p 12345


.. index::
   single: SE Shell; stopping

Stopping
--------

Stopping SE Shell after it has been started is as simple as one of the
following options:

- Pressing ``<Control>-C``

- Entering the command ``exit;``, ``quit;``, or ``bye;`` and then pressing ``<Enter>``.

SE Shell will exit out back to the command line.



.. index::
   single: SE Shell; entering commands

Entering Commands
-----------------

After starting up SE Shell, a prompt will be displayed that indicates that
SE Shell is ready to accept commands. The prompt will look like this:

.. code::

  rapids >

Commands can be executed by entering them at the prompt and terminating the
command with a semicolon (``;``) before pressing enter.
Commands that are not terminated with a semicolon are assumed to continue on
the next line, until a semicolon and enter key combination is detected.

SE Shell accepts commands being split over multiple lines. This can make it
easier to type and edit long commands. SE Shell will submit the command to RapidsSE
as it is written, with any line breaks in the middle of the command.

.. index::
   single: SE Shell; history recall

.. rubric:: Command History, Recall & Editing

SE Shell supports line-editing and recall of commands. Pressing the up arrow key will
cycle through and recall the past history of commands. Pressing the down arrow
will cycle through the history in the opposite direction. This makes it easier to
re-execute previous commands rather than re-typing them. SE Shell will only
remember commands from the current session. Once the user has exited out of
SE Shell the history will be forgotten.

Please note that while SE Shell allows for commands to span multiple lines,
any recalled commands can only be displayed on a single line. For this reason,
SE Shell will display any line breaks in the command as the single printable character
``^J``. This is purely for display and internal limitation reasons. If the
multi-line command is submitted to RapidsSE the command will contain the
correct line breaks.

.. rubric:: Comments

SE Shell supports the following comment markers in commands:

- ``//`` for C++ style line comments
- ``--`` for line comments
- ``/*`` and ``*/`` for C style block comments


Cancelling Queries
------------------

SE Shell does not yet support cancelling queries that are currently running
because RapidsSE does not support this just yet.


.. index::
   single: SE Shell; built-in commands

Built-in Commands
-----------------

SE Shell has a few commands built in that it will recognise and execute without
passing the command off to the server. These built-in commands typically control
the execution of the query or how the results are displayed. The commands are
typed like existing SQL commands and are terminated with a semicolon (``;``).
Commands that aren't recognised are treated as SQL commands and are sent to
the server for execution.

These internal commands are described in the following sub-sections.

.. figure:: rr/seShellCommands.*
   :align: center

   The built-in SE Shell commands


The Query Repeat Command
````````````````````````

The query repeat command automatically re-executes the specified query on the
server a given number of times. It does not wait any time between receiving the
last row of the previous result and re-submitting the query for execution again.

.. image:: rr/repeatCommand.*
   :align: center

This command is specified by writing a positive number immediately before the
command to be executed. When a result is returned that contains rows, instead of
printing each row SE Shell will simply print the number of rows that were returned
each time and how long each iteration took. An example is shown below.

.. rubric:: Example:

.. code::

  rapids > 3 SELECT * FROM lineitem;
  6,000 row(s) returned (0.57 sec)
  6,000 row(s) returned (1.06 sec)
  6,000 row(s) returned (1.55 sec)


The RUN Command
```````````````

.. image:: rr/runCommand.*
   :align: center

This command will execute the commands stored in the specified local file that
is local to the SE Shell (not the server). SE Shell will read the contents of
the local file and execute the commands that it contains, as if they were typed
into the SE Shell manually.

If not given as an absolute path, the path of the file will be relative to the
location where SE Shell is being executed from.

.. rubric:: Example:

.. code::

  rapids > RUN sample.run;
  Executing file sample.run...
  rapids > CREATE TABLE nation ( n_nationkey INTEGER NOT NULL, n_name CHAR(25) NOT NULL, n_regionkey INTEGER NOT NULL, n_comment VARCHAR(152) ) REPLICATED ;
  0 row(s) affected (0.03 sec)
  rapids > LOAD nation "../TPCH/SF_0001/nation.csv";
  25 row(s) affected (0.02 sec)
  rapids > SELECT COUNT(*) FROM nation;

    COL1
    ----
      25

  1 row(s) returned (0.46 sec)


The SET PAGE Command
````````````````````
.. image:: rr/setPageCommand.*
   :align: center

This command sets the number of rows that make up a 'page' when  SE Shell
formats query results. Each page will start with the column headings for the
result set, and then the column headings will be repeated after <page size>
rows have been displayed. The default page size is ``25``. To disable paging
and just have display the column headings once use ``-1`` for the page size.

.. rubric:: Example:

.. code::

  rapids > SELECT l_orderkey, l_linenumber, l_shipinstruct FROM lineitem LIMIT 10;

    l_orderkey   l_linenumber l_shipinstruct
    ----------   ------------ --------------
             3              3 TAKE BACK RETURN
             4              4 NONE
             6              6 DELIVER IN PERSON
             7              1 TAKE BACK RETURN
            29              4 NONE
            34              3 TAKE BACK RETURN
            41              3 NONE
            45              1 COLLECT COD
            50              1 COLLECT COD
            67              6 DELIVER IN PERSON

  10 row(s) returned (0.49 sec)
  rapids >
  rapids > SET PAGE 3;
  rapids >
  rapids > SELECT l_orderkey, l_linenumber, l_shipinstruct FROM lineitem LIMIT 10;

    l_orderkey   l_linenumber l_shipinstruct
    ----------   ------------ --------------
             2              2 TAKE BACK RETURN
             8              1 NONE
             9              2 TAKE BACK RETURN

    l_orderkey   l_linenumber l_shipinstruct
    ----------   ------------ --------------
            10              3 DELIVER IN PERSON
            18              1 TAKE BACK RETURN
            22              4 NONE

    l_orderkey   l_linenumber l_shipinstruct
    ----------   ------------ --------------
            23              5 DELIVER IN PERSON
            24              6 TAKE BACK RETURN
            26              1 TAKE BACK RETURN

    l_orderkey   l_linenumber l_shipinstruct
    ----------   ------------ --------------
            30              5 DELIVER IN PERSON

  10 row(s) returned (0.49 sec)


The SET SEPARATOR Command
`````````````````````````

.. image:: rr/setSeparatorCommand.*
   :align: center

This command can be used when SE Shell is executed in
:ref:`scripting mode <seShellScriptingMode>`. By default, SE Shell will write
out results in a comma-separated format. This command can be used to change
the separator that is used. This command has no effect when SE Shell is not
started in scripting mode.

.. rubric:: Example:

.. code::

  craig@machine$ ./seshell.sh -s

  select l_orderkey, l_linenumber, l_shipinstruct from lineitem limit 10;
  "l_orderkey","l_linenumber","l_shipinstruct"
  3,3,"TAKE BACK RETURN"
  4,4,"NONE"
  6,6,"DELIVER IN PERSON"
  7,1,"TAKE BACK RETURN"
  29,4,"NONE"
  34,3,"TAKE BACK RETURN"
  41,3,"NONE"
  45,1,"COLLECT COD"
  50,1,"COLLECT COD"
  67,6,"DELIVER IN PERSON"

  set separator pipe;

  select l_orderkey, l_linenumber, l_shipinstruct from lineitem limit 10;
  l_orderkey|l_linenumber|l_shipinstruct
  2|2|TAKE BACK RETURN
  8|1|NONE
  9|2|TAKE BACK RETURN
  10|3|DELIVER IN PERSON
  18|1|TAKE BACK RETURN
  22|4|NONE
  23|5|DELIVER IN PERSON
  24|6|TAKE BACK RETURN
  26|1|TAKE BACK RETURN
  30|5|DELIVER IN PERSON


The SET PRECISION Command
`````````````````````````

.. image:: rr/setPrecisionCommand.*
   :align: center

This command changes the precision that floating point numbers are displayed
with. That is, the number of digits that are displayed. The default precision
is ``11`` digits.

.. rubric:: Example:

.. code::

  rapids > SELECT AVG(l_extendedprice)/7.1e0 FROM lineitem;

                COL1
                ----
    5.3542154930e+03

  1 row(s) returned (0.55 sec)
  rapids > set precision 3;
  rapids > SELECT AVG(l_extendedprice)/7.1e0 FROM lineitem;

        COL1
        ----
    5.35e+03

  1 row(s) returned (0.54 sec)


The EXIT Command
````````````````

.. image:: rr/exitCommand.*
   :align: center

This command exits the SE Shell.

.. rubric:: Example:

.. code::

  rapids > bye;
  craig@machine$



.. index::
   single: SE Shell; scripting mode

.. _seShellScriptingMode:

Scripting Mode
--------------





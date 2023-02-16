.. _commandList:

Alphabetical Command List
=========================

.. contents:: Contents of this section:
   :depth: 1
   :backlinks: none


RapidsSE has a number of internal commands that can be used to inspect and
modify the system. These commands and their syntax are detailed below in
alphabetical order.

All RapidsSE commands are terminated with a semicolon, like this:

.. image:: /rr/diagram/statement.png
   :alt: All commands are terminated with a semicolon
   :align: center

In this command reference, literal values are represented in uppercase with a
yellow background with rounded corners. The definition of a boolean literal is
an example here of a rule that contains two literal values.

.. image:: /rr/diagram/booleanLiteral.png
   :alt: A rule that contains one of two literal values
   :align: center

Sub-rules are represented in lowercase with a blue background and square
corners. The meaning of this rule is defined elsewhere.

Finally, some elements of a statement are expressed as a grammar, typically to
represent character or number ranges. These are shown in lowercase with a
salmon coloured background in a hexagonal shaped box. The content of these
boxes are usually a
`regular expression <https://en.wikipedia.org/wiki/Regular_expression>`_.
Here is an example of a rule that defines a digit, using a regular expression
node.

.. image:: /rr/diagram/digit.png
   :alt: A rule that contains a regular expression node
   :align: center

.. note::
   The :ref:`complete syntax tree <syntaxTree>` contains the full syntax for
   all statements. Click on any of the syntax diagrams below to see them in
   context in the full syntax tree.


.. _countCmd:

``COUNT``
---------
Returns the number of rows in a given table across all nodes.

.. rubric:: Syntax:

.. image:: /rr/diagram/countStmt.png
   :alt: ``COUNT`` statement syntax
   :align: center
   :target: syntax-tree.html#countStmt


.. _createIndexCmd:

``CREATE INDEX``
----------------
Creates an index on a table.

.. seealso::

  :ref:`indexes`

.. rubric:: *CREATE INDEX* Syntax:

.. image:: /rr/diagram/createIndexStmt.png
   :alt: ``CREATE INDEX`` statement syntax
   :align: center
   :target: syntax-tree.html#createIndexStmt

.. rubric:: *identifierList* definition:

.. image:: /rr/diagram/identifierList.png
   :alt: *identifierList* definition
   :align: center
   :target: syntax-tree.html#identifierList


.. _createTableCmd:

``CREATE TABLE``
----------------
Creates a table in the default catalog and schema (``rapidsse.public``).

If the ``PARTITION BY`` clause is not supplied then the table will
automatically be partitioned across the cluster by the first column in the
table.

.. seealso::

  :ref:`tables`

  :ref:`tablePartitioning`

  :ref:`creatingTables`

.. rubric:: Syntax:

.. image:: /rr/diagram/createTableStmt.png
   :alt: ``CREATE TABLE`` statement syntax
   :align: center
   :target: syntax-tree.html#createTableStmt

.. rubric:: *columnDefinition* definition:

.. image:: /rr/diagram/columnDefinition.png
   :alt: *columnDefinition* definition
   :align: center
   :target: syntax-tree.html#columnDefinition

.. rubric:: *dataType* definition:

.. image:: ../rr/diagram/dataType.png
   :alt: *dataType* definition
   :align: center
   :target: syntax-tree.html#dataType

.. rubric:: *identifierList* definition:

.. image:: /rr/diagram/identifierList.png
   :alt: *identifierList* definition
   :align: center
   :target: syntax-tree.html#identifierList


.. _dloadCmd:

``DLOAD``
---------
Distributed load. Loads data from a given file (or files) on all nodes in the
cluster into a table. Does not return until all files have been loaded. **The
paths/files specified must exist in the same location on each node.**

A distributed load is useful when the data you wish to load is already chunked
and stored on separate nodes. In that case the processing power of the whole
cluster can be used to read and parse the data rather than it all being done
by a single machine. The data read by each node will still partitioned and
sent across the network to the correct node that holds that partition.


.. seealso::

  :ref:`loadingData`

  :ref:`singleNodeLoad`

  :ref:`distributedLoad`

.. rubric:: *DLOAD* Syntax:

.. image:: /rr/diagram/dloadStmt.png
   :alt: ``DLOAD`` statement syntax
   :align: center
   :target: syntax-tree.html#dloadStmt

Refer to the :ref:`loadCmd` command for the valid values of *<loadOption>*
and *<loadValue>*.

Also refer to the :ref:`loadCmd` command for loading data from a single node in
the cluster only.

.. rubric:: Example:

To load ``/mnt/data/file1.csv`` and ``/mnt/data/file2.csv`` from each node in
the cluster into table ``table1``:

.. code:: sql

  dload table1 paths "/mnt/data/file1.csv" "/mnt/data/file2.csv" ;



.. _dropIndexCmd:

``DROP INDEX``
----------------
Drops an index of a given name from a table.

.. seealso::

  :ref:`indexes`

.. rubric:: *DROP INDEX* Syntax:

.. image:: /rr/diagram/dropIndexStmt.png
   :alt: ``DROP INDEX`` statement syntax
   :align: center
   :target: syntax-tree.html#dropIndexStmt



.. _dropTableCmd:

``DROP TABLE``
----------------
Removes an existing table and all of its indexes and data.

.. seealso::

  :ref:`tables`

.. rubric:: *DROP TABLE* Syntax:

.. image:: /rr/diagram/dropTableStmt.png
   :alt: ``DROP TABLE`` statement syntax
   :align: center
   :target: syntax-tree.html#dropTableStmt



.. _loadCmd:

``LOAD``
--------
Load data into a table from a file on the current node only.


.. seealso::

  :ref:`loadingData`

  :ref:`singleNodeLoad`

  :ref:`distributedLoad`

.. rubric:: *LOAD* Syntax:

.. image:: /rr/diagram/loadStmt.png
   :alt: ``LOAD`` statement syntax
   :align: center
   :target: syntax-tree.html#loadStmt

Options are specified in angle brackets and their values are surrounded by
double quotes.

In the above syntax, the value of *loadOption* can be any of the following:

.. list-table:: Load options
   :widths: 25 15 60
   :header-rows: 1

   * - Option
     - Default Value
     - Description
   * - ``<field_delimiter>``
     - ``,`` or ``|``
     - The character that separates fields in a row. RapidsSE will automatically decide
       between comma and pipe characters based on the data if the ``<field_delimiter>``
       option is not specified.
   * - ``<escape_char>``
     - ``\``
     - The character used in a string field to escape the meaning of the subsequent
       character. For example, if a comma was used as the field_separator then a comma
       could be included in a string field if it is prepended with the escape character
       before it.

       Note: Specifying the backslash as the escape character would require itself be
       escaped so it is interpreted correctly. For example ``<escape_char> = "\\"``
   * - ``<date_format>``
     - ``YYYY-MM-DD``
     - The format to be used when dates are being interpreted in the data.


.. rubric:: Example:

(1) To use forward slashes in dates instead of dashes:

.. code:: sql

  load table1
  "/mnt/data/userdata.csv"
  <date_format> = "YYYY/MM/DD" ;

(2)	To explicitly define the pipe symbol as the field delimiter,
and use the ``#`` symbol as an escape character:

.. code:: sql

  load table1
  "/mnt/data/userdata.csv"
  <field_delimiter> = "|"
  <escape_char> = "#" ;



.. _lscolCmd:

``LSCOL``
---------
List and describe all of the columns that exist for a given table.

.. rubric:: Syntax:

.. image:: /rr/diagram/lsColStmt.png
   :alt: ``LSCOL`` statement syntax
   :align: center
   :target: syntax-tree.html#lsColStmt



.. _lsidxCmd:

``LSIDX``
---------
Lists all of the indexes that have been created on a given table.

.. rubric:: Syntax:

.. image:: /rr/diagram/lsIdxStmt.png
   :alt: ``LSIDX`` statement syntax
   :align: center
   :target: syntax-tree.html#lsIdxStmt



.. _lsidxcolCmd:

``LSIDXCOL``
------------
Lists the columns of a given index on a particular table.

.. rubric:: Syntax:

.. image:: /rr/diagram/lsIdxColStmt.png
   :alt: ``LSIDXCOL`` statement syntax
   :align: center
   :target: syntax-tree.html#lsIdxColStmt



.. _lstblCmd:

``LSTBL``
---------
Lists all of the tables in the default catalog and schema.

.. rubric:: Syntax:

.. image:: /rr/diagram/lsTblStmt.png
   :alt: ``LSTBL`` statement syntax
   :align: center
   :target: syntax-tree.html#lsTblStmt




.. _printCmd:

``PRINT``
---------
Prints a string to stdout on the SE Prompt.

.. rubric:: Syntax:

.. image:: /rr/diagram/printStmt.png
   :alt: ``PRINT`` statement syntax
   :align: center
   :target: syntax-tree.html#printStmt




.. _selectCmd:

``SELECT``
----------
Reads data from a table and returns a result set.

This is broken down into the :ref:`command <selectCmdRef>` and the
:ref:`expression <selectExprRef>` reference as well as a summary of
:ref:`supported operators <supportedSelectOperators>` and
:ref:`supported expressions <supportedSelectExprs>`.

.. seealso::

  :ref:`queries`

  :ref:`expressions`


.. _selectCmdRef:

``SELECT`` Command
``````````````````

.. rubric:: Syntax:

.. image:: /rr/diagram/selectQuery.png
   :alt: ``SELECT`` statement syntax
   :align: center
   :target: syntax-tree.html#selectQuery


.. rubric:: *setExpr* definition:

.. image:: /rr/diagram/setExpr.png
   :alt: *setExpr* statement syntax
   :align: center
   :target: syntax-tree.html#setExpr


.. rubric:: *selectExpr* definition:

.. image:: /rr/diagram/selectExpr.png
   :alt: *selectExpr* statement syntax
   :align: center
   :target: syntax-tree.html#selectExpr


.. rubric:: *queryHint* definition:

.. image:: /rr/diagram/queryHint.png
   :alt: *queryHint* statement syntax
   :align: center
   :target: syntax-tree.html#queryHint


.. rubric:: *hint* definition:

.. image:: /rr/diagram/hint.png
   :alt: *hint* statement syntax
   :align: center
   :target: syntax-tree.html#hint


.. rubric:: *corrClause* definition:

.. image:: /rr/diagram/corrClause.png
   :alt: *corrClause* statement syntax
   :align: center
   :target: syntax-tree.html#corrClause


.. rubric:: *withQuery* definition:

.. image:: /rr/diagram/withQuery.png
   :alt: *withQuery* statement syntax
   :align: center
   :target: syntax-tree.html#withQuery


.. rubric:: *tableExpr* definition:

.. image:: /rr/diagram/tableExpr.png
   :alt: *tableExpr* statement syntax
   :align: center
   :target: syntax-tree.html#tableExpr


.. rubric:: *tableReference* definition:

.. image:: /rr/diagram/tableReference.png
   :alt: *tableReference* statement syntax
   :align: center
   :target: syntax-tree.html#tableReference


.. rubric:: *joinType* definition:

.. image:: /rr/diagram/joinType.png
   :alt: *joinType* statement syntax
   :align: center
   :target: syntax-tree.html#joinType


.. rubric:: *selectList* definition:

.. image:: /rr/diagram/selectList.png
   :alt: *selectList* statement syntax
   :align: center
   :target: syntax-tree.html#selectList


.. rubric:: *selectListItem* definition:

.. image:: /rr/diagram/selectListItem.png
   :alt: *selectListItem* statement syntax
   :align: center
   :target: syntax-tree.html#selectListItem


.. rubric:: *exprList* definition:

.. image:: /rr/diagram/exprList.png
   :alt: *exprList* statement syntax
   :align: center
   :target: syntax-tree.html#exprList


.. rubric:: *identifierList* definition:

.. image:: /rr/diagram/identifierList.png
   :alt: *identifierList* statement syntax
   :align: center
   :target: syntax-tree.html#identifierList


.. rubric:: *orderByList* definition:

.. image:: /rr/diagram/orderByList.png
   :alt: *orderByList* statement syntax
   :align: center
   :target: syntax-tree.html#orderByList


.. rubric:: *orderByListItem* definition:

.. image:: /rr/diagram/orderByListItem.png
   :alt: *orderByListItem* statement syntax
   :align: center
   :target: syntax-tree.html#orderByListItem


.. rubric:: *colAliases* definition:

.. image:: /rr/diagram/colAliases.png
   :alt: *colAliases* statement syntax
   :align: center
   :target: syntax-tree.html#colAliases


.. _selectExprRef:

Expression Reference
````````````````````

.. rubric:: *expr* definition:

.. image:: /rr/diagram/expr.png
   :alt: *expr* statement syntax
   :align: center
   :target: syntax-tree.html#expr


.. rubric:: *columnRef* definition:

.. image:: /rr/diagram/columnRef.png
   :alt: *columnRef* statement syntax
   :align: center
   :target: syntax-tree.html#columnRef


.. rubric:: *columnWildcard* definition:

.. image:: /rr/diagram/columnWildcard.png
   :alt: *columnWildcard* statement syntax
   :align: center
   :target: syntax-tree.html#columnWildcard
   
Refer to the :ref:`complete syntax tree <syntaxTree>` for further definitions 
of any rules.


.. _supportedSelectOperators:

Supported SELECT Operators
``````````````````````````
Note: In release v1.1, only limited ``SELECT`` statement functionality have
been enabled. This is described in the table below. Additional ``SELECT``
functionality will be enabled in subsequent releases.

.. list-table:: SQL Operator Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - Table scans
     - Enabled
   * - Index scans
     - Enabled
   * - Subqueries
     - **DISABLED**
   * - Joins
     - **DISABLED**
   * - Projections
     - Enabled, but with expression limitations documented
       :ref:`below <supportedSelectExprs>`
   * - Aggregations
     - Enabled
   * - Limits
     - **DISABLED**
   * - Order By
     - **DISABLED**
   * - Filters (``WHERE``, ``HAVING``)
     - Enabled, but with expression limitations documented
       :ref:`below <supportedSelectExprs>`
   * - Unions
     - **DISABLED**
   * - Explain
     - Enabled


.. _supportedSelectExprs:

Supported Expressions
`````````````````````
Note: In release v1.1, only limited ``SELECT`` expressions have been enabled.
This is described in the table below. Additional expression operators will be
enabled in subsequent releases.

.. list-table:: Supported :ref:`Literal Expression <literals>` Operator Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``BOOLEAN``
     - Enabled
   * - ``DATE``
     - Enabled
   * - ``DECIMAL``
     - Enabled
   * - ``FLOAT``
     - Enabled
   * - ``INTEGER``
     - Enabled
   * - ``NULL``
     - Enabled
   * - ``CHAR``/``VARCHAR``
     - Enabled
   * - ``TIME``
     - Enabled
   * - ``TIMESTAMP``
     - Enabled
   * - ``INTERVAL``
     - **DISABLED**, interval types are not yet supported.


.. list-table:: Supported :ref:`Logical Expression <logicalOperators>` Operator Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``AND``
     - Enabled
   * - ``OR``
     - Enabled
   * - ``XOR``
     - Enabled
   * - ``NOT``
     - Enabled


.. list-table:: Supported :ref:`Mathematical Expression <mathematicalOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - Addition (``+``)
     - Enabled
   * - Subtraction (``-``)
     - Enabled
   * - Multiplication (``*``)
     - Enabled
   * - Division (``/``)
     - Enabled
   * - Modulus (``%``)
     - Enabled
   * - Unary Plus (``+``)
     - Enabled
   * - Unary Negation (``-``)
     - Enabled

.. list-table:: Supported :ref:`Comparison Expression <comparisonOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - Equality (``=``)
     - Enabled
   * - Inequality (``!=``, ``<>``)
     - Enabled
   * - Less-than (``<``)
     - Enabled
   * - Less-than-or-equal-to (``<=``)
     - Enabled
   * - Greater-than (``>``)
     - Enabled
   * - Greater-than-or-equal-to (``>=``)
     - Enabled
   * - ``IS NULL`` / ``IS NOT NULL``
     - **DISABLED**, not yet implemented
   * - ``BETWEEN``
     - Enabled

.. list-table:: Supported :ref:`Conditional Expression <conditionalOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``CASE``
     - Enabled

.. list-table:: Supported :ref:`String Expression <stringOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``LIKE``
     - Enabled
   * - ``SUBSTRING``
     - Enabled

.. list-table:: Supported :ref:`Date and Time Expression <datetimeOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``EXTRACT``
     - Enabled

.. list-table:: Supported :ref:`Aggregate Expression <aggregateOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``COUNT``
     - Enabled
   * - ``MIN``
     - Enabled
   * - ``MAX``
     - Enabled
   * - ``SUM``
     - Enabled
   * - ``AVG``
     - Enabled

.. list-table:: Supported :ref:`Type Casting Expression <typeCastingOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``CAST``
     - Enabled

.. list-table:: Supported :ref:`Subquery Expression <subqueryOperators>` Operators Summary
   :header-rows: 1
   :widths: 25 75

   * - Operator
     - Comment
   * - ``EXISTS``
     - **DISABLED**
   * - ``IN``
     - **DISABLED**





.. _sleepCmd:

``SLEEP``
---------
Causes the SE Prompt to sleep and not accept input for a given number of seconds.

.. rubric:: Syntax:

.. image:: /rr/diagram/sleepStmt.png
   :alt: ``SLEEP`` statement syntax
   :align: center
   :target: syntax-tree.html#sleepStmt
   


.. _spaceCmd:

``SPACE``
---------
Shows the amount of memory that is free and being used by each node in the
cluster.

.. rubric:: Syntax:

.. image:: /rr/diagram/spaceStmt.png
   :alt: ``SPACE`` statement syntax
   :align: center
   :target: syntax-tree.html#spaceStmt



.. _statusCmd:

``STATUS``
----------
Returns a row for each node in the cluster indicating it’s health and whether
it is connected to the current node.

.. rubric:: Syntax:

.. image:: /rr/diagram/statusStmt.png
   :alt: ``STATUS`` statement syntax
   :align: center
   :target: syntax-tree.html#statusStmt



.. _szidxCmd:

``SZIDX``
----------
Shows how much memory is being used to store the indexes for a given table 
against each node in the cluster. 

.. rubric:: Syntax:

.. image:: /rr/diagram/szIdxStmt.png
   :alt: ``SZIDX`` statement syntax
   :align: center
   :target: syntax-tree.html#szIdxStmt
   


.. _sztblCmd:

``SZTBL``
----------
Shows how much memory is being used to store the data for a given table 
against each node in the cluster.

.. rubric:: Syntax:

.. image:: /rr/diagram/szTblStmt.png
   :alt: ``SZTBL`` statement syntax
   :align: center
   :target: syntax-tree.html#szTblStmt
   
   

.. _truncateCmd:

``TRUNCATE``
------------
Removes all data from a table without dropping the table or its indexes.

.. rubric:: Syntax:

.. image:: /rr/diagram/truncateStmt.png
   :alt: ``TRUNCATE`` statement syntax
   :align: center
   :target: syntax-tree.html#truncateStmt
   



.. _unloadCmd:

``UNLOAD``
----------
Writes a snapshot of the data in RapidsSE (tables, index, row data, etc) to
files on each node. The cluster must be quiesced (no other user activity)
while this operation occurs. A name must be given to the snapshot to separate
it from any other snapshots.

.. seealso::

  :ref:`unloadReload`

  :ref:`unload`

  :ref:`reload`


.. rubric:: Syntax:

.. image:: /rr/diagram/unloadStmt.png
   :alt: ``UNLOAD`` statement syntax
   :align: center
   :target: syntax-tree.html#unloadStmt
   

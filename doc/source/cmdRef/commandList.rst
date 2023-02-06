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

.. rubric:: *datatype* definition:

.. image:: /rr/diagram/datatype.png
   :alt: *datatype* definition
   :align: center
   :target: syntax-tree.html#datatype

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

.. image:: /rr/diagram/lscolStmt.png
   :alt: ``LSCOL`` statement syntax
   :align: center
   :target: syntax-tree.html#lscolStmt



.. _lsidxCmd:

``LSIDX``
---------
Lists all of the indexes that have been created on a given table.

.. rubric:: Syntax:

.. image:: /rr/diagram/lsidxStmt.png
   :alt: ``LSIDX`` statement syntax
   :align: center
   :target: syntax-tree.html#lsidxStmt



.. _lsidxcolCmd:

``LSIDXCOL``
------------
Lists the columns of a given index on a particular table.

.. rubric:: Syntax:

.. image:: /rr/diagram/lsidxcolStmt.png
   :alt: ``LSIDXCOL`` statement syntax
   :align: center
   :target: syntax-tree.html#lsidxcolStmt



.. _lstblCmd:

``LSTBL``
---------
Lists all of the tables in the default catalog and schema.

.. rubric:: Syntax:

.. image:: /rr/diagram/lstblStmt.png
   :alt: ``LSTBL`` statement syntax
   :align: center
   :target: syntax-tree.html#lstblStmt




.. _printCmd:

``PRINT``
---------
Prints a string to stdout on the SE Prompt.

.. rubric:: Syntax:

.. image:: /rr/diagram/printStmt.png
   :alt: ``PRINT`` statement syntax
   :align: center
   :target: syntax-tree.html#printStmt


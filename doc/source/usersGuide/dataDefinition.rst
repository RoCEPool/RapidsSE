.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: RapidsSE; DDL
   single: DDL

.. _dataDefinition:

Data Definition
===============

Data Organisation
-----------------
RapidsSE organises data in a hierarchy of catalogs, schemas and tables. This
logical hierarchy allows data to be clearly grouped or separated and (when
supported) different security privileges applied.

.. note::

  RapidsSE does not yet support the creation and destruction of catalogs
  and schemas.

The default catalog is called ``rapids``. The default schema is called
``public``. All newly created tables will be created in this catalog
and schema, and can be qualified with these names.

For example, a table ``t1`` can be fully qualified and referenced as
``rapids.public.t1``.


.. index::
   single: RapidsSE; tables
   single: DDL; tables

.. _tables:

Tables
------
Tables are the principal method for storing data in RapidsSE. Data is stored in
rows within the table. Tables have a definition that is specified when
the table is created. This definition describes the format of rows within the
table - at the minimum it describes the names and data types of columns within
the rows.

Tables in RapidsSE are optimised to store many billions of rows. RapidsSE helps
support queries on very large tables through table partitioning and indexing.

Table partitioning splits a single logical table into multiple separate parts
that can be worked on concurrently. Table indexes organise rows within each
partition into an order based on values in particular columns. This allows
queries to more quickly find particular rows it is interested in without having
to scan all of the data.


.. index::
   single: RapidsSE; table partitioning
   single: DDL; table partitioning
   single: CREATE TABLE (command); partitioning

.. _tablePartitioning:

Table Partitioning
``````````````````
In RapidsSE a table is not one physical structure but a collection of
partitions on each node. Each partition holds a roughly equal amount of rows
in the table. The purpose and benefit of partitions is that it allows
operations on the table to often happen in parallel, maximising the use of
computing power on each node and therefore providing a faster query response
time.

The number of partitions on a node should roughly match the number of CPUs
cores that the node has. We want each individual CPU core to be reading data
and processing it concurrently and as independently as possible for a query
in order to minimise the query time.

Data in a table is partitioned based on the values in the partitioning columns.
The values in these columns of each row are read and hashed, and this gives
the partition number that each row should be assigned to. Rows with the same
values in the partitioning columns will end up in the same partition.

Care should be taken when choosing the partitioning columns. One wouldn't want
to partition a table based on a single ``BOOLEAN`` column, for example, as
there are only three unique values (``true``, ``false`` and ``NULL``) and this
would result in data being split across three or less partitions, leaving
other partitions with no data to process.

A good choice of partitioning columns are those whose combination give unique
values, like a primary key.

The partitioning columns can be specified in the ``CREATE TABLE`` statement
with the ``PARTITION BY`` clause after the column definitions. If no
``PARTITION BY`` clause is specified then the first column of the table is
automatically chosen as the partitioning column. For example:

.. code:: sql

   CREATE TABLE t1
   (
     col1 INTEGER,
     col2 VARCHAR,
     col3 TIMESTAMP
     PARTITION BY col2, col3
   ) ;

It is not possible to change the columns that a table is partitioned on without
dropping and recreating the table.


.. index::
   single: RapidsSE; tables (creating)
   single: DDL; creating tables
   single: tables; creating
   single: CREATE TABLE (command); overview

.. _creatingTables:

Creating Tables
```````````````
Tables in RapidsSE are created with the ``CREATE TABLE`` command. A table
must have a unique table name for the schema it is created in.

A table can either be created as a clustered or a replicated table. By
default tables are created as clustered tables. This means that the data in
the table is split across partitions on all nodes in the cluster according to
the values in the partitioning columns. A replicated table is one where the
data is duplicated on each node in the cluster. Replicated tables are suitable
for small dimension tables that can be joined with clustered fact tables
concurrently on each node in the cluster.

Replicated tables are specified by including the term ``REPLICATED`` after
specifying the column definitions. For example:

.. code:: sql

   CREATE TABLE t1
   (
     col1 INTEGER,
     col2 VARCHAR,
     REPLICATED
   ) ;

Regardless of whether the table is being created as a clustered or replicated
table, the ``CREATE TABLE`` command executes on all nodes in the cluster. It
is not possible to create a table local to one node only.

When a table is being created the definitions for each column in the table
must be specified. This includes the column name and data type, as well as
any optional column constraints such as ``NOT NULL``, data type lengths
and precision, etc.

.. note:: Table Constraints

  Tables in RapidsSE do not currently support table clauses such as ``KEY``,
  ``PRIMARY KEY``, ``FOREIGN KEY`` or other table constraints.

.. note:: Data type lengths

  RapidsSE does not currently obey specified data type column lengths such as
  ``VARCHAR(40)`` or ``DECIMAL(9,3)``. While it will accept this syntax the
  system does not currently obey these constraints that are specified.


.. index::
   single: RapidsSE; tables (dropping)
   single: DDL; dropping tables
   single: tables; dropping
   single: DROP TABLE (command); overview

.. _droppingTables:

Dropping Tables
```````````````
A table in RapidsSE can be removed with the ``DROP TABLE`` command. This
command removes the definition of the table as well as all data and indexes
associated with that table.


.. index::
   single: RapidsSE; tables (altering)
   single: DDL; altering tables
   single: tables; altering

.. _alteringTables:

Altering Tables
```````````````
RapidsSE does not support altering existing tables to add or remove columns,
or alter column constraints.


.. index::
   single: RapidsSE; indexes
   single: DDL; indexes
   single: indexes; overview

.. _indexes:

Indexes
-------
An index provides a sorted order of rows in a table according to the columns
an index is sorting on. This allows a query to very quickly find rows with
particular values on the indexed columns, saving the work of scanning all rows
in the table.

RapidsSE supports multiple indexes per table so that each index can have a
different set of indexed columns that can better match the way the table is
queried. Each index that is created on a table is not free, however. An index
uses memory to maintain rows in their particular order, leaving less memory for
actually storing more data. Having more indexes also increases the time to load
data into the system as each row needs to find its ordered position within
each index.

When an index is created on a table, one or more column names must be specified
that indicate how rows will be ordered. If more than one column names are
specified then rows will be ordered according to the value in the first
specified column. If two or more rows have the same value for that first column
then they are ordered on the next specified index column. This continues until
either the rows are uniquely sorted or there are no more index columns to sort
by.

An index can be unique or non-unique. A unique index is one where there cannot
be two or more rows that have the exact same values across the indexed columns.
A non-unique index allows multiple rows to have same values (and therefore the
same sorted position) across all indexed columns.

.. note::

  RapidsSE does not currently support unique indexes, only non-unique indexes.

An index can be created before a table is populated with data, or afterwards.

As an in-memory system, RapidsSE uses skiplists to implement indexes. These
provide a reasonably low memory footprint and a good balance of insertion
speed and search speed.

.. index::
   single: DDL; creating indexes
   single: DDL; dropping indexes
   single: indexes; creating
   single: indexes; dropping
   single: CREATE INDEX (command); overview
   single: DROP INDEX (command); overview

Indexes are created with the ``CREATE INDEX`` command and dropped with the
``DROP INDEX`` command.

Dropping an index does not delete the data in the table, just that indexes
way of ordering the data.


.. index::
   single: RapidsSE; privileges
   single: DDL; privileges
   single: security; privileges

.. _privileges:

Privileges
----------
RapidsSE does not currently have any privilege system to restrict the ability
of users to view or edit data within the system.

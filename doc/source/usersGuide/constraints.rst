.. index::
   single: RapidsSE; constraints
   single: constraints of RapidsSE

.. _constraints:

Constraints and Limitations
===========================

RapidsSE has several system limitations. These are documented below:

#. **Data type value ranges**

   Each data type can have specific limitations with their range of values.
   Refer to the section Data Types for more details.

#. **CHAR and VARCHAR length**

   ``CHAR`` and ``VARCHAR`` values are currently limited to 4,000 bytes
   (not characters) in length. This will be expanded in a future release.

#. **Character encoding**

   RapidsSE does not currently recognise any character encoding. All strings
   are treated as ASCII strings. While multi-byte character values can be
   loaded, these are not interpreted with respect to any specific encoding
   other than ASCII. As such RapidsSE considers each byte in a string to
   represent one character. This will affect expression operators that
   manipulate string values such as ``SUBSTRING``.

#. **Maximum of 255 characters for table, index and column names**

   RapidsSE does not support table, index and column names greater than
   255 characters.

#. **Maximum of 48 columns per table**

   RapidsSE does not support tables having more than 48 columns.

#. **Maximum of 16 keys per index**

   A composite key index (an index on more than one column) on a table cannot
   have more than 16 keys.

#. **Statement length**

   A statement issued to RapidsSE cannot have more than 2048 tokens (words)
   or be longer than 48,000 bytes.

#. **Logging defaults**

   RapidsSE currently supports 4 historical log files each with a maximum size
   of 20MB. RapidsSE currently does not allow the user to alter the logging
   parameters.

#. **Node loss**

   RapidsSE currently does not tolerate the loss of a node in the cluster. If
   a node in the cluster is to go offline then commands being submitted to
   other nodes in the cluster will return an error message indicating that a
   node has been lost. The current version of RapidsSE does not support the
   restarting of the node or adding of a node to the cluster to compensate.
   This will be addressed in a future version.

#. **Low memory**

   As an in-memory system, if RapidsSE runs out of space during the execution
   of a command then an error message will be returned to the user. This can
   occur due to loading more data into the system than available memory, or
   through the use of temporary buffers used during the execution of a
   complex query.

   A future version of RapidsSE will pre-allocate enough memory for an
   administrator to try to recover some space, e.g. by dropping a table.

#. **Large scale factors**

   The current version of RapidsSE has not been validated for clusters
   of more than 10 nodes or scale factors in excess of SF1000. This will
   be validated and addressed in a future version.

#. **Join Type**

   RapidsSE only supports INNER joins (the default).

   RapidsSE also only supports equi-joins. That is, join predicates that are
   compared for equality.

#. **Index-joins**

   RapidsSE only supports joining a table on an indexed column only. Refer to
   the section :ref:`joinSupport`.

#. **Unique indexes**

   RapidsSE does not yet support unique indexes or primary keys, only
   non-unique indexes.

#. **Interval columns**

   RapidsSE does not support ``INTERVAL`` columns in ``CREATE TABLE`` statements,
   however intervals are supported as literals and in expression calculations. This
   constraint also means that intervals cannot be directly loaded with commands
   like ``load`` and ``dload``.

#. **Interval aggregates**

   Aggregate expressions (``SUM``, ``MIN``, ``MAX``, etc) are not currently
   supported on interval expressions.
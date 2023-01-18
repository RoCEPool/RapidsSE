.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _queries:

Queries
=======
This section describes how to retrieve data from tables within RapidsSE.
This is known as a query.

Form of a SQL Query
```````````````````

A ``SELECT`` query in RapidsSE is constructed from SQL clauses with the
following general form:

.. code:: sql

  [ WITH <subqueries> ]
  SELECT <exprList>
  FROM ( <tableName> | <subquery> )
  [ JOIN <tableName> ... ]
  [ WHERE <expr> ]
  [ GROUP BY <exprList> ]
  [ HAVING <expr> ]
  [ <setOperator> <selectQuery> ... ]
  [ ORDER BY <exprList> ]
  [ LIMIT <expr> ]

These clauses are described in more detail in the sections below. Expressions
are described :ref:`here <expressions>`.

Supported Query Clauses
```````````````````````
RapidsSE supports the following syntactic SQL clauses in a
``SELECT`` query:

* Table subqueries via the ``WITH`` clause;
* Table scans via the ``FROM`` clause;
* Table joins via the ``JOIN`` clause;
* Row filtering via the ``WHERE`` clause;
* Row aggregation via the ``GROUP BY`` clause;
* Filtering of aggregated rows via the ``HAVING`` clause;
* Set operations, such as ``UNION ALL``;
* Row ordering via the ``ORDER BY`` clause;
* Result set curtailment via the ``LIMIT`` clause;

RapidsSE does not currently support the following SQL clauses:

* The ``DISTINCT`` keyword, as applied to selected column expressions;
* ``UNION`` without the ``ALL`` keyword (this is effectively a ``UNION DISTINCT``);
* Other set operators: ``EXCEPT`` and ``INTERSECT``;
* Recursive table subqueries (``WITH`` keyword)
* Any ``OUTER JOIN`` (e.g. ``LEFT``/``RIGHT``/``FULL OUTER JOIN``)

SELECT List
```````````
The ``SELECT`` list is the list of expressions that occurs after
the ``SELECT`` keyword in a query. It defines the list of column values
that are to be returned in each row of the query. These expressions can
include references to table columns, expression operators, expression
literals, and combinations of all of these.

Refer to the :ref:`expressions` topic for more details about the types
of expressions, how they are used and what expression operators are
supported.

FROM clause
```````````
The ``FROM`` clause, together with any ``JOIN`` clauses, describes the source
of the data for the query. Typically the ``FROM`` clause states the name of a
table or the name of a :ref:`subquery <subqueries>` from a ``WITH`` clause,
but it can also include an inline definition of another subquery.

Examples:

Selecting data from a simple table:

.. code:: sql

  SELECT a, b FROM t1 ;

Selecting data from a subquery defined in a ``WITH`` clause:

.. code:: sql

  WITH subquery1 AS (SELECT ...)
  SELECT a, b FROM subquery1 ;

Selecting data from an inline subquery in the ``FROM`` clause.

.. code:: sql

  SELECT a, b FROM (SELECT ...) ;


JOIN Clause
```````````
The ``JOIN`` clause allows a query to combine rows from multiple tables.
There are many different ways in which two tables can be joined together,
including inner and outer joins.

The most basic type of join is the ``CROSS JOIN``, which is a type of inner
join. Given two tables, ``t1`` and ``t2``, a ``CROSS JOIN`` will return every
combination of rows from each of the two tables. This can naturally result in
a very large result set. This can be specified as follows:

.. code:: sql

  SELECT t1.a, t2.b FROM t1 JOIN t2 ;

It can also be specified in the following alternate syntaxes:

.. code:: sql

  SELECT t1.a, t2.b FROM t1, t2 ;
  SELECT t1.a, t2.b FROM t1 CROSS JOIN t2 ;

Typically a user will wish to only join rows from two tables when a certain
condition involving both rows is met. This is referred to as a join predicate.
A join predicate is an :ref:`expression <expressions>` that specifies the
relationship between rows in the two tables. The join predicate is evaluated
for each combination of rows and if the join predicate evaluates to true then
the two rows from the tables are combined and emitted for further processing.
If the join predicate evaluates to false or ``NULL`` then the rows are not
combined.

A join predicate can be specified in multiple ways:

1. By using the ``ON`` clause;
2. By using the ``USING`` clause;
3. By specifying the join predicate as part of a WHERE clause.

These three methods are shown in the examples below where rows from table ``t1``
are joined with rows from table ``t2`` if column ``c`` from ``t1`` is equal to
column ``c`` from ``t2``.

.. code:: sql

  SELECT t1.a, t2.b FROM t1 JOIN t2 ON t1.c = t2.c ;
  SELECT t1.a, t2.b FROM t1 JOIN t2 USING c ;
  SELECT t1.a, t2.b FROM t1, t2 WHERE t1.c = t2.c ;


What is the default join type?

inner/outer/cross

join on
join using

WHERE Clause
````````````

HAVING Clause
`````````````

GROUP BY Clause
```````````````


Set Operators
`````````````
UNION
EXCEPT
INTERSECT

ALL or DISTINCT

Ordering
````````

Limiting
````````

.. _subqueries:

Subqueries
``````````


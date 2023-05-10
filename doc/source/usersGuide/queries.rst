.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: RapidsSE; queries
   single: queries; overview

.. _queries:

Queries
=======
This section describes how to retrieve data from tables within RapidsSE.
This is known as a query.


.. index::
   single: queries; form
   single: SELECT (command); form

.. _formOfASqlQuery:

Form of a SQL Query
-------------------

A :ref:`SELECT <selectCmd>` query in RapidsSE is constructed from SQL clauses with the
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
are described :ref:`here <expressions>`. Detailed syntax of ``SELECT`` queries
can be found in the :ref:`Command Reference <selectCmd>`.

.. index::
   single: RapidsSE; supported query clauses
   single: queries; supported clauses
   single: SQL; supported query clauses

Supported Query Clauses
-----------------------
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


Query Clauses
-------------

.. index::
   single: queries; select list
   single: select list

.. _selectlist:

SELECT List
```````````
The ``SELECT`` list is the list of :ref:`expressions <expressions>` that occurs
after the ``SELECT`` keyword in a query. It defines the list of column values
that are to be returned in each row of the query. These expressions can
include references to
:ref:`table columns <columnReference>`,
:ref:`expression operators <exprOperators>`,
:ref:`expression literals <literals>`,
:ref:`expression keywords <exprKeywords>` and combinations of all of these.
They may be simple scalar values or they may contain
:ref:`aggregations <aggregateOperators>`.

Below are some simple examples of ``SELECT`` list expressions.

.. code:: sql

  SELECT a, b + 1 FROM t1 ;
  SELECT SUM(a), SUM(b + 1) FROM t1 ;
  SELECT * FROM t1 ;

The ``*`` notation in a ``SELECT`` list means to select all columns from
the specified table(s).

Expressions in the ``SELECT`` list can sometimes be long and complex to write.
As such, an alias can be given to individual projected expressions so that
the expression can be referred to in other parts of the query without having
to rewrite the complex expression. Aliases are specified by using the ``AS``
clause after each expression in the ``SELECT`` list where an alias is
desired, or by simply leaving a space between the expression and its alias.
The query can then make use of the alias in other clauses where it
is allowed.

For example, the query below uses the alias ``pct`` in the ``WHERE`` clause:

.. code:: sql

  SELECT a / b * 100.0 AS pct, c
  FROM t1
  WHERE pct >= 10.0 AND c > 0 ;

The example below shows an incorrect use of the alias ``total`` in the ``WHERE``
clause because ``total`` is synonymous with the expression ``SUM(b)`` and
this aggregated expression cannot appear in the ``WHERE`` clause.

.. code:: sql

  SELECT a, SUM(b) AS total
  FROM t1
  WHERE total > 0
  GROUP BY a ;

  error: No such column: total;

However the above erroneous query could be correctly written with a ``HAVING``
clause instead because the aggregation is calculated before the ``HAVING``
clause (and therefore the alias is visible to it):

.. code:: sql

  SELECT a, SUM(b) AS total
  FROM t1
  GROUP BY a
  HAVING total > 0 ;

The entire ``SELECT`` list can also be prepended with the optional keywords ``ALL``
or ``DISTINCT``. Use of ``DISTINCT`` will eliminate any duplicate rows that have
all the same column values as a previous row. The use of ``ALL`` retains all rows,
and this is the default behaviour when neither keyword is specified.

.. note::
  RapidsSE does not yet support the use of ``DISTINCT`` in the ``SELECT`` list.

.. note::

  Refer to the :ref:`expressions` topic for more details about the types
  of expressions, how they are used and what expression operators are
  supported.


.. index::
   single: queries; FROM clause
   single: FROM (clause)

.. _from:

FROM Clause
```````````
The ``FROM`` clause, together with any ``JOIN`` clauses, describes the source
of the data for the query. Typically the ``FROM`` clause states the name of a
table or the name of a :ref:`subquery <subqueries>` from a ``WITH`` clause,
but it can also include an inline definition of another subquery.

Examples:

Selecting data from a simple table:

.. code:: sql

  SELECT a, b FROM t1 ;

Selecting data from a :ref:`subquery <subqueries>` defined in a ``WITH`` clause:

.. code:: sql

  WITH subquery1 AS (SELECT ...)
  SELECT a, b FROM subquery1 ;

Selecting data from an inline subquery in the ``FROM`` clause.

.. code:: sql

  SELECT a, b FROM (SELECT ...) ;

.. _tableAlias:

A table whose name is long and difficult to type can be given an alias that
can then be used in other parts of the query. A table alias is specified
after the table name in the ``FROM`` clause (or ``JOIN`` clauses) separated
by a space. For example:

.. code:: sql

  SELECT t.a, t.b
  FROM really_long_table_name t
  WHERE t.a > 0 ;

In the example above, the table ``really_long_table_name`` is aliased to ``t``
for simplicity.


.. index::
   single: queries; JOIN clause
   single: JOIN (clause)

.. _join:

JOIN Clause
```````````

Overview
::::::::
The ``JOIN`` clause allows a query to combine rows from multiple tables.
There are many different ways in which two tables can be joined together,
including *inner* and *outer* joins.

The most basic type of join is the ``CROSS JOIN``, which is a type of inner
join. Given two tables, ``t1`` and ``t2``, a ``CROSS JOIN`` will return every
combination of rows from each of the two tables. This can naturally result in
a very large result set. A cross join can be specified as follows:

.. code:: sql

  SELECT t1.a, t2.b FROM t1 JOIN t2 ;

A cross join can also be specified in the following alternate syntaxes:

.. code:: sql

  SELECT t1.a, t2.b FROM t1, t2 ;
  SELECT t1.a, t2.b FROM t1 CROSS JOIN t2 ;

Typically a user will wish to only join rows from two tables when a certain
condition involving both rows is met. This is referred to as a join predicate.
A join predicate is an :ref:`expression <expressions>` that tests the
relationship between rows in the two tables. The join predicate is evaluated
for each combination of rows and if the join predicate evaluates to true then
the two rows from the tables are combined and emitted for further processing.
If the join predicate evaluates to false or ``NULL`` then the rows are not
combined.

In RapidsSE a join predicate can be specified in multiple ways:

1. By using the ``ON`` clause;
2. By using the ``USING`` clause;
3. By specifying the join predicate as part of a ``WHERE`` clause.

These three methods are shown in the examples below where rows from table ``t1``
are joined with rows from table ``t2`` if column ``c`` from ``t1`` is equal to
column ``c`` from ``t2``.

.. code:: sql

  SELECT t1.a, t2.b FROM t1 JOIN t2 ON t1.c = t2.c ;
  SELECT t1.a, t2.b FROM t1 JOIN t2 USING c ;
  SELECT t1.a, t2.b FROM t1, t2 WHERE t1.c = t2.c ;

The ``USING`` clause in the above example is a shorthand way to construct the
join predicate ``t1.c = t2.c``. It is only applicable if the join predicate
is to compare columns from the two tables that have the same name.

A cross join can also be represent as an inner join with a join predicate
of ``true``. For example:

.. code:: sql

  SELECT t1.a, t2.b FROM t1 JOIN t2 ON TRUE ;

There are three variants of outer joins - ``LEFT OUTER JOIN``, ``RIGHT OUTER
JOIN``, and ``FULL OUTER JOIN``. Outer joins produce the same output as inner
joins when the join predicate evaluates to ``true``. They differ from inner
joins when the join predicate evaluates to ``false``.

In a ``LEFT OUTER JOIN``, if a row from the first (left) table does not get
joined with **any** rows from the second (right) table then the row from
the first (left) table is combined with a row of ``NULL``\s for the second
(right) table and this is emitted once.

In a ``RIGHT OUTER JOIN``, if a row from the second (right) table does not get
joined with **any** rows from the first (left) table then the row from
the second (right) table is combined with a row of ``NULL``\s for the first
(left) table and this is emitted once.

A ``FULL OUTER JOIN`` is a union of both a ``LEFT OUTER JOIN`` and a
``RIGHT OUTER JOIN``. Any rows from either table that are not previously
matched are combined with a row of ``NULL``\s of the opposite table and then
emitted.

.. index::
   single: queries; JOIN support
   single: JOIN (support)

.. _joinSupport:

RapidsSE Join Support
:::::::::::::::::::::
RapidsSE currently supports Index-Joins only. An index-join is a join which
requires the table being joined to have a index on the join column (refer to join
example below).

Index-Joins are the only join technique supported at this stage.  Future
versions of RapidsSE will support additional techniques such as hash-joins.
Queries which attempt to join tables where there isn’t a supporting index will
result in a user error. The supplied error message will enumerate the tables
which do not have suitable indexes.

.. rubric:: Example

In the following example we see a join between the Lineitem and Orders tables drawn from the TPC-H schema:

.. code:: sql

   SELECT l_returnflag, l_linestatus, COUNT(*)
     FROM lineitem, orders
    WHERE l_orderkey = o_orderkey
    GROUP BY l_returnflag, l_linestatus
   ;

In the case of RapidsSE, for this join to succeed either there must be an
index on ``l_orderkey``, and/or an index on ``o_orderkey``. If there isn’t a
suitable index then RapidsSE will return a user error.

Also refer to the section :ref:`constraints`.

Index-Join Errors
:::::::::::::::::
In the case where a join is not possible because suitable indexes are not
available, RapidsSE will return an ``EC_MISSING_JOIN_INDEX`` user error.
The supplied error text will enumerate the tables which do not have suitable
indexes.

.. rubric:: No Indexes At All

Let us assume the ``lineitem``, ``orders`` and ``customer`` have been created
with no indexes. Running a 3-way join query will result in the following error
being returned:

.. code:: sql

  error: NODE1; EC_MISSING_JOIN_INDEX; Tables: lineitem, orders, customer;

.. rubric:: Missing Index

Let us assume the ``lineitem``, ``orders`` and ``customer`` have been created
with only the index on ``l_orderkey``. Running a 3-way join query will result
in the following error being returned:

.. code:: sql

  error: NODE1; EC_MISSING_JOIN_INDEX; Tables: Orders, Customer;


Join Ordering
:::::::::::::
In the previous example it may be possible for RapidsSE to scan the
``lineitem`` table first, and then for every row of ``lineitem`` attempt to
join it to each matching row in the ``orders`` table.  This matching would be
based on the ``WHERE``/``JOIN`` predicate: ``l_orderkey = o_orderkey``.  For
this to be possible, there would need to be an index on the column
``o_orderkey`` of the ``orders`` table.

Alternatively it may also be possible for RapidsSE to scan the ``orders``
table first, and then for every row of ``orders`` attempt to join it to each
matching row in ``lineitem``.  This would be based on the ``WHERE``/``JOIN``
predicate: ``l_orderkey = o_orderkey``. For this to be possible, there would
need to be an index on column ``l_orderkey`` of the ``lineitem`` table.

If there were indexes on both ``l_orderkey`` and ``o_orderkey``, then both of
the join permutations are possible.  In this case the :ref:`RapidsSE SQL Optimiser <optimisation>`
will choose one of the permutations based on estimating which has the lowest
network overhead.

If nether of the mentioned indexes is available then RapidsSE will return a
user error.


.. index::
   single: queries; WHERE clause
   single: WHERE (clause)

.. _where:

WHERE Clause
````````````
The ``WHERE`` clause is used to filter rows based on an expression predicate.
The predicate is evaluated for each row and if the predicate evaluates to
``TRUE`` then the row is emitted. If it evaluates to ``FALSE`` then the row
is dropped.

The ``WHERE`` clause is applied before any aggregations are done, and
typically as close to source of data as possible for performance purposes.

An expression in a ``WHERE`` clause may contain multiple logical predicates
separated by a logic operator such as ``AND`` and ``OR``. The RapidsSE
optimiser will use this information to help determine the fastest way to
execute this query. Filtering out as much unnecessary data as possible with
a ``WHERE`` clause is one of the simplest ways to speed up the execution
time of a query.

.. code:: sql

  SELECT a, b FROM t1 WHERE a > 10 AND b = 20 ;

The predicate in the WHERE clause can also contain columns that are not being
returned in the select list. These temporary columns will be read from the
table but will be removed before the result set is returned to the user.

.. code:: sql

  SELECT a, b FROM t1 WHERE c < 0 ;

Because the ``WHERE`` clause is executed before any aggregations table place,
it is an error to refer to any computed aggregated values in a ``WHERE``
clause. Instead, use a ``HAVING`` clause.


.. index::
   single: queries; GROUP BY clause
   single: GROUP BY (clause)

.. _groupby:

GROUP BY Clause
```````````````
The ``GROUP BY`` clause allows aggregates expressions to be calculated on
groups of rows that share the same unique value for same ``GROUP BY``
expression. For example, the query below could be used to to provide a count
of the number of employees in each department.

.. code:: sql

  SELECT departmentId, COUNT(*)
  FROM t1
  GROUP BY departmentId;

The ``GROUP BY`` clause specifies a list of non-aggregate expressions that will
be used to determine the unique groups for aggregation. This may be one or more
expressions that can reference columns from the table and joined tables
specified in the query.

If the ``GROUP BY`` contains multiple terms then grouping is done by evaluating
and comparing the first grouping expressions in the order that they were
specified. For example, we can modify the above query to first group on the
nation that employees reside in, then their department. This will show us the
count of employees in each department in each country.

.. code:: sql

  SELECT nationId, departmentId, COUNT(*)
  FROM t1
  GROUP BY nationId, departmentId;

When a ``GROUP BY`` is used in a query, the query can only potentially return
the aggregated columns as well as the expressions in the ``GROUP BY`` list.
It is possible to not include the grouping columns in the select list, however
then you cannot see the identifiers for each of the aggregated groups.

It is not possible to return a non-aggregated expression that isn't in the
``GROUP BY`` list.

Without a ``GROUP BY`` in a statement then any aggregates are calculated on all
rows in the query. These are called grand total aggregates. A query with grand
total aggregates cannot have non-aggregated columns (other than literals) in
the select list as there is no ``GROUP BY`` clause to specify how the aggregate
terms are to be grouped.

For example, this query is disallowed:

.. code:: sql

  SELECT a, SUM(b) FROM t1 ;
  error: Cannot select both aggregate and non-aggregate expressions without a GROUP BY clause;


.. index::
   single: queries; HAVING clause
   single: HAVING (clause)

.. _having:

HAVING Clause
`````````````
A ``HAVING`` clause is similar to a ``WHERE`` clause. Both clauses are
used to filter out rows in query where the predicate evaluates to ``FALSE``.
Where the two clauses differ is in when they are applied. A ``WHERE`` clause
occurs *before* any aggregations are calculated, whereas a ``HAVING`` clause
is applied *after* any aggregations are calculated. A ``HAVING`` clause
is not applicable if the query does not contain any aggregates.

In the example below, the minimum value of column ``a`` is calculated.
It is then filtered by the ``HAVING`` clause - if this minimum value is
greater than zero then the row is emitted, otherwise no rows will be returned.

.. code:: sql

  SELECT MIN(a) FROM t1
  HAVING MIN(a) > 0 ;

The example below first reads columns ``a`` and ``b`` from table ``t1`` and
calculates the average of column ``b`` for each unique value of column ``a``.
After this calculation is complete the aggregated rows are then evaluated
against the ``HAVING`` clause and only those rows where column ``a`` has a
value between 0 and 10 are returned. All other rows are dropped.

.. code:: sql

  SELECT a, AVG(b) FROM t1
  GROUP BY a
  HAVING a > 0 AND a < 10 ;

A ``HAVING`` clause can be combined with a ``WHERE`` clause as well, though
care must be taken to ensure that their join predicates are correct. In the
example below columns ``a`` and ``b`` from table ``t1`` are read. The
``WHERE`` clause then eliminates rows where the value of column ``b`` is
negative. The remaining rows are then grouped and aggregated before the
aggregated rows are then filtered according to the predicate in the
``HAVING`` clause.

.. code:: sql

  SELECT a, AVG(b) FROM t1
  WHERE b >= 0
  GROUP BY a
  HAVING a > 0 AND a < 10 ;


.. index::
   single: queries; set operators
   single: set operators
   single: UNION (clause)
   single: INTERSECT (clause)
   single: EXCEPT (clause)

.. _setoperators:

Set Operators
`````````````
The SQL set operators allow for result sets from multiple queries to be
combined. These include the set operators ``UNION``, ``EXCEPT`` and
``INTERSECT``. These set operators can be chained together to combine rows
from more than two queries.

One constraint with using set operators is that the result set definition
of each query must be compatible. That is, each query must return rows
with the same number of columns and each corresponding column from each query
must have the same data type. The column names need not match between the
queries.

The keywords ``ALL`` and ``DISTINCT`` can be used to further control the
nature of the set operation. If the ``ALL`` keyword is not specified then the
``DISTINCT`` keyword is assumed.

The ``UNION ALL`` operator emits all rows from both the specified queries.
If the ``ALL`` keyword is left off, or the operation is specified as
``UNION DISTINCT`` then only one copy of any duplicate rows are returned and
the rest are filtered out.

The ``INTERCEPT ALL`` operator emits all rows that occur in both the specified
queries. If the ``ALL`` keyword is left off, or the operation is specified as
``INTERSECT DISTINCT`` then only one copy of any duplicate rows are returned and
the rest are filtered out.

The ``EXCEPT ALL`` operator emits all rows that only occur in the first query and
not in the second. If the ``ALL`` keyword is left off, or the operation is specified as
``EXCEPT DISTINCT`` then only one copy of any duplicate rows are returned and
the rest are filtered out.

.. attention::

   RapidsSE currently only supports the ``UNION ALL`` set operator.
   It does not support the ``INTERSECT`` or ``EXCEPT`` operators. It also does
   not support any ``DISTINCT`` variants of any of the operators.

.. rubric:: Examples:

The example below reads columns ``a`` and ``b`` from table ``t1`` as well as
reading columns ``c`` and ``d`` from table ``t2``. All rows from both queries
are returned. The data type of columns ``t1.a`` and ``t2.c`` must match, and
so too must the data type of columns ``t1.b`` and ``t2.d``.

.. code:: sql

  SELECT a, b FROM t1
  UNION ALL
  SELECT c, d FROM t2 ;

The example below combines the results from three queries

.. code:: sql

  SELECT a, b FROM t1
  UNION ALL
  SELECT c, d FROM t2
  UNION ALL
  SELECT e, f FROM t3 ;


.. rubric:: ``CORRESPONDING`` clause

If two queries contain some common column names then they can be combined in a
simpler way by using the ``CORRESPONDING`` clause. The ``CORRESPONDING`` clause
allows the common column names across the two queries to be specified and only
those columns will be returned. All other columns will be dropped.

For example, if two tables contain columns named ``a`` and ``b`` then the
following query would be valid:

.. code:: sql

  CREATE TABLE t1 (a INTEGER, b VARCHAR, c DATE);
  CREATE TABLE t2 (a INTEGER, b VARCHAR, d TIME);

  SELECT * FROM t1
  UNION ALL CORRESPONDING BY (a, b)
  SELECT * FROM t2 ;

In this example the ``UNION ALL`` operator would only return the two named
columns, ``a`` and ``b``, even though each query is initially selecting all
of its columns.

This can be simplified even further by specifying an empty ``CORRESPONDING``
clause. RapidsSE will then determine the columns with common names and equal
data types and only those columns will be returned. For example:


.. code:: sql

  CREATE TABLE t1 (a INTEGER, b VARCHAR, c DATE, d BOOLEAN);
  CREATE TABLE t2 (b VARCHAR, c TIMESTAMP, d BOOLEAN, e VARCHAR);

  SELECT * FROM t1
  UNION ALL CORRESPONDING
  SELECT * FROM t2 ;

The above example will return a result set with columns ``b`` (``VARCHAR``) and
``d`` (``BOOLEAN``). Column ``t1.a`` is unique to table ``t1`` only, so it won't
be included. Same with column ``t2.e``. Meanwhile column ``c`` is common to both
tables, but the data type does not match. Only columns ``b`` and ``d`` are
suitable for the set operation.


.. index::
   single: queries; ORDER BY clause
   single: ORDER BY (clause)

.. _orderby:

ORDER BY Clause
```````````````
The ``ORDER BY`` operator allows the rows in the result set to be ordered by
one or more expressions. If there is more than one ordering expression specified
then the result set is ordered according to the order in which the ordering
expressions were specified.

For example, in the query below the result set is ordered first by the value
of the (column) expression ``a``. If this is not a unique value then it is
further ordered by the projected expression ``b/2``.

.. code:: sql

  SELECT a, b/2, c FROM t1
  ORDER BY a, b/2 ;

By default, results are ordered in an ascending direction. This can be altered
by specifying ``ASC`` for ascending or ``DESC`` for descending after each
ordering expression. Each ordering column can have a different ordering direction,
as per the example below where expression ``a`` is ordered ascending (because
neither ``ASC`` or ``DESC`` was specified, so ascending is the default), and
column ``b/2`` is ordered in a descending direction.

.. code:: sql

  SELECT a, b/2, c FROM t1
  ORDER BY a, b/2 DESC ;

The ordering expressions specified need to be a valid expression that could be
included in the query's select list, but they do not necessarily have to be
specified in the select list. That is, they can cause the query to temporarily
project a column for the purposes of ordering, which is then removed prior
to the results being sent to the user. These temporary ordering expressions
must comply to the rules and constraints of any SQL operators that come before
them.

The example below shows this where the expressions ``a`` and ``b/2`` are
selected, but the result set is ordered by column ``c``. When this query
executes, columns ``a``, ``b`` and ``c`` are read from table ``t1``. The
expression ``b/2`` is calculated and the results are then ordered by
column expression ``c``. This column is then stripped from the results
before it is sent to the user.

.. code:: sql

  SELECT a, b/2 FROM t1
  ORDER BY c ;

The ``ORDER BY`` clause always executes after any set operations.
For example, in the following query the position of the ``ORDER BY``
clause makes it appear that it applies to the second query reading
from table ``t2``.

.. code:: sql

  SELECT a, b FROM t1
  UNION ALL
  SELECT a, b FROM t2 ORDER BY a ;

However this is not the case as the ``ORDER BY`` clause is applied
to the result of any set operations. We can more clearly rewrite
the query like this:

.. code:: sql

  (
    SELECT a, b FROM t1
    UNION ALL
    SELECT a, b FROM t2
  )
  ORDER BY a ;

.. attention::

  RapidsSE supports ordering by multiple expressions, however it currently
  only supports ordering in the ascending direction only.


.. index::
   single: queries; LIMIT clause
   single: LIMIT (clause)

.. _limit:

LIMIT Clause
````````````
The ``LIMIT`` clause reduces the number of rows that appear in the result set
of a query. The ``LIMIT`` clause requires an integer parameter to specify
the maximum number of rows to be returned. If the result set would normally
return less rows than the limit specified then the ``LIMIT`` clause has no
effect.

The ``LIMIT`` clause looks like this:

.. code:: sql

  SELECT a, b FROM t1 LIMIT 10 ;

The above example will only include the first 10 rows in the result set. All
other rows will be dropped.

Typically the ``LIMIT`` operator is used in conjunction with the ``ORDER
BY`` operator to select the top or botton *N* number of rows. For example,
to get the top 10 employees with the highest salary, one could write:

.. code:: sql

  SELECT name, salary
  FROM employees
  ORDER BY salary DESC
  LIMIT 10 ;

When a ``LIMIT`` clause is used without an ``ORDER BY`` clause it is not
deterministic which rows will be returned as the ``LIMIT`` operator will
simply count the rows that it reads first.

The ``LIMIT`` clause can also be specified with an ``OFFSET``. The ``OFFSET``
keyword requires an integer parameter that specifies how many rows to skip
(and not return in the result set) before counting rows towards the limit.

For example, to expand upon the previous example, let's say we wish to see
the next 10 employees with the highest salarys. That is, the 11th to 20th
highest paid employees. We could write this as:

.. code:: sql

    SELECT name, salary
    FROM employees
    ORDER BY salary DESC
    LIMIT 10 OFFSET 10 ;

The ``OFFSET 10`` will skip the first 10 rows in the result set and not
include them in the result. The next 10 rows received will then be included
towards the limit, stopping after the 20th row is processed by the ``LIMIT``
operator. If there were only 15 employees then the query would only return
5 rows of the employees in 11th to 15th position.

The ``LIMIT`` clause always executes after any set operations.
For example, in the following query the position of the ``LIMIT``
clause makes it appear that it applies to the second query reading
from table ``t2``.

.. code:: sql

  SELECT a, b FROM t1
  UNION ALL
  SELECT a, b FROM t2 LIMIT 10 ;

However this is not the case as the ``LIMIT`` clause is applied
to the result of any set operations. We can more clearly rewrite
the query like this:

.. code:: sql

  (
    SELECT a, b FROM t1
    UNION ALL
    SELECT a, b FROM t2
  )
  LIMIT 10 ;


.. attention::

  RapidsSE currently does not support the ``OFFSET`` clause.


.. index::
   single: queries; subqueries
   single: subqueries
   single: WITH (clause)

.. _subqueries:

Subqueries (Common Table Expressions)
`````````````````````````````````````
Common Table Expression (CTE) subqueries are a separate query (or queries)
that execute in the context of another query. A CTE subquery produces results
that look and behave like tables. Sometimes they can be pre-computed
beforehand and the results can be used multiple times in a larger outer query.

CTE subqueries can be expressed in a ``WITH`` clause or as another ``SELECT``
query specified in parentheses in the ``FROM`` clause. Subqueries in the
``WITH`` clause must be given a name so they can be referred to from the
parent query. The syntax looks like this:

.. code:: sql

  [ WITH
     <subqueryName> AS ( <subquery> )
     [ , ... ]
  ] <query> ;

Multiple subqueries can be specified in a ``WITH`` clause by using a comma
after the closing parenthesis of the subquery.

A subquery given in a ``FROM`` clause looks has syntax like this:

.. code:: sql

  SELECT <selectList>
  FROM ( <subquery> )
  ... ;

Here is an example of specifying and using subquery in a ``WITH`` clause.
This example first calculates the average salary of employees in each
department. It then uses that to list all of the employees whose salary is
less than the department average. The subquery is used in the ``JOIN`` clause.

.. code:: sql

  WITH deptSalaryAvg AS (
    SELECT departmentId, AVG(salary) AS avgSalary
    FROM employees
    GROUP BY departmentId
  )
  SELECT name, salary
  FROM employees
  JOIN deptSalaryAvg
  USING departmentId
  WHERE employees.salary < deptSalaryAvg.avgSalary ;

The query below shows an example of defining a subquery in a ``FROM`` clause.
This example uses the subquery to select the top 10 employees with the
highest salary, however the outer query then reorders them to display them
in ascending order.

.. code:: sql

  SELECT *
  FROM (
    SELECT name, salary
    FROM employees
    ORDER BY salary DESC
    LIMIT 10
  )
  ORDER BY salary ASC ;

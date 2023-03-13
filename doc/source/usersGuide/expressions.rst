.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: expressions; overview

.. _expressions:

Expressions
===========

Overview
--------
An expression is a symbolic representation of a calculation. Expressions are
used extensively in ``SELECT`` queries, both to specify the :ref:`data to be
returned <selectlist>` but also how to :ref:`filter <where>` and
:ref:`combine <join>` data when the query executes (as well as in many other
operators).

An expression can be as simple as ``1 + 2``. This expression contains two
literal values and the addition operator. Expressions can also refer to columns
of tables, such as ``columnA + 1`` which means to add ``1`` to the value of
``columnA`` in the current row.

The taxonomy of expressions in RapidsSE can include:

* literal values, including ``NULL``\s;
* references to columns;
* expression operators, such as ``+`` and ``-``;
* aggregations, such as ``MAX()`` and ``COUNT()``;
* references to expression aliases; and
* expression subqueries

Expressions are typically symbolic (that is, they can contain names of
references to columns and other expressions). When the query is being
executed, these symbolic expressions are evaluated in the context of each row
and a final value is determined. For example, a column reference is replaced by
the value of that column for the current row. The final value may evaluate to
an integer literal, a string literal or some other data type, but that result
can also be considered to be an expression too.

All expressions have a :ref:`data type <dataTypes>`, even in their symbolic
form. For example, the type of a column reference is determined by looking up
the metadata for the table that the column belongs to.

Some expression operators work on a variety of data types, whereas other
expression operators will only work on particular data types. For example,
the ``COUNT()`` operator will work on all data types, but the ``LIKE`` operator
will only work on string types.

In some expressions the types of the operands need to be converted in order to
make the expression executable. RapidsSE will automatically (implicitly)
:ref:`coerce <implicitTypeCoercion>` data types where it is required and
unambiguous. In other cases it may require the user to manually and
explicitly :ref:`cast <explicitTypeCasting>` an expression to another type for
RapidsSE to be able to execute it. If this is required, RapidsSE will return an
error indicating that there is a type mismatch in the expression on a
particular operator.


.. index::
   single: expressions; nulls
   single: nulls (expressions)

.. _nulls:

Nulls
-----
When an expression is evaluated, it can return a range of values as described
by the resulting :ref:`data type <dataTypes>`. An expression can also return a
``NULL`` value too. A ``NULL`` value is a special value that effectively
means "unknown".

A ``NULL`` *value* is not to be confused with an expression's *data type*. In fact,
a ``NULL`` value can be evaluated from an expression with any data type.

In many cases, an expression that involves a ``NULL`` operand will often result
in a ``NULL`` output. For example, consider the following expression:

.. code:: sql

  columnA + columnB

If the value of ``columnA`` for the current row is ``NULL`` then the result
will always be ``NULL`` regardless of the value of ``columnB``.

Similarly, logical expression operators are also affected by ``NULL`` values
too, as shown by the truth table below:

.. index::
   single: expressions; truth table

.. _truthTableNulls:

.. table:: Truth table for logical operators with ``NULL``\s
   :name: logical-and-truth-table
   :class: centered

   ======= ======= ========= ======== ========= =======
      A       B     A AND B   A OR B   A XOR B   NOT A
   ======= ======= ========= ======== ========= =======
    TRUE    TRUE     TRUE      TRUE     FALSE    FALSE
    TRUE    FALSE    FALSE     TRUE     TRUE     FALSE
    TRUE    NULL     NULL      TRUE     NULL     FALSE
    FALSE   TRUE     FALSE     TRUE     TRUE     TRUE
    FALSE   FALSE    FALSE     FALSE    FALSE    TRUE
    FALSE   NULL     FALSE     NULL     NULL     TRUE
    NULL    TRUE     NULL      TRUE     NULL     NULL
    NULL    FALSE    FALSE     NULL     NULL     NULL
    NULL    NULL     NULL      NULL     NULL     NULL
   ======= ======= ========= ======== ========= =======

A ``NULL`` literal in an expression is simply written as ``NULL``. It's type
will automatically be coerced to match other operands around it. For
example, given the expression ``NULL + 1`` then the ``NULL`` will be treated
as an integer data type to match the other integer operand ``1``.

.. index::
   single: expressions; literals
   single: literals (expressions)

.. _literals:

Literals
--------
SQL literals are written in a particular way to convey their value as well as
their type. As RapidsSE supports three main numeric types (``INTEGER``,
``DECIMAL`` and ``FLOAT``), care must be taken with how literal values are
written as their type can affect the output value.

For example, ``FLOAT`` types have approximate values so not all numbers in the
supported range can be represented. Meanwhile ``INTEGER`` has no fractional
components, so a calculation involving it may lack the necessary accuracy.


.. index::
   single: BOOLEAN (expression literal)

.. rubric:: ``BOOLEAN``

``BOOLEAN`` values are simply written as ``true`` or ``false``, case
insensitive. Do not enclose them in quotes otherwise they will be treated
as strings.


.. index::
   single: CHAR (expression literal)
   single: VARCHAR (expression literal)

.. rubric:: ``CHAR`` and ``VARCHAR``

The ``CHAR`` and ``VARCHAR`` string types are enclosed in single quotes like
this: ``'This is a string'``. If the string requires a single quote inside it
then it must be escaped by repeating the same character.

For example, the string ``Craig's cat`` would be represented in a literal as:
``'Craig''s cat'``.

.. index::
   single: DATE (expression literal)

.. rubric:: ``DATE``

A ``DATE`` literal is represented in the form:

.. code:: sql

   DATE 'yyyy-mm-dd'

where ``yyyy`` represents the year in 4 digits, ``mm`` represents the month in 2
digits, and ``dd`` represents the day of the month in 2 digits. The ``DATE``
keyword and a space must appear before the first single quote so that RapidsSE
does not interpret the value as a string.

.. index::
   single: DECIMAL (expression literal)

.. rubric:: ``DECIMAL``

A ``DECIMAL`` literal is typically written as a digit followed by a decimal
point, then zero or more digits. The decimal point is what distinguishes a
``DECIMAL`` literal from an ``INTEGER`` literal, and the lack of an exponent
is what distinguishes a ``DECIMAL`` literal from a ``FLOAT`` literal.

Examples of ``DECIMAL`` literals include:

  ``123.45``

  ``.1234``

  ``123.``

.. index::
   single: FLOAT (expression literal)

.. rubric:: ``FLOAT``

A ``FLOAT`` literal is often confused with ``DECIMAL`` literals. While a
``FLOAT`` literal may or may not contain a decimal point, it must include
an exponent. That is, a ``FLOAT`` literal is always written in scientific
notation.

The exponent is always separated from the mantissa by an ``e`` or ``E``
(case insensitive). This may be followed by a ``-`` or ``+`` before the
exponent value is included.

Examples of ``FLOAT`` literals (with values similar to the ``DECIMAL``
examples previously shown) include:

  ``12345.e-2``

  ``.1234e0``

  ``12.3e+1``


.. index::
   single: INTEGER (expression literal)

.. rubric:: ``INTEGER``

An ``INTEGER`` literal is simply written as a whole number. For example ``123``
or ``-24``. There is no fractional component or exponent.


.. index::
   single: INTERVALs (expression literal)

.. rubric:: ``INTERVAL``

An ``INTERVAL`` is represented in the form:

.. code:: sql

   INTERVAL '<value>' <interval_type>

Where ``<interval_type>`` is one of:

.. code:: sql

   ``YEAR``
   ``MONTH``
   ``YEAR TO MONTH``
   ``DAY``
   ``HOUR``
   ``MINUTE``
   ``SECOND``
   ``DAY TO HOUR``
   ``DAY TO MINUTE``
   ``DAY TO SECOND``
   ``HOUR TO MINUTE``
   ``HOUR TO SECOND``
   ``MINUTE TO SECOND``

And ``<value>`` is enclosed in single quotes and fits one of the following formats
below (the enclosing quotes are also shown). Any ``<seconds>`` field includes
optional fractional seconds up to 6 decimal places.

  | ``'<years>'``
  | ``'<months>'``
  | ``'<years>-<months>'``
  | ``'<days>'``
  | ``'<hours>'``
  | ``'<minutes>'``
  | ``'<seconds>'``
  | ``'<days> <hours>'``
  | ``'<days> <hours>:<minutes>'``
  | ``'<days> <hours>:<minutes>:<seconds>'``
  | ``'<hours>:<minutes>'``
  | ``'<hours>:<minutes>:<seconds>'``
  | ``'<minutes>:<seconds>'``

Examples of interval literals include:

.. code:: sql

   INTERVAL '1' YEAR
   INTERVAL '2' MONTH
   INTERVAL '1-2' YEAR TO MONTH
   INTERVAL '3' DAY
   INTERVAL '4' HOUR
   INTERVAL '5' MIN
   INTERVAL '6' SECOND
   INTERVAL '6.789012' SECOND
   INTERVAL '3 4' DAY TO HOUR
   INTERVAL '3 4:5' DAY TO MINUTE
   INTERVAL '3 4:5:6' DAY TO SECOND
   INTERVAL '3 4:5:6.789012' DAY TO SECOND
   INTERVAL '4:5' HOUR TO MINUTE
   INTERVAL '4:5:6.789012' HOUR TO SECOND
   INTERVAL '5:6.789012' MINUTE TO SECOND

A negative interval value can be represented by placing the minus sign (``-``)
immediately before the opening single quote. For example:

.. code:: sql

   INTERVAL -'1 2:3:4' DAY TO SECOND


.. index::
   single: TIME (expression literal)

.. rubric:: ``TIME``

A ``TIME`` literal is represented in the form:

.. code:: sql

   TIME 'hh:mm:ss'

where ``hh`` represents the hour in 2 digit 24 hour representation, ``mm``
represents the minutes in 2 digits, and ``ss`` represents the whole in 2
digits. The ``TIME`` keyword and a space must appear before the first
single quote so that RapidsSE does not interpret the value as a string.


.. index::
   single: TIMESTAMP (expression literal)

.. rubric:: ``TIMESTAMP``

A ``TIMESTAMP`` literal is represented in the form:

.. code:: sql

   TIMESTAMP 'yyyy-mm-dd hh:mm:ss[.ffffff]'

where:
  ``yyyy`` represents the year in 4 digits,

  ``mm`` represents the month in 2 digits,

  ``dd`` represents the day of the month in 2 digits

  ``hh`` represents the hour in 2 digit 24 hour representation,

  ``mm`` represents the minutes in 2 digits,

  ``ss`` represents the whole in 2 digits, and

  ``ffffff`` represents the optional fractional microseconds. The decimal
  point and fractional component can be left off entirely.

The ``TIMESTAMP`` keyword and a space must appear before the first single quote
so that RapidsSE does not interpret the value as a string.


Named References
----------------
An expression can contain a reference by name to a value. This is often
represented by a column reference or an alias reference.


.. index::
   single: expressions; column references
   single: column references (expressions)

.. _columnReference:

Column References
`````````````````
A column reference is an expression that refers to a column in a table. When
the expression is evaluated, the expression is replaced by the value of that
column for the current row. A column reference is expressed simply by the name
of the column, or by the qualified name of the column if the column name may
be ambiguous.

For example, below are four ways of referencing a column, ``columnA``, with
differing levels of qualification.

.. code:: sql

  columnA
  mytable.columnA
  mySchema.myTable.columnA
  myCatalog.mySchema.myTable.columnA

If an unqualified column reference is provided and that unqualified column name
is ambiguous (for example, the same column name appears in two tables in the
query), then RapidsSE will issue an error that the column name is ambiguous.

If a query makes use of a :ref:`table alias <tableAlias>` to simplify a table
name, and the query wishes to qualify an associated column reference, then the
column reference *must* use the table's alias and not the original table name.

For example, this query is incorrect because the column reference is qualified
against the original table name when that table name has been aliased.

.. code:: sql

  SELECT nation.n_name FROM nation n ;        (INCORRECT!)

This is the above query re-written correctly:

.. code:: sql

  SELECT n.n_name FROM nation n ;             (CORRECT!)


.. index::
   single: expressions; aliases
   single: aliases (expressions)

.. _expressionAliases:

Expression Alias References
```````````````````````````
A complicated expression in a :ref:`select list <selectlist>` can be given an
alias by which it is known, and this alias can be used in other expressions to
avoid re-typing the complex original expression. This is specified in the
select list by using the ``AS`` clause, as shown below:

.. code:: sql

  SELECT a, SUM(b) AS total
  FROM t1
  GROUP BY a ;

This alias can then be used in other expressions, however it can only be used
in expressions that are executed after the point where the aliased expression
is projected. Based on the above example, the following example is incorrect
and would give an error message indicating that the aliased name is unknown.

.. code:: sql

  SELECT a, SUM(b) AS total
  FROM t1
  WHERE total > 0
  GROUP BY a ;

The reason the above example is incorrect is because the alias ``total`` is
defined at the point where the aggregation ``SUM(b)`` occurs, and this happens
*after* the execution of the ``WHERE`` clause. At the point of the ``WHERE``
clause there are no columns or aliased expressions known as ``total``.

The following is a correctly written example of this query:

.. code:: sql

  SELECT a, SUM(b) AS total
  FROM t1
  GROUP BY a
  HAVING total > 0 ;

This query is valid because the ``HAVING`` clause executes *after* the
aggregation and alias is defined, therefore the ``HAVING`` operator receives
rows where one of the projected columns is known as ``total``. The aliased
name can be successfully resolved.

Typically expression aliases can be referenced in expressions in the
``GROUP BY``, ``HAVING`` and ``ORDER BY`` clauses. Expression aliases are
typically not resolvable in a ``WHERE`` clause.


.. index::
   single: expressions; keywords
   single: keywords (expressions)

.. _exprKeywords:

Keywords
--------
RapidsSE does not currently support expression keywords such as ``NOW()``,
``CURRENT_TIMESTAMP`` or ``CURRENT_USER``.


.. index::
   single: expressions; supported operators
   single: operators (expressions)

.. _exprOperators:

Supported Expression Operators
------------------------------
RapidsSE supports a variety of expression operators, with more operators being
added regularly. These are described in the following sub-categories.

In the following subsections the syntax descriptions use angle brackets 
(``<`` ``>``) to represent expressions of specific or arbitrary data
types. 

For example, ``<expr>`` is used to represent an expression of any data type,
whereas ``<booleanExpr>`` is used to represent an expression that evaluates
to a boolean value.

The syntax ``-->`` is used to indicate the resulting data type that will be 
returned from the operator.


.. index::
   single: expressions; logical operators
   single: logical operators (expressions)
   single: AND (expression operator)
   single: OR (expression operator)
   single: XOR (expression operator)
   single: NOT (expression operator)

.. _logicalOperators:

Logical Operators
`````````````````
RapidsSE supports the following logical operators:

* ``AND``
* ``OR``
* ``XOR``
* ``NOT``

``AND``, ``OR`` and ``NOT`` are the basic logical operators.

``XOR`` is a logical exclusive-OR operator. Refer to
:ref:`this truth table <truthTableNulls>` for details.

The syntax for these operators are as follows:

.. code:: sql

  <booleanExpr> AND <booleanExpr>   --> <booleanValue>
  <booleanExpr> OR  <booleanExpr>   --> <booleanValue>
  <booleanExpr> XOR <booleanExpr>   --> <booleanValue>
                NOT <booleanExpr>   --> <booleanValue>

RapidsSE uses :ref:`three-valued logic <nulls>` when it comes to evaluating
logical expressions that can contain ``NULL``\s. Refer to this
:ref:`truth table <truthTableNulls>` for details.

.. index::
   single: expressions; mathematical operators
   single: mathematical operators (expressions)
   single: addition (expression operator)
   single: subtraction (expression operator)
   single: multiplication (expression operator)
   single: division (expression operator)
   single: modulus (expression operator)
   single: unary plus (expression operator)
   single: unary negation (expression operator)

.. _mathematicalOperators:

Mathematical Operators
``````````````````````
RapidsSE supports the following mathematical operators:

* ``+`` (addition)
* ``-`` (subtraction)
* ``*`` (multiplication)
* ``/`` (division)
* ``%`` (modulus)
* ``+`` (unary plus)
* ``-`` (unary negation)

Apart from the modulus operator, ``%``, all other mathematical operators
accept any of the numeric types for their operands: ``INTEGER``, ``DECIMAL``
and ``FLOAT``. Where the operands have differing numeric types, the output type
is determined by the RapidsSE compiler
:ref:`type coercion <implicitTypeCoercion>` rules.

If necessary, the compiler will automatically convert the data type of an
operand so that all operands are of the same type. An ``INTEGER`` may be
promoted to a ``DECIMAL``, which may be promoted to a ``FLOAT``.

The modulus operator only accepts ``INTEGER`` operands.

The syntax for these operators are as follows:

.. code:: sql

  <numericExpr> + <numericExpr>    --> <numericValue>    (addition)
  <numericExpr> - <numericExpr>    --> <numericValue>    (subtraction)
  <numericExpr> * <numericExpr>    --> <numericValue>    (multiplication)
  <numericExpr> / <numericExpr>    --> <numericValue>    (division)
  <integerExpr> % <integerExpr>    --> <integerValue>    (modulus)
                + <numericExpr>    --> <numericValue>    (unary plus)
                - <numericExpr>    --> <numericValue>    (unary negation)


.. index::
   single: expressions; comparison operators
   single: comparison operators (expressions)
   single: equality (expression operator)
   single: inequality (expression operator)
   single: less-than (expression operator)
   single: less-than-or-equal-to (expression operator)
   single: greater-than (expression operator)
   single: greater-than-or-equal-to (expression operator)
   single: IS NULL (expression operator)
   single: BETWEEN (expression operator)

.. _comparisonOperators:

Comparison Operators
````````````````````
RapidsSE supports the following comparison operators:

* ``=``
* ``!=``
* ``<>``
* ``<``
* ``<=``
* ``>``
* ``>=``
* ``IS NULL``
* ``IS NOT NULL``
* ``BETWEEN``

The ``=`` and ``!=`` operators are equality and inequality operators. The
``<>`` operator is synonymous with the ``!=`` operator.

The ``<``, ``<=``, ``>`` and ``>=`` are the less-than, less-than-or-equal-to,
greater-than and greater-than-or-equal-to.

The ``BETWEEN`` operator is semantically equivalent to using the ``>=`` and
``<=`` operators directly. For example, the following two expressions are
equivalent:

.. code:: sql

  columnA >= 1 AND columnA <= 10
  columnA BETWEEN 1 AND 10


``IS NULL`` and ``IS NOT NULL`` return a ``BOOLEAN`` true/false value depending
whether the operand has a ``NULL`` value or not.

The syntax for these operators are as follows:

.. code:: sql

   <expr> =  <expr>                   --> <booleanValue>
   <expr> != <expr>                   --> <booleanValue>
   <expr> <> <expr>                   --> <booleanValue>
   <expr> <  <expr>                   --> <booleanValue>
   <expr> <= <expr>                   --> <booleanValue>
   <expr> >  <expr>                   --> <booleanValue>
   <expr> >= <expr>                   --> <booleanValue>
   <booleanExpr> IS NULL              --> <booleanValue>
   <booleanExpr> IS NOT NULL          --> <booleanValue>
   <expr> BETWEEN <expr> AND <expr>   --> <booleanValue>


.. index::
   single: expressions; conditional operators
   single: conditional operators (expressions)
   single: CASE (expression operator)

.. _conditionalOperators:

Conditional Operators
`````````````````````
RapidsSE supports the following conditional operators:

* ``CASE``


``CASE`` Operator
:::::::::::::::::

The ``CASE`` operator is similar to a programming ``if...then...else``
construct using ``WHEN``, ``THEN`` and ``ELSE`` keywords instead.
It evaluates expressions in ``WHEN`` clauses and if they evaluate to
``true`` then the value in the corresponding ``THEN`` clause is returned.
If none of the ``WHEN`` conditions are met then the value in the ``ELSE``
clause is returned if one is specified. Otherwise ``NULL`` is returned.

There are two syntactic ways to write ``CASE`` statement. The first one has
an expression after the ``CASE`` keyword and before the first ``WHEN`` keyword.
This expression is evaluated and compared against the expressions in each of
the ``WHEN`` statements.

.. code:: sql

  CASE <expr>
    WHEN <expr> THEN <expr>
    [ WHEN ... ]
    [ ELSE <expr> ]
  END

The second variant of the ``CASE`` operator does not have an expression
immediately after the ``CASE`` keyword. Instead a boolean expression is
specified after each ``WHEN`` keyword. Multiple ``WHEN`` expressions need not
be related in any way, other than the fact that they must evaluate to a
boolean value.

.. code:: sql

  CASE
    WHEN <booleanExpr> THEN <expr>
    [ WHEN ... ]
    [ ELSE <expr> ]
  END

The following two examples illustrate the two different ways a ``CASE``
operator can be written. The two examples are equivalent.

.. code:: sql

  CASE n_nation
    WHEN 'Australia' THEN 'G''day'
    WHEN 'China'     THEN 'Ni hao'
    WHEN 'India'     THEN 'Namaste'
    ELSE 'Hello'
  END

.. code:: sql

  CASE
    WHEN n_nation = 'Australia' THEN 'G''day'
    WHEN n_nation = 'China'     THEN 'Ni hao'
    WHEN n_nation = 'India'     THEN 'Namaste'
    ELSE 'Hello'
  END

The ``ELSE`` clause is optional in a ``CASE`` statement. If it is left off and
none of the ``WHEN`` conditions are matched then the ``CASE`` statement will
return a ``NULL`` value.

The ``CASE`` statement must be consistent with the data types of values that
can be returned. The data type of all ``THEN`` and ``ELSE`` expressions must
be the same or :ref:`implicitly coercible <implicitTypeCoercion>`.


.. index::
   single: expressions; string operators
   single: string operators (expressions)

.. _stringOperators:

String Operators
````````````````
RapidsSE supports the following string operators:

* ``LIKE``
* ``SUBSTRING``

.. index::
   single: LIKE (expression operator)

LIKE Operator
:::::::::::::
The ``LIKE`` operator allows a string expression to be compared against
another string or pattern. It always returns a boolean data type (including the
``NULL`` value).

The syntax for the ``LIKE`` operators is as follows:

.. code:: sql

  <stringExpr> [NOT] LIKE <stringPatternExpr> [ ESCAPE <stringEscapeExpr> ]

The ``<stringExpr>`` is compared against the pattern in ``<stringPatternExpr>``
with an optional escape character in ``<stringEscapeExpr>``.

If the value in ``<stringPatternExpr>`` contains any underscore (``_``) or
percent (``%``) characters, these are interpreted to mean the following:

  ``_``   matches any single character

  ``%``   matches a sequence of zero or more characters

If the value in ``<stringPatternExpr>`` does not contain any underscore or
percent characters then it is compared against ``<stringExpr>`` as if the
equality operator (``=``) was being used.

Here are some examples:

.. code:: sql

  'abcd' LIKE 'abcd'        (returns true)
  'abcd' LIKE 'abc'         (returns false)
  'abcd' LIKE 'abcde'       (returns false)
  'abcd' LIKE 'a_cd'        (returns true)
  'abcd' LIKE 'a_c'         (returns false)
  'abcd' LIKE 'a__d'        (returns true)
  'abcd' LIKE 'a%'          (returns true)
  'abcd' LIKE 'a%d'         (returns true)
  'abcd' LIKE 'a_%d'        (returns true)
  'abcd' LIKE '_%d'         (returns true)
  'abcd' LIKE '%b%'         (returns true)
  'abcd' LIKE '%x%'         (returns false)

If you wish to include a literal underscore or percent character in the pattern
and do not want the character being interpreted then the character needs to be
escaped. This is done by specifying the character in the ``ESCAPE`` clause and
then using that character in the pattern immediately before the ``_`` or ``%``
character that is to be interpreted literally. The expression in the ``ESCAPE``
clause must evaluate to a single character string.

If no ``ESCAPE`` clause is specified then the special interpretations of ``_``
and ``%`` will apply.

To match the ``ESCAPE`` character in the pattern, simply repeat the ``ESCAPE``
character.

Here are some examples using ``ESCAPE`` characters:

.. code:: sql

  'ab%d' LIKE 'ab_d'                  (returns true. Using the pattern)
  'ab%d' LIKE 'ab#_d'  ESCAPE '#'     (returns false)
  'ab_d' LIKE 'ab#_d'  ESCAPE '#'     (returns true)
  'ab%d' LIKE 'ab#%d'  ESCAPE '#'     (returns true)
  'ab%'  LIKE 'ab#%%'  ESCAPE '#'     (returns true. Only first '%' is escaped.)
  'ab#%' LIKE 'ab###%' ESCAPE '#'     (returns true)


.. index::
   single: SUBSTRING (expression operator)

SUBSTRING Operator
::::::::::::::::::
The ``SUBSTRING`` operator returns a portion of a string value starting at a
given index. It always returns a value of a string data type (``CHAR`` or
``VARCHAR``).

The keyword ``SUBSTR`` can be used interchangably with the keyword ``SUBSTRING``.

The syntax for the ``SUBSTRING`` operators is as follows:

.. code:: sql

  SUBSTRING '(' <stringExpr> FROM <integerExpr> [ FOR <integerExpr> ] ')'
  SUBSTRING '(' <stringExpr>, <integerExpr> [ , <integerExpr> ] ')'

The first parameter of the ``SUBSTRING`` function is the the string expression
from which a substring will be extracted. The second parameter is the 1-based
index of the first character to extract. This parameter must be an integer.
The optional third parameter, also an integer, is the number of characters to
be extracted. If this third parameter is left off then all characters until
the end of the string will be returned.


.. index::
   single: expressions; date and time operators
   single: date and time operators (expressions)
   single: EXTRACT (expression operator)

.. _datetimeOperators:

Date and Time Operators
```````````````````````
RapidsSE supports the following string operators:

* ``EXTRACT``

``EXTRACT`` Operator
::::::::::::::::::::
The ``EXTRACT`` operator returns an integer value corresponding to one of the
fields from a ``DATE``, ``TIME`` or ``TIMESTAMP`` expression.

The syntax for the ``EXTRACT`` operator is as follows:

.. code:: sql

  EXTRACT '(' (SECOND | MINUTE | HOUR | DAY | MONTH | YEAR) FROM <expr> ')'

The field to be extracted must be compatible with the type of the expression.
For example, you cannot try to extract a ``SECOND`` field from a ``DATE``
expression. This operator is only applicable to ``DATE``, ``TIME`` or
``TIMESTAMP`` expressions.

Examples:

.. code:: sql

  EXTRACT (HOUR FROM time '14:23:54')                   (returns 14)
  EXTRACT (DAY FROM timestamp '2023-01-20 14:23:54')    (returns 20)
  EXTRACT (YEAR FROM timestamp '2023-01-20 14:23:54')   (returns 2023)


.. index::
   single: expressions; aggregate operators
   single: aggregate operators (expressions)
   single: COUNT (expression operator)
   single: MIN (expression operator)
   single: MAX (expression operator)
   single: SUM (expression operator)
   single: AVG (expression operator)

.. _aggregateOperators:

Aggregate Operators
```````````````````
An aggregate operator receives a stream of rows and at the end of this stream
it emits one or more rows with computed aggregations across all of its input
rows.

RapidsSE supports the following aggregation expression operators:

* ``COUNT``
* ``MIN``
* ``MAX``
* ``SUM``
* ``AVG``

Without the presence of a ``GROUP BY`` :ref:`clause <groupby>`, each
aggregate expression will emit one row for all input rows it receives
(a grand total aggregate). However, if the query was specified with a
``GROUP BY`` clause then the aggregates will emit a single row for each unique
grouping value. Refer to the ``GROUP BY`` :ref:`clause <groupby>` for
more details.

The syntax for these aggregate operators is as follows:

.. code:: sql

  COUNT(*)              --> <integerExpr>
  COUNT(<expr>)         --> <integerExpr>
  MIN(<expr>)           --> <expr>
  MAX(<expr>)           --> <expr>
  SUM(<numericExpr>)    --> <numericExpr>
  AVG(<integerExpr>)    --> <decimalExpr>
  AVG(<decimalExpr>)    --> <decimalExpr>
  AVG(<floatExpr>)      --> <floatExpr>

The ``COUNT(*)`` aggregation will return a count of the number of rows it
receives. The ``COUNT(<expr>)`` form will only count the number of rows where
``<expr>`` is not ``NULL``.

The ``MIN`` and ``MAX`` aggregates work on a variety of data types, including
numerics (``INTEGER``, ``DECIMAL`` and ``FLOAT``), datetimes (``DATE``, ``TIME``,
``TIMESTAMP``) and string types (``CHAR`` and ``VARCHAR``).

The ``SUM`` aggregate works on numeric data types and the returned data type
is the same as the input data type. For example a ``SUM`` of a decimal
expression will return a decimal expression.

The ``AVG`` aggregate only works on numeric data types. The returned data type
always contains fractional precision but it depends on whether the input data
type is an exact numeric or an approximate numeric type. If the input data
type is an ``INTEGER`` or ``DECIMAL`` (both are exact numerics) the the output
data type will be a ``DECIMAL``. However if the input data type is a ``FLOAT``
(an approximate numeric type) then the output data type will also be a
``FLOAT``.

.. note::

   RapidsSE does not currently support the ``COUNT`` function with the
   ``DISTINCT`` keyword. That is, expressions of the form
   ``COUNT(DISTINCT <expr>)``


.. index::
   single: expressions; casting operator
   single: casting; operator
   single: CAST (expression operator)

.. _typeCastingOperators:

Data Type Casting Operators
```````````````````````````
RapidsSE supports the ability to explicitly convert the data type of an
expression to an alternate type. This can be done with the use of the
``CAST`` expression operator.

The syntax supported for casting is as follows:

.. code:: sql

  CAST '(' <expr> AS <datatype> ')'         --> <datatype>
  '(' <datatype> ')' <expr>                 --> <datatype>

In the above syntax, ``<datatype>`` is a valid RapidsSE
:ref:`data type <dataTypes>`.

.. note:
  Not all data types can be converted to others. Please refer to
  :ref:`this table <explicit-type-casting-table>` for details on supported type
  conversions.

Examples:

.. code:: sql

  (1)   CAST(col1 + 2 AS VARCHAR)
  (2)   (VARCHAR) col1 + 2
  (3)   (VARCHAR) (col1 + 2)

In example (1) above, the expression ``col1 + 2`` is evaluated and then
converted to a ``VARCHAR`` representation.

Example (2) is similar, but slightly different. In this case only ``col1``
is cast to a ``VARCHAR`` before ``2`` is attempted to be added to it. This
will result in an error as we are trying to add a string and an integer
type together.

Example (3) shows a correction to example (2) where the entire ``col1 + 2``
expression is converted to a ``VARCHAR``. This is the same as example (1).


.. index::
   single: expressions; subquery operators
   single: subquery operators (expressions)

.. _subqueryOperators:

Subquery Operators
``````````````````
RapidsSE supports the following subquery expression operators:

* ``EXISTS``
* ``IN``

These subquery operators are described in the following sub-sections.

.. index::
   single: EXISTS (expression operator)

EXISTS Operator
:::::::::::::::
The ``EXISTS`` operator executes a subquery and returns a boolean value.
The value ``true`` is returned if the subquery would return any rows, and
``false`` if the subquery returns an empty result set. None of the returned
rows are used in the parent query or the ``EXISTS`` clause, simply the presence
of rows from the subquery.

The syntax for the ``EXISTS`` operator is as follows:

.. code:: sql

  EXISTS '(' <subquery> ')'     --> <booleanExpr>

The subquery being executed may also refer to variables from the parent query
(the one containing the ``EXISTS`` operator). This is called a correlated
subquery. A correlated subquery places constraints on the subquery being executed.
It also means that in a pure logical or naive implementation the subquery needs to
be executed for each row being processed by the parent query.

A subquery that does not refer to any variables from the parent query is an
uncorrelated subquery. Uncorrelated subqueries can be executed once and their
result set reused as many times as is needed for the query.

Since it is not important what data the subquery returns, it is common practice
to write ``EXISTS`` subqueries in this form:

.. code:: sql

  EXISTS ( SELECT 1 FROM ... WHERE ... )

Below is a contrived example that shows the use of an ``EXISTS`` clause:

.. code:: sql

  SELECT e1.name
  FROM employees e1
  WHERE EXISTS (
    SELECT 1
    FROM employees e2
    WHERE e2.id = e1.managerId AND e2.managerId IS NULL
  ) ;

This example returns a list of employee names who are first reports. That is,
they have a manager, but their manager does not have manager. It is a
correlated subquery because the subquery refers to variables from the parent
subquery (``e1.managerId``). So this subquery cannot be calculated ahead of
time.


.. index::
   single: IN (expression operator)

IN Operator
:::::::::::
The ``IN`` operator executes a subquery and returns a boolean value.

The syntax for the ``IN`` operator is as follows:

.. code:: sql

  <expr> IN '(' <subquery> ')'     --> <booleanExpr>

The ``IN`` operator returns true if the current value of ``<expr>`` matches
the value of any of the rows returned by ``<subquery>``. The subquery must
return a result set with a single column only. It can return zero or more rows
in the result set. The ``IN`` operator returns false if the value of ``<expr>``
does not match *any* the value of any of the rows returned by the subquery.

The subquery being executed may also refer to variables from the parent query
(the one containing the ``IN`` operator). This is called a correlated
subquery. A correlated subquery places constraints on the subquery being executed.
It also means that in a pure logical or naive implementation the subquery needs to
be executed for each row being processed by the parent query.

A subquery that does not refer to any variables from the parent query is an
uncorrelated subquery. Uncorrelated subqueries can be executed once and their
result set reused as many times as is needed for the query.

Below is a contrived example that demonstrates the use of the ``IN`` operator:

.. code:: sql

  SELECT e.name
  FROM employees e
  WHERE e.departmentId IN (
    SELECT id
    FROM departments d
    WHERE d.name = 'SALES' OR d.name = 'ENGINEERING'
  ) ;

This example returns the name of all employees who are part of the sales or
engineering departments. The subquery returns the ``id`` of the sales and
engineering departments as two rows with a single column value each. The parent
query then compares the ``employees.departmentId`` column value against the
results of the subquery and returns the employee name if the ``WHERE``
predicate evaluates true. Note that this subquery is also an example of an
uncorrelated subquery. It can be calculated once and the result can be reused.


.. index::
   single: expressions; order of precedence
   single: order of precedence (expressions)

Operator Order of Precedence
----------------------------
RapidsSE interprets SQL expressions according to its inbuilt order of
precedence. This ordering is show in the table below, from highest precedence
to lowest. Brackets can be used to group expressions to form a different
order of precedence, if required.

.. table::
   :name: expr-operator-precedence-table

   ======================================== ==================================
   Operator	                                Description
   ======================================== ==================================
   ``(`` ``)``	                            Brackets (parentheses)
   ``(<dataType>) <expr>``                  Inline type cast
   ``+`` ``-``	                            Unary plus and unary minus
   ``*`` ``/`` ``%``	                      Multiplication, division, modulus
   ``+`` ``-``                              Addition, subtraction
   ``<`` ``<=`` ``=`` ``!=`` ``>=`` ``>``   Comparison operators
   ``<funcName>(<params>)``                 All other function-like operators
   ``IS NULL``, ``IS NOT NULL``             Comparison to SQL NULL
   ``LIKE``	                                String comparison
   ``EXISTS``	                              Existence discovery
   ``IN``	                                  Set membership
   ``BETWEEN``	                            Range comparison
   ``NOT``	                                Logical negation
   ``AND``	                                Logical AND
   ``XOR``	                                Logical XOR
   ``OR``	                                  Logical OR
   ======================================== ==================================


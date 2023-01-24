.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _expressions:

Expressions
===========

Overview
````````
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


.. _nulls:

Nulls
`````
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


.. _literals:

Literals
````````
SQL literals are written in a particular way to convey their value as well as
their type. As RapidsSE supports three main numeric types (``INTEGER``,
``DECIMAL`` and ``FLOAT``), care must be taken with how literal values are
written as their type can affect the output value.

For example, ``FLOAT`` types have approximate values so not all numbers in the
supported range can be represented. Meanwhile ``INTEGER`` has no fractional
components, so a calculation involving it may lack the necessary accuracy.


.. rubric:: ``BOOLEAN``

``BOOLEAN`` values are simply written as ``true`` or ``false``, case
insensitive. Do not enclose them in quotes otherwise they will be treated
as strings.


.. rubric:: ``CHAR`` and ``VARCHAR``

The ``CHAR`` and ``VARCHAR`` string types are enclosed in single quotes like
this: ``'This is a string'``. If the string requires a single quote inside it
then it must be escaped by repeating the same character.

For example, the string ``Craig's cat`` would be represented in a literal as:
``'Craig''s cat'``.


.. rubric:: ``DATE``

A ``DATE`` literal is represented in the form:

.. code:: sql

   DATE 'yyyy-mm-dd'

where ``yyyy`` represents the year in 4 digits, ``mm`` represents the month in 2
digits, and ``dd`` represents the day of the month in 2 digits. The ``DATE``
keyword and a space must appear before the first single quote so that RapidsSE
does not interpret the value as a string.


.. rubric:: ``DECIMAL``

A ``DECIMAL`` literal is typically written as a digit followed by a decimal
point, then zero or more digits. The decimal point is what distinguishes a
``DECIMAL`` literal from an ``INTEGER`` literal, and the lack of an exponent
is what distinguishes a ``DECIMAL`` literal from a ``FLOAT`` literal.

Examples of ``DECIMAL`` literals include:

  ``123.45``

  ``.1234``

  ``123.``


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


.. rubric:: ``INTEGER``

An ``INTEGER`` literal is simply written as a whole number. For example ``123``
or ``-24``. There is no fractional component or exponent.


.. rubric:: ``INTERVAL YEAR TO MONTH``

RapidsSE does not currently support interval types.


.. rubric:: ``INTERVAL DAY TO SECOND``

RapidsSE does not currently support interval types.


.. rubric:: ``TIME``

A ``TIME`` literal is represented in the form:

.. code:: sql

   TIME 'hh:mm:ss'

where ``hh`` represents the hour in 2 digit 24 hour representation, ``mm``
represents the minutes in 2 digits, and ``ss`` represents the whole in 2
digits. The ``TIME`` keyword and a space must appear before the first
single quote so that RapidsSE does not interpret the value as a string.


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



Supported Expression Operators
``````````````````````````````
RapidsSE supports a variety of expression operators, with more operators being
added regularly. These are described in the following sub-categories.

The syntax descriptions use angle brackets (``<`` ``>``) to represent data
types and separate them from the expression operators. The syntax ``-->``
is used to indicate the resulting data type that will be returned from the
operator.

Logical Operators
:::::::::::::::::
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

  <boolean> AND <boolean> --> <boolean>
   <boolean> OR <boolean> --> <boolean>
  <boolean> XOR <boolean> --> <boolean>
            NOT <boolean> --> <boolean>

RapidsSE uses :ref:`three-valued logic <nulls>` when it comes to evaluating
logical expressions that can contain ``NULL``\s. Refer to this
:ref:`truth table <truthTableNulls>` for details.


Mathematical Operators
::::::::::::::::::::::
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

  <numeric_type> + <numeric_type> --> <numeric_type>    (addition)
  <numeric_type> - <numeric_type> --> <numeric_type>    (subtraction)
  <numeric_type> * <numeric_type> --> <numeric_type>    (multiplication)
  <numeric_type> / <numeric_type> --> <numeric_type>    (division)
       <integer> % <integer>      --> <integer>         (modulus)
                 + <numeric_type> --> <numeric_type>    (unary plus)
                 - <numeric_type> --> <numeric_type>    (unary negation)


Comparison Operators
::::::::::::::::::::
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

      <datatype> =  <datatype>  --> <boolean>
      <datatype> != <datatype>  --> <boolean>
      <datatype> <> <datatype>  --> <boolean>
      <datatype> <  <datatype>  --> <boolean>
      <datatype> <= <datatype>  --> <boolean>
      <datatype> >  <datatype>  --> <boolean>
      <datatype> >= <datatype>  --> <boolean>
             <boolean> IS NULL  --> <boolean>
         <boolean> IS NOT NULL  --> <boolean>
   <datatype> BETWEEN <datatype> AND <datatype> --> <boolean>

Conditional Operators
:::::::::::::::::::::

* ``CASE``


String Operators
::::::::::::::::

* ``LIKE``
* ``SUBSTR``

Date and Time Operators
:::::::::::::::::::::::

* ``EXTRACT``
*

Aggregate Operators
:::::::::::::::::::

* ``COUNT``
* ``COUNT(*)``
* ``MIN``
* ``MAX``
* ``SUM``
* ``AVG``

Data Type Casting Operators
:::::::::::::::::::::::::::

* ``CAST``

Subquery Operators
::::::::::::::::::

* ``EXISTS``
* ``IN``

The syntax for these operators are as follows:

.. code:: sql

              EXISTS <subquery> --> <boolean>
   <datatype> IN     <subquery> --> <boolean>

Keywords
````````
RapidsSE does not currently support expression keywords such as ``NOW()``,
``CURRENT_TIMESTAMP`` or ``CURRENT_USER``.


Order of Precedence
```````````````````

RapidsSE interprets SQL expressions according to its inbuilt order of
precedence. This ordering is show in the table below, from highest precedence
to lowest. Brackets can be used to group expressions to form a different
order of precedence, if required.

.. table::
   :name: expr-operator-precedence-table
   :class: longtable

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


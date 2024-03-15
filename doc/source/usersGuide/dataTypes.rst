.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: data types
   single: RapidsSE; data types

.. _dataTypes:

Data Types
==========

.. index::
   single: data types; supported
   single: RapidsSE; supported data types
   single: ``INTEGER`` (supported data types)
   single: ``FLOAT`` (supported data types)
   single: ``DECIMAL`` (supported data types)
   single: ``BOOLEAN`` (supported data types)
   single: ``DATE`` (supported data types)
   single: ``TIME`` (supported data types)
   single: ``TIMESTAMP`` (supported data types)
   single: ``CHAR`` (supported data types)
   single: ``VARCHAR`` (supported data types)
   single: ``INTERVAL`` (supported data types)

Supported Data Types
--------------------
RapidsSE supports most basic SQL types. Apart from the string data types,
all data types are internally represented as a 64 bit value. A future version
of SE will expand this and provide a much greater range of values for all
associated data types.

The data types supported by SE and their limitations are captured in the table
below.

.. table:: Supported Data Types
   :name: data-types-table
   :widths: 20, 80

   =============  ===========================================================================
   Data Type      Valid Range / Comment
   =============  ===========================================================================
   ``INTEGER``    ``-9223372036854775806`` to ``9223372036854775806``
   ``FLOAT``      ``-1.797693134862315708145274237317043567981e+308`` to
                  ``1.797693134862315708145274237317043567981e+308``
   ``DECIMAL``    RapidsSE v1.x supports decimals with a maximum precision and scale of 17 digits.
                  The range of the mantissa (the un-scaled value) is from
                  ``-99,999,999,999,999,999`` to ``99,999,999,999,999,999``.

                  Decimals also support a scale (the number of digits to the right of the
                  decimal point) from 0 to 17. As the scale is increased, the range of the
                  integral (whole number) portion is reduced accordingly. For example, with a
                  scale of 2 the supported value range is ``-999,999,999,999,999.99`` to
                  ``999,999,999,999,999.99``. With a scale of 17 the supported value range is
                  ``-0.999,999,999,999,999,99`` to ``0.999,999,999,999,999,99``. This is covered
                  further in the :ref:`section below<decimals>`.

                  If not specified, the default precision of a ``DECIMAL`` is ``17``. The
                  default scale is ``0``.
   ``BOOLEAN``    No limits
   ``DATE``       For RapidsSE v1.x the range of valid years is ``1900`` to ``2155``
   ``TIME``       No limits
   ``TIMESTAMP``  For RapidsSE v1.x the range of valid years is ``1900`` to ``2155``
   ``CHAR``       For RapidsSE v1.x  ``CHAR`` is synonymous with ``VARCHAR``. The length of
                  the ``CHAR``, if specified, is currently ignored. No explicit character
                  encoding is currently supported. Multi-byte characters must fit within the
                  above byte limit.
   ``VARCHAR``    For RapidsSE v1.x  ``VARCHAR`` is limited to 4,000 bytes. The length of the
                  ``VARCHAR``, if specified, is currently ignored. No explicit character
                  encoding is currently supported. Multi-byte characters must fit within the
                  above byte limit.
   ``INTERVAL``   The range of intervals depends on the interval type:

                  All year-month intervals (``YEAR``, ``MONTH`` and ``YEAR TO MONTH``) store
                  their value internally in months. They have a range of ``-7.205759403792794e+16``
                  to ``7.205759403792794e+16`` months, or ``-6,004,799,503,160,661`` to
                  ``6,004,799,503,160,661`` years.

                  All day-time intervals (``DAY``, ``HOUR``, ``MINUTE``, ``SECOND``, ``DAY TO HOUR``
                  ``DAY TO MINUTE``, etc) store their value internally in microseconds. This
                  equates as follows:

                    | ``DAY``              : +/-        833,999 days
                    | ``HOUR``             : +/-     20,015,998 hours
                    | ``MINUTE``           : +/-  1,200,959,900 minutes
                    | ``SECOND``           : +/- 72,057,594,037.927935 seconds
                    | ``DAY TO HOUR``      : +/-        833,999 days, 22 hours
                    | ``DAY TO MINUTE``    : +/-        833,999 days, 22 hours, 20 minutes
                    | ``DAY TO SECOND``    : +/-        833,999 days, 22 hours, 20 minutes, 37.927935 seconds
                    | ``HOUR TO MINUTE``   : +/-     20,015,998 hours, 20 minutes
                    | ``HOUR TO SECOND``   : +/-     20,015,998 hours, 20 minutes, 37.927935 seconds
                    | ``MINUTE TO SECOND`` : +/-  1,200,959,900 minutes, 37.927935 seconds
   =============  ===========================================================================


.. index::
   single: data types; decimals

.. _decimals:

Decimals
---------

.. index::
   single: decimals; overview
   single: precision (decimal)
   single: scale (decimal)

Decimal Overview
`````````````````

The ``DECIMAL`` data type is an exact data type that precisely represents
values and calculations within a limited range of precision and scale.
``DECIMAL``\ s are often used for precise calculations where the approximate
representation of a floating point number is not appropriate (e.g. operations
involving money).

The *precision* of a ``DECIMAL`` data type is the maximum number of digits
(base 10) that are allowed to be represented. RapidsSE has supports a maximum
precision of 17, meaning that it can represent a value as large as
``99,999,999,999,999,999`` or as precise as ``0.999,999,999,999,999,99``.

A column or expression can also be defined with a smaller precision. If a value
exceeds the precision and cannot be represented then an error is returned.
For example, if a column is defined as a ``DECIMAL`` with a precision of 3 then
it cannot return a value greater than ``999``.

The *scale* of a ``DECIMAL`` data type is the maximum number of digits that can
appear to the right of a decimal point. These digits are also counted towards
the precision of the ``DECIMAL`` too. So a value typically cannot have a very
large integral (whole number) component as well as a very large fractional
component at the same time. A scale value of ``0`` means that the ``DECIMAL``
does not have any digits to the right of a decimal place and therefore it has
integral (whole number) values only. While this is similar to an ``INTEGER`` it
is important to note that a ``DECIMAL`` with a zero scale does not have the
same range as as ``INTEGER``.


The precision and scale of a ``DECIMAL`` is specified by writing the precision
and (optionally) the scale, separated by a single comma, in parentheses immediately
after the ``DECIMAL`` keyword. Here are some examples:

.. code:: sql

   DECIMAL
   DECIMAL(4)
   DECIMAL(4,2)
   DECIMAL(4,4)

When a ``DECIMAL`` data type is specified without a precision and scale then
RapidsSE will use the default precision of ``17`` and a default scale of ``0``. If a
precision is specified but not a scale then RapidsSE will use the default scale
of ``0``. A scale cannot be specified without specifying a precision.

RapidsSE supports a precision in the range [``1``, ``17``]. A negative or zero
precision does not make sense.

RapidsSE supports a scale in the range [``0``, precision]. That is, the maximum
value for the scale cannot exceed the maximum value of the precision for that
column or expression type. So if a ``DECIMAL`` column is defined with a precision
of ``4`` then the scale for that column cannot be any larger than ``4``.

RapidsSE does not support negative scales.


.. index::
   single: decimals; expressions affecting precision & scale
   single: expressions; decimal precision & scale

Decimal Operations Affecting Precision and Scale
````````````````````````````````````````````````

``DECIMAL`` expressions support a range of arithmetic and aggregate operations.
The evaluation of a ``DECIMAL`` expression will typically produce a result as
another ``DECIMAL`` that can have a different precision or scale. This is because
calculations can cause the number of digits in the integral component to overflow
or the number of fractional digits to be increased to accurately capture the exact
result.

For example the expression ``9. + 2.`` is the addition of two ``DECIMAL`` values
that each have a precision of ``1``. However the result of this operation (``11.``)
has a precision of ``2`` because the integral component has overflowed into two
digits.

Similarly, the expression ``9.2 * 2.0`` has two operands that each have a precision
of ``2`` and a scale of ``1``. The result, ``18.40``, has a precision of ``4`` and a
scale of ``2``. The last digit, ``0``, is significant to the accuracy of the calculation.

There are rules that govern how much precision and scale a result of an expression
will have. These are described in :numref:`decimal-prec-scale-operations-table` below.
An important point to remember is that the actual precision and scale of the result
will depend on the precision and scale of the specific operands involved, not the
precision and scale of any column definitions that they may have come from.

.. table:: Precision and scale in decimal expression operations
   :name: decimal-prec-scale-operations-table
   :widths: 15, 53, 32

   ================ ============================================ =============================
   Operation        Precision                                    Scale
   ================ ============================================ =============================
   ``arg1 + arg2``  ``max(s1, s2) + max(p1-s1, p2-s2) + 1``      ``max(s1, s2)``
   ``arg1 - arg2``  ``max(s1, s2) + max(p1-s1, p2-s2) + 1``      ``max(s1, s2)``
   ``arg1 * arg2``  ``p1 + p2``                                  ``s1 + s2``
   ``arg1 / arg2``  ``p1 - s1 + s2 + max(6, s1+p2+1)``           ``max(6, s1 + p2 + 1)``
   ``SUM(arg1)``    ``p_max``                                    ``s1``
   ``COUNT(arg1)``  ``19``                                       ``0``
   ``AVG(arg1)``    ``p_max``                                    ``max(6, s1 + p_count + 1``)
   ``MIN(arg1)``    ``p1``                                       ``s1``
   ``MAX(arg1)``    ``p1``                                       ``s1``
   ================ ============================================ =============================

In the above table:

* ``p1`` and ``p2`` refer to the precision of the value of ``arg1``
  and ``arg2`` respectively;

* ``s1`` and ``s2`` refer to the scale of the value of ``arg1``
  and ``arg2`` respectively;

* ``p_max`` refers to the maximum ``DECIMAL`` precision supported, which is ``17``;

* ``p_count`` in an ``AVG()`` calculation refers to the precision of the ``COUNT()``
  expression that is divided into the ``SUM()`` expression to produce the average.

* ``max(a, b)`` means that larger value of ``a`` or ``b``.

The precision and scale of a ``COUNT()`` aggregation is always (19,0) because
the result is actually of an ``INTEGER`` and not a DECIMAL.

Finally, the ``AVG()`` expression is executed as a ``SUM()`` divided by a
``COUNT()``. Thus the rules for decimal division apply. The scale of the
result ultimately depends on the precision of the ``COUNT()`` expression
result - When more rows are counted then the result of ``AVG()`` will have
more decimal places. At the minimum an ``AVG()`` will produce at least
6 digits of scale, unless the scale needs to be reduced in order to fit
the integral (whole number) component. This is described in the following
section.


Scale Truncation & Rounding
```````````````````````````

While :numref:`decimal-prec-scale-operations-table` details how the result
expressions precision and scale are calculated, it is important to note that
RapidsSE does not do infinite arbitrary precision calculations.

If the calculated precision or scale exceeds the maximum supported precision or
scale of RapidsSE then the result will be limited to the RapidsSE maximum
precision and scale. If the integral (whole number) portion of the value cannot
be completely represented in this precision then an error is returned that the
value is out of bounds.

However in the case where the integral portion of a result can be represented
but there isn't enough precision (digits) to represent all of the calculated
scale then the scale digits will be truncated such that the result fits all of the
integral portion and as much fractional portion (scale digits) as possible. The
least significant digit of the fractional portion will be rounded up or down
as appropriate when any scale digits are truncated.

Similarly, when decimal values are loaded into a table their values will
automatically be truncated and rounded to fit the desired scale of the table's
column definition. e.g. if the value ``12.345`` is being loaded into a table
whose column is defined as ``DECIMAL(4,1)`` then the value will be loaded
as ``12.3`` instead.




.. index::
   single: data types; intervals
   single: intervals; overview

.. _intervals:

Intervals
---------

The ``INTERVAL`` data type is subtly different from other data types, and
therefore deserves some careful explanation.


.. index::
   single: intervals; types

Interval Types
``````````````
The ``INTERVAL`` type is often thought of as a single data type but it is
not. Instead it is a collection of 13 separate data types that fall into
two different interval categories: **year-month** intervals and **day-time**
intervals. This distinction is shown in :numref:`interval-data-types-table` below.

.. table:: Interval data types
   :name: interval-data-types-table
   :widths: 40, 60

   ===================== ===================================
   Category              SQL Type Name
   ===================== ===================================
   year-month intervals   | ``INTERVAL YEAR``
                          | ``INTERVAL MONTH``
                          | ``INTERVAL YEAR TO MONTH``
   day-time intervals     | ``INTERVAL DAY``
                          | ``INTERVAL HOUR``
                          | ``INTERVAL MINUTE``
                          | ``INTERVAL SECOND``
                          | ``INTERVAL DAY TO HOUR``
                          | ``INTERVAL DAY TO MINUTE``
                          | ``INTERVAL DAY TO SECOND``
                          | ``INTERVAL HOUR TO MINUTE``
                          | ``INTERVAL HOUR TO SECOND``
                          | ``INTERVAL MINUTE TO SECOND``
   ===================== ===================================

Interval data types within the same category are compatible and comparable. An
``INTERVAL YEAR`` can be converted to an ``INTERVAL MONTH`` or ``INTERVAL YEAR TO MONTH``.
Alternatively an ``INTERVAL MINUTE`` can be converted to an ``INTERVAL SECOND``.

But interval data types from the two different categories are not compatible with
each other. We cannot convert an ``INTERVAL MONTH`` to ``INTERVAL DAY`` because
there are a variable number of days in a month. This is why intervals are split into
two distinct categories. Expressions involving intervals cannot mix interval
types if they come from differing interval categories.

Internally, RapidsSE stores all year-month intervals as a count of months. All
day-time intervals are stored as a count of microseconds. This makes calculations
involving intervals fast and it allows for RapidsSE to store a greater value
range in a smaller amount of memory.

The type of an interval ultimately affects how the value is displayed, as only those
fields in the intervals type will be shown. If the type of an interval contains multiple
fields (e.g. a ``DAY TO MINUTE`` interval) then the interval value will be normalised for
the smaller fields and unbounded for the largest field. For example, the microseconds
value ``93,780,000,000`` that is stored in an ``INTERVAL HOUR TO MINUTE`` will be displayed
as 26 hours, 3 minutes. That is, the smallest field (minutes) is normalised to the range
[0-59] but the largest field (hours) is unbounded.


.. index::
   single: intervals; literal format

.. _intervalLiterals:

Interval Literal Format
```````````````````````
Interval literals are written as per the syntax diagrams below:

.. figure:: /rr/diagram/sql92CalendarIntervalLiteral.png

   SQL year-month interval literal syntax

.. figure:: /rr/diagram/sql92TimeIntervalLiteral.png

   SQL day-time interval literal syntax

In each case the value of the interval is specified as a single quoted string.
This single quoted string has the following rules:

* An optional sign (``+`` or ``-``) may be placed immediately before the single
  quoted string.
* Alternatively, a sign character can be immediately placed inside the single
  quoted string, however this is not recommended as it is less readable.
* All values within the single quoted string are unsigned (that is, non-negative).
* A dash is used to separate a year value and a month value.
* A space is used to separate a day value from an hour value.
* A colon (``:``) is used to separate hours, minutes or seconds from each other.
* A seconds value may contain a decimal point and up to 6 decimal places,
  representing microseconds.

.. rubric:: Examples

.. code:: sql

   INTERVAL '1' YEAR
   INTERVAL '2' MONTH
   INTERVAL '1-2' YEAR TO MONTH
   INTERVAL -'1-2' YEAR TO MONTH
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
   INTERVAL -'5:6.789012' MINUTE TO SECOND


.. index::
   single: intervals; supported expression operators
   single: expressions; interval supported operations

.. _intervalSupportedOperators:

Expression Operators Supporting Intervals
`````````````````````````````````````````
Intervals are supported in the operations show below. For simplicity, in this
list ``INTERVAL`` is used to refer to any of the 13 varieties of interval types,
``INTERVAL_DT`` is used to refer to any of the 10 day-time intervals variants
and ``INTERVAL_YM`` is used to refer to any of the 3 year-month interval variants.
Again, year-month and day-time intervals cannot be mixed in expressions because
there is no valid conversion between the two categories.

.. code:: sql

   INTERVAL + INTERVAL
   INTERVAL - INTERVAL
   INTERVAL * INTEGER    (and vice versa)
   INTERVAL / INTEGER

   INTERVAL + DATE    (and vice versa)
   DATE - INTERVAL

   INTERVAL_DT + TIME    (and vice versa)
   TIME - INTERVAL_DT

   INTERVAL + TIMESTAMP    (and vice versa)
   TIMESTAMP - INTERVAL

   DATE - DATE
   TIME - TIME
   TIMESTAMP - TIMESTAMP

   INTERVAL <  INTERVAL
   INTERVAL <= INTERVAL
   INTERVAL =  INTERVAL
   INTERVAL != INTERVAL
   INTERVAL >= INTERVAL
   INTERVAL >  INTERVAL

   INTERVAL <  INTEGER     (and vice versa; single field intervals only)
   INTERVAL <= INTEGER     (and vice versa; single field intervals only)
   INTERVAL =  INTEGER     (and vice versa; single field intervals only)
   INTERVAL != INTEGER     (and vice versa; single field intervals only)
   INTERVAL >= INTEGER     (and vice versa; single field intervals only)
   INTERVAL >  INTEGER     (and vice versa; single field intervals only)
   INTERVAL BETWEEN INTERVAL AND INTERVAL (and combinations with INTEGER; single field intervals only)

.. rubric:: Notes on operations involving intervals

When an expression involves intervals of different types (but within the same
category), the result may yield an interval of a type that encompasses all of
the fields in the input interval types. For example, if an ``INTERVAL HOUR``
and an ``INTERVAL SECOND`` were added together, the result would be an
``INTERVAL HOUR TO SECOND`` so that it encompasses all of the fields of the
two operands (the ``HOUR`` field and the ``SECOND`` field).

Similarly, the addition of an ``INTERVAL HOUR TO SECOND`` and an
``INTERVAL MINUTE`` would yield an ``INTERVAL HOUR TO SECOND`` because that
already encompasses the ``MINUTE`` field of the second operand.

Interval types are not expanded where it is not required. That is, if an expression
takes an interval as an input and returns an interval as an output, where it is
possible RapidsSE tries to keep the same interval type on the output as was provided
on the input.

For example, ``INTERVAL '1' YEAR / 2`` will give the result
``INTERVAL '0' YEAR`` and not ``INTERVAL '6' MONTH`` because the input was of
type ``INTERVAL YEAR``. If the user wants the result to be in years and months
then the input interval value can be cast or rewritten with the appropriate type.
For example, ``INTERVAL '1-0' YEAR TO MONTH / 2`` will give the result
``INTERVAL '0-6' YEAR TO MONTH``.

For convenience, a single field interval (e.g. ``INTERVAL HOUR``) can be directly
compared to an integer with any of the comparison operators (``<``, ``<=``, ``=``,
``!=``, ``>=``, ``>`` and ``BETWEEN``). Multi-field intervals (e.g.
``INTERVAL HOUR TO MINUTE``) cannot be coerced to an ``INTEGER``. For example,
``INTERVAL '1' HOUR = 1`` would evaluate to ``TRUE``.

When intervals are involved with calculations with dates and times, RapidsSE
tries to be concise with the resulting data type. It tries to honour the data
types in the original calculation instead of a looser approach where the
result is often converted to a timestamp. Here are some examples of that:

  #. A ``TIME`` ``+`` or ``-`` a year-month interval is considered by RapidsSE
     to be an invalid operation.

  #. A ``TIME`` ``+`` or ``-`` a day-time interval is a valid operation and
     will result in a ``TIME`` value being returned.

  #. A ``DATE`` ``+`` or ``-`` a year-month interval will result in a ``DATE``
     being returned (rather than a ``TIMESTAMP``). Here RapidsSE is choosing
     to honour the ``DATE`` input and keep the output data type narrow.

  #. However a ``DATE`` ``+`` or ``-`` a day-time interval will result in a ``TIMESTAMP``
     being returned as the ``DATE`` can be considered as being at midnight on
     the given date. The calculation with the interval can then proceed.


.. index::
   single: intervals; constraints

Interval Constraints
````````````````````
RapidsSE currently does not support use of an interval data type as a column
in a ``CREATE TABLE`` statement. This also means that they cannot be loaded
from external data files with commands like ``LOAD`` or ``DLOAD``. Instead,
intervals can be specified as literal values or they may be the result of an
expression calculation instead.

Interval expressions also currently cannot be used with aggregates (e.g. ``SUM``,
``MIN``, ``MAX``, etc). This support will come in a later release.


.. index::
   single: data types; conversion
   single: RapidsSE; type conversion
   single: type conversion

.. _typeConversion:

Type Conversion
---------------
In the RapidsSE SQL compiler, an :ref:`expression <expressions>` is represented as a tree of
operators and operands. This tree allows for the compiler to make any necessary
changes to the expression without affecting the order of precedence.

The evaluation of most expression operators in RapidsSE generally expects its
operands to be of the same type. It is the job of the SQL compiler to ensure
that the SE execution engine does not get a set of operands to an expression
operator that are incompatible. The SQL compiler achieves this by detecting
invalid combinations within the expression tree and trying to convert the
types of one or more operands in an expression such that the expression
operator is valid and the types are unambiguous.

If the SQL compiler cannot determine a valid and unambiguous combination of
operands then an error is returned to the user. The user may be able to correct
the query by the way any expression literals are specified, or by explicitly
requesting the result of an expression to be cast to a different type.

RapidsSE supports expression type conversion in two ways:

#. Through the SQL compiler automatically converting the type to make the
   expression work. This is called implicit type coercion.

#. Through the user manually converting the type with a ``CAST`` expression.
   This is called explicit type casting.

This is described in the sections below.


.. index::
   single: data types; conversion (implicit)
   single: RapidsSE; type conversion (implicit)
   single: type conversion; implicit

.. _implicitTypeCoercion:

Implicit type coercion
``````````````````````
Implicit type coercion is where the SQL compiler tries to automatically
convert one or more operands in an expression to make the expression valid
and executable. The compiler only does this if the expression cannot be
executed the way it was written.

For example, an ``INTEGER + DECIMAL`` expression cannot be natively evaluated
due to the mixed types of the operands. However the ``INTEGER`` operand could
be coerced into a ``DECIMAL`` such that the expression becomes
``DECIMAL + DECIMAL``, producing a result that is also a ``DECIMAL``.

The SQL compiler will only do this if an invalid expression can be turned into
a valid expression by following a set of rules. It will not perform all
possible type conversions because some conversions may affect the result of
the query and produce unexpected answers. In the above example, it doesn’t make
sense to convert the ``DECIMAL`` operand to an ``INTEGER`` for an
``INTEGER + INTEGER`` expression because there would be a loss of precision with
the result and that precision might be important for the user.

The supported type conversions that the SQL compiler follows for implicit type
coercion is detailed in the table below. The SQL compiler will try coercing
the operands of an expression in multiple ways and if there is more than one
valid answer then it is ambiguous as to which result the SQL compiler should
choose. This will result in an error to the user who should then either rewrite
the query or use an CAST on the appropriate expression to make it clearer to
the compiler how the expression is to be executed.

.. table:: Implicit Type Coercions Supported
   :widths: 15, 40, 45
   :class: longtable

   ============================= ================================ ======================
   Source Expression Type        Can Be Converted To              Comments / Limitations
   ============================= ================================ ======================
   ``INTEGER``                   ``DECIMAL`` or ``FLOAT```
   ``DECIMAL``                   ``FLOAT``
   ``VARCHAR`` literal           ``INTEGER``                      Only if the ``VARCHAR`` literal contains a valid ``INTEGER`` value. E.g. ``'123'``.
   ``VARCHAR`` literal           ``DECIMAL``                      Only if the ``VARCHAR`` literal contains a valid ``INTEGER`` or ``DECIMAL`` value. E.g. ``'123.45'``.
   ``VARCHAR`` literal           ``FLOAT``                        Only if the ``VARCHAR`` literal contains a valid ``INTEGER``, ``DECIMAL`` or ``FLOAT`` value. E.g. ``'123.45e0'``.
   ``VARCHAR`` literal           ``DATE``                         Only if the ``VARCHAR`` literal contains a valid ``DATE`` value. E.g. ``'2000-12-31'``.
   ``VARCHAR`` literal           ``TIME``                         Only if the ``VARCHAR`` literal contains a valid ``TIME`` value. E.g. ``'12:31:45'``.
   ``VARCHAR`` literal           ``TIMESTAMP``                    Only if the ``VARCHAR`` literal contains a valid ``TIMESTAMP`` value. E.g. ``'2000-12-31 12:31:45'``.
   ``DATE``                      ``TIMESTAMP``
   ``BOOLEAN``                   ``VARCHAR``
   ``INTERVAL YEAR``             | ``INTERVAL YEAR TO MONTH``
                                 | ``INTEGER`` [#1]_
   ``INTERVAL MONTH``            | ``INTERVAL YEAR TO MONTH``
                                 | ``INTEGER`` [#1]_
   ``INTERVAL DAY``              | ``INTERVAL DAY TO HOUR``
                                 | ``INTERVAL DAY TO MINUTE``
                                 | ``INTERVAL DAY TO SECOND``
                                 | ``INTEGER`` [#1]_
   ``INTERVAL HOUR``             | ``INTERVAL DAY TO HOUR``
                                 | ``INTERVAL DAY TO MINUTE``
                                 | ``INTERVAL DAY TO SECOND``
                                 | ``INTERVAL HOUR TO MINUTE``
                                 | ``INTERVAL HOUR TO SECOND``
                                 | ``INTEGER`` [#1]_
   ``INTERVAL MINUTE``           | ``INTERVAL DAY TO MINUTE``
                                 | ``INTERVAL DAY TO SECOND``
                                 | ``INTERVAL HOUR TO MINUTE``
                                 | ``INTERVAL HOUR TO SECOND``
                                 | ``INTERVAL MINUTE TO SECOND``
                                 | ``INTEGER`` [#1]_
   ``INTERVAL SECOND``           | ``INTERVAL DAY TO SECOND``
                                 | ``INTERVAL HOUR TO SECOND``
                                 | ``INTERVAL MINUTE TO SECOND``
                                 | ``INTEGER`` [#1]_
   ``INTERVAL DAY TO HOUR``      | ``INTERVAL DAY TO MINUTE``
                                 | ``INTERVAL DAY TO SECOND``
   ``INTERVAL DAY TO MINUTE``    | ``INTERVAL DAY TO SECOND``
   ``INTERVAL HOUR TO MINUTE``   | ``INTERVAL DAY TO SECOND``
                                 | ``INTERVAL DAY TO MINUTE``
                                 | ``INTERVAL HOUR TO SECOND``
   ``INTERVAL HOUR TO SECOND``   | ``INTERVAL DAY TO SECOND``
   ``INTERVAL MINUTE TO SECOND`` | ``INTERVAL DAY TO SECOND``
                                 | ``INTERVAL HOUR TO SECOND``
   ============================= ================================ ======================

.. [#1] Coercion of a single field interval to an integer only occurs when that
        single field interval is used in a comparison with an integer (for example,
        a ``<``, ``<=``, ``=``, ``!=``, ``>=``, ``>`` or ``BETWEEN`` expression).

If the source expression is a literal value then the SQL compiler will replace
this value with the coerced value of the new type directly in the plan. If the
source expression is not a literal then the type coercion is applied by
inserting a CAST operator into the plan so that the source expression type is
converted as the query is executed.

In the case of coercing literals, it is possible that the coercion from the
source to the target type may exceed the supported range of the target type.
In that case an error is returned to the user.

.. index::
   single: data types; conversion (explicit)
   single: data types; casting
   single: RapidsSE; type conversion (explicit)
   single: type conversion; explicit
   single: casting; supported casts
   single: expressions; casting

.. _explicitTypeCasting:

Explicit type casting
`````````````````````
Where the SQL compiler is unable to coerce operands to produce a valid
expression, the user may be able to resolve the ambiguity by explicitly
converting operands in an expression. This can be done by using the ``CAST``
operator. The supported explicit type conversions are listed below.

.. note::
   For simplicity, each of the 13 specific ``INTERVAL`` types are not shown.
   Instead we are showing the category of interval they belong to (Day-Time
   or Year-Month). All intervals within the same interval category are
   convertable to each other.

.. table:: Explicit Type Casting Supported
   :name: explicit-type-casting-table
   :class: widetable centered

   +---------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
   | Source        |                                                      Target Expression Type                                                                              |
   | Expr Type     |                                                                                                                                                          |
   +===============+=============+==========+==========+=============+===========+=============+==================+==================+==========+===============+=============+
   |               | ``BOOLEAN`` | ``CHAR`` | ``DATE`` | ``DECIMAL`` | ``FLOAT`` | ``INTEGER`` | ``INTERVAL D-T`` | ``INTERVAL Y-M`` | ``TIME`` | ``TIMESTAMP`` | ``VARCHAR`` |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``BOOLEAN``   |     Yes     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``CHAR``      |     ---     |   Yes    |Yes [#2]_ |   Yes [#2]_ | Yes [#2]_ |  Yes [#2]_  |     ---          |     ---          |Yes [#2]_ |   Yes [#2]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``DATE``      |     ---     |   Yes    |   Yes    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |   Yes [#3]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``DECIMAL``   |     ---     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``FLOAT``     |     ---     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``INTEGER``   |     ---     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``INTERVAL``  |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     |
   | ``D-T``       |             |          |          |             |           |             |                  |                  |          |               |             |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``INTERVAL``  |     ---     |   ---    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |      ---      |     ---     |
   | ``Y-M``       |             |          |          |             |           |             |                  |                  |          |               |             |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``TIME``      |     ---     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   Yes    |   Yes [#4]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``TIMESTAMP`` |     ---     |   Yes    |   Yes    |    ---      |    ---    |     ---     |     ---          |     ---          |   Yes    |      Yes      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``VARCHAR``   |     ---     |   Yes    |Yes [#2]_ |   Yes [#2]_ | Yes [#2]_ |  Yes [#2]_  |     ---          |     ---          |Yes [#2]_ |   Yes [#2]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+


.. [#2] The content of the CHAR or VARCHAR string must contain in string form a
       valid representation of a value of the target expression type.
.. [#3] When casting a date to a timestamp the time portion of the timestamp
       will be set to all zeros (midnight).
.. [#4] When casting a time to a timestamp the date portion of the timestamp
       will reflect the local current date on the server.

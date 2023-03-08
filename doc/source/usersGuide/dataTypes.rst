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
   ``DECIMAL``    RapidsSE v1.x only supports a fixed scale of 2 (2 decimal places).
                  Specifying the precision and range in a statement is currently ignored.
                  The range of the mantissa (the un-scaled value) is from
                  ``-140,737,488,355,327`` to ``140,737,488,355,327``. With a scale of 2 the
                  range of the overall value is ``-1,407,374,883,553.27`` to
                  ``1,407,374,883,553.27``
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
   ``INTERVAL``   Not yet supported
   =============  ===========================================================================


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

.. csv-table:: Implicit Type Coercions Supported
   :header: "Source Expression Type", "Can Be Converted To", "Comments / Limitations"
   :widths: 25, 15, 60

   "``INTEGER``", "``DECIMAL``", ""
   "``INTEGER``", "``FLOAT``", ""
   "``DECIMAL``", "``FLOAT``", ""
   "``VARCHAR`` literal", "``INTEGER``",  "Only if the ``VARCHAR`` literal contains a valid ``INTEGER`` value. E.g. ``'123'``."
   "``VARCHAR`` literal", "``DECIMAL``",  "Only if the ``VARCHAR`` literal contains a valid ``INTEGER`` or ``DECIMAL`` value. E.g. ``'123.45'``."
   "``VARCHAR`` literal", "``FLOAT``",    "Only if the ``VARCHAR`` literal contains a valid ``INTEGER``, ``DECIMAL`` or ``FLOAT`` value. E.g. ``'123.45e0'``."
   "``VARCHAR`` literal", "``DATE``",     "Only if the ``VARCHAR`` literal contains a valid ``DATE`` value. E.g. ``'2000-12-31'``."
   "``VARCHAR`` literal", "``TIME``",     "Only if the ``VARCHAR`` literal contains a valid ``TIME`` value. E.g. ``'12:31:45'``."
   "``VARCHAR`` literal", "``TIMESTAMP``","Only if the ``VARCHAR`` literal contains a valid ``TIMESTAMP`` value. E.g. ``'2000-12-31 12:31:45'``."
   "``DATE``",            "``TIMESTAMP``", ""
   "``BOOLEAN``",         "``VARCHAR``",   ""

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
   | ``CHAR``      |     ---     |   Yes    | Yes [3]_ |    Yes [3]_ | Yes [3]_  |   Yes [3]_  | Yes [3]_ [4]_    | Yes [3]_ [4]_    | Yes [3]_ |    Yes [3]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``DATE``      |     ---     |   Yes    |   Yes    |    ---      |    ---    |     ---     |     ---          |     ---          |   ---    |    Yes [1]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``DECIMAL``   |     ---     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``FLOAT``     |     ---     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``INTEGER``   |     ---     |   Yes    |   ---    |    Yes      |    Yes    |     Yes     |     ---          |     ---          |   ---    |      ---      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``INTERVAL``  |     ---     | Yes [4]_ |   ---    |    ---      |    ---    |     ---     |   Yes [4]_       |     ---          |   ---    |      ---      |   Yes [4]_  |
   | ``D-T``       |             |          |          |             |           |             |                  |                  |          |               |             |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``INTERVAL``  |     ---     | Yes [4]_ |   ---    |    ---      |    ---    |     ---     |     ---          |   Yes [4]_       |   ---    |      ---      |   Yes [4]_  |
   | ``Y-M``       |             |          |          |             |           |             |                  |                  |          |               |             |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``TIME``      |     ---     |   Yes    |   ---    |    ---      |    ---    |     ---     |     ---          |     ---          |   Yes    |    Yes [2]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``TIMESTAMP`` |     ---     |   Yes    |   Yes    |    ---      |    ---    |     ---     |     ---          |     ---          |   Yes    |      Yes      |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+
   | ``VARCHAR``   |     ---     |   Yes    | Yes [3]_ |    Yes [3]_ | Yes [3]_  |   Yes [3]_  | Yes [3]_ [4]_    | Yes [3]_ [4]_    | Yes [3]_ |    Yes [3]_   |     Yes     |
   +---------------+-------------+----------+----------+-------------+-----------+-------------+------------------+------------------+----------+---------------+-------------+


.. [1] When casting a date to a timestamp the time portion of the timestamp
       will be set to all zeros (midnight).
.. [2] When casting a time to a timestamp the date portion of the timestamp
       will reflect the local current date on the server.
.. [3] The content of the CHAR or VARCHAR string must contain in string form a
       valid representation of a value of the target expression type.
.. [4] Interval support will be provided in a future RapidsSE release.
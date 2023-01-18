.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _expressions:

Expressions
===========


Supported Expression Operators
``````````````````````````````

select expressions


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

Logical Operators
`````````````````

Mathematical Operators
``````````````````````

Comparison Operators
````````````````````

Conditional Operators
`````````````````````

String Operators
````````````````

Aggregate Operators
```````````````````

Data Type Casting Operators
```````````````````````````

Keywords
````````
RapidsSE does not currently support expression keywords such as ``NOW()``,
``CURRENT_TIMESTAMP`` or ``CURRENT_USER``.

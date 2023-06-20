.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: queries; optimisation
   single: optimiser
   single: SQL; query optimisation

.. _optimisation:

Query Optimisation
==================

Query optimisation is the process of changing *how* a query executes so that it
completes faster and uses less computing resources. This is provided by a
part of RapidsSE called the RapidsSE Optimiser (refer to the section
:ref:`sqlArchitecture`). The optimiser must not alter *what* the query does -
that is, an optimised and unoptimised query must return the same results.

The optimiser tries to optimise a query in many different ways. These are
described in the following sub-sections.

Join Order Optimisation
-----------------------

RapidsSE attempts to optimise the execution of queries. The RapidsSE optimiser
does this by evaluating the various ways joins can be performed (permutations)
and estimating the network overhead for each permutation. The optimiser chooses
the join order permutation with the lowest estimated network overhead.

Let us use the following example:

.. code:: sql

  SELECT c_nationkey, l_returnflag, l_linestatus, COUNT(*)
    FROM lineitem, orders, customer
   WHERE l_orderkey = o_orderkey
     AND o_custkey  = c_custkey
   GROUP BY c_nationkey, l_returnflag, l_linestatus
  ;

The above example presents a join between the lineitem, orders and customer
tables drawn from the TPC-H schema.

At a minimum, for this join to succeed either there must be indexes on two
of the columns from the following set: ``l_orderkey``,  ``o_orderkey``, ``c_custkey``.

Additionally there could be an index on ``o_custkey``.

Based on the available indexes, possible permutations for this join are:

a. Scan ``lineitem`` join to ``orders`` join to ``customer`` (based on
   indexes: ``o_orderkey``, ``c_custkey``);

b. Scan ``orders`` join to ``lineitem`` join to ``customer`` (based on
   indexes: ``l_orderkey``, ``c_custkey``);

c. Scan ``orders`` join to ``customer`` join to ``lineitem`` (based on
   indexes: ``c_custkey``, ``l_orderkey``);

d. Scan ``customer`` join to ``orders`` join to ``lineitem`` (based on
   indexes: ``o_orderkey``, ``l_orderkey``).

If only two indexes are available then the RapidsSE optimiser will only consider
the join permutations permitted by those indexes.

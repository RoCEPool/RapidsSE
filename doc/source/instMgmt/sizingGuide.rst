.. _sizingGuide:

Sizing Guide
------------
Because RapidsSE is an in-memory system, it is important to determine how much
memory is required on each node and how many nodes are required to fit a given
data set.

The actual requirement can vary with the nature of the data, the schema and
indexes, and how the data is being queried. As a rough rule of thumb,
if RapidsSE reads in data from a CSV file it will store it in memory in a
compressed form that occupies about 75% of its original CSV on-disk size.

Typically the biggest consumption of memory occurs in the usage of indexes.
While indexes significantly speed up the execution of complimentary queries
by allowing RapidsSE to skip over the reading large swathes of data, they do
occupy a significant amount of memory while they exist.

.. sidebar:: Factors affecting memory usage

   #. Amount of data;
   #. Number of indexes;
   #. Temporary storage space needed by queries;
   #. Poor usage of indexes by queries;
   #. Number of concurrent queries being executed;

If we consider the `TPC-H <https://www.tpc.org/tpch>`_ schema and query set as
a representative workload then we can give some concrete sizing
recommendations. As a rough rule of thumb, we typically aim for the total
shared memory size in the cluster to be about 2.5 times the size of
the TPC-H scale factor (SF) data size as this allows for a good number of
indexes to help speed up the query time.

  TPC-H SF100 (100GB) Size Guide:

  .. csv-table::

     "**Nodes:**", "3 nodes"
     "**CPU:**", "16+ cores"
     "**RAM:**", "128GB"
     "**Shared Memory:**", "``/dev/shm`` should have at least 100GB allocated to it."
     "**Network:**", "10Gb ethernet"


  TPC-H SF400 (400GB) Size Guide:

  .. csv-table::

     "**Nodes:**", "5 nodes"
     "**CPU:**", "32 cores"
     "**RAM:**", "256GB"
     "**Shared Memory:**", "``/dev/shm`` should have at least 240GB allocated to it."
     "**Network:**", "10Gb ethernet"


  TPC-H SF1000 (1TB) Size Guide:

  .. csv-table::

     "**Nodes:**", "5 nodes"
     "**CPU:**", "64 cores"
     "**RAM:**", "512GB"
     "**Shared Memory:**", "``/dev/shm`` should have at least 500GB allocated to it."
     "**Network:**", "10Gb ethernet"




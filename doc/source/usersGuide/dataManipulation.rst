.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _dataManipulation:

Data Manipulation
=================

.. index::
   single: TPC-H; generating

Generating TPC-H Data
---------------------
The installation of RapidsSE includes a copy of ``dbgen`` – the utility for
generating data for TPC-H benchmarks. This utility can generate data for any
scale factor. However, generating data for large scale factors is very slow
since ``dbgen`` is limited by being single threaded, and limited by disk
throughput.

The RapidsSE installation includes a script file that minimises this by
generating data across multiple nodes in parallel. The data for the desired
scale factor is broken into an equal number of chunks on each machine and
generated concurrently. If each machine has more than one storage drive then
this can also be used to help parallelise the data generation by splitting
the data into one chunk per storage drive per node.

To invoke the data generation simply run the following script, specifying
the desired scale factor (``--sf``) and output directories (``--dir``).
Note that the output directories will be written to on each node in the cluster.

.. code:: console

   dbgen/generateData.sh --sf <scale_factor> --dir <output_dir>

e.g., to generate SF100 data on a cluster with one storage drive per node
simply run:

.. code:: console

   dbgen/generateData.sh --sf 100 --dir /mnt/disk1/tpch/sf100

On a two node cluster, this will split the data into two (roughly) equal
sized chunks for each table.

If there are multiple disk drives on each node then these can be used to
further parallelise the data generation by specifying multiple output
directory parameters (``--dir``).

e.g., to generate SF1000 data on a cluster with four hard drives per node
simply run:

.. code:: console

   dbgen/generateData.sh --sf 100 --dir /mnt/disk1/tpch/sf100 \
                                  --dir /mnt/disk2/tpch/sf100 \
                                  --dir /mnt/disk3/tpch/sf100 \
                                  --dir /mnt/disk4/tpch/sf100

On a four node cluster with four hard drives each, this will split the data
into 16 (roughly) equal chunks for each table.

Below are real world example load times for various scale factor and node
configurations. Performance is affected by the number of cores in each
node as well as the number of disks and the throughput of each disk.

.. table:: TPC-H Data Generation Times
   :name: tpch-data-gen-times

   ============ ========= =============== ========== ================ ====================
   Scale Factor Num Nodes Drives per Node Drive Type Total Num Chunks Data Generation Time
   ============ ========= =============== ========== ================ ====================
            10         1               4  SSD                       4  0m 51s
            10         2               4  SSD                       8  0m 49s
            10         4               4  SSD                      16  0m 48s
           100         1               4  SSD                       4  1m 39s
           100         2               4  SSD                       8  1m 10s
           100         4               4  SSD                      16  0m 58s
           250         1               4  SSD                       4  3m 54s
           250         2               4  SSD                       8  2m 01s
           250         4               4  SSD                      16  1m 18s
           500         1               4  SSD                       4  7m 45s
           500         2               4  SSD                       8  3m 56s
           500         4               4  SSD                      16  2m 01s
          1000         1               4  SSD                       4 15m 28s
          1000         2               4  SSD                       8  7m 48s
          1000         4               4  SSD                      16  3m 57s
          1000         6               4  SSD                      24  2m 22s
   ============ ========= =============== ========== ================ ====================



By default, the ``dbgen`` utility outputs files with a numeric file extension
(e.g. “*.1”, “*.2”, etc) that represents the chunk number. The data generation
script removes these numeric extensions to leave just the ``*.tbl`` extension
as it is easier to do a distributed load in RapidsSE this way. If the numeric
extensions are desired, the --norename parameter can be given to the
``generateData.sh`` script file to skip this file renaming.


.. index::
   single: RapidsSE; loading data
   single: loading data

.. _loadingData:

Loading Data
------------
RapidsSE supports loading data in two forms – 1) loading from a single node
and 2) loading from all nodes. The latter is called a distributed load as it
loads from the same path on all nodes simultaneously. This has a performance
benefit because it has more disk throughput and the parsing and partitioning
of the data can all be done in parallel across all nodes in the cluster. In
both cases, data will be partitioned and sent across the network to the correct
node.

RapidsSE currently supports pipe-delimited or comma-delimited file formats
such as that produced by the ``dbgen`` utility.

.. index::
   single: loading data; single node load
   single: LOAD (command); overview
   single: DML; load (single node)

.. _singleNodeLoad:

Single Node Load
````````````````
The load command is used to load data from a single node only. This is
appropriate when you only have one node in the cluster, or when the data
only exists on a single node. The data will be read in by that node and
distributed across the cluster as required. Because a single node needs
to read and parse all of the data, we would expect that the performance
of a single node load will be lower than that of a distributed load in a
multi-node cluster.

The format for a single node load is as follows:

.. code:: sql

   load <tablename> "<path_of_file>"
        [ <option> = "<value>" ... ];

Refer to the :ref:`load <loadCmd>` command in the :ref:`Command Reference <commandList>`.

.. index::
   single: loading data; distributed load
   single: DLOAD (command); overview
   single: DML; load (distributed)

.. _distributedLoad:

Distributed Load
````````````````
A distributed load will load one or more files from each node in the cluster
in parallel. It requires that the path and filename of each file to be loaded
must exist on each node. This is the quickest way to load data into RapidsSE
because it maximises disk throughput as well as CPU time for parsing the data.

To invoke a distributed load simply execute the command below in SE Shell:

.. code::

   dload <tablename> paths “<path_of_file>”
         [ <option> = "<value>" ... ] ;

If each system has multiple disks, or files destined for the same table in
multiple paths then they can all be loaded concurrently by specifying multiple
file paths, as per the following example:

.. code::

   dload <tablename> paths “<file1>” “<file2>” “<file3>” “<file4>” ;

This will cause RapidsSE to load ``<file1>``, ``<file2>``, ``<file3>`` and
``<file4>`` on every node in the cluster and insert them into the
``<tablename>`` table. The command will return once all data has been loaded and inserted.

Refer to the :ref:`dload <dloadCmd>` command in the :ref:`Command Reference <commandList>`.


.. index::
   single: inserting data
   single: DML; inserting data

.. _insertingData:

Inserting Data
--------------
RapidsSE does not currently support any form of INSERT command.


.. index::
   single: updating data
   single: DML; updating data

.. _updatingData:

Updating Data
-------------
RapidsSE does not currently support any form of UPDATE command.


.. index::
   single: deleting data
   single: DML; deleting data
   single: DML; truncating tables

.. _deletingData:

Deleting Data
-------------
RapidsSE does not currently support any form of DELETE command, however
it does support the :ref:`truncate <truncateCmd>` command to remove all rows from a table
in one go. Refer to the Command Reference for more details.


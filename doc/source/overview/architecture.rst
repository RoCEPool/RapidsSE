.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _architecture:

RapidsSE Architecture
=====================

Database systems are complex beasts. They need to be highly stable, manage
huge quantities of data, and concurrently and fairly execute many queries
simultaneously with high performance. At the same time they must also use all
available computing resources efficiently and thoroughly so that end users can
get the most utilisation out of their infrastructure for the least cost.

Managing this is difficult enough, but a relational SQL database also needs
to contend with the fact that a user's SQL query is declarative - that is, the
query specifies *what* the user wants but not *how* to get it. The database
is therefore also responsible for choosing an efficient means for implementing
the query when there might be multiple ways to do so.

The ability to do all of this requires a strong architecture. This section
will explain RapidsSE's architecture from an outside-in approach, starting with
the model of the cluster before delving into the model within a node, the
model of how work is executed within a node, and finally how all of this is
put together to execute a user's SQL query.


.. _clusterArchitecture:

The Cluster Model
-----------------
RapidsSE is a clustered database. It can be ran as a single node, however it
scales well horizontally across multiple machines to use the combined computing
resources that are available. Doing so allows it to store more data in memory
and use the additional computation power available to parallelise operations
for faster query responses.

Within the cluster model there are one or more machines. Each machine has a
local installation of RapidsSE and this represents a *node* in the cluster
model. Each node is considered equal, and each node is running the exact same
software. A client can connect to any node in the cluster, and that node is
responsible for managing the execution of that query across the cluster and
returning responses back to the user.

.. figure:: clusterModel.png
   :align: center

   The model of a RapidsSE cluster.

From a query execution perspective, the cluster model is peer-to-peer. There
is no hierarchical order and each node is able to talk to every other node.
A query could bounce data to and from nodes in the cluster if it determined
that it was the most efficient way to execute the user's query.

Because of this peer-to-peer nature, each node has a TCP/IP connection to
every other node in the cluster. This is used for both data transfer but also
for information and control purposes. Because these channels can send significant
amounts of data, it is recommended that all nodes in the cluster be
geo-located and connected together with a high speed networking fabric (10Gbps
or greater is recommended).

Additionally each node in the cluster emits a regular heartbeat status message
every few seconds over UDP. This message contains health information of the
node for cluster management. RapidsSE can be configured to send these UDP
messages over the network broadcast (if allowed) or multicast (if supported).
One of these techniques must be permitted.

The cluster is controlled (started, stopped, status) via shell scripts that
are available in the installation directory of any of the nodes. Since the nodes
are all equal, the cluster can be started and stopped from any node in the
cluster. The shell scripts coordinate this by using public key SSH to remotely
login to each node and start/stop each RapidsSE instance. This means that each
machine in the cluster must be configured for password-less SSH access.


.. _nodeArchitecture:

The Node Model
--------------
While the cluster model allows for RapidsSE to scale well horizontally,
RapidsSE also scales very well vertically on each node.

RapidsSE is written in C++ so it is compiled to machine code for the target
platform. This gives it fast machine-native performance. It runs as a single
process with multiple threads in order to utilise all available CPUs in the
machine.

The RapidsSE process is configured by an external config file that is created
during the installation process. The installer prompts the user for certain
information (such as the installation path) as well as inspecting the hardware
of the machines in the cluster in order to set other parameters (such as the
number of partitions). This information about the configuration of each machine
in the cluster gets written out to a config file that is then copied onto each
node. The information in this config file is not only used to start the
RapidsSE process of the local node with the correct parameters, but to also
start all other remote nodes as well.

Once started, each node in the cluster can establish a TCP/IP socket to every
other node for control and data transfer purposes.

.. figure:: nodeModel.png
   :align: center

   The model of a RapidsSE node.

Each RapidsSE node uses a shared memory segment as its memory allocation pool.
The shared memory segment is used for storing table data, temporary query data
as well as general runtime memory usage. This has a number of benefits over
regular heap allocations:

#. It allows other compatible machine-local processes to interact with RapidsSE
   through this high speed shared memory interface.

   The shared memory interface has a minimal overhead cost and avoids memory
   copying that can occur with other inter-process communication techniques.

#. It constrains the upper amount of memory that RapidsSE can use.

   RapidsSE can readily monitor its own memory usage and take steps to prevent
   itself from exceeding this shared memory limit. In a heap-allocated approach
   it can be more difficult for the process to keep track of its memory
   utilisation and if it allocates more memory than is available it is liable
   to be terminated by the system's out-of-memory killer.

The shared memory segment of the local node is visible in the Linux filesystem
under the ``/dev/shm`` device.

The amount of shared memory that RapidsSE can use is defined in the cluster
configuration file, and this is set during the installation of the cluster.
RapidsSE *reserves* this maximum shared memory at start up in order to get a
contiguous shared memory address space, but RapidsSE does not use all of that
memory initially.

This will often be represented in tools like ``top`` as the ``rapidsse`` process
having a large reserved (column ``RES`` in ``top``) memory space but a much
smaller *committed* (column ``VIRT`` in ``top``) memory space. While the memory
is reserved but not committed the system is able to use the available memory
for any other tasks. As RapidsSE actually uses the shared memory for data and
queries, the process's *committed* memory usage will increase. This *committed*
memory will stay associated with the ``rapidsse`` process even after
significant table data is deleted as RapidsSE will reallocate this unused
memory as it is needed.

The RapidsSE memory allocator is responsible for fulfilling memory requests
by the rest of the process. It does this by serving the requested contiguous
blocks of memory from the shared memory pool. When the process no longer needs
a given block of memory it is immediately deallocated back into the pool to be
reused by another task in the system. RapidsSE is **not** a garbage collected
memory model. This means that the execution of work within RapidsSE occurs in
a highly deterministic way because there are no indeterminate pauses while the
system searches for and cleans up memory.

### Partitioning ###

.. _jobArchitecture:

The Job Model
-------------
.. ::

  - Is built on a series of recursive patterns
    + that not only execute queries, but execute the core system itself.
    + reduces idiosyncratic code for fewer bugs and lower complexity through
    a parsimony of concepts.

  - Workload in the system (e.g. a user query) is defined in isolated units called jobs.

  - The work that a job does is defined by a pipeline.
    + A pipeline may be created dynamically, e.g. to represent a SQL query being executed,
    or a pipeline may be predefined and executed repeatedly (e.g. to read messages from
    a client and send back responses).

  - A pipeline consists of a series of tasks connected together in a topology across the cluster.
    + A pipeline serves as a communication channel between tasks. Tasks can send data
    and/or control messages to other parts of the pipeline in response to its own activity.
    + A pipeline could run purely on the local node, a remote node, or any or all of the
    nodes in the cluster.
    + A pipeline is not necessarily linear. It can fan out across all partitions on a node,
    or fan out across across selected/all nodes in the cluster (like a broadcast).
    Similarly, a pipeline can fan in across nodes and partitions to collate or aggregate
    results.

  - Each of the tasks in a job can be allocated to different threads in the system
    - Tasks can execute in parallel if they have work to do, or they can be scheduled
    as work arrives for them.
    - Threads execute tasks in their queue in a round-robin fashion

      + This gives a fair execution model
      + Control of this scheduling and execution also gives RapidsSE the power to prioritise
      certain internal tasks where it is required, so that user tasks spend the most time
      in an executable state. This would not be possible to the same degree in a thread-centric
      model where scheduling is purely controlled by the kernel.

    - minimises thread context switching overheads compared to a model where each job has its own threads

  - The work to be done by each task is broken down into a series of small, discrete chunks called steps
    + A thread may execute one or more steps from a particular task before putting it to
    the end of its run queue. The thread would then execute steps from the next task at
    the top of its run queue.
    + In this way steps from multiple tasks can be interleaved so they all make forward progress
    + steps are designed to not block or sleep so they do not hold up a thread from being able
    to execute other tasks

.. _sqlArchitecture:

The SQL Query Model
-------------------
.. ::

  - When a SQL query is submitted to RapidsSE an instance of a job is created to execute
  a pipeline that logically does the following:
    - Parsing
    - Compilation
    - Transliteration into a pipeline
    - Optimising
    - Code synthesis
    - Job execution

  - Parsing turns the text-based SQL query into a basic plan graph (tree) of SQL operators
  that represents the SQL text. Any syntactic errors in the query are picked up at this stage.

  - Compilation is taking the initial plan graph and validating the semantics of the statement.
  This involves checking for the existence of tables/columns, validating that compatible types
  are involved in expressions, and simplifying the graph where possible. At this point the
  plan graph still represents *what* the user has requested, not *how* it is going to be executed.

  - Transliteration is the process of converting the plan graph into a pipeline definition that details
  how it is going to be executed across the cluster. The pipeline could now be executed, however
  it is not optimised.

  - Optimising is the process of taking the above transliterated pipeline and making it much more
  efficient. The output is another pipeline definition that should execute far more quickly
  because it minimises data movement and copying. Common SQL optimisations such as join ordering
  or subquery flattening are added at this stage.

  - Code synthesis is the process of dynamically writing C/C++ code to implement the expression
  projections and predicates in the query. This dynamic code is compiled to machine code via
  a C/C++ compiler and loaded into the RapidsSE process space, where it is executed by tasks
  in the pipeline. This is much faster and efficient than an interpreted or bytecode approach.
  It also allows the dynamic code to directly call and make use of primitives and structures
  in RapidsSE, such as indexes, lists, sets, etc.

  - Job execution is the execution of the final optimised pipeline with tasks executing custom
  compiled expression logic as machine code. This job may return rows back to the user or an
  appropriate error message.


.. _architectureSummary:

Summary
-------
.. ::

  Goals:
  - stable
  - high performance
  - efficient use of hardware computing resources
  - ...leads to lower TCO
  - optimised for large SF

  How?
  - Instructions are moved to the location of the data
  - Both SE and queries are executed in natively compiled code.
  (not bytecode or interpreted)
  - Rigorous and self-describing memory model to catch typical memory problems early
  (under/over flow, leaks, etc)
  - Custom memory allocator gives deterministic memory usage
  - Task based threading model allows large query concurrency with fair execution
  (one long running query won't hold short running queries back)
  - Compressed in-memory data format
.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: RapidsSE; architecture
   single: architecture

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


.. index::
   single: RapidsSE; cluster model
   single: cluster model

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


.. figure:: clusterModel.*
   :align: center

   The model of a RapidsSE cluster.


From a query execution perspective, the cluster model is peer-to-peer. There
is no hierarchical order and each node is able to talk to every other node.
A query could bounce data to and from nodes in the cluster if it determined
that it was the most efficient way to execute the user's query.

Because of this peer-to-peer nature, each node has a TCP/IP connection to
every other node in the cluster. This is used for data transfer but also
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


.. index::
   single: RapidsSE; node model
   single: node model

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


.. figure:: nodeModel.*
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
having a large reserved or virtual memory space (column ``VIRT`` in ``top``) but a much
smaller *committed* or resident (column ``RES`` (resident) in ``top``) memory space. While the memory
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


.. index::
   single: RapidsSE; job model
   single: job model

.. _jobArchitecture:

The Job Model
-------------
This section describes how work is performed within RapidsSE. Internally,
RapidsSE is built upon a series of recursive patterns that are used not only
to execute user queries but to also execute one-off and recurring internal
tasks as well. This helps reduce complexity through having a minimum of
idiosyncratic code.

Workloads within RapidsSE are defined in isolated units called *jobs*. A job can
execute across any or all nodes in the cluster. Jobs are uniquely
identifiable within the system.

The work that a job will do is defined by a *pipeline*. A pipeline may have been
created dynamically (for example, to fulfill an external user's query), or it
may be executing a pipeline that has been previously defined and used multiple
times (for example, reading and processing messages from other nodes in the cluster).

A pipeline consists of a series of *tasks* connected together in a topology
across the cluster. A pipeline that connects two tasks serves as a communication
channel between those tasks, irrespective of where they exist in the cluster.
Tasks can send data and/or control messages downstream through the pipeline to
other tasks in response to its own activity.

So a pipeline defines what tasks are run, where in the cluster they are run
and in what order messages flow between them. A pipeline could run purely on
the local node, a remote node, or any or all of the nodes in the cluster
depending on the nature of the work and the location of the data. A pipeline
is not necessarily a linear sequence. It can fan out to multiple tasks (like
a broadcast), fan in from multiple tasks to a single task (for example, when
aggregating a value), and invoke other pipelines. Fanning out is useful for
parallelising an operation across all data partitions on a node or across all
nodes in a cluster. At some point, pipelines that have a fan out will fan in
to collect or aggregate the results before they are sent back to the user.

  The fictitious pipeline in figure :numref:`pipelineExampleFigure`
  example shows that there are 3 tasks that first
  execute on the originating node (the node that first received the query).
  Each of these tasks is fed with data from the previous task.

  After the 3rd task the data is distributed to 1 task on each partition on
  each node in the cluster, parallelising that particular operation.

  The tasks on each partition are then fanned-in to a single task on each
  node in the cluster (this example is a cluster of 2 nodes with 4
  partitions on each node). These tasks might be aggregating results in
  parallel on each node.

  The results of this then get sent back to a single task on the originating
  node for further consolidation. A final task on this node may limit the
  results, for example.

.. _pipelineExampleFigure:

.. figure:: pipelineExample.*

   Logical view of a pipeline.

When a pipeline is being executed, all tasks in that pipeline execute
concurrently, so long as they have work to do. Some tasks require data from a
previous task in the pipeline before they can do anything. Other tasks are
independently capable of generating output data for downstream tasks on their
own.

On each node in the cluster, each task can be executed by a different *thread*.
This allows multiple parts of the pipeline to be executed in parallel, maximising
the CPU usage on the node and shortening the query execution time.


.. only:: html

  .. figure:: jobModel.*
     :target: ../_images/jobModel.png

     Physical representation of jobs, tasks and threads.

     This figure represents two jobs being executed in the system (for example
     two user queries). Jobs can run across all nodes in the cluster. Tasks for
     each job can be spread amongst threads on the nodes they are assigned to.
     The pipeline forms an independent logical connection between the tasks.

.. only:: latex

  .. figure:: jobModel.*

     Physical representation of jobs, tasks and threads.

     This figure represents two jobs being executed in the system (for example
     two user queries). Jobs can run across all nodes in the cluster. Tasks for
     each job can be spread amongst threads on the nodes they are assigned to.
     The pipeline forms an independent logical connection between the tasks.


A given thread may have multiple tasks ready to be executed. Generally each task
is executed in a fair and round-robin nature. The thread will regularly change
which task in its queue it is executing, giving the appearance that all tasks
are being executed concurrently. This control of the task scheduling and
execution gives RapidsSE the power to prioritise any important
internal tasks where it is required. This could favour certain admin actions,
or internal operations such that user queries are not impeded.

Compared to a thread-centric model where each unit of work was parallelised by
its own thread, scheduling of threads occurs in the operating system's kernel.
This not only means that fine grained scheduling is not possible by the
application, but a thread-centric approach also incurs more context switching
overheads too.

But if a RapidsSE thread has a queue of tasks to run, how does it ensure that
all the tasks run fairly? After all, one long-running task could easily prevent
other tasks in the thread's queue from being executed.

RapidsSE solves this by designing tasks as a series of small executable
sequences called *steps*. All tasks in RapidsSE are designed this way.
While a step is being executed by a thread it cannot be interrupted. All steps
are designed to not take excessively long to execute or to block on I/O. After
a step completes the thread may choose to stop executing that task and to
begin executing steps from the next task in its queue. Alternatively, at the
end of a step perhaps a task has finished its work and that task is no longer
runnable (for example, maybe the task has no more data to work with).

This natural breakpoint of finishing a step before a thread potentially changes
which task it is executing means that the thread does not need to preempt the
task and spend time saving its state. At the end of executing a step a task
simply needs to remember which step it was due to execute next. This
simplification saves much time and complexity compared to arbitrarily
interrupting a task.

A single thread may execute a number of steps from a task before deciding
to switch execution to the next task in the queue. In this way, a single
thread can be time-shared across many independent executable tasks. This
allows every task in the system the fair ability to make forward progress.

This is important when we consider concurrent workloads. Without a proper
concurrency strategy a single very long running query could prevent subsequent
short-lived queries from running to completion. The interleaving of steps in
RapidsSE tasks allows both long and short-running queries to be treated fairly.



.. index::
   single: RapidsSE; SQL model
   single: SQL model

.. _sqlArchitecture:

The SQL Query Model
-------------------
Given that the above sections have described how a RapidsSE cluster operates,
how a node operates and how work is done within a node, let's now apply this
to what happens when a SQL query is received by RapidsSE and executed.

When RapidsSE receives a SQL query from a user a job is created to understand
it and execute it. This involves multiple stages and numerous child jobs may
be created in order to break the problem down and solve a part of it.

A SQL query undergoes these transformations from plain query text to a
result set:

.. figure:: sqlModel.*
   :align: center
   :height: 454px

   The transformations of a SQL query to user results.


Each of these stages are described further below.

**Parsing** is the first stage of the process. It turns the text-based SQL query
into a basic plan graph (tree) representation of SQL operators that represents
the SQL text. This makes it much easier for RapidsSE to work with. This stage
will also pick up any syntactic errors that may exist in the query text.

The **Compilation** stage involves taking the initial plan graph that was produced
during the Parsing stage and validating the semantics (meaning) of the statement.
This involves things like checking that the referenced tables or columns
actually exist, validating that expressions involve compatible data types, and
simplifying the plan graph where possible. At this point the plan graph still
represents *what* the user has requested, not *how* it is going to be executed.

**Pipeline Transliteration** is the process of converting the logical plan graph
into a concrete form that starts to describe *how* the query will be
executed. This is represented as a pipeline definition that details the tasks
that need to be run, where the tasks are run and how they are interconnected.
At this point in time the pipeline could now be executed, however it is not
an optimised pipeline so the query would not be very efficient.

The **Optimisation** stage is the process of taking the pipeline definition
generated above and making it much more efficient. This is done by making
logical changes to the pipeline that change how it runs without affecting
the overall result that the pipeline would generate. The less data that the
plan needs to read or copy around the cluster, the faster the query will
complete. Parts of the pipeline plan may be combined or reordered to achieve
this.

**Code Synthesis** is the process of dynamically writing C/C++ code to implement
the expression operations in the query. This dynamic code is compiled to
native machine code via a C/C++ compiler which is then loaded into the RapidsSE
process space. At that point the customised code can be directly executed by
tasks in the pipeline. This is a much faster and more efficient approach than
executing expression operations via an interpreted or bytecode method. It also
allows the dynamic code to directly call back into RapidsSE and make use of
code primitives and structures in RapidsSE, such as indexes, lists, sets, etc.

**Job Execution** is the final stage in the sequence. In this stage a job is
created that executes the customised pipeline definition and native expression
operations that were created across all of the previous stages. This job may
return rows back to the user or an appropriate error message may be generated.

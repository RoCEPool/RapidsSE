.. _architecture:

RapidsSE Architecture
=====================

Goals:
  - stable
  - high performance
  - efficient use of hardware computing resources
  - ...leads to lower TCO

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


Clustered model:
  - peer to peer model, not a master-slave
  - each node is identical
  - each node can receive client connections
  - each node communicates with any other node over a TCP/IP socket
  - each node broadcasts a UDP heartbeat (broadcast or multicast)
  - cluster is controlled by shell scripts that access each node via public key SSH

Node model:
  - Is a single binary written in C++
    - for fast native and deterministic performance

  - Runs as a single process

  - Is configured by arguments derived from a configuration file.
    - Config file exists on each node and is identical
    - Tells the local node its configuration
    - Tells the local node how to find and contact other nodes

  - Has a shared memory store
    - Used for both storing data, temporary query data as well as runtime memory usage.
    - allows greater flexibility for interaction between other processes
      on the same node (e.g. SE prompt)
    - constrains the upper amount of memory that RapidsSE can use
      (so SE doesn't starve other processes on the same node of memory. Good citizen, etc).
    - Seen in /dev/shm on Linux

  - Uses threads to make use of all CPUs on the machine

Job model:
  - Is built on a series of recursive patterns
    - that not only execute queries, but execute the core system itself.
    - reduces idiosyncratic code for fewer bugs and lower complexity through
      a parsimony of concepts.

  - Workload in the system (e.g. a user query) is defined in isolated units called jobs.

  - The work that a job does is defined by a pipeline.
    - A pipeline may be created dynamically, e.g. to represent a SQL query being executed,
      or a pipeline may be predefined and executed repeatedly (e.g. to read messages from
      a client and send back responses).

  - A pipeline consists of a series of tasks connected together in a topology across the cluster.
    - A pipeline serves as a communication channel between tasks. Tasks can send data
      and/or control messages to other parts of the pipeline in response to its own activity.
    - A pipeline could run purely on the local node, a remote node, or any or all of the
      nodes in the cluster.
    - A pipeline is not necessarily linear. It can fan out across all partitions on a node,
      or fan out across across selected/all nodes in the cluster (like a broadcast).
      Similarly, a pipeline can fan in across nodes and partitions to collate or aggregate
      results.

  - Each of the tasks in a job can be allocated to different threads in the system
    - Tasks can execute in parallel if they have work to do, or they can be scheduled
      as work arrives for them.
    - Threads execute tasks in their queue in a round-robin fashion
      - This gives a fair execution model
      - Control of this scheduling and execution also gives RapidsSE the power to prioritise
        certain internal tasks where it is required, so that user tasks spend the most time
        in an executable state. This would not be possible to the same degree in a thread-centric
        model where scheduling is purely controlled by the kernel.
    - minimises thread context switching overheads compared to a model where each job has its own threads

  - The work to be done by each task is broken down into a series of small, discrete chunks called steps
    - A thread may execute one or more steps from a particular task before putting it to
      the end of its run queue. The thread would then execute steps from the next task at
      the top of its run queue.
    - In this way steps from multiple tasks can be interleaved so they all make forward progress
    - steps are designed to not block or sleep so they do not hold up a thread from being able
      to execute other tasks


Query model:
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
.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. _requirements:

System Requirements
===================

.. _supportedOperatingSystems:

Supported Operating Systems
---------------------------
RapidsSE is formally supported on the following operating systems:

•	CentOS 7 (glibc 2.17)
•	CentOS Stream 8 and 9
•	Red Hat Enterprise Linux 8
•	Ubuntu 18, 20 and 22
•	Amazon AWS Linux 2

RapidsSE has not been tested on other operating systems, though it may work
if the target system has a compatible version of glibc and all of the
software requirements are met. RapidsSE has been compiled on CentOS 7 for
the greatest forward compatibility.


Minimum Requirements
--------------------
RapidsSE has the following minimum system requirements:

.. csv-table:: Minimum RapidsSE requirements
   :name: minimum-requirements
   :class: longtable
   :widths: 20, 80

   "**Nodes:**", "1 node"
   "**CPU:**", "4 cores"
   "**RAM:**", "4GB"
   "**Shared Memory:**", "``/dev/shm`` must have at least 3GB allocated to it."
   "**Network:**", "1Gb ethernet"
   "**Ports:**", "Two TCP ports are needed (defaults: client port 9123 and internal port 5541).

   One UDP port is needed (default: 8888), either for UDP broadcast or UDP multicast."
   "**Operating System:**", "Any of the following:

   * CentOS 7,
   * CentOS Stream 8 or 9,
   * Red Hat Enterprise Linux 8,
   * Ubuntu 18, 20 or 22"
   "**Software Dependencies:**", "The following software is required on each node in the cluster:

   * ``gcc`` >= 4.8.5
   * ``libatomic.so.1``
   * ``bash`` >= 4.2
   * ``tar``
   * ``nc`` (netcat)
   * ``ss`` (socket statistics)
   * ``bc``
   * ``awk``
   * ``python`` (either 2.7 or 3.x) is required during installation"
   "**Other requirements:**", "Password-less public key SSH authentication is required
   between all nodes in the cluster, including each node to itself, for the desired
   operating system user account that RapidsSE will operate under."

.. important:: SSH Configuration

   RapidsSE requires password-less SSH access from each node to all nodes in the
   cluster for cluster management operations, including installation, starting and
   stopping nodes. This includes the ability to SSH from one node to itself without
   requiring a key. There are multiple ways to achieve this, but one method is to
   create a password-less SSH key and share that private and public keys with all
   other nodes. The public key needs to be appended to the ``~/.ssh/authorized_keys``
   file on each node for SSH to accept the connection automatically.

The RapidsSE dependencies can be installed with the following command:

  On CentOS 7, CentOS Stream 8 & 9, Red Hat Enterprise Linux 8:

  ``sudo yum install bash gcc libatomic tar nmap-ncat iproute bc python3 gawk ncurses``

  On Ubuntu 18, 20 & 22:

  ``sudo apt install bash gcc libatomic1 tar netcat iproute2 bc python3 gawk ncurses-bin``


Recommended Requirements
------------------------
RapidsSE is a highly parallelised system. It benefits from additional CPU cores
as well as additional nodes in a cluster. In addition, the more system RAM that
is available, the more data that can be loaded into RapidsSE for querying.

A comfortable environment for evaluating RapidsSE would meet all of the
`Minimum Requirements`_ above, with these additional changes:

.. csv-table:: Recommended RapidsSE requirements for evaluation

   "**Nodes:**", "2 nodes"
   "**CPU:**", "16 cores"
   "**RAM:**", "64GB"
   "**Shared Memory:**", "``/dev/shm`` should have at least 60GB allocated to it."
   "**Network:**", "10Gb ethernet"

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

If we consider the `TPC-H <https://www.tpc.org/tpch>`_ schema and query set as
a representative workload then we can give some concrete sizing
recommendations. As a rough rule of thumb, we typically aim for the total
shared memory size in the cluster to be about 2.5 to 3 times the size of
the TPC-H scale factor (SF) data size.

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

     "**Nodes:**", "6 nodes"
     "**CPU:**", "64 cores"
     "**RAM:**", "512GB"
     "**Shared Memory:**", "``/dev/shm`` should have at least 500GB allocated to it."
     "**Network:**", "10Gb ethernet"

Restrictions
------------
RapidsSE will not allow the administrator to configure it to use more than 95%
of the system’s RAM, in order to leave some memory for other system processes.
Because RapidsSE is a large user of system memory, it is expected that RapidsSE
is not run on the same node as other large memory-consuming applications.
.. index::
   single: RapidsSE; features
   single: features of RapidsSE

.. _features:

Features of RapidsSE
====================

Current Features
----------------
RapidsSE currently sports the following features:

* Native-compiled C++ binary;
* Peer-to-peer, multi-node cluster model;
* SQL-based command model;
* Natively compiled and executed dynamic query engine;
* Query and join optimiser;
* Deterministic & efficient non-garbage collected memory model;
* Table partitioning across nodes;
* Table indexes;
* CLI (:ref:`SE Shell <seShellInterface>`) and programmatic
  (:ref:`JDBC <jdbcInterface`) interfaces;
* Data unload and reload;
* Low-memory job management

In broad terms, RapidsSE supports the following command features. Refer to the
:ref:`usersGuide` for more details.

* Table :ref:`create <createTableCmd>`, :ref:`truncate <truncateCmd>`, :ref:`drop <dropTableCmd>`;
* Index :ref:`create <createIndexCmd>`, :ref:`drop <dropIndexCmd>`;
* :ref:`SELECT <selectCmdRef>` queries, including:

  * correlated and non-correlated subqueries;
  * joins (inner only), via index joins;
  * arbitrary expression evaluation;
  * aggregates;

* Distributed and single-origin :ref:`data loading <loadingData>`;

.. index::
   single: RapidsSE; roadmap
   single: roadmap features

Future Roadmap Features
-----------------------

These roadmap features are not in any order of priority, and are subject to
change. Nor is this a complete list of future RapidsSE features.

* Node data rebalancing;
* Query cancellation;
* High Availability;
* Dynamic clustering (node addition & removal);
* Data persistence to disk;
* Paging (overflow memory to disk);
* Schema metadata;
* Hashmap joins;
* Data export;
* Transactions;
* Additional DML commands (``INSERT``, ``UPDATE``, ``DELETE``);
* Security (authentication & authorisation);
* Optimised query compilation;
* Further ``SELECT`` query support, including:

  * Outer joins;
  * ``DISTINCT``;
  * ``UNION``, ``INTERSECT``;
  * Windowing;

* Ongoing query optimiser improvements;
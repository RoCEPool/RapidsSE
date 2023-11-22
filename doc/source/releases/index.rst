.. index::
   single: RapidsSE; releases
   single: releases

.. _releases:

Releases
========

.. only:: html

  .. contents:: Contents of this section:
     :depth: 2
     :backlinks: none

.. _releaseSummary:

Summary of Releases
-------------------
.. Reminder - put newest releases at the top of this table

.. list-table::
  :header-rows: 1
  :widths: 20 40 40

  * - Version
    - Date
    - Release Type
  * - :ref:`v1.1.0 <release1.1.0>`
    - 2023-??-??
    - Minor
  * - :ref:`v1.0.0 <release1.0.0>`
    - 2022-09-28
    - Major


.. COMMENT
   Release note style
   ******************
   1. Date
   2. Reason for release
   3. List of components and versions (shell, jdbc, wireline protocol)
   4. List of changes, including bug ID. Each one should start with a word like:
       1. Fixed
       2. Added / Removed / Replaced
       3. Implemented
       4. Changed
       5. Upgraded
       6. Improved

.. COMMENT
  Helpful Jira Filters
  ********************
  .
  For listing open issues:
    https://rapidsdataproject.atlassian.net/jira/software/c/projects/SE/issues/?filter=allopenissues&jql=project%20%3D%20%22SE%22%20AND%20status%20in%20%28%22Backlog%22%2C%20%22In%20Progress%22%29%20ORDER%20BY%20key%20DESC
    project = "SE" AND status in ("Backlog", "In Progress") ORDER BY key DESC
  .
  For listing fixed issues:
    https://rapidsdataproject.atlassian.net/jira/software/c/projects/SE/issues/?filter=allopenissues&jql=project%20%3D%20%22SE%22%20AND%20status%20in%20%28Fixed%2C%20Verified%29%20AND%20fixVersion%20in%20%281.1.0-dev1%2C%201.1.0-dev2%2C%201.1.0-dev3%2C%201.1.0-dev4%2C%201.1.0-dev5%2C%201.1.0-dev6%2C%201.1.0-dev7%2C%201.1.0-dev8%29%20ORDER%20BY%20key%20DESC
    project = "SE" AND status in (Fixed, Verified) AND
      fixVersion in (1.1.0-dev1, 1.1.0-dev2, 1.1.0-dev3, 1.1.0-dev4, 1.1.0-dev5, 1.1.0-dev6, 1.1.0-dev7, 1.1.0-dev8)
      ORDER BY key DESC


.. _release1.1.0:

Release v1.1.0
------------------
RapidsSE v1.1.0 was released on ??? ???? 2023.

.. rubric:: Purpose:

This release deprecates the SE Prompt (the original mechanism for ad-hoc entry
of commands) in favour of the SE Shell, a JDBC-based terminal application.
RapidsSE also introduces an updated code synthesizer, which is responsible for
writing out the code that is dynamically executed for each query. This
synthesized code is now run through an optimising compiler such as gcc or clang.


.. rubric:: Component versions:

.. list-table::
  :header-rows: 1

  * - Component
    - Version
  * - RapidsSE
    - v1.1.0
  * - SE Prompt
    - v1.0.4
  * - RapidsSE JDBC Driver
    - v1.0.4
  * - Wireline Protocol
    - v5

.. rubric:: Changes in this release:

.. list-table::
  :header-rows: 0
  :widths: 15 85

  * - **NEW**
    - [345] Added SE Shell as a replacement for SE Prompt for ad-hoc human
      interactions with RapidsSE.
  * -
    - [344] Added a JDBC driver based on the RapidsSE wireline protocol (v5)
      to the package. This is used by the SE Shell, but can also be used by
      other client applications.
  * -
    - [343] RapidsSE now accepts connections and clients using the
      :ref:`wireline protocol <wirelineProtocolInterface>`.
  * -
    - [309] If any node fails to start when the cluster is started then the user
      will be directed to look at the log files for error details.
  * - **CHANGED**
    - The code synthesizer has been updated and now compiles the dynamically-generated
      code through an optimising compiler, such as gcc or clang.
  * - **REMOVED**
    - SE Prompt has been removed and replaced by SE Shell.
  * - **FIXED**
    - [314, 310, 306-304, 302-293, 291-289, 287] Various ``INTERVAL`` type fixes.
  * -
    - [308] RapidsSE would fail if it was started with ``--reload`` but without
      the associated argument.
  * -
    - [241, 258] Tables can be created with column names that use reserved words
  * -
    - [240] Low memory protection may not catch a case of repeatedly loading data.
  * -
    - [229] RapidsSE may fail when a query containing a ``LIKE`` expression pattern
      which is not a literal value.
  * -
    - [277] The load of string values greater than the documented limit of 4,000 bytes
      was not generating an error.
  * -
    - [276] Loading of ``DATE`` values with years greater than 2126 would result in
      incorrect values.
  * -
    - [268] Cast of a string containing whitespace to a ``FLOAT`` would return
      inconsistent and incorrect results.
  * -
    - [256] In the installer, the name of the parent directory mentioned in an
      error message was not correct when that parent should be the root directory (``/``)
  * -
    - [247] An expression ``SUM(expr) - NULL`` was not returning a ``NULL`` result.
  * -
    - [241] ``LSIDXCOL`` command would not report an error when the index name
      was left off.
  * -
    - [240] A load when RapidsSE was low on available memory would crash RapidsSE.

.. rubric:: Known Issues:

.. list-table::
  :header-rows: 0
  :widths: 15 85

  * - [232]
    - An internal error can occur when a ``LIKE`` wildcard is repeatedly applied
      to an empty table.
  * - [230]
    - RapidsSE may fail when querying a table with excessive columns.
  * - [227]
    - SE may fail when a query string is excessively long.

TODO: Complete this


.. _release1.0.0:

Release v1.0.0
------------------
RapidsSE v1.0.0 was released on 28th September 2022.

.. rubric:: Purpose:

This is the first major release of RapidsSE and the associated SE Prompt.
The aim of this release is to get initial feedback and testing of the core
RapidsSE features.


.. rubric:: Component versions:

.. list-table::
  :header-rows: 1

  * - Component
    - Version
  * - RapidsSE
    - v1.0.0
  * - SE Prompt
    - v1.0.0

.. rubric:: Changes in this release:

.. list-table::
  :header-rows: 0
  :widths: 15 85

  * - **NEW**
    - Everything!
  * - **CHANGED**
    - None
  * - **REMOVED**
    - None
  * - **FIXED**
    - None

.. rubric:: Known Issues:

.. list-table::
  :header-rows: 0
  :widths: 15 85

  * - [241, 232]
    - Tables can be created with column names that use reserved words, which
      could lead to internal errors and potentially failure of a node.
  * - [240]
    - Low memory protection may not catch a case of repeatedly loading data.
  * - [237]
    - RapidsSE prompt can fail when run as root user.
  * - [230]
    - RapidsSE may fail when querying a table with excessive columns.
  * - [229]
    - RapidsSE may fail when a query containing a ``LIKE`` expression pattern
      which is not a literal value.
  * - [227]
    - SE may fail when a query string is excessively long.





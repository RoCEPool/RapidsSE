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
  * - :ref:`v1.0.0 <release1.0.0>`
    - 2022-09-28
    - Major


.. Release note style
.. 1. Date
.. 2. Reason for release
.. 3. List of components and versions (shell, jdbc, wireline protocol)
.. 4. List of changes, including bug ID. Each one should start with a word like:
..     1. Fixed
..     2. Added / Removed / Replaced
..     3. Implemented
..     4. Changed
..     5. Upgraded
..     6. Improved

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
  :widths: 85 15

  * - Tables can be created with column names that use reserved words, which
      could lead to internal errors and potentially failure of a node.
    - [241, 232]
  * - Low memory protection may not catch a case of repeatedly loading data.
    - [240]
  * - RapidsSE prompt can fail when run as root user.
    - [237]
  * - RapidsSE may fail when querying a table with excessive columns.
    - [230]
  * - RapidsSE may fail when a query containing a ``LIKE`` expression pattern
      which is not a literal value.
    - [229]
  * - SE may fail when a query string is excessively long.
    - [227]





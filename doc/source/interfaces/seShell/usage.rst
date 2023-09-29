.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. index::
   single: SE Shell; usage

.. _seShellUsage:

Using SE Shell
==============

.. index::
   single: SE Shell; starting

Starting
--------

SE Shell is started up by running the ``seshell.sh`` script file on a Linux
platform or the ``seshell.bat`` file on a Windows platform. If you are running
SE Shell from the installation directory of one of the RapidsSE nodes, then the
``seshell.sh`` file can be found in the root of the installation directory
(typically ``/opt/rapidsse``).

Running this script file will, by default, attempt to connect to a RapidsSE
node running on the localhost and listening on the default port (``9123``).
An error will be displayed if SE Shell cannot connect to this RapidsSE node.

SE Shell will also accept the following command line arguments:

.. list-table:: SE Shell Command Line Arguments
  :widths: 20, 15, 65
  :header-rows: 1

  * - Option
    - Default
    - Description
  * - ``-h <host/ip_address>``
    - ``localhost``
    - The hostname or IP address of the node in the RapidsSE cluster that
      SE Shell should connect to.
  * - ``-p <port_number>``
    - ``9123``
    - The port number of the node in the RapidsSE cluster that SE Shell
      should connect to.
  * - ``-s``
    -
    - Runs SE Shell in :ref:`scripting mode <seShellScriptingMode>`
  * - ``-t``
    -
    - Specifies that when a ResultSet is being displayed the data types for
      each column should be printed on the next line after printing the column headings


.. rubric:: Examples

.. code:: bash

  cd /opt/rapidsse

  # RapidsSE running on localhost:9123
  ./seshell.sh

  # RapidsSE running on a host resolvable as "node1" with port 9123
  ./seshell.sh -h node1

  # RapidsSE running on a host resolvable as "node1" with port 12345
  ./seshell.sh -h node1 -p 12345


.. index::
   single: SE Shell; stopping

Stopping
--------


.. index::
   single: SE Shell; entering commands

Entering Commands
-----------------

semi colons
multi-line commands
comments
up arrow



.. index::
   single: SE Shell; built-in commands

Built-in Commands
-----------------
repeat
run or source
set page/separator/precision/stack trace
set stack trace
exit, bye, quit



.. index::
   single: SE Shell; scripting mode

.. _seShellScriptingMode:

Scripting Mode
--------------




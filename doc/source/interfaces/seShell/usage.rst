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

Stopping SE Shell after it has been started is as simple as one of the
following options:

- Pressing ``<Control>-C``

- Entering the command ``exit;``, ``quit;``, or ``bye;``

SE Shell will exit out back to the command line.



.. index::
   single: SE Shell; entering commands

Entering Commands
-----------------

After starting up SE Shell, a prompt will be displayed that indicates that
SE Shell is ready to accept commands. The prompt will look like this:

.. code::

  rapids >

Commands can be executed by entering them at the prompt and terminating the
command with a semicolon (``;``) before pressing enter.
Commands that are not terminated with a semicolon are assumed to continue on
the next line, until a semicolon and enter key combination is detected.

SE Shell accepts commands being split over multiple lines. This can make it
easier to type and edit long commands. SE Shell will submit the command to RapidsSE
as it is written, with any line breaks in the middle of the command.

.. index::
   single: SE Shell; history recall

.. rubric:: Command History, Recall & Editing

SE Shell supports line-editing and recall of commands. Pressing the up arrow key will
cycle through and recall the past history of commands. Pressing the down arrow
will cycle through the history in the opposite direction. This makes it easier to
re-execute previous commands rather than re-typing them. SE Shell will only
remember commands from the current session. Once the user has exited out of
SE Shell the history will be forgotten.

Please note that while SE Shell allows for commands to span multiple lines,
any recalled commands can only be displayed on a single line. For this reason,
SE Shell will display any line breaks in the command as the single printable character
``^J``. This is purely for display and internal limitation reasons. If the
multi-line command is submitted to RapidsSE the command will contain the
correct line breaks.

.. rubric:: Comments

SE Shell supports the following comment markers in commands:

- ``//`` for C++ style line comments
- ``--`` for line comments
- ``/*`` and ``*/`` for C style block comments


Cancelling Queries
------------------

SE Shell does not yet support cancelling queries that are currently running
because RapidsSE does not support this just yet.


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




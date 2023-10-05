.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: RapidsSE; using
   single: using RapidsSE

.. _usingSE:

Using RapidsSE
==============
This section describes the ways that users can interact with RapidsSE once
it has been installed and started by an administrator. Further details on
each of the components below can be found in the :ref:`interfaces` section.


.. index::
   single: SE Prompt; overview

SE Prompt
---------
SE Prompt is the original mechanism for submitting commands and queries to
RapidsSE and getting back responses. It was never intended to be a complete
shell-like experience. Instead it is intended to be deprecated and replaced
by the :ref:`SE Shell <seShellInterface>`.

SE Prompt is started by running the command ``./prompt.sh`` in the installation
directory of any of the nodes in the RapidsSE cluster. SE Prompt will establish
a direct connection to the local node via the shared memory segment and it will
use this for submitting commands and returning results. It is not possible to
run SE Prompt across a network connection from a client machine to a server.

Once started, SE Prompt will display the line prefix ``se>`` to indicate that it is
ready to accept user input.

SE Prompt does not support "up-arrow" command history or command editing other
than simple backspaces.

Commands are semicolon terminated. SE Prompt will not submit the command until
it comes across the terminating semicolon.

To exit SE Prompt, either press ``Control-C`` or enter the command ``bye;``.

Here's an example of starting and exiting the SE Prompt:

.. code:: console

   craig@hubble:rapidsse$ ./prompt.sh
   RapidsSE 1.0.0 (release version, pid 15863, git 3225a7b5bb386300)
   (c) 2020 Rapids Data Technology Co. Ltd
   se>
   se> bye;

   Good Bye!

If SE Prompt is executed when the current node is not running then an error will
be produced:

.. code:: console

   craig@hubble:rapidsse$ ./prompt.sh
   ERROR: Cannot start SE Prompt - RapidsSE is not running on this node.


.. index::
   single: SE Shell; overview

SE Shell
--------
SE Shell is a command line interface (CLI) method of executing commands and viewing
results that are formatted for human readability. SE Shell is built for the
ad-hoc entry and execution of commands by users. It is not built to be a
programmatic interface. SE Shell offers a more user-friendly CLI experience by supporting:

* The ability to run SE Shell on a remote client computer (without having to
  install RapidsSE), connecting to RapidsSE over a socket connection.
* Recent command history recall via the up-arrow key.
* A more intuitive experience when editing commands.
* Pretty-printing of query results for easier reading.

SE Shell utilises the capabilities of the RapidsSE :ref:`JDBC Driver <jdbcInterface>` for
submitting commands and extracting results.

SE Shell is started by running ``./seShell.sh`` in the root of the RapidsSE
installation directory. When SE Shell has started it will display the prompt
``rapids >``. To exit the SE Shell simply enter the command ``exit;`` or
press ``<Control>``-``<C>``. This can be seen in the example below:

.. code:: console

  craig@machine$ ./seshell.sh
  rapids > exit;
  craig@machine$

Further information on interfacing to RapidsSE with SE Shell can be found
:ref:`here <seShellInterface>`.

.. index::
   single: JDBC driver; overview

JDBC Driver
-----------
The :ref:`RapidsSE JDBC Driver <jdbcInterface>` is a feature that will appear with `SE Shell`_ in
the near future. It allows for easy programmatic access to RapidsSE from
Java applications for submitting commands and getting results. The RapidsSE
JDBC Driver communicates with RapidsSE over a socket connection, and it
transmits and receives messages in accordance with the :ref:`RapidsSE wireline
protocol <wirelineProtocolInterface>`.

To use the JDBC driver, Java applications must first include the driver's
jar file in the Java application's classpath. Then the Java application
must request a new connection using a JDBC URL that identifies the JDBC
driver to be used as well as the connection parameters (host, port, etc).

The base of the RapidsSE JDBC URL is ``jdbc::se://``.

Further information on interfacing to RapidsSE with SE Shell can be found
:ref:`here <seShellInterface>`.
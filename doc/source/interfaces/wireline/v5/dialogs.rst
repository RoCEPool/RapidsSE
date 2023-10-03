.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: wireline protocol; v5, dialogs

.. _wlpv5MessageDialogs:

Message Dialogs
===============

.. _wlpv5FlowControl:

Flow Control of Message Dialogs
-------------------------------

Flow control within a dialog is currently the responsibility of the underlying
transport mechanism and not a feature of the messages or dialogs. In the
current implementation it is TCP that provides flow control of messages sent
within a dialog. Flow control can stop either the client or server from sending
further messages until the other side has read the prior message that have been
sent. Thus, both the client and server must be well behaved and read from the
transport mechanism when a message is waiting.

Generally this happens naturally due to the synchronous and serial nature of
the wireline protocol design - after the client sends a request it will typically
wait for 1 or more responses from the server, and only when the current request
dialog is complete can it expect the server to read the next request.

However one example where flow control can be used by the client is in processing
large data sets that are sent as a result of a query. When RapidsSE sends rows
of data it does not do so in one huge chunk. If the result data is large enough,
RapidsSE will split the data into multiple ``RowDataResponse`` messages and send
them individually. The size of each ``RowDataResponse`` message might be limited
by the number of rows (e.g. 10,000) or by size (e.g. 1MB). A client may not wish to
buffer in memory all of the result data that a query generates. So instead of reading
all the result data and then processing it, a client could potentially read it
in a piecemeal and sequential fashion, only reading the next ``RowDataResponse``
message when it has finished processing the previous one. If the server was
sending these ``RowDataResponse`` messages faster than the client is reading them
off the transport then the server will eventually get blocked from sending any
more of them. This would allow the client to process the messages with a minimal
memory requirement.


.. index::
   single: wireline protocol; v5, dialogs - connection

.. _wlpv5ConnectionDialog:

Connection Dialog
-----------------

The Connection Dialog is shown in the diagram below. It describes the sequence
of messages that are exchanged between the client and server before the server
will accept any other dialogs from the client.

.. figure:: /interfaces/wireline/v5/dialogs/ConnectionDialog.*
   :align: center

   The Connection Dialog

In the Connection Dialog the concepts of protocol compatibility, tunnel
establishment, user authentication, and session startup are clearly
separated out.

The first interactions in this dialog involves checking to see if the client
and server are both talking with a compatible protocol version that they both
understand. A ``ProtocolIncompatibleResponse`` message is sent in the case of
an incompatibility and the socket is then disconnected. Please note that we use
a specific message to indicate protocol incompatibility rather than the generic
``ErrorResponse`` message because we want to protect the format of the messages
involved in the compatibility check as much as possible. i.e., this allows the
generic ``ErrorResponse`` message to be changed without it affecting the
protocol compatibility check.

It is expected that this preliminary protocol compatibility interaction will
remain largely unchanged for some significant period of time. This would allow
the rest of the protocol to be updatable but still allow a client to easily
determine if it is compatible with the protocol or not.

After ensuring that both the client and server are talking a compatible protocol
version, the client negotiates the confidentiality of the connection it wants.
At this point in time RapidsSE only supports clear text tunnels (i.e., no
encryption) however part of the dialog caters for allowing SSL tunnels or other
encryption mechanisms to provide confidentiality on the connection at some
point in the future.

The client sends the server a ``ClearTextTunnelRequest`` and the server duly
acknowledges this by replying with a ``TunnelSupportedResponse``, or an
``ErrorResponse`` if something has gone wrong or the given tunnel type could
not be established. The server may also send zero or more warning messages
(as ``WarningResponse`` message types) before sending the ``TunnelSupportedResponse``
or ``ErrorResponse``. A ``WarningResponse`` is a non-error condition that the
client may want to know about. These warnings do not need to be handled, but
the end used may want to know that they occurred somehow (e.g. by printing
or logging them perhaps).

At this point the client and server are both talking a compatible protocol
version over an appropriate tunnel. Now the connection needs to be authenticated.
Currently RapidsSE does not have an authentication mechanism, but the protocol
has been designed to at least accommodate the fact that this will happen at a
later stage. Having defined the basis for an authentication dialog will allow
for greater backwards compatibility with clients that use an older version of
the wireline protocol.

Since RapidsSE doesn’t do any actual authentication the credentials provided in
the ``PlainAuthenticationRequest`` message are ignored and the server responds
with an ``AuthenticationOkResponse`` acknowledging to the client that
authentication is successful.

After authenticating the connection, the client then needs to start a session
with RapidsSE and provide it with any starting session values (like a default
catalog or schema to use). This is done by the client sending a
``SessionChangeRequest`` message. RapidsSE may not support or use these properties
just yet, but this support will grow over time.

Once RapidsSE responds to this ``SessionChangeRequest`` message with a
``SessionChangeResponse`` then the Connection Dialog is complete. The client
is then able to start executing another dialog such as the Query Dialog in
order to execute SQL commands in the system.


.. only:: html

  The implementation of this Connection Dialog in a client can be shown with the
  following state transition diagram (:numref:`wlpv5ConnectionDialogStateDiagramHtml`).
  This shows an alternate view on what messages can be sent or received at
  different times and how the client should respond. Clients and libraries do not
  need to be represented in this way, it is purely for understanding purposes.

  .. _wlpv5ConnectionDialogStateDiagramHtml:

  .. figure:: /interfaces/wireline/v5/dialogs/ConnectionDialogStateDiagram.*
     :align: center
     :alt: Client state transitions during the Connection Dialog
     :target: ../../../_images/ConnectionDialogStateDiagram.png

     Client state transitions during the Connection Dialog

.. only:: latex

  The implementation of this Connection Dialog in a client can be shown with the
  following state transition diagram (:numref:`wlpv5ConnectionDialogStateDiagramLatex`).
  This shows an alternate view on what messages can be sent or received at
  different times and how the client should respond. Clients and libraries do not
  need to be represented in this way, it is purely for understanding purposes.

  .. raw:: latex

      \begin{landscape}

  .. _wlpv5ConnectionDialogStateDiagramLatex:

  .. figure:: /interfaces/wireline/v5/dialogs/ConnectionDialogStateDiagram.*
     :align: center

     Client state transitions during the Connection Dialog

  .. raw:: latex

      \end{landscape}


.. index::
   single: wireline protocol; v5, dialogs - query

.. _wlpv5QueryDialog:

Query Dialog
------------

The Query Dialog is an exchange of messages by which a client can execute a
query in the system. The query being executed may return a result set with rows
of data, or it may make a change in the system and return no result set at all.

To clarify some terminology used in this dialog, a *Query* is a single request
from the client to the server. That request may contain one or more *Statements*
that are executed. A query with a single statement will contain a command string
with a single logical statement such as ``"SELECT * FROM t1;"``. However a
query with multiple statements will contain a command string with multiple
commands separated by semicolons, such as
``"SELECT * FROM t1; TRUNCATE t1; DROP TABLE t1;"``. This can also be referred
to as a batched statement because the client has asked the server to execute
multiple commands and return a set of results.

RapidsSE supports both single statement queries and multiple statement queries.
This Query Dialog will handle both cases. We will examine both cases separately.


Single Statement Queries
````````````````````````

The Query Dialog for single statements is shown below in
:numref:`wlpv5QueryDialog-SingleStatement`. This dialog can be run
after a client has successfully completed the Connection Dialog. It starts with
the client sending a ``QueryExecuteRequest`` message, which contains the command
to be executed along with some other optional flags to control how results and
errors are handled.

.. _wlpv5QueryDialog-SingleStatement:

.. figure:: /interfaces/wireline/v5/dialogs/QueryDialog-SingleStatement.*
   :align: center

   The Query Dialog for a single statement

When the server receives this message it extracts out the command string and
parses it to ensure it contains one or more valid statements. If this command
string has a syntax error then the server will return an ``ErrorResponse``
message and the dialog is complete at that point. No statement could be
executed.

If the command string is successfully parsed then the server will send the
client a ``QueryReceiptResponse`` message. This message is an acknowledgement
of the statements that were parsed out of the command string in the
``QueryExecuteRequest``. It contains a unique ID and a secret key for each
separate statement that will be executed in this query. This information is
critical for cancelling long running statements, and the client should hold
on to this information until the statements have completed.

At this point the RapidsSE server is ready to begin executing each of statements
that were parsed out of the ``QueryExecuteRequest``. Let us assume that there
is only one logical statement to be executed.

While the statement is being executed, RapidsSE might generate some warnings.
These are non-fatal conditions that might affect or might help describe what
any abnormal conditions that RapidsSE encounters during the execution of this
statement. There may be more than one warning generated, and these warnings
can be sent back to the client as ``WarningResponse`` messages at at time
up until the statement has finished executing.

From here there are three alternate scenarios for the result of the statement:

#. The statement encounters an error.

#. The statement generates a result set with zero or more rows.

#. The statement is successful but the nature of the statement does not generate
   a result set.

If an error is encountered after the server has previously sent a
``QueryReceiptResponse`` message (case 1) then that ``ErrorResponse`` indicates that
the execution of the current statement has now ended. No further data or results
for the current statement will be returned.

If the nature of the statement is such that it generates a result set (case 2, e.g.
it is a ``SELECT`` statement), then the server will first send a ``RowMetadataResponse``
message that describes the column names and types of the result data that
will follow. After that the server will send zero or more ``RowDataResponse``
messages that contain the actual row data for the result set. The
``RowMetadataResponse`` message will always be sent, even if there are no
rows being returned in the result set. This indicates to the client that it has
received an empty result set.

If there are rows to be returned then the server will group them into a number
of ``RowDataResponse`` messages. The server wants to strike a balance between
not having too few rows in a single ``RowDataResponse`` message and not having
the ``RowDataResponse`` message being too large. These messages will typically
not exceed 1MB each and by default will usually contain no more than 10,000 rows
each.

While ``RowDataResponse`` messages are being sent back to the client, it is still
possible for an error to be encountered. e.g. the values in a row may cause a
divide-by-zero error to be detected. Like case (1), if an ``ErrorResponse`` is
received by the client at this point then the execution of that statement is
considered finished.

Once all ``RowDataResponse`` messages have been sent back to the client, a
``StatementResponse`` message is sent to indicate that the statement
completed its execution and no errors were encountered. This is not the end
of the dialog however, as a query may contain multiple statements to be
executed.

Similarly, if the statement being executed did not generate a result set (case 3) then
a ``StatementResponse`` is sent back to the client to indicate that the
statement has finished executing and it was successful.

Finally, the server indicates that there are no more statements to be executed
and that the dialog is complete by sending a ``QueryExecuteResponse`` message.
When the client receives this it is then able to start a new dialog with the
server.

A fictitious example of a client executing a query that contains a single
statements is show in :numref:`wlpv5QueryDialog-Example-SingleStatement`.
In this example we can see the statement returns a result set with
row data sent over multiple ``RowDataResponse`` messages. The statement
execution is concluded by a ``StatementResponse`` message being sent and the
dialog is concluded by a ``QueryResponse`` message being sent by the server.

.. _wlpv5QueryDialog-Example-SingleStatement:

.. figure:: /interfaces/wireline/v5/dialogs/QueryDialog-Example-SingleStatement.*
   :align: center

   A fictitious example of the Query Dialog for a query that contains
   a single statements



Multiple Statement Queries
``````````````````````````

RapidsSE supports the ability to execute batched statements. That is, a single
``QueryExecuteRequest`` is sent to the server that contains a command string
that is parsed into more than one individual statements to be executed. Each of
these statements may generate an error, a result set or no results at all.

The general message sequence for the Query Dialog when there are batched
statements is shown in :numref:`wlpv5QueryDialogMultipleStatements`.

.. _wlpv5QueryDialogMultipleStatements:

.. figure:: /interfaces/wireline/v5/dialogs/QueryDialog-MultipleStatements.*
   :align: center

   The Query Dialog for a query that contains multiple (batched) statements

In this sequence, each statement is executed sequentially. The next statement
is not started until the previous one has completed. Let us assuming that the
command string in the ``QueryExecuteRequest`` was parsed successfully and the
server returned a ``QueryReceiptResponse`` message to the client. The server
immediately starts executing the first statement in the batch. The end of each
statement is marked by the client either receiving an ``ErrorResponse`` or a
``StatementResponse`` message. Any messages received after this then pertain
to the next statement in the batch being executed. If it is important for the
client to know what results correlate to which statement then it is up to the
client to track this. RapidsSE will always execute the statements in the order
that they were parsed from the original command string in the ``QueryExecuteRequest``,
and this is also reflected in the number and order of receipts that is sent
back to the client in the ``QueryReceiptResponse`` message.

After the last statement has been executed the server will send a
``QueryExecuteResponse`` message back to the client to indicate that the
dialog is now complete. The client can now initiate another dialog if it
chooses to.

A fictitious example of a client executing a query that contains four
batched statements is show in :numref:`wlpv5QueryDialogExampleMultipleStatements`.
In this example we can see the first statement returns a result set with
row data sent over multiple ``RowDataResponse`` messages. The second batched
statement completes execution successfully without generating a result set.
The third batched statement returns an empty result set with no rows and
the fourth batched statement generates an error.

.. _wlpv5QueryDialogExampleMultipleStatements:

.. figure:: /interfaces/wireline/v5/dialogs/QueryDialog-Example-MultipleStatements.*
   :align: center

   A fictitious example of the Query Dialog for a query that contains
   multiple (batched) statements


Client State Transitions
````````````````````````

.. only:: html

  The implementation of the Query Dialog in a client can be shown with the
  following state transition diagram (:numref:`wlpv5QueryDialogStateDiagramHtml`).
  This shows an alternate view on what messages can be sent or received at
  different times and how the client should respond. Clients and libraries do not
  need to be represented in this way, it is purely for understanding purposes.

  .. _wlpv5QueryDialogStateDiagramHtml:

  .. figure:: /interfaces/wireline/v5/dialogs/QueryDialogStateDiagram.*
     :align: center
     :alt: Client state transitions during the Query Dialog
     :target: ../../../_images/QueryDialogStateDiagram.png

     Client state transitions during the Query Dialog


.. only:: latex

  The implementation of the Query Dialog in a client can be shown with the
  following state transition diagram (:numref:`wlpv5QueryDialogStateDiagramLatex`).
  This shows an alternate view on what messages can be sent or received at
  different times and how the client should respond. Clients and libraries do not
  need to be represented in this way, it is purely for understanding purposes.

  .. raw:: latex

      \begin{landscape}

  .. _wlpv5QueryDialogStateDiagramLatex:

  .. figure:: /interfaces/wireline/v5/dialogs/QueryDialogStateDiagram.*
     :align: center

     Client state transitions during the Query Dialog

  .. raw:: latex

      \end{landscape}


.. index::
   single: wireline protocol; v5, dialogs - query cancellation

.. _wlpv5QueryCancellationDialog:

Query Cancellation Dialog
-------------------------
.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation. Clients may wish to structure their
  code to prepare for this dialog being available in the future.

The query cancellation dialog allows a client to cancel a *statement* that is
currently running. This is useful when a statement may be running for longer
than anticipated, or if the statement contains an error and is processing too
much data.

.. note::
  While this dialog is named *query* cancellation, it really is about cancelling
  *statements* instead. A ``QueryExecuteRequest`` may contain a command string
  that is parsed into multiple separate statements that are executed (a batched
  statement string). A cancellation request operates on these individual statements
  rather than on the original query request as a whole.

The wireline protocol prioritises the simplicity of the more common
dialogs (such as the :ref:`Query Dialog <wlpv5QueryDialog>`) rather than having
a singular, highly complex approach that fits all operations. As a result the
query cancellation dialog is a bit more complicated to implement. The wireline
protocol is designed to be largely synchronous and serial, so cancelling a
statement that a client is currently waiting on cannot happen on the same
connection. Instead a new connection is required.

If the request to cancel a statement is occurring on a new connection then there
are a few issues that need to be dealt with. Namely:

#. How does the client specify the statement(s) to be cancelled on this new connection?

#. How does the server validate that the client on this new connection has the
   authority to cancel this statement? After all, the server cannot tell that
   the two separate connections are from the same logical client.

This is achieved with the information that is returned by the server to the
client in the ``QueryReceiptResponse`` message. When a client starts the
:ref:`Query Dialog <wlpv5QueryDialog>` it sends the server a ``QueryExecuteRequest``.
The server parses the command string that is contained in that request and
it returns a ``QueryReceiptResponse`` message. This message contains the
following information for each statement in the command string that will be
executed:

#. A unique ID that identifies the statement.

#. A secret key for this statement.

#. The subset of the command string that pertains to the parsed statement.

The unique ID and secret key of the statement is only known by the server and
the client that submitted the request. The client can now establish a second,
independent connection to RapidsSE and use this information to uniquely
identify the statement to be cancelled (the unique ID) and prove that it has
the authority to do so (by knowing the secret key). The server will look up
the statement by its ID and if it is still active and the secret key provided
by the client matches what the server has on record, then the server will
begin cancelling the query.

In the time for all of this to occur, a number of things can happen:

- The credentials of the statement ID and secret key do not match;

- The statement could have already finished executing;

- The statement is actively running and the cancellation is acknowledged.

It should be noted that the query cancellation is a best-effort endeavour.
When the cancellation request is acknowledged this does not mean that the
cancellation has completed. This may take some time to do so.

We can see this interaction happening in :numref:`wlpv5QueryCancellationDialogFigure`.
The client remembers the statement ID and secret key that was provided to it
in the ``QueryReceiptResponse`` message and later establishes a new connection
on which to send the ``QueryCancellationRequest`` message with these details.
The server may respond with an ``ErrorResponse`` indicating that the
cancellation failed (e.g. the credentials did not authenticate) or a
``QueryCancellationResponse`` message indicating that the cancellation was
accepted.

After this, the client no longer needs the second connection it created, so it
can safely disconnect it. The long running query on the first client connection
will then receive an ``ErrorResponse`` message indicating that it has been
cancelled.

.. _wlpv5QueryCancellationDialogFigure:

.. figure:: /interfaces/wireline/v5/dialogs/QueryCancellationDialog.*
   :align: center

   The Query Cancellation Dialog


.. index::
   single: wireline protocol; v5, dialogs - ping

Ping Dialog
-----------

The ping dialog is a very simple dialog whereby the client can check that the
server is alive and responding to requests. This could, of course, be achieved
by sending a Query to the server to execute, however the Ping Dialog is simpler
and faster. The server responds with a ``PingResponse`` message. That message
does not contain any useful information however its presence indicates that the
server is alive and responding.

.. _wlpv5PingDialog:

.. figure:: /interfaces/wireline/v5/dialogs/PingDialog.*
   :align: center

   The Ping Dialog


.. index::
   single: wireline protocol; v5, dialogs - termination

Termination Dialog
------------------

The termination dialog specifies how a client can close the connection between
itself and the server. Clients can disconnect from their socket connection to
the server at any time, however if they wish they can also inform the server
that they are disconnecting too, as shown in :numref:`wlpv5TerminationDialog`.
Like other dialogs, this dialog cannot occur unless other dialogs are complete.

.. _wlpv5TerminationDialog:

.. figure:: /interfaces/wireline/v5/dialogs/TerminationDialog.*
   :align: center

   The Termination Dialog

When the server receives a ``DisconnectRequest`` message it never responds.
Instead it cleans up the resources allocated by the client and closes the
socket connection. The client is free to close its socket after sending the
request.


Asynchronous Dialogs
--------------------

All the previous dialogs were synchronous - that is, one side issues a request
and then does not send any more messages until the closing response is received.
Asynchronous dialogs are those that can happen at any time. They are typically
implemented as messages that are sent from the server to the client, so the
client must be prepared to receive them at any point during any other dialog.

.. index::
   single: wireline protocol; v5, dialogs - server shutdown

Server Shutdown Dialog
``````````````````````

.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation. Clients may wish to structure their
  code to prepare for this dialog being available in the future.

The server shutdown dialog is a future dialog that tells the client that the
server intends to shutdown soon. This gives the client the ability to handle it
as they deem necessary. This dialog is implemented as the server sending a
``ShutdownNotification`` message to the client and can be see in
:numref:`wlpv5ServerShutdownDialog`.

.. _wlpv5ServerShutdownDialog:

.. figure:: /interfaces/wireline/v5/dialogs/ServerShutdownDialog.*
   :align: center

   The Server Shutdown Dialog


.. index::
   single: wireline protocol; v5, dialogs - client disconnection

Client Disconnection Dialog
```````````````````````````

.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation. Clients may wish to structure their
  code to prepare for this dialog being available in the future.

The client disconnection dialog is a future dialog where the server is telling
the client that they specifically are being disconnected. This could be for a
number of possible reasons, including but not limited to:

- an administrator is disconnecting a session;

- a node is being taken offline;

- lack of resources.

The server will send the client a ``DisconnectNotification`` message at any time
during any dialog. This is a courtesy message from the server so that the client
knows that a specific action is leading to their connection being terminated,
and not some other cause or failure. The ``DisconnectNotification`` may optionally
include a reason as to why it was being disconnected, which the client may use
to provide useful feedback to the user.

After sending the client the ``DisconnectNotification`` the server closes the
connection to the client. This can be seen in :numref:`wlpv5ClientDisconnectionDialog`.

.. _wlpv5ClientDisconnectionDialog:

.. figure:: /interfaces/wireline/v5/dialogs/ClientDisconnectionDialog.*
   :align: center

   The Client Disconnection Dialog



.. index::
   single: Interfaces; wireline protocol
   single: wireline protocol; version 5

.. _wirelineProtocolInterfaceV5:

Wireline Protocol Version 5
===========================




..   Introduction, semantics
..   Constraints, limitations, dependencies
..   Dialogs
..   State transitions
..   Message Definitions
..   Supported messages


Overview, Concepts & Semantics
------------------------------

The wireline protocol is a transport and language agnostic protocol for
communicating with RapidsSE from a client application or library. This allows
the protocol to be built using different programming languages and for the client and
RapidsSE server to communicate using potentially different transport mechanisms.

Currently, RapidsSE supports clients to connect to it using a TCP/IP socket.
By default RapidsSE will listen on port ``9123``, however this is configurable when
RapidsSE is being installed. A client may connect to any node in the cluster - all
RapidsSE nodes are considered equal and are capable of servicing the query.

The client and RapidsSE server exchange messages between one another in a
defined sequence. A sequence of messages exchanged between client and server
that fulfills a purpose is called a *dialog*. This wireline protocol specification
identifies and describes the dialogs that are supported in this version of
RapidsSE.

The exchange of messages in a dialog is largely synchronous. That is, a client
typically sends a *request* message and then waits until the server responds
with a *response* message. Most messages are named with ``Request`` or
``Response`` suffixes to clearly indicate who is the initiator of the dialog.
This synchronous messaging design makes the common dialogs simpler to understand
and implement, leaving some of the less common and more sophisticated dialogs to
be more complex to implement.

A request message may be answered by one or more response messages, but the end
of the sequence of messages in the dialog is always clearly defined, typically
by a ``Response`` message that is named similarly to the original ``Request``
message that initiated the dialog. e.g. a ``QueryRequest`` message that starts
the Query Dialog is always terminated by a ``QueryResponse`` message.

A connection between client and server only supports a single dialog running at
a time. It is not possible to run multiple dialogs concurrently across the same
connection. However the design of some dialogs may contain inner nested dialogs.
The inner dialog is processed synchronously and serially within the outer dialog
following all of the same rules. When the inner dialog is complete the rest of
the outer dialog can then run to completion. The inner dialog may involve the
server asking a request of the client and the client answering it with an
appropriate ``Response`` message.

Asynchronous messaging is allowed for in the wireline protocol design, however
it is not yet implemented in RapidsSE. These asynchronous messages are only
reserved for exceptional situations that are time sensitive or ad-hoc in nature.
One such example may be a notification that the server is shutting down. These
asynchronous messages are named with a ``Notification`` suffix that distinguishes
them from ``Request`` and ``Response`` message definitions.

The messages that are sent between the client and server are defined in
message definition files written for `Apache Thrift <https://thrift.apache.org/>`_.
This allows the messages to be defined at a high level without needing to
specify the exact byte format, layout and endianness. A Thrift compiler can
compile the message definitions into many different programming languages
which can then construct and send the messages with fewer errors compared to
hand crafted data structures. This approach also provides benefits to data
compression and allows for flexibility with mandatory and optional fields in
message definitions.

When the message definitions are compiled by Apache Thrift, they do not contain
any self identifying information. This means that a custom header needs to be
sent with every message that identifies the type of serialised message that
follows as well as its length (in bytes). This allow a client or server to
read the header first, then read the entire payload and then decode the
payload appropriately.

Constraints, Limitations & Dependencies
---------------------------------------

RapidsSE does not currently implement all of the messages or dialogs that have
been defined. This protocol has been designed with more advanced features in
mind than what is currently supported. Some of these features are included in
this specification for example or forward planning purposes, but this does
not guarantee that they will remain unchanged in a future version of this
specification.

The wireline protocol was designed to keep the most common operations as simple
as possible. The tradeoff of this is that less common operations may be more
complicated to implement. An example of this is the Query Cancellation Dialog
(not yet supported by RapidsSE). Because the wireline protocol is designed to
be synchronous and serial in nature (keeping the common operations simple), a
query cancellation request cannot be sent asynchronously over the same
connection that is also running the query that is to be cancelled. As a result
a query cancellation request must be sent over a separate connection. This is
a design principle of the protocol - keep the common operations simple and let
the exceptional operations be arbitrarily complex.

Because the wireline protocol makes use of `Apache Thrift <https://thrift.apache.org/>`_
for message definitions, it is also limited by the programming languages
supported by Thrift. However Thrift currently supports a wide variety of
languages including all of the most common ones. In the advent that a user
wishes to interface to RapidsSE from a language that isn't supported then any
of the language implementations could be studied to learn and copy how thrift
constructs and formats a message as bytes.

A client application or library that is implementing this wireline protocol will
typically have a dependency on Apache Thrift for the runtime creation and
populating of wireline messages. Apache Thrift also offers additional message
transport mechanisms such as RPCs (Remote Procedure Calls) but these are not
used by RapidsSE. This dependency would not be needed if the client or library
was able to construct a byte-compatible representation of each message
independently.


Protocol Compatibility
----------------------

The wireline protocol includes a mechanism for establishing whether the client
and server can communicate with each other with the same version of the
wireline protocol specification. This is evaluated immediately after a client
has connected to a server. If the client and server do not speak the same
version of the wireline protocol then an error is returned and the connection
is closed.

This approach allows for the potential of the client and/or server to be able
to implement multiple versions of the wireline protocol for backwards
compatibility reasons, if desired.

In order to achieve this compatibility validation, the messages involved in
checking for compatibility must remain stable with respect to future versions
of the protocol specification. That way a client that is running an old
version of the wireline protocol can connect to a server running a newer
version and the client can be gracefully told that it is not compatible.

This protocol compability is described in the Connection Dialog.



Message Dialogs
---------------

Flow Control of Message Dialogs
```````````````````````````````

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

Connection Dialog
`````````````````

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

The implementation of this Connection Dialog in a client can be shown with the
following state transition diagram. This shows an alternate view on what
messages can be sent or received at different times and how the client should
respond.

.. only:: html

  .. figure:: /interfaces/wireline/v5/dialogs/ConnectionDialogStateDiagram.*
     :align: center
     :alt: Client state transitions during the Connection Dialog
     :target: ../../../_images/ConnectionDialogStateDiagram.png

     Client state transitions during the Connection Dialog

.. only:: latex

  .. raw:: latex

      \begin{landscape}

  .. figure:: /interfaces/wireline/v5/dialogs/ConnectionDialogStateDiagram.*
     :align: center

     Client state transitions during the Connection Dialog

  .. raw:: latex

      \end{landscape}



Query Dialog
````````````


Single Statement Queries
::::::::::::::::::::::::

Multiple Statement Queries
::::::::::::::::::::::::::


Query Cancellation Dialog
`````````````````````````

.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation.


Termination Dialog
``````````````````

.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation.

Asynchronous Dialogs
````````````````````

Server Shutdown Notification
::::::::::::::::::::::::::::

.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation.

Client Disconnection Notification
:::::::::::::::::::::::::::::::::

.. note::
  Please note that this dialog is not yet supported in RapidsSE. It is included
  here for reference and education purposes and it is always possible that it
  may change in its final implementation.


Messages
--------

String Encoding
```````````````

All strings being sent across the wireline protocol are assumed to be in UTF-8
encoding. If the interfacing client or library require strings to be in a
different encoding then they should convert the string after receiving it
from the server. Similarly, all strings being sent to RapidsSE should be
converted into UTF-8 format before being sent to the server.

Message Header
``````````````


Message Definitions
```````````````````

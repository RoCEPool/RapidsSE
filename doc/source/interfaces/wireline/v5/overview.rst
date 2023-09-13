.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: wireline protocol; version 5 ; overview

.. _wlpv5Overview:

Overview
========

Concepts & Semantics
--------------------

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

.. _wlpv5Constraints:

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

.. _wlpv5Compatibility:

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


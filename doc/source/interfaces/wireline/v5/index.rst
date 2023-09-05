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
it to be built using different programming languages and for the client and
RapidsSE server to communicate using potentially different transport mechanisms.

Currently, RapidsSE supports clients to connect to it using a TCP/IP socket.
By default RapidsSE will listen on port 9123, however this is configurable when
RapidsSE is installed. A client may connect to any node in the cluster - all
nodes are considered equal and are capable of servicing the query.

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

It is also possible that one dialog may contain a nested inner dialog. The
inner dialog is processed synchronously and serially within the outer dialog
following all of the same rules. When the inner dialog is complete the rest of
the outer dialog can then run to completion. The inner dialog may involve the
server asking a request of the client and the client answering it with an
appropriate ``Response`` message.

Asynchronous messaging is allowed for in the wireline protocol design, however
it is not yet implemented in RapidsSE. These asynchronous messages are only
reserved for exceptional situations that are time sensitive. One such example
may be a notification that the server is shutting down.

The messages that are sent between the client and server are defined in
message definition files written for `Apache Thrift <https://thrift.apache.org/>`_.
This allows the messages to be defined at a high level without needing to
specify the exact byte format, layout and endianness. A Thrift compiler can
compile the message definitions into many different programming languages
which can then construct and send the messages with fewer errors constructing
them.


Constraints, Limitations & Dependencies
---------------------------------------

.. note::
  RapidsSE does not currently implement all of the messages or dialogs that have
  been defined. This protocol has been designed with more advanced features in
  mind than what is currently supported. Some of these features are included in
  this specification for example or forward planning purposes, but this does
  not guarantee that they will remain unchanged in a future version of this
  specification.


Subheading 2
```````````````

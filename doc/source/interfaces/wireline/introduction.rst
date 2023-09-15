.. index::
   single: wireline protocol; introduction

.. _wirelineProtocolIntro:

Introduction
============

The wireline protocol is a specification of how external clients can communicate
with RapidsSE. This protocol specification allows for clients and interfaces to
be developed in a wide variety of languages - that is, it is language agnostic.
The wireline protocol was originally developed for RapidsDB and then later
extended for RapidsSE.

The wireline protocol is a specification of the sequences of messages (referred
to as dialogs) as well as the specification of the messages themselves. This
specification could occur over a variety of transport mechanisms, though RapidsSE
currently only supports the TCP/IP socket.

The wireline protocol is occasionally updated or enhanced to provide new features
or increased performance. These changes may or may not be backwards compatible.

The wireline protocol has a provision in it for versioning the protocol itself.
Different versions of the protocol may exchange different messages, or they may
exchange messages in different sequences (dialogs).

When a client connects to RapidsSE using the wireline protocol, the first
interaction that takes place is for the client to announce what protocol version
it supports. The server will then respond with a message to indicate whether it
can accept this protocol version or not. If it does not support the same protocol
version that the client supports then the server will send an error message back
to the client and then disconnect the socket connection. Otherwise if there is
agreement between the client and server then all communication from then on will
occur in a form that is specific to the client’s requested protocol version.

The current supported version of the wireline protocol for this version of RapidsSE
is **wireline protocol version 5**. It is described in the next section. RapidsSE
does not currently support older versions of the wireline protocol.

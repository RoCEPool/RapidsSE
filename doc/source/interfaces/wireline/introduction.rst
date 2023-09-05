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

The current supported version of the wireline protocol for this version of RapidsSE
is **wireline protocol version 5**. It is described in the next section.

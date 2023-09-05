.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none

.. index::
   single: wireline protocol; change history

.. _wirelineProtocolChangeHistory:

Change History
==============

Periodically the wireline protocol undergoes changes in order to provide
additional functionality or performance to clients. These changes may be simple
changes to message definitions or larger changes to dialog sequences. A
summary of the changes that were made in each version is summarised below.


Version 5
---------

- **Note:** This is the first version of the wireline protocol that RapidsSE supports.

- **Updated:** All messages:

  - Added a namespace for compiling to C++

  - Changed interval enum types to reflect the full range of interval types


Version 4
---------

- **Note:** This version of the protocol is *not* supported by RapidsSE.

- **Updated:** ``MessageTypes`` message

  - Added/updated more message enums, removed enums for deleted messages

- **Added:** ``PingRequest`` message

- **Added:** ``PingResponse`` message

- **Updated:** ``QueryCancellationRequest`` message

  - renamed field 1 from ``queryId`` to ``statementId``

  - renamed field 2 from ``queryKey`` to ``statementKey``

  - made field 2 optional

- **Updated:** ``QueryCancellationResponse`` message

  - Renamed field 1 from ``queryId`` to ``statementId``

  - Added an enum to this message as field 2


Version 3
---------

- **Note:** This version of the protocol is *not* supported by RapidsSE.

- **Added:** password authentication related requests and responses.

- **Replaced:** ``AuthenticatePasswordRequest`` message with ``PlainAuthenticationRequest``

- **Removed:** ``AuthenticatePasswordResponse`` message

- **Removed:** ``AuthenticationRequest`` message

- **Added:** ``GssNegotiationMsg`` message

- **Added:** ``KerberosAuthenticationRequest`` message

- **Updated:** ``MessageTypes`` message

  - Added extra enums, per the new and changed messages

- **Updated:** ``SessionChangeResponse`` message

  - Changed the field ``sessionId`` from an ``i64`` to a ``string``.

- **Updated:** ``StatementResponse`` message

  - Added an optional field ``statementName``


Version 2
---------

- **Note:** This version of the protocol is *not* supported by RapidsSE.

- **Changed:** ``QueryExecuteRequest`` message

  - Added a field for limiting number of rows returned from server.


Version 1
---------

- **Notes:** Initial version

- **Note:** This version of the protocol is *not* supported by RapidsSE.

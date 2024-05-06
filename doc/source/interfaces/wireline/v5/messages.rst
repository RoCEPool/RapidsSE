.. only:: html

  .. contents:: Contents of this section:
     :depth: 3
     :backlinks: none


.. _wlpv5Messages:

Wireline Messages
=================

Overview
--------

Apart from the initial :ref:`welcome message <wlpv5WelcomeMessage>`,
the messages that are sent between the client and server are defined in
message definition files written for `Apache Thrift <https://thrift.apache.org/>`_.
An interface definition language such as Thrift was chosen to define messages
because it alleviates developers from the tedious and error-prone work of
serializing and deserializing messages manually. Instead, developers can focus
on the sequencing of messages. These message definitions can be compiled to
many different target languages as supported by Thrift. Please refer to the
`Apache Thrift website <https://thrift.apache.org/>`_ for more details.

This section contains the message definitions, as well as other factors such as
string encoding, message headers and which messages are currently supported by
this version of RapidsSE.


.. index::
   single: wireline protocol; v5, message encoding

String Encoding
---------------

All strings being sent across the wireline protocol are assumed to be in UTF-8
encoding. If the interfacing client or library require strings to be in a
different encoding then they should convert the string after receiving it
from the server. Similarly, all strings being sent to RapidsSE should be
converted into UTF-8 format before being sent to the server.

.. index::
   single: wireline protocol; v5, message header

.. _wlpv5MessageHeader:

Message Header
--------------

The Thrift message definitions are not self-describing. If a message is serialised
to a sequence of bytes, the receiver cannot determine the type of message being
received and therefore how to decode it. So whenever a wireline message is being
sent, the sender must prepend a message header that contains two important bits
of information:

#. The type of wireline message that follows; and

#. The total length (in bytes) of the wireline message, including the header.

The definition of the message header is shown in :numref:`wlpv5MessageHeaderDefinition`.

The message header is 8 bytes long. Of these 8 bytes, 4 bytes are used to
represent the entire message length (header + payload) and 1 byte is used for
the message type. The remaining 3 bytes are reserved for future use. The 8
byte length also ensures that the rest of the message is 64-bit aligned and
as such will not incur any data re-alignment penalties on CPUs that prefer
data to be aligned. The 4 byte message length uses big endian (or network
byte) ordering as this is common on messaging protocols and it is also the
default integer byte ordering for Java.

.. _wlpv5MessageHeaderDefinition:

.. figure:: /interfaces/wireline/v5/messages/MessageHeader.*
   :align: center

   The wireline message header definition


The interpretation of the message type field will be determined by an enumeration
in each version of the Wireline Protocol. Refer to the :ref:`Message Types <wlpv5MessageTypes>`
section as well as the definition of the ``MessageTypes`` Thrift enumeration
for more details.

The message header is expected to remain unchanged across different versions
of the Wireline Protocol.

Reading messages off a socket is as simple as reading the first 8 bytes (the
message header), then extract the total message length from the first 4 bytes
of the header (converting this value from big to little endian if necessary),
before reading the payload (totalMessageLength – 8 bytes). By knowing the
``messageType`` ID and the version of the wireline protocol being used, the
payload can then be deserialized.


.. index::
   single: wireline protocol; v5, compiling messages

Compiling Messages
------------------

The wireline message definition files must be compiled to a target language
before they can be used in a client or interface. To do this you must install
the Thrift compiler from the `Apache Thrift website <https://thrift.apache.org/>`_.

The current version of Thrift used by RapidsSE is Thrift v0.18.1.

Once thrift is installed, a sample script can be used to compile all wireline
message definition files to the target language. Here is an example of a ``bash``
script that converts all of the message definitions files to Java:

.. code:: bash

  #!/usr/bin/env bash

  DIR=$(dirname "$0")
  THRIFT_FILES=$(find "${DIR}" -name *.thrift | xargs)
  OUTPUT_DIR="${DIR}/../java"

  for f in $THRIFT_FILES
  do
    echo -n "Compiling ${f}... "
    thrift -r --out $OUTPUT_DIR --gen java $f
    echo "Done."
  done

This sample script searches for all thrift files under the directory where the
script file is found. It converts each of them to a corresponding Java class
and writes the output to ``${OUTPUT_DIR}``.


.. index::
   single: wireline protocol; v5, message types

.. _wlpv5MessageTypes:

Message Types
-------------

The messageType field from the header maps a simple 8-bit integer to a Thrift
message definition. The interpretation of this integer value depends on the
version of the Wireline Protocol being used. Different versions of the
Wireline Protocol may add or remove messages (and therefore mappings), or they
may even renumber existing messages with a new messageType value.

Because the Protocol Compatibility dialog is compatible across all versions of
the Wireline Protocol, the messages involved in this dialog must also have
fixed values for their messageType field. As such, the following messages will
always have these values for the messageType field, regardless of the
protocol version used:

- ``ProtocolCompatibleRequest`` will always has a ``messageType`` of ``1``,

- ``ProtocolCompatibleResponse`` will always has a ``messageType`` of ``2``,

- ``ProtocolIncompatibleResponse`` will always has a ``messageType`` of ``4``.

For ease of use, the structure of the messageType enum has the following properties:

- All client initiated requests and notifications will have an odd numbered
  message type.

- All server sent requests and notifications will have an even numbered message
  type.

- Any message types that can be sent by either client or server will have an
  even numbered message type.

The odd/even numbered assigning of message type IDs is simply to help any
debugging efforts if one is looking at a network hex dump.

To determine what Thrift message is mapped to a particular value of the
``messageType`` field, consult the ``MessageTypes`` Thrift file. This contains
an enumeration that holds these values. As an example, the mappings for version 5
of the Wireline Protocol are as follows:

==================================  ===============   ==========
Message Name                        MessageType ID    Currently
                                                      Supported?
==================================  ===============   ==========
``ProtocolCompatibleRequest``     	  ``1``             Yes
``ProtocolCompatibleResponse``    	  ``2``             Yes
``ProtocolIncompatibleResponse``  	  ``4``             Yes
``ErrorResponse``                 	  ``6``             Yes
``WarningResponse``               	  ``8``             Yes
``ShutdownNotification``          	  ``10``            No
``DisconnectNotification``        	  ``12``            No
``DisconnectRequest``             	  ``13``            Yes
``PingRequest``                       ``15``            Yes
``PingResponse``                      ``16``            Yes
``ClearTextTunnelRequest``        	  ``21``            Yes
``TunnelSupportedResponse``       	  ``22``            Yes
``PlainAuthenticationRequest``     	  ``31``            Yes
``KerberosAuthenticationRequest``	    ``33``            No
``GssNegotiationMsg``	                ``34``            No
``AuthenticationOkResponse``      	  ``38``            Yes
``SessionChangeRequest``          	  ``41``            Yes
``SessionChangeResponse``         	  ``42``            Yes
``QueryExecuteRequest``           	  ``51``            Yes
``QueryReceiptResponse``          	  ``52``            Yes
``RowMetadataResponse``           	  ``54``            Yes
``RowDataResponse``               	  ``56``            Yes
``StatementResponse``             	  ``58``            Yes
``QueryExecuteResponse``          	  ``60``            Yes
==================================  ===============   ==========


.. index::
   single: wireline protocol; v5, message definitions

Message Definitions
-------------------

.. note::
  Please note that there are additional wireline messages in this set that
  are not currently being used or supported by RapidsSE. These are included
  for completeness, but they may change in the future if and when RapidsSE
  supports them. Refer to the :ref:`Message Types <wlpv5MessageTypes>` section
  for details on which messages are supported.

.. only:: html

  This HTML documentation does not list the contents of each message. Instead
  you can download the wireline message definition files directly and browse
  them in any external text editor or IDE.

  .. important::

    :download:`Click here </_static/RapidsSEWirelineMessages.zip>` to download
    the wireline protocol v5 message definitions in a compressed zip file.


.. only:: latex

  .. important::

    To download the message definitions as a single compressed file please refer
    to the HTML version of this documentation. This PDF documentation instead
    lists the contents of the wireline definition messages.


.. include:: messages/messages.rst.fragment

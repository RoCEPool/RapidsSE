/*============================================================================*/
/* AdaptorWL.cpp -- Wire Line Adaptor                                         */
/*============================================================================*/

#include "BASE.h"

#include "thrift/protocol/TCompactProtocol.h"
#include "thrift/transport/TBufferTransports.h"

#include "AdaptorWL.hpp"
#include "BufferTransport.hpp"
#include "WlpHeader.hpp"
#include "Version.h"

// All of the wireline protocol messages
#include "wlp/thrift/generated/AuthenticationOkResponse_types.h"
#include "wlp/thrift/generated/ClearTextTunnelRequest_types.h"
#include "wlp/thrift/generated/ClosePreparedStatementRequest_types.h"
#include "wlp/thrift/generated/ClosePreparedStatementResponse_types.h"
#include "wlp/thrift/generated/ColumnMetadata_types.h"
#include "wlp/thrift/generated/ColumnValue_types.h"
#include "wlp/thrift/generated/DisconnectNotification_types.h"
#include "wlp/thrift/generated/DisconnectRequest_types.h"
#include "wlp/thrift/generated/ErrorResponse_types.h"
#include "wlp/thrift/generated/GssNegotiationMsg_types.h"
#include "wlp/thrift/generated/KerberosAuthenticationRequest_types.h"
#include "wlp/thrift/generated/MessageTypes_types.h"
#include "wlp/thrift/generated/ParseStatementRequest_types.h"
#include "wlp/thrift/generated/ParseStatementResponse_types.h"
#include "wlp/thrift/generated/PingRequest_types.h"
#include "wlp/thrift/generated/PingResponse_types.h"
#include "wlp/thrift/generated/PlainAuthenticationRequest_types.h"
#include "wlp/thrift/generated/ProtocolCompatibleRequest_types.h"
#include "wlp/thrift/generated/ProtocolCompatibleResponse_types.h"
#include "wlp/thrift/generated/ProtocolIncompatibleResponse_types.h"
#include "wlp/thrift/generated/QueryExecuteRequest_types.h"
#include "wlp/thrift/generated/QueryExecuteResponse_types.h"
#include "wlp/thrift/generated/QueryProgressRequest_types.h"
#include "wlp/thrift/generated/QueryProgressResponse_types.h"
#include "wlp/thrift/generated/QueryReceiptResponse_types.h"
#include "wlp/thrift/generated/RowDataResponse_types.h"
#include "wlp/thrift/generated/RowMetadataResponse_types.h"
#include "wlp/thrift/generated/SessionChangeRequest_types.h"
#include "wlp/thrift/generated/SessionChangeResponse_types.h"
#include "wlp/thrift/generated/SessionDataResponse_types.h"
#include "wlp/thrift/generated/SessionDroppedMessagesResponse_types.h"
#include "wlp/thrift/generated/SessionMonitoringRequest_types.h"
#include "wlp/thrift/generated/SessionMonitoringResponse_types.h"
#include "wlp/thrift/generated/SessionProtocolVersionResponse_types.h"
#include "wlp/thrift/generated/ShutdownNotification_types.h"
#include "wlp/thrift/generated/StatementBindAndExecuteRequest_types.h"
#include "wlp/thrift/generated/StatementResponse_types.h"
#include "wlp/thrift/generated/TunnelSupportedResponse_types.h"
#include "wlp/thrift/generated/WarningResponse_types.h"

const i8 C_MIN_SUPPORT_PROTOCOL_VER = 4;
const i8 C_MAX_SUPPORT_PROTOCOL_VER = 4;

using namespace com::rapidsdata::wirelineprotocol::messages::compatibility;
using namespace com::rapidsdata::wirelineprotocol::messages::v5;

Stz* AdaptorWL::aState[] {
    /* NONE */                "NONE"_Stz,
    /* CONNECTED */           "CONNECTED"_Stz,
    /* PROTOCOL_COMPATIBLE */ "PROTOCOL_COMPATIBLE"_Stz,
    /* TUNNELED */            "TUNNELED"_Stz,
    /* AUTHENTICATED */       "AUTHENTICATED"_Stz,
    /* READY */               "READY"_Stz,
    /* PROCESSING_MSG */      "PROCESSING_MSG"_Stz,
    /* WAITING_FOR_API */     "WAITING_FOR_API"_Stz,
    /* DISCONNECTED */        "DISCONNECTED"_Stz,
    /* NENUMS */              "NENUM"_Stz
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, AdaptorWL, CN_ADAPTORWL)
__aNewFunction(AdaptorWL, CN_ADAPTORWL)
__vDeleteFunction(AdaptorWL, CN_ADAPTORWL)
__aFooterFunction(AdaptorWL)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AdaptorWL::AdaptorWL(void)
: Object(CN_ADAPTORWL)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ADAPTORWL, _FOOTER_);

  m_pCurrentMsg = nullptr;
  m_iCurrentMsgType = -1;
  m_pPayloadSendList = new PayloadList();
  m_bContinueProcessingMsg = false;
  m_eProtocolError = EC_NONE;
  m_eProtocolState = CONNECTED;

  m_pTransport = std::make_shared<WLP::BufferTransport>();
  m_pProtocol  = std::make_shared<apache::thrift::protocol::TCompactProtocol>(m_pTransport);

  _checkInitialised_(CN_ADAPTORWL);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_ADAPTORWL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AdaptorWL::~AdaptorWL(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ADAPTORWL, _FOOTER_);
  if (m_pCurrentMsg) {
    delete m_pCurrentMsg;
    m_pCurrentMsg = nullptr;
  }
  m_pPayloadSendList->vDeleteAll();
  delete m_pPayloadSendList;

  // m_pTransport will be cleaned up automatically
  // m_pProtocol  will be cleaned up automatically
}

/*----------------------------------------------------------------------------*/
/* repair the aggregate time                                                  */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vRepair(Object* pObject)
{
  _static_

  AdaptorWL* pAdaptorWL = _pAdaptorWL(pObject);

  delete pAdaptorWL;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Decode the payload into a wireline message                                 */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vDecodePayload(WLP::PayloadObj* pPayload)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  ASSERT(m_eProtocolError == EC_NONE);
  ASSERT(!m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == -1);

  ASSERT(pPayload->iPayloadSize() > WLP::C_WLPHEADER_SIZE);
  WLP::WlpHeader* pHeader = (WLP::WlpHeader*) pPayload->pcBuf();

  ASSERT(pHeader->iMsgSize() > WLP::C_WLPHEADER_SIZE);
  ASSERT(pHeader->iMsgType() >= 0);

  i8 iMsgBodySize = pHeader->iMsgSize() - WLP::C_WLPHEADER_SIZE;
  MessageTypes::type eMsgType = (MessageTypes::type) pHeader->iMsgType();

  m_pTransport->vUseBuffer(pHeader->pcMsgBody(), iMsgBodySize);
  vDeserialiseMsg(eMsgType);
  m_pTransport->vRemoveBuffer();
}

/*----------------------------------------------------------------------------*/
/* Process or continue processing the current decoded wireline message        */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vProcessWLMsg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);

  ASSERT(m_iCurrentMsgType != -1);
  ASSERT(m_pCurrentMsg);
  ASSERT(m_eProtocolError == EC_NONE);

  MessageTypes::type eMsgType = (MessageTypes::type) m_iCurrentMsgType;

  switch (eMsgType) {

    case MessageTypes::type::ProtocolCompatibleRequest:       processProtocolCompatibleRequest(); break;
    case MessageTypes::type::DisconnectRequest:                                                   break;
    case MessageTypes::type::PingRequest:                                                         break;
    case MessageTypes::type::ClearTextTunnelRequest:                                              break;
    case MessageTypes::type::PlainAuthenticationRequest:                                          break;
    case MessageTypes::type::KerberosAuthenticationRequest:                                       break;
    case MessageTypes::type::GssNegotiationMsg:                                                   break;
    case MessageTypes::type::SessionChangeRequest:                                                break;
    case MessageTypes::type::QueryExecuteRequest:                                                 break;
    case MessageTypes::type::QueryProgressRequest:                                                break;
    case MessageTypes::type::SessionMonitoringRequest:                                            break;
    case MessageTypes::type::ParseStatementRequest:                                               break;
    case MessageTypes::type::StatementBindAndExecuteRequest:                                      break;
    case MessageTypes::type::ClosePreparedStatementRequest:                                       break;

      // The following are messages that should never be received by RapidsSE
      // as they should never be sent by a client. Only sent by a server.
      //   ProtocolCompatibleResponse:
      //   ProtocolIncompatibleResponse:
      //   ErrorResponse:
      //   WarningResponse:
      //   ShutdownNotification:
      //   DisconnectNotification:
      //   PingResponse:
      //   TunnelSupportedResponse:
      //   AuthenticationOkResponse:
      //   SessionChangeResponse:
      //   QueryReceiptResponse:
      //   RowMetadataResponse:
      //   RowDataResponse:
      //   StatementResponse:
      //   QueryExecuteResponse:
      //   QueryProgressResponse:
      //   SessionProtocolVersionResponse:
      //   SessionDataResponse:
      //   SessionDroppedMessagesResponse:
      //   SessionMonitoringResponse:
      //   ParseStatementResponse:
      //   ClosePreparedStatementResponse:

    default:
      // Respond to the client with an error, and set the m_eProtocolError
      // which will cause the socket to be closed.
      std::string sMsgType = to_string(eMsgType);
      vSerialiseErrorResponse(EC_WL_PROTOCOL_ERROR,
                              "Wireline protocol error: Processing an unexpected wireline message type of %s."_Stz,
                              sMsgType.c_str());
      m_eProtocolError = EC_WL_PROTOCOL_ERROR;
      m_eProtocolState = DISCONNECTED;
      m_bContinueProcessingMsg = false;
      vCleanupCurrentMsg();
      return;
  }


  // Delete m_pCurrentMsg when processing is all done
  if (!m_bContinueProcessingMsg) {
    vCleanupCurrentMsg();
  }
}

/*----------------------------------------------------------------------------*/
/* Return true if vProcessWLMsg() needs to be called again                    */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bKeepProcessingMsg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  return m_bContinueProcessingMsg && m_eProtocolError == EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* Return true if a protocol error was hit and the socket should be closed    */
/*----------------------------------------------------------------------------*/
eEC AdaptorWL::eProtocolError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);

  return m_eProtocolError;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is at least 1 PayloadObj to be sent                   */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bHasPayloadToSend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);

  return (m_pPayloadSendList->nItems() > 0);
}

/*----------------------------------------------------------------------------*/
/* Return the next PayloadObj to be sent                                      */
/*----------------------------------------------------------------------------*/
WLP::PayloadObj* AdaptorWL::pNextPayloadToSend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  return m_pPayloadSendList->pPop();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Return the next PayloadObj to be sent                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vDeserialiseMsg(i8 iMessageType)
{
  MessageTypes::type eMsgType = (MessageTypes::type) iMessageType;
  ASSERT(!m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == -1);

  switch (eMsgType) {

    case MessageTypes::type::ProtocolCompatibleRequest:      m_pCurrentMsg = new ProtocolCompatibleRequest();      break;
    case MessageTypes::type::DisconnectRequest:              m_pCurrentMsg = new DisconnectRequest();              break;
    case MessageTypes::type::PingRequest:                    m_pCurrentMsg = new PingRequest();                    break;
    case MessageTypes::type::ClearTextTunnelRequest:         m_pCurrentMsg = new ClearTextTunnelRequest();         break;
    case MessageTypes::type::PlainAuthenticationRequest:     m_pCurrentMsg = new PlainAuthenticationRequest();     break;
    case MessageTypes::type::KerberosAuthenticationRequest:  m_pCurrentMsg = new KerberosAuthenticationRequest();  break;
    case MessageTypes::type::GssNegotiationMsg:              m_pCurrentMsg = new GssNegotiationMsg();              break;
    case MessageTypes::type::SessionChangeRequest:           m_pCurrentMsg = new SessionChangeRequest();           break;
    case MessageTypes::type::QueryExecuteRequest:            m_pCurrentMsg = new QueryExecuteRequest();            break;
    case MessageTypes::type::QueryProgressRequest:           m_pCurrentMsg = new QueryProgressRequest();           break;
    case MessageTypes::type::SessionMonitoringRequest:       m_pCurrentMsg = new SessionMonitoringRequest();       break;
    case MessageTypes::type::ParseStatementRequest:          m_pCurrentMsg = new ParseStatementRequest();          break;
    case MessageTypes::type::StatementBindAndExecuteRequest: m_pCurrentMsg = new StatementBindAndExecuteRequest(); break;
    case MessageTypes::type::ClosePreparedStatementRequest:  m_pCurrentMsg = new ClosePreparedStatementRequest();  break;

    // The following are messages that should never be received by RapidsSE
    // as they should never be sent by a client. Only sent by a server.
    //   ProtocolCompatibleResponse:
    //   ProtocolIncompatibleResponse:
    //   ErrorResponse:
    //   WarningResponse:
    //   ShutdownNotification:
    //   DisconnectNotification:
    //   PingResponse:
    //   TunnelSupportedResponse:
    //   AuthenticationOkResponse:
    //   SessionChangeResponse:
    //   QueryReceiptResponse:
    //   RowMetadataResponse:
    //   RowDataResponse:
    //   StatementResponse:
    //   QueryExecuteResponse:
    //   QueryProgressResponse:
    //   SessionProtocolVersionResponse:
    //   SessionDataResponse:
    //   SessionDroppedMessagesResponse:
    //   SessionMonitoringResponse:
    //   ParseStatementResponse:
    //   ClosePreparedStatementResponse:

    default:
      // Respond to the client with an error, and set the m_eProtocolError
      // which will cause the socket to be closed.
      std::string sMsgType = to_string(eMsgType);
      vSerialiseErrorResponse(EC_WL_PROTOCOL_ERROR,
                              "Wireline protocol error: Received an unexpected wireline message type of %s."_Stz,
                              sMsgType.c_str());
      m_eProtocolError = EC_WL_PROTOCOL_ERROR;
      m_eProtocolState = DISCONNECTED;
      m_bContinueProcessingMsg = false;
      vCleanupCurrentMsg();
      return;
  }

  // Set the current message type and decode the message
  m_iCurrentMsgType = eMsgType;
  i8 iBytesRead = m_pCurrentMsg->read(m_pProtocol.get());

  if (iBytesRead == 0) {
    // Respond to the client with an error, and set the m_eProtocolError
    // which will cause the socket to be closed.
    std::string sMsgType = to_string(eMsgType);
    vSerialiseErrorResponse(EC_WL_PROTOCOL_ERROR,
                            "Wireline protocol error: Unable to decode wireline message of type %s."_Stz,
                            sMsgType.c_str());
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    m_eProtocolState = DISCONNECTED;
    m_bContinueProcessingMsg = false;
    vCleanupCurrentMsg();
  }

  vTrace("Decoded a wireline message of type: %s\n"_Stz, to_string(eMsgType).c_str());
}

/*----------------------------------------------------------------------------*/
/* Deallocates the current message                                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vCleanupCurrentMsg()
{
  delete m_pCurrentMsg;
  m_pCurrentMsg = nullptr;
  m_iCurrentMsgType = -1;
}


/*----------------------------------------------------------------------------*/
/* Serialises a thrift ErrorResponse with a given message.                    */
/* Adds the serialised message to the output queue.                           */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vSerialiseErrorResponse(eEC eErrorCode, Stz* sFormat, ...)
{
  va_list args;

  // Create the error message
  s16KB sBuf;

  va_start(args,sFormat);
  vsnprintf(sBuf.azChar(), sBuf.nSpace(), sFormat->azChar(),args);
  va_end(args);


  // Create and populate the error response object to be serialised
  ErrorResponse oErrorResponse;
  oErrorResponse.__set_errorString(sBuf.azChar());

  if (eErrorCode != EC_NONE) {
    oErrorResponse.__set_errorCode(eErrorCode);
  }

  // Serialise the error message to a payload and add it to the output queue
  vSerialiseMsg(&oErrorResponse, MessageTypes::ErrorResponse);
}

/*----------------------------------------------------------------------------*/
/* Serialises an abstract thrift message and adds it to the output queue.     */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vSerialiseMsg(apache::thrift::TBase* pMsg, i8 iMsgType)
{
  MessageTypes::type eMsgType = (MessageTypes::type) iMsgType;

  WLP::PayloadObj* pPayload = new WLP::PayloadObj();

  // Serialise the error message to a payload
  m_pTransport->vUseBuffer(pPayload->pcBuf(), WLP::PayloadObj::C_MAX_PAYLOAD_BYTES);
  WLP::WlpHeader* pHeader = m_pTransport->pAllocHeader(eMsgType);
  pMsg->write(m_pProtocol.get());

  // Message size needs to be written into the header of the message
  // as well as the payload object.
  i8 iMsgSize = m_pTransport->iBytesWritten() + WLP::C_WLPHEADER_SIZE;
  pHeader->vMsgSize(iMsgSize);
  pPayload->vSetPayloadSize(iMsgSize);
  m_pTransport->vRemoveBuffer();

  // Add it to the send list
  m_pPayloadSendList->vAppend(pPayload);
}

/*----------------------------------------------------------------------------*/
/* Change the state and check the state transition                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vNextState(eState eNextState)
{
  switch(m_eProtocolState) {

    case CONNECTED:
      if (eNextState != PROTOCOL_COMPATIBLE && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case PROTOCOL_COMPATIBLE:
      if (eNextState != TUNNELED && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case TUNNELED:
      if (eNextState != AUTHENTICATED && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case AUTHENTICATED:
      if (eNextState != READY && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case READY:
      if (eNextState != PROCESSING_MSG && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case PROCESSING_MSG:
      if (eNextState != READY && eNextState != WAITING_FOR_API && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case WAITING_FOR_API:
      if (eNextState != READY && eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    case DISCONNECTED:
      if (eNextState != DISCONNECTED) {
        vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
        return;
      }
      break;

    default:
      NEVER_REACH
  }

  // Do the transition
  vTrace("Wireline client transitioning from state %s to state %s\n"_Stz,
         aState[m_eProtocolState],
         aState[eNextState]);

  m_eProtocolState = eNextState;
}

/*----------------------------------------------------------------------------*/
/* Deallocates the current message                                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processProtocolCompatibleRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::ProtocolCompatibleRequest);
  ASSERT(m_eProtocolState == CONNECTED);

  ProtocolCompatibleRequest* pMsg = dynamic_cast<ProtocolCompatibleRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::ProtocolCompatibleRequest).c_str());
    return;
  }

  // Create the server build info, required for any response here.
  s2K sBuildInfo;
  sBuildInfo.vSprint("RapidsSE build information:\n" \
                                "  version     %s\n" \
                                "  from branch %s\n" \
                                "  by          %s\n" \
                                "  on          %s\n" \
                                "  at          %s"_Stz,
                                C_VERSION_STRING,
                                C_BUILD_BRANCH,
                                C_BUILD_USERNAME,
                                C_BUILD_DATE,
                                C_BUILD_HOSTNAME );

  // Check if the client is speaking the same protocol version
  i8 iClientProtocolVer = pMsg->clientProtocolVersion;

  if (iClientProtocolVer < C_MIN_SUPPORT_PROTOCOL_VER ||
      iClientProtocolVer > C_MAX_SUPPORT_PROTOCOL_VER) {

    // Serialise a ProtocolIncompatibleResponse

    s256 sErrMsg;
    const eEC eErrType = EC_WL_PROTOCOL_VER_NOT_SUPPORTED;

    if (C_MIN_SUPPORT_PROTOCOL_VER == C_MAX_SUPPORT_PROTOCOL_VER) {
      sErrMsg.vSprint("%s: client supports %lld, server supports version %lld."_Stz,
                      aErrorMsg[eErrType],
                      iClientProtocolVer,
                      C_MIN_SUPPORT_PROTOCOL_VER);
    } else {
      sErrMsg.vSprint("%s: client sent %lld, server supports versions %lld to %lld."_Stz,
                      aErrorMsg[eErrType],
                      iClientProtocolVer,
                      C_MIN_SUPPORT_PROTOCOL_VER,
                      C_MAX_SUPPORT_PROTOCOL_VER);
    }

    ProtocolIncompatibleResponse oIncompatibleResponse;
    oIncompatibleResponse.__set_clientProtocolVersion((int32_t) iClientProtocolVer);
    oIncompatibleResponse.__set_minimumProtocolVersion(C_MIN_SUPPORT_PROTOCOL_VER);
    oIncompatibleResponse.__set_maximumProtocolVersion(C_MAX_SUPPORT_PROTOCOL_VER);
    oIncompatibleResponse.__set_serverSoftwareVersion(const_cast<Stz*>(C_VERSION_STRING)->azChar());
    oIncompatibleResponse.__set_serverBuildInfo(sBuildInfo.azChar());
    oIncompatibleResponse.__set_errorMessage(sErrMsg.azChar());

    vSerialiseMsg(&oIncompatibleResponse, MessageTypes::type::ProtocolIncompatibleResponse);

    m_eProtocolError = EC_WL_PROTOCOL_VER_NOT_SUPPORTED;
    m_bContinueProcessingMsg = false;
    vNextState(DISCONNECTED);
    return;
  }


  // Serialise a ProtocolCompatibleResponse

  ProtocolCompatibleResponse oCompatibleResponse;
  oCompatibleResponse.__set_protocolVersion((int32_t) iClientProtocolVer);
  oCompatibleResponse.__set_serverSoftwareVersion(const_cast<Stz*>(C_VERSION_STRING)->azChar());
  oCompatibleResponse.__set_serverBuildInfo(sBuildInfo.azChar());

  vSerialiseMsg(&oCompatibleResponse, MessageTypes::type::ProtocolCompatibleResponse);

  m_bContinueProcessingMsg = false;
  vNextState(PROTOCOL_COMPATIBLE);
}

/*----------------------------------------------------------------------------*/
/* Internal error doing dynamic cast of received wireline message             */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorDynamicCast(const char* pcTargetType)
{
  Stz* szErrMsg = "%s: Unable to cast decoded message to type %s"_Stz;
  const eEC eErrType = EC_WL_INTERNAL_ERROR;

  vSerialiseErrorResponse(EC_WL_INTERNAL_ERROR,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          pcTargetType);
  m_bContinueProcessingMsg = false;
}

/*----------------------------------------------------------------------------*/
/* Internal error doing protocol state transition                             */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorStateTransition(Stz* szFromState, Stz* szToState)
{
  Stz* szErrMsg = "%s: Invalid state transition from state %s to state %s"_Stz;
  const eEC eErrType = EC_WL_INTERNAL_ERROR;

  vSerialiseErrorResponse(EC_WL_INTERNAL_ERROR,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          szFromState,
                          szToState);
  m_bContinueProcessingMsg = false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

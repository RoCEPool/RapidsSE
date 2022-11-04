/*============================================================================*/
/* AdaptorWL.cpp -- Wire Line Adaptor                                         */
/*============================================================================*/

#include "BASE.h"

#include "thrift/protocol/TCompactProtocol.h"
#include "thrift/transport/TBufferTransports.h"

#include "AdaptorWL.hpp"
#include "BufferTransport.hpp"
#include "WlpHeader.hpp"

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


using namespace com::rapidsdata::wirelineprotocol::messages::compatibility;
using namespace com::rapidsdata::wirelineprotocol::messages::v5;

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
  m_bProtocolError = false;

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
  ASSERT(!m_bProtocolError);
  ASSERT(!m_pCurrentMsg);

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
  ASSERT(!m_bProtocolError);

  // TODO

  // Delete m_pCurrentMsg when processing is all done
  m_bContinueProcessingMsg = false;
  vCleanupCurrentMsg();
}

/*----------------------------------------------------------------------------*/
/* Return true if vProcessWLMsg() needs to be called again                    */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bKeepProcessingMsg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  return m_bContinueProcessingMsg && !m_bProtocolError;
}

/*----------------------------------------------------------------------------*/
/* Return true if a protocol error was hit and the socket should be closed    */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bHasProtocolError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);

  return m_bProtocolError;
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
      // TODO: respond with an error and close the socket
      m_bProtocolError = true;
      NEVER_REACH
  }

  // Decode the message
  i8 iBytesRead = m_pCurrentMsg->read(m_pProtocol.get());

  if (iBytesRead == 0) {
    // TODO: respond with an error and close the socket
    m_bProtocolError = true;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
#include "InfoSet.hpp"

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
    /* 00 NONE */                     "NONE"_Stz,
    /* 01 CONNECTED */                "CONNECTED"_Stz,
    /* 02 PROTOCOL_COMPATIBLE */      "PROTOCOL_COMPATIBLE"_Stz,
    /* 03 TUNNELED */                 "TUNNELED"_Stz,
    /* 04 KERBEROS_NEGOTIATING */     "KERBEROS_NEGOTIATING"_Stz,
    /* 05 AUTHENTICATED */            "AUTHENTICATED"_Stz,
    /* 06 READY */                    "READY"_Stz,
    /* 07 QUERY_BEGUN */              "QUERY_BEGUN"_Stz,
    /* 08 API_STATEMENT_TO_SUBMIT */  "API_STATEMENT_TO_SUBMIT"_Stz,
    /* 09 API_STATEMENT_SUBMITTED */  "API_STATEMENT_SUBMITTED"_Stz,
    /* 10 API_METADATA_RECEIVED */    "API_METADATA_RECEIVED"_Stz,
    /* 11 API_DATA_RECEIVED */        "API_DATA_RECEIVED"_Stz,
    /* 12 API_STATEMENT_COMPLETED */ "API_STATEMENT_COMPLETED"_Stz,
    /* 13 API_CHILD_JOB_EXITED */    "API_CHILD_JOB_EXITED"_Stz,
    /* 14 QUERY_COMPLETED */         "QUERY_COMPLETED"_Stz,
    /* 15 DISCONNECTED */            "DISCONNECTED"_Stz,
    /* 16 NENUMS */                  "NENUM"_Stz
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

    case MessageTypes::type::ProtocolCompatibleRequest:       processProtocolCompatibleRequest();       break;
    case MessageTypes::type::DisconnectRequest:               processDisconnectRequest();               break;
    case MessageTypes::type::PingRequest:                     processPingRequest();                     break;
    case MessageTypes::type::ClearTextTunnelRequest:          processClearTextTunnelRequest();          break;
    case MessageTypes::type::PlainAuthenticationRequest:      processPlainAuthenticationRequest();      break;
    case MessageTypes::type::KerberosAuthenticationRequest:   processKerberosAuthenticationRequest();   break;
    case MessageTypes::type::GssNegotiationMsg:               processGssNegotiationMsg();               break;
    case MessageTypes::type::SessionChangeRequest:            processSessionChangeRequest();            break;
    case MessageTypes::type::QueryExecuteRequest:             processQueryExecuteRequest();             break;
    case MessageTypes::type::QueryProgressRequest:            processQueryProgressRequest();            break;
    case MessageTypes::type::SessionMonitoringRequest:        processSessionMonitoringRequest();        break;
    case MessageTypes::type::ParseStatementRequest:           processParseStatementRequest();           break;
    case MessageTypes::type::StatementBindAndExecuteRequest:  processStatementBindAndExecuteRequest();  break;
    case MessageTypes::type::ClosePreparedStatementRequest:   processClosePreparedStatementRequest();   break;

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
      eEC eErrCode = EC_WL_PROTOCOL_ERROR;
      vSerialiseErrorResponse(eErrCode,
                              "Error: %s: Processing an unexpected wireline message type of %s."_Stz,
                              aErrorMsg[eErrCode],
                              sMsgType.c_str());
      m_eProtocolError = EC_WL_PROTOCOL_ERROR;
      m_eProtocolState = DISCONNECTED;
      vCleanupCurrentMsg();
      return;
  }


  // Delete m_pCurrentMsg when processing is all done
  vCleanupCurrentMsg();
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
/* Get the next statement to be sent through the SE API                       */
/*----------------------------------------------------------------------------*/
Stz* AdaptorWL::szStatement()
{
  return m_xStmtCtx.m_sSmtBuf.szStz();
}

/*----------------------------------------------------------------------------*/
/* Is there a statement to be sent through the SE API                         */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bHasStatement()
{
  return !m_xStmtCtx.m_sSmtBuf.bIsEmpty();
}

/*----------------------------------------------------------------------------*/
/* Reset the variables used during execution of a statement                   */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vResetStmtCtx()
{
  // TODO: Get rid of this. Move it elsewhere, like when we move to StatementComplete
  m_xStmtCtx.vResetContext();
}

/*----------------------------------------------------------------------------*/
/* Update the state that a statement has been submitted                       */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vStatementSubmitted()
{
  ASSERT(bCheckState( {API_STATEMENT_TO_SUBMIT} ));

  vNextState(API_STATEMENT_SUBMITTED);
}

/*----------------------------------------------------------------------------*/
/* Update the state that a statement has failed to submit                     */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vStatementSubmissionFailed(eEC eErrCode)
{
  ASSERT(bCheckState( {API_STATEMENT_TO_SUBMIT} ));
  Stz* szErrMsg= "Error: %s"_Stz;

  vSerialiseErrorResponse(eErrCode, szErrMsg, aErrorMsg[eErrCode]);

  // The child job failed during submission.
  vNextState(API_STATEMENT_COMPLETED);
  vChildJobExited();
}

/*----------------------------------------------------------------------------*/
/* Update the state after an error is received from the SQL compiler          */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vCompilerError(InfoSet* pInfoSet)
{
  if (m_xStmtCtx.m_bGotSmtError) {
    return;
  }

  ASSERT(bCheckState( {API_STATEMENT_SUBMITTED, API_METADATA_RECEIVED, API_DATA_RECEIVED} ));
  ASSERT(pInfoSet->szBufferNm()->bSame(C_TDEF_COMPILEERRORS));

  Row *pInfoRow = pInfoSet->pFirstRow();

  Stz* szNodeName  = pInfoRow->szColVal_ofs(0);
  eEC eErrorCode   = (eEC) pInfoRow->iColVal_int(1);
  i8 iLineNo       = pInfoRow->iColVal_int(2);
  i8 iColumnNo     = pInfoRow->iColVal_int(3);
  Stz* szErrorText = pInfoRow->szColVal_ofs(4);

  Stz* szMsgFmt = "Error: %s; %s; Line: %lld; Column: %lld; %s"_Stz;
  vSerialiseErrorResponse(eErrorCode,
                          szMsgFmt,
                          szNodeName,
                          aEC2[eErrorCode],
                          iLineNo,
                          iColumnNo,
                          szErrorText);

  m_xStmtCtx.m_bGotSmtError = true;

  // An error at this point ends the current statement
  vNextState(API_STATEMENT_COMPLETED);
}

/*----------------------------------------------------------------------------*/
/* Update the state after an error is received from the execution engine      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vExecutionError(InfoSet* pInfoSet)
{
  if (m_xStmtCtx.m_bGotSmtError) {
    return;
  }

  ASSERT(bCheckState( {API_STATEMENT_SUBMITTED, API_METADATA_RECEIVED, API_DATA_RECEIVED} ));
  ASSERT(pInfoSet->szBufferNm()->bSame(C_TDEF_EXECERRORS));

  Row *pInfoRow = pInfoSet->pFirstRow();

  Stz* szNodeName  = pInfoRow->szColVal_ofs(0);
  eEC eErrorCode   = (eEC) pInfoRow->iColVal_int(1);
  Stz* szErrorText = pInfoRow->szColVal_ofs(2);

  Stz* szMsgFmt = "Error: %s; %s; %s"_Stz;
  vSerialiseErrorResponse(eErrorCode,
                          szMsgFmt,
                          szNodeName,
                          aEC2[eErrorCode],
                          szErrorText);

  m_xStmtCtx.m_bGotSmtError = true;

  // An error at this point ends the current statement
  vNextState(API_STATEMENT_COMPLETED);
}

/*----------------------------------------------------------------------------*/
/* Update the state after the child job has exited                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vChildJobExited()
{
  ASSERT(bCheckState( {API_STATEMENT_COMPLETED} ));

  m_xStmtCtx.vResetContext();
  vNextState(API_CHILD_JOB_EXITED);

  if (bExtractNextStatement()) {
    vNextState(API_STATEMENT_TO_SUBMIT);

  } else {
    // Send a QueryExecuteResponse to terminate the query and update the state appropriately
    vFinishQueryDialog();
  }
}

/*----------------------------------------------------------------------------*/
/* Return true if a child job is currently executing                          */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bIsChildJobActive()
{
  switch (m_eProtocolState) {
    case API_STATEMENT_SUBMITTED:
    case API_METADATA_RECEIVED:
    case API_DATA_RECEIVED:
    case API_STATEMENT_COMPLETED:
      return true;

    default:
      return false;
  }
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
      eEC eErrCode = EC_WL_PROTOCOL_ERROR;
      vSerialiseErrorResponse(eErrCode,
                              "Error: %s: Received an unexpected wireline message type of %s."_Stz,
                              aErrorMsg[eErrCode],
                              sMsgType.c_str());
      m_eProtocolError = EC_WL_PROTOCOL_ERROR;
      m_eProtocolState = DISCONNECTED;
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
    eEC eErrCode = EC_WL_PROTOCOL_ERROR;
    vSerialiseErrorResponse(eErrCode,
                            "%s: Unable to decode wireline message of type %s."_Stz,
                            aErrorMsg[eErrCode],
                            sMsgType.c_str());
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    m_eProtocolState = DISCONNECTED;
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
  vSerialiseAndSendMsg(&oErrorResponse, MessageTypes::ErrorResponse);
}


/*----------------------------------------------------------------------------*/
/* Serialises an abstract thrift message and adds it to the output queue.     */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vSerialiseAndSendMsg(apache::thrift::TBase* pMsg, i8 iMsgType)
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
  typedef std::initializer_list<eState> StateList;

  // The list of valid next states from a given state.
  // There is a state transition diagram that describes the valid transitions between
  // these states on the Sharepoint fileserver under the Wire Line Protocol directory.
  /*01*/ static StateList aNextStates_Connected          = { PROTOCOL_COMPATIBLE, DISCONNECTED };
  /*02*/ static StateList aNextStates_ProtocolCompatible = { TUNNELED, DISCONNECTED };
  /*03*/ static StateList aNextStates_Tunneled           = { AUTHENTICATED, KERBEROS_NEGOTIATING, DISCONNECTED };
  /*04*/ static StateList aNextStates_KerberosAuth       = { AUTHENTICATED, DISCONNECTED };
  /*05*/ static StateList aNextStates_Authenticated      = { READY, DISCONNECTED };
  /*06*/ static StateList aNextStates_Ready              = { READY, QUERY_BEGUN, DISCONNECTED };
  /*07*/ static StateList aNextStates_QueryBegun         = { READY, API_STATEMENT_TO_SUBMIT, QUERY_COMPLETED, DISCONNECTED };
  /*08*/ static StateList aNextStates_ApiSmtToSubmit     = { API_STATEMENT_SUBMITTED, API_CHILD_JOB_EXITED, DISCONNECTED };
  /*08*/ static StateList aNextStates_ApiSmtSubmitted    = { API_METADATA_RECEIVED, API_STATEMENT_COMPLETED };
  /*09*/ static StateList aNextStates_ApiMetadataRcvd    = { API_DATA_RECEIVED, API_STATEMENT_COMPLETED };
  /*10*/ static StateList aNextStates_ApiDataRcvd        = { API_DATA_RECEIVED, API_STATEMENT_COMPLETED };
  /*11*/ static StateList aNextStates_ApiSmtCompleted    = { API_CHILD_JOB_EXITED };
  /*11*/ static StateList aNextStates_ApiChildJobExited  = { QUERY_COMPLETED, API_STATEMENT_TO_SUBMIT, DISCONNECTED };
  /*12*/ static StateList aNextStates_QueryCompleted     = { READY, DISCONNECTED };
  /*13*/ static StateList aNextStates_Disconnected       = { DISCONNECTED };

  // We'll use a reference to one of these static initializer lists above.
  std::initializer_list<eState> eValidNextStates = aNextStates_Connected;

  switch(m_eProtocolState) {
    /*01*/ case CONNECTED:               eValidNextStates = aNextStates_Connected;           break;
    /*02*/ case PROTOCOL_COMPATIBLE:     eValidNextStates = aNextStates_ProtocolCompatible;  break;
    /*03*/ case TUNNELED:                eValidNextStates = aNextStates_Tunneled;            break;
    /*04*/ case KERBEROS_NEGOTIATING:    eValidNextStates = aNextStates_KerberosAuth;        break;
    /*05*/ case AUTHENTICATED:           eValidNextStates = aNextStates_Authenticated;       break;
    /*06*/ case READY:                   eValidNextStates = aNextStates_Ready;               break;
    /*07*/ case QUERY_BEGUN:             eValidNextStates = aNextStates_QueryBegun;          break;
    /*08*/ case API_STATEMENT_TO_SUBMIT: eValidNextStates = aNextStates_ApiSmtToSubmit;      break;
    /*09*/ case API_STATEMENT_SUBMITTED: eValidNextStates = aNextStates_ApiSmtSubmitted;     break;
    /*10*/ case API_METADATA_RECEIVED:   eValidNextStates = aNextStates_ApiMetadataRcvd;     break;
    /*11*/ case API_DATA_RECEIVED:       eValidNextStates = aNextStates_ApiDataRcvd;         break;
    /*12*/ case API_STATEMENT_COMPLETED: eValidNextStates = aNextStates_ApiSmtCompleted;     break;
    /*12*/ case API_CHILD_JOB_EXITED:    eValidNextStates = aNextStates_ApiChildJobExited;   break;
    /*13*/ case QUERY_COMPLETED:         eValidNextStates = aNextStates_QueryCompleted;      break;
    /*14*/ case DISCONNECTED:            eValidNextStates = aNextStates_Disconnected;        break;
    default:
      NEVER_REACH
  }

  // Check the transition
  bool bMatched = false;
  for (eState eValidNextState : eValidNextStates) {
    if (eNextState == eValidNextState) {
      bMatched = true;
      break;
    }
  }

  if (!bMatched) {
    // Connection is no longer viable
    vErrorStateTransition(aState[m_eProtocolState], aState[eNextState]);
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    m_eProtocolState = DISCONNECTED;
    return;
  }

  // Do the transition
  vTrace("Wireline client transitioning from state %s to state %s\n"_Stz,
         aState[m_eProtocolState],
         aState[eNextState]);

  m_eProtocolState = eNextState;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Message Handlers Here                                                      */

/*----------------------------------------------------------------------------*/
/* Processes a ProtocolCompatibleRequest message                              */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processProtocolCompatibleRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::ProtocolCompatibleRequest);

  if (!bCheckState({CONNECTED})) { return; }

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

    vSerialiseAndSendMsg(&oIncompatibleResponse, MessageTypes::type::ProtocolIncompatibleResponse);

    m_eProtocolError = EC_WL_PROTOCOL_VER_NOT_SUPPORTED;
    vNextState(DISCONNECTED);
    return;
  }


  // Serialise a ProtocolCompatibleResponse

  ProtocolCompatibleResponse oCompatibleResponse;
  oCompatibleResponse.__set_protocolVersion((int32_t) iClientProtocolVer);
  oCompatibleResponse.__set_serverSoftwareVersion(const_cast<Stz*>(C_VERSION_STRING)->azChar());
  oCompatibleResponse.__set_serverBuildInfo(sBuildInfo.azChar());

  vSerialiseAndSendMsg(&oCompatibleResponse, MessageTypes::type::ProtocolCompatibleResponse);

  vNextState(PROTOCOL_COMPATIBLE);
}

/*----------------------------------------------------------------------------*/
/* Processes a DisconnectRequest message                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processDisconnectRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::DisconnectRequest);

  //if (!bCheckState({CONNECTED})) { return; }

//  DisconnectRequest* pMsg = dynamic_cast<DisconnectRequest*>(m_pCurrentMsg);
//  if (!pMsg) {
//    vErrorDynamicCast(to_string(MessageTypes::type::DisconnectRequest).c_str());
//    return;
//  }

  // Nothing to do in this message. We set m_eProtocolError to cause the socket
  // to be closed.

  m_eProtocolError = EC_WL_SOCKET_CLOSED; // Cause the socket to be closed.
  vNextState(DISCONNECTED);
}

/*----------------------------------------------------------------------------*/
/* Processes a PingRequest message                                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processPingRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::PingRequest);

  if (!bCheckState({READY})) { return; }

  PingRequest* pMsg = dynamic_cast<PingRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::PingRequest).c_str());
    return;
  }

  // Serialise a PingResponse. It has no fields to set.
  PingResponse oPingResponse;
  vSerialiseAndSendMsg(&oPingResponse, MessageTypes::type::PingResponse);

  vNextState(READY);
}

/*----------------------------------------------------------------------------*/
/* Processes a ClearTextTunnelRequest message                                 */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processClearTextTunnelRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::ClearTextTunnelRequest);

  if (!bCheckState({PROTOCOL_COMPATIBLE})) { return; }

  ClearTextTunnelRequest* pMsg = dynamic_cast<ClearTextTunnelRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::ClearTextTunnelRequest).c_str());
    return;
  }

  // Send a TunnelSupportedResponse. No fields to set.
  TunnelSupportedResponse oTunnelResponse;
  vSerialiseAndSendMsg(&oTunnelResponse, MessageTypes::type::TunnelSupportedResponse);

  vNextState(TUNNELED);
}

/*----------------------------------------------------------------------------*/
/* Processes a PlainAuthenticationRequest message                             */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processPlainAuthenticationRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::PlainAuthenticationRequest);

  if (!bCheckState({TUNNELED})) { return; }

  PlainAuthenticationRequest* pMsg = dynamic_cast<PlainAuthenticationRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::PlainAuthenticationRequest).c_str());
    return;
  }

  // At this point we aren't supporting authentication, so it doesn't matter
  // what credentials are passed to in the message.

  // Send a AuthenticationOkResponse. No fields to set.
  AuthenticationOkResponse oAuthResponse;
  vSerialiseAndSendMsg(&oAuthResponse, MessageTypes::type::AuthenticationOkResponse);

  vNextState(AUTHENTICATED);
}

/*----------------------------------------------------------------------------*/
/* Processes a KerberosAuthenticationRequest message                          */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processKerberosAuthenticationRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::KerberosAuthenticationRequest);

  if (!bCheckState({TUNNELED})) { return; }

  KerberosAuthenticationRequest* pMsg = dynamic_cast<KerberosAuthenticationRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::KerberosAuthenticationRequest).c_str());
    return;
  }

  // TODO: populate

  vErrorFeatureNotSupported("Kerberos authentication"_Stz);

  m_eProtocolError = EC_WL_FEATURE_NOT_SUPPORTED;   // cause socket to disconnect
  vNextState(DISCONNECTED);

  //vNextState(KERBEROS_NEGOTIATING);
}

/*----------------------------------------------------------------------------*/
/* Processes a GssNegotiationMsg message                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processGssNegotiationMsg()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::GssNegotiationMsg);

  if (!bCheckState({KERBEROS_NEGOTIATING})) { return; }

  GssNegotiationMsg* pMsg = dynamic_cast<GssNegotiationMsg*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::GssNegotiationMsg).c_str());
    return;
  }

  // TODO: populate

  vErrorFeatureNotSupported("Kerberos authentication"_Stz);

  m_eProtocolError = EC_WL_FEATURE_NOT_SUPPORTED;   // cause socket to disconnect
  vNextState(DISCONNECTED);

//  vNextState(AUTHENTICATED);
}

/*----------------------------------------------------------------------------*/
/* Processes a SessionChangeRequest message                                   */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processSessionChangeRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::SessionChangeRequest);

  if (!bCheckState({AUTHENTICATED})) { return; }

  SessionChangeRequest* pMsg = dynamic_cast<SessionChangeRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::SessionChangeRequest).c_str());
    return;
  }

  const char* pcDefaultCatalog = nullptr;
  const char* pcDefaultSchema  = nullptr;
  // Note: we ignore the defaultFederation field in this message. Doesn't apply to SE.

  if (pMsg->__isset.defaultCatalog) {
    pcDefaultCatalog = pMsg->defaultCatalog.c_str();
  }

  if (pMsg->__isset.defaultSchema) {
    pcDefaultSchema = pMsg->defaultSchema.c_str();
  }

  // TODO: do something with the default catalog and schema


  // Send a SessionChangeResponse.
  SessionChangeResponse oSsnResponse;
  oSsnResponse.sessionId = "ABCD";    // TODO
  oSsnResponse.sessionKey = 1234;     // TODO
  vSerialiseAndSendMsg(&oSsnResponse, MessageTypes::type::SessionChangeResponse);

  vNextState(READY);
}

/*----------------------------------------------------------------------------*/
/* Processes a QueryExecuteRequest message                                    */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processQueryExecuteRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::QueryExecuteRequest);

  if (!bCheckState({READY})) { return; }

  QueryExecuteRequest* pMsg = dynamic_cast<QueryExecuteRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::QueryExecuteRequest).c_str());
    return;
  }

  // Sanity check the parameters in the message
  if (pMsg->rowsPerRowDataResponse < 0) {
    Stz* szMsg = "Error: %s: Message QueryExecuteRequest field rowsPerRowDataResponse cannot be negative. Current value: %lld"_Stz;
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    vSerialiseErrorResponse(m_eProtocolError,
                            szMsg,
                            aState[m_eProtocolError],
                            pMsg->rowsPerRowDataResponse);
    vNextState(READY);
    return;
  }

  if (pMsg->maxRowsToReturn < -1) {
    Stz* szMsg = "Error: %s: Message QueryExecuteRequest field maxRowsToReturn cannot be less than -1. Current value: %lld"_Stz;
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    vSerialiseErrorResponse(m_eProtocolError,
                            szMsg,
                            aState[m_eProtocolError],
                            pMsg->rowsPerRowDataResponse);
    vNextState(READY);
    return;
  }

  vNextState(QUERY_BEGUN);

  m_xQueryCtx.vResetContext();
  m_xStmtCtx.vResetContext();

  m_xQueryCtx.m_iQueryStartNanos = Timer::iGetTimeOfDay();

  // Move the current QueryExecuteRequest to the QueryContext object
  // so that we can keep it until the query is completed.
  m_xQueryCtx.m_pQueryExecuteRequest = pMsg;
  m_pCurrentMsg = nullptr;

  // The query string in the QueryExecuteRequest may contain more than one statement
  // to be executed. So let's walk that string and find the next statement based on
  // semicolons. We will then copy this to m_sSmtBuf which will then be submitted
  // through the API
  if (bExtractNextStatement()) {
    vNextState(API_STATEMENT_TO_SUBMIT);

  } else {
    // Send a QueryExecuteResponse to terminate the query and update the state appropriately
    vFinishQueryDialog();
  }

  ASSERT(!m_pCurrentMsg);
}

/*----------------------------------------------------------------------------*/
/* Processes a QueryProgressRequest message                                   */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processQueryProgressRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::QueryProgressRequest);

  //TODO: if (!bCheckState({CONNECTED})) { return; }

  QueryProgressRequest* pMsg = dynamic_cast<QueryProgressRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::QueryProgressRequest).c_str());
    return;
  }

  vErrorMessageNotSupported();

  // TODO: populate

//  vNextState(DISCONNECTED);
}

/*----------------------------------------------------------------------------*/
/* Processes a SessionMonitoringRequest message                               */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processSessionMonitoringRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::SessionMonitoringRequest);

  //TODO: if (!bCheckState({CONNECTED})) { return; }

  SessionMonitoringRequest* pMsg = dynamic_cast<SessionMonitoringRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::SessionMonitoringRequest).c_str());
    return;
  }

  vErrorMessageNotSupported();

  // TODO: populate

//  vNextState(DISCONNECTED);
}

/*----------------------------------------------------------------------------*/
/* Processes a ParseStatementRequest message                                  */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processParseStatementRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::ParseStatementRequest);

  //TODO: if (!bCheckState({CONNECTED})) { return; }

  ParseStatementRequest* pMsg = dynamic_cast<ParseStatementRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::ParseStatementRequest).c_str());
    return;
  }

  vErrorMessageNotSupported();

  // TODO: populate

//  vNextState(DISCONNECTED);
}

/*----------------------------------------------------------------------------*/
/* Processes a StatementBindAndExecuteRequest message                         */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processStatementBindAndExecuteRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::StatementBindAndExecuteRequest);

  //TODO: if (!bCheckState({CONNECTED})) { return; }

  StatementBindAndExecuteRequest* pMsg = dynamic_cast<StatementBindAndExecuteRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::StatementBindAndExecuteRequest).c_str());
    return;
  }

  vErrorMessageNotSupported();

  // TODO: populate

//  vNextState(DISCONNECTED);
}

/*----------------------------------------------------------------------------*/
/* Processes a ClosePreparedStatementRequest message                          */
/*----------------------------------------------------------------------------*/
void AdaptorWL::processClosePreparedStatementRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::ClosePreparedStatementRequest);

  //TODO: if (!bCheckState({CONNECTED})) { return; }

  ClosePreparedStatementRequest* pMsg = dynamic_cast<ClosePreparedStatementRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::ClosePreparedStatementRequest).c_str());
    return;
  }

  vErrorMessageNotSupported();

  // TODO: populate

//  vNextState(DISCONNECTED);
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*----------------------------------------------------------------------------*/
/* Check that we are in the correct state when receiving the current message  */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bCheckState(std::initializer_list<eState> eProtocolStates)
{
  for (eState eProtocolState : eProtocolStates) {
    if (m_eProtocolState == eProtocolState) {
      return true;
    }
  }

  // Make a string list of all applicable states for the error message.
  s2K sBuf;
  bool bFirst = true;
  for (eState eProtocolState : eProtocolStates) {
    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vConcat(aState[eProtocolState]);
  }

  Stz* szErrMsg = "Error: %s: Current protocol state is %s. Expected to be in one of the following states [%s]"_Stz;
  const eEC eErrType = EC_WL_PROTOCOL_ERROR;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          aState[m_eProtocolState],
                          sBuf.szStz());
  m_eProtocolError = eErrType;
  m_eProtocolState = DISCONNECTED;

  return false;
}

/*----------------------------------------------------------------------------*/
/* Internal error doing dynamic cast of received wireline message             */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorDynamicCast(const char* pcTargetType)
{
  Stz* szErrMsg = "Error: %s: Unable to cast decoded message to type %s"_Stz;
  const eEC eErrType = EC_WL_INTERNAL_ERROR;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          pcTargetType);
}

/*----------------------------------------------------------------------------*/
/* Internal error doing protocol state transition                             */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorStateTransition(Stz* szFromState, Stz* szToState)
{
  Stz* szErrMsg = "Error: %s: Invalid state transition from state %s to state %s"_Stz;
  const eEC eErrType = EC_WL_PROTOCOL_ERROR;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          szFromState,
                          szToState);
}

/*----------------------------------------------------------------------------*/
/* Wireline message is not supported yet                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorMessageNotSupported()
{
  Stz* szErrMsg = "Error: %s: %s"_Stz;
  const eEC eErrType = EC_WL_MSG_NOT_SUPPORTED;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          to_string((MessageTypes::type)m_iCurrentMsgType).c_str());
}

/*----------------------------------------------------------------------------*/
/* Wireline feature is not supported yet                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorFeatureNotSupported(Stz* szFeature)
{
  Stz* szErrMsg = "Error: %s: %s"_Stz;
  const eEC eErrType = EC_WL_FEATURE_NOT_SUPPORTED;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aErrorMsg[eErrType],
                          szFeature);
}

/*----------------------------------------------------------------------------*/
/* Returns true if a query is currently being executed                        */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bQueryInProgress()
{
  switch (m_eProtocolState) {
    case QUERY_BEGUN:
    case API_STATEMENT_SUBMITTED:
    case API_METADATA_RECEIVED:
    case API_DATA_RECEIVED:
    case API_STATEMENT_COMPLETED:
      return true;

    default:
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* Get the next statement from the QueryExecuteRequest & copy it to m_sSmtBuf.*/
/* Returns true if a next statement was found.                                */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bExtractNextStatement()
{
  ASSERT(m_xQueryCtx.m_iSmtCrs >= -1);
  ASSERT(m_xQueryCtx.m_pQueryExecuteRequest);
  ASSERT(!m_pCurrentMsg);
  ASSERT(bCheckState({QUERY_BEGUN, API_CHILD_JOB_EXITED}));

  QueryExecuteRequest* pMsg = m_xQueryCtx.m_pQueryExecuteRequest;
  ASSERT(pMsg);

  i8 nQueryLen = (i8) pMsg->query.length();

  i8 iStartCrs = m_xQueryCtx.m_iSmtCrs + 1;

  while (++m_xQueryCtx.m_iSmtCrs < nQueryLen) {
    char c = pMsg->query.at(m_xQueryCtx.m_iSmtCrs);

    switch (c) {

      case '/':
        // Check for SQL comments and advance over the comment block
        if (    (m_xQueryCtx.m_iSmtCrs + 1 < nQueryLen)
             && pMsg->query.at(m_xQueryCtx.m_iSmtCrs + 1) == '*'
           ) {
          ++m_xQueryCtx.m_iSmtCrs;  // Cursor points at the first '*'

          // Advance to the "*/" or the EOS
          while (++m_xQueryCtx.m_iSmtCrs < nQueryLen) {
            char d = pMsg->query.at(m_xQueryCtx.m_iSmtCrs);
            if (d == '*' && (m_xQueryCtx.m_iSmtCrs + 1 < nQueryLen) && pMsg->query.at(m_xQueryCtx.m_iSmtCrs + 1) == '/') {
              break;
            }
          }
        }
        break;

      case '\'':
        // Advance over the single-quoted string
        while (++m_xQueryCtx.m_iSmtCrs < nQueryLen && pMsg->query.at(m_xQueryCtx.m_iSmtCrs) != '\'') {}
        break;

      case '"':
        // Advance over the double-quoted string
        while (++m_xQueryCtx.m_iSmtCrs < nQueryLen && pMsg->query.at(m_xQueryCtx.m_iSmtCrs) != '"') {}
        break;

      case '`':
        // Advance over the backtick identifier
        while (++m_xQueryCtx.m_iSmtCrs < nQueryLen && pMsg->query.at(m_xQueryCtx.m_iSmtCrs) != '`') {}
        break;

      case ';':
        // Extract the semicolon terminated statement
        // TEMP: Don't include the terminating semicolon as this gets interpreted as another
        //       argument to the sqlCompiler procedure.
        m_xStmtCtx.m_sSmtBuf.vCopy((Stz*) (pMsg->query.c_str() + iStartCrs), m_xQueryCtx.m_iSmtCrs - iStartCrs);
//        m_xStmtCtx.m_sSmtBuf.vCopy((Stz*) (pMsg->query.c_str() + m_xQueryCtx.iStartCrs),
//                                   m_xQueryCtx.m_iSmtCrs - iStartCrs + 1);
        ++m_xQueryCtx.m_nSmtParsed;
        return true;

      default:
        continue;
    }
  }

  // No semicolon terminated statement found
  return false;
}

/*----------------------------------------------------------------------------*/
/* Send a QueryExecuteResponse, update the state & clean up the current query */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vFinishQueryDialog()
{
  vNextState(QUERY_COMPLETED);

  i8 iQueryEndNanos = Timer::iGetTimeOfDay();
  double dQueryTime = (double) (iQueryEndNanos - m_xQueryCtx.m_iQueryStartNanos) / C_1B;

  QueryExecuteResponse xResponse;
  xResponse.executionTime = dQueryTime;
  xResponse.numStatementsParsed = m_xQueryCtx.m_nSmtParsed;
  xResponse.numStatementsSucceeded = m_xQueryCtx.m_nSmtSucceeded;
  xResponse.numStatementsFailed = m_xQueryCtx.m_nSmtFailed;

  vSerialiseAndSendMsg(&xResponse, MessageTypes::type::QueryExecuteResponse);

  // Doing this after changing the state to QUERY_COMPLETED will delete m_pCurrentMsg
  m_xQueryCtx.vResetContext();

  vNextState(READY);
}

/*============================================================================*/
/* QueryContext struct                                                        */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryContext::QueryContext(void)
{
  m_pQueryExecuteRequest = nullptr;
  m_iSmtCrs           = -1;
  m_nSmtParsed        = 0;
  m_nSmtSucceeded     = 0;
  m_nSmtFailed        = 0;
  m_iQueryStartNanos  = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueryContext::~QueryContext(void) noexcept(false)
{
  delete m_pQueryExecuteRequest;
}

/*----------------------------------------------------------------------------*/
/* Reset                                                                      */
/*----------------------------------------------------------------------------*/
void QueryContext::vResetContext()
{
  delete m_pQueryExecuteRequest;
  m_pQueryExecuteRequest = nullptr;
  m_iSmtCrs           = -1;
  m_nSmtParsed        = 0;
  m_nSmtSucceeded     = 0;
  m_nSmtFailed        = 0;
  m_iQueryStartNanos  = 0;
}

/*============================================================================*/
/* StatementContext struct                                                    */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StatementContext::StatementContext(void)
{
  vResetContext();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StatementContext::~StatementContext(void) noexcept(false)
{

}

/*----------------------------------------------------------------------------*/
/* Reset                                                                      */
/*----------------------------------------------------------------------------*/
void StatementContext::vResetContext()
{
  m_sSmtBuf.vReset();
  m_bGotSmtError = false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

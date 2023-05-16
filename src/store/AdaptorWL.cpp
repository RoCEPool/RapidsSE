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
#include "StatementContext.hpp"
#include "IndexDef.hpp"
#include "TailLine.hpp"
#include "RowInflator.hpp"
#include "SP_Bcd.hpp"
#include "Job.hpp"

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

const i8 C_MIN_SUPPORT_PROTOCOL_VER = 5;
const i8 C_MAX_SUPPORT_PROTOCOL_VER = 5;

// If the RowDataResponse message is too big to fit into the payload then we scale
// the number of rows in it by this amount until it does fit.
const d8 C_ROWDATARESP_SCALE_DOWN = 0.8;  // Multiplied
const d8 C_ROWDATARESP_SCALE_UP   = 0.05; // Added

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

// Static function definitions
static com::rapidsdata::wirelineprotocol::messages::v5::AffectedVerb::type
  eToWLAffectedVerb(WLP::StatementContext::eAffectedVerb eVerb);



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

  //m_oQueryCtx
  m_pStmtCtx = nullptr;

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

  delete m_pStmtCtx;

  // m_oQueryCtx  will be cleaned up automatically
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

    case MessageTypes::type::ProtocolCompatibleRequest:       vProcessProtocolCompatibleRequest();      break;
    case MessageTypes::type::DisconnectRequest:               vProcessDisconnectRequest();              break;
    case MessageTypes::type::PingRequest:                     vProcessPingRequest();                    break;
    case MessageTypes::type::ClearTextTunnelRequest:          vProcessClearTextTunnelRequest();         break;
    case MessageTypes::type::PlainAuthenticationRequest:      vProcessPlainAuthenticationRequest();     break;
    case MessageTypes::type::KerberosAuthenticationRequest:   vProcessKerberosAuthenticationRequest();  break;
    case MessageTypes::type::GssNegotiationMsg:               vProcessGssNegotiationMsg();              break;
    case MessageTypes::type::SessionChangeRequest:            vProcessSessionChangeRequest();           break;
    case MessageTypes::type::QueryExecuteRequest:             vProcessQueryExecuteRequest();            break;
    case MessageTypes::type::QueryProgressRequest:            vProcessQueryProgressRequest();           break;
    case MessageTypes::type::SessionMonitoringRequest:        vProcessSessionMonitoringRequest();       break;
    case MessageTypes::type::ParseStatementRequest:           vProcessParseStatementRequest();          break;
    case MessageTypes::type::StatementBindAndExecuteRequest:  vProcessStatementBindAndExecuteRequest(); break;
    case MessageTypes::type::ClosePreparedStatementRequest:   vProcessClosePreparedStatementRequest();  break;

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
                              "Error: %s; %s; Processing an unexpected wireline message type of %s."_Stz,
                              aEC2[eErrCode],
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
/* Is there a query to be parsed through the SE API                           */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bHasQuery()
{
  return (m_oQueryCtx.szQueryText() != nullptr);
}

/*----------------------------------------------------------------------------*/
/* Get the query string to be parsed through the SE API.                      */
/* May contain multiple statements.                                           */
/*----------------------------------------------------------------------------*/
Stz* AdaptorWL::szQuery()
{
  Stz* szQuery = m_oQueryCtx.szQueryText();
  ASSERT(szQuery);
  return szQuery;
}

/*----------------------------------------------------------------------------*/
/* Do we need to abort any remaining statements?                              */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bAbortEarly()
{
  return m_oQueryCtx.bAbortEarly();
}

/*----------------------------------------------------------------------------*/
/* Update the state that a statement has been submitted                       */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vStatementToSubmit(Task* pNextTask)
{
  ASSERT(bCheckState( {QUERY_BEGUN, API_CHILD_JOB_EXITED} ));

  vNextState(API_STATEMENT_TO_SUBMIT);

  // Create a statement context to hold information about the running of
  // this current statement in the (potential) batch.

  ASSERT(!m_pStmtCtx);

  Stz* szCommand = pNextTask->pJob()->szCmd();
  ASSERT(szCommand);

  s256 sJobId;
  sJobId.vSprint("%lld"_Stz, pNextTask->pJob()->tJobId().iAsI8());

  m_pStmtCtx = new WLP::StatementContext(szCommand,
                                         sJobId.szStz());

  m_oQueryCtx.vIncStmtParsed();
}

/*----------------------------------------------------------------------------*/
/* Update the state that a statement has been submitted                       */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vStatementSubmitted()
{
  ASSERT(m_pStmtCtx);
  ASSERT(bCheckState( {API_STATEMENT_TO_SUBMIT} ));

  vNextState(API_STATEMENT_SUBMITTED);
}

/*----------------------------------------------------------------------------*/
/* Update the state that a statement has failed to submit                     */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vStatementParsingFailed(eEC eErrCode, Stz* szErrMsg)
{
  ASSERT(bCheckState( {QUERY_BEGUN} ));
  Stz* szErrFmt= "Error: %s; %s: %s"_Stz;

  vSerialiseErrorResponse(eErrCode, szErrFmt, aEC2[eErrCode], aErrorMsg[eErrCode], szErrMsg);
  ASSERT(!m_pStmtCtx);
}

/*----------------------------------------------------------------------------*/
/* Update the state that a statement has failed to submit                     */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vStatementSubmissionFailed(eEC eErrCode)
{
  ASSERT(bCheckState( {API_STATEMENT_TO_SUBMIT} ));
  Stz* szErrMsg= "Error: %s; %s"_Stz;

  vSerialiseErrorResponse(eErrCode, szErrMsg, aEC2[eErrCode], aErrorMsg[eErrCode]);
  m_pStmtCtx->vGotError(true);

  // The child job failed during submission.
  vChildJobExited();
}

/*----------------------------------------------------------------------------*/
/* Update the state after an error is received from the SQL compiler          */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vCompilerError(InfoSet* pInfoSet)
{
  ASSERT(m_pStmtCtx);

  if (m_pStmtCtx->bGotError()) {
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

  m_oQueryCtx.vIncStmtFailed();
  m_pStmtCtx->vGotError(true);

  // An error at this point ends the current statement
  vNextState(API_STATEMENT_COMPLETED);
}

/*----------------------------------------------------------------------------*/
/* Update the state after an error is received from the execution engine      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vExecutionError(InfoSet* pInfoSet)
{
  ASSERT(m_pStmtCtx);

  if (m_pStmtCtx->bGotError()) {
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

  m_pStmtCtx->vGotError(true);

  // An error at this point ends the current statement
  vNextState(API_STATEMENT_COMPLETED);
}

/*----------------------------------------------------------------------------*/
/* Update the state after the child job has exited                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vChildJobExited()
{
  ASSERT(m_pStmtCtx);
  ASSERT(bCheckState( { API_STATEMENT_TO_SUBMIT,
                        API_STATEMENT_SUBMITTED,
                        API_METADATA_RECEIVED,
                        API_DATA_RECEIVED,
                        API_STATEMENT_COMPLETED } ));

  if (!m_pStmtCtx->bGotError()) {
    // We need to send a StatementResponse message to the client
    m_oQueryCtx.vIncStmtSucceeded();

    StatementResponse xMsg;
    xMsg.__set_statementId(m_pStmtCtx->szStmtId()->azChar());
    xMsg.__set_statementString(m_pStmtCtx->szStmt()->azChar());
    xMsg.__set_numMatchedRows(m_pStmtCtx->nAffectedRows());
    xMsg.__set_numAffectedRows(m_pStmtCtx->nAffectedRows());
    xMsg.__set_affectedVerb(eToWLAffectedVerb(m_pStmtCtx->eVerb()));

    vSerialiseAndSendMsg(&xMsg, MessageTypes::type::StatementResponse);
  }

  bool bAbortRemaining = (m_pStmtCtx->bGotError() && !m_oQueryCtx.bContinueOnError());

  delete m_pStmtCtx;
  m_pStmtCtx = nullptr;

  vNextState(API_CHILD_JOB_EXITED);

  if (bAbortRemaining) {
    m_oQueryCtx.vAbortEarly(true);
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
/* Notify the client about the resultset definition                           */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vResultMetadata(StreamDef* pStreamDef)
{
  ASSERT(m_pStmtCtx);
  ASSERT(bCheckState( {API_STATEMENT_SUBMITTED} ));
  vNextState(API_METADATA_RECEIVED);

  if (m_pStmtCtx->bGotError()) {
    return;
  }

  // Check if a resultset was expected for this query
  if (m_oQueryCtx.bMustNotReturnRows()) {
    // This is an error
    m_pStmtCtx->vGotError(true);

    eEC eErrCode = EC_WL_RESULTSET_RETURNED;
    Stz* szErrMsg = "Error: %s: %s"_Stz;
    vSerialiseErrorResponse(eErrCode,
                            szErrMsg,
                            aErrorMsg[eErrCode],
                            m_pStmtCtx->szStmt());
    return;
  }

  // Save the stream definition so we can interpret the row data
  m_pStmtCtx->vStreamDef(pStreamDef);

  // Build the list of columns for the thrift message
  std::vector<ColumnMetadata> xColumns;
  i8 nColumns = pStreamDef->nColumns();

  for (i8 i=0; i<nColumns; i++) {
    Column* pColumn = pStreamDef->pGetColumn(i);

    // Convert the SE eCT type to a wireline type
    eCT eColType = pColumn->eColumnType();
    i8 iWLType = eToWLType(eColType);
    DataType::type eWLType = (DataType::type) iWLType;

    if (iWLType < 0) {
      eEC eErrCode = EC_WL_INTERNAL_ERROR;

      if (eColType < CT_NONE || eColType > CT_ENUMS) {
        Stz* szErrMsg = "Error: %s: Could not convert internal type %d to a wireline type"_Stz;
        vSerialiseErrorResponse(eErrCode,
                                szErrMsg,
                                aErrorMsg[eErrCode],
                                eColType);

      } else {
        Stz* szErrMsg = "Error: %s: Could not convert internal type %s to a wireline type"_Stz;
        vSerialiseErrorResponse(eErrCode,
                                szErrMsg,
                                aErrorMsg[eErrCode],
                                aCT[eColType]);
      }

      m_pStmtCtx->vGotError(true);
      vNextState(API_STATEMENT_COMPLETED);
      return;
    }

    ColumnMetadata xColumn{};
    xColumn.__set_columnName(pColumn->szColNm()->azChar());
    xColumn.__set_columnType(eWLType);

    if (pColumn->iColumnLength() > 0)   xColumn.__set_precision(pColumn->iColumnLength());
    if (pColumn->iColumnScale() > 0)    xColumn.__set_scale(pColumn->iColumnScale());

    xColumn.__set_isNullable(pColumn->bIsNullable());

    // TODO: Fill in when supported
//    xColumn.__set_charsetName();
//    xColumn.__set_collationName();
//    xColumn.__set_isAutoIncrement();

    xColumns.push_back(xColumn);
  }

  RowMetadataResponse xRowMetadataResponse;
  xRowMetadataResponse.__set_numColumns(nColumns);
  xRowMetadataResponse.__set_isBoundedResult(true);
  xRowMetadataResponse.__set_columns(xColumns);

  vSerialiseAndSendMsg(&xRowMetadataResponse, MessageTypes::type::RowMetadataResponse);
}

/*----------------------------------------------------------------------------*/
/* Send row data to the client                                                */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vRowData(CrwBuffer* pCrwBuffer)
{
  ASSERT(m_pStmtCtx);
  ASSERT(m_pStmtCtx->pStreamDef());

  ASSERT(bCheckState({API_METADATA_RECEIVED, API_DATA_RECEIVED}));
  vNextState(API_DATA_RECEIVED);

  if (m_pStmtCtx->bGotError()) {
    return;
  }

  StreamDef* pDef = m_pStmtCtx->pStreamDef();
  RowPix* pRowPix = pDef->pRowPix();

  HV_RowSingle hvRowSingle(pDef);

  // Work out how many rows from this CrwBuffer to serialise
  i8 iStartingRow = 0;
  d8 dScalingFactor = 1.0;  // if the serialised form doesn't fit in a single payload buffer
  i8 iRowsInBuf = pCrwBuffer->nHeads();
  i8 iMaxRowsThatFit = 0; // how many did we sucessfully fit in a RowDataResponse?

  // We might need to send multiple RowDataResponses for this CrwBuffer.
  // This can be due to a limit of rows per response
  while(iStartingRow < iRowsInBuf) {
    std::vector<com::rapidsdata::wirelineprotocol::messages::v5::Row> xRowList;

    i8 iRowsToSend = nRowsToSend(iRowsInBuf - iStartingRow);
    if (iRowsToSend <= 0) {
      break;
    }

    // If we are sending a smaller number of rows then don't scale the quantity.
    // We don't want the last partial filled RowDataResponse to be split into
    // many more smaller ones.
    if (iMaxRowsThatFit == 0 || iRowsToSend > iMaxRowsThatFit) {
      iRowsToSend = (i8) (iRowsToSend * dScalingFactor + 0.5);
      if (iRowsToSend == 0) {
        iRowsToSend = 1;
      }

    } else {
      // We're sending less rows than we have previously fit. If these don't
      // fit then we ought to restart the scaling once again.
      dScalingFactor = 1.0;
      iMaxRowsThatFit = 0;
    }

    for (i8 i_Crw = iStartingRow; i_Crw < iRowsToSend; i_Crw++) {
      // get the compact row
      Crw* aCrw = (Crw*) pCrwBuffer->aGetHead(i_Crw);

      // ignore deleted rows
      if (aCrw->bIsDeleted()) {
        continue;
      }

      // inflate compact row into a RowSingle
      RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle, aCrw);
      Row* pRow = hvRowSingle->pRowCurrent();

      vCreateAndAddRow(&xRowList, pRow, pRowPix);
    }

    ASSERT(xRowList.size() == iRowsToSend);

    RowDataResponse xRowDataResponse;
    xRowDataResponse.__set_rows(xRowList);

    // Serialise and send. Ensure that the message is < 1MB.
    try {
      vSerialiseAndSendMsg(&xRowDataResponse, MessageTypes::type::RowDataResponse, false);

    } catch (WLP::BufferFullException& e) {

      // Special case where we cannot serialise a single row.
      if (dScalingFactor < 1.0 && iRowsToSend == 1) {
        m_pStmtCtx->vGotError(true);
        eEC eErrCode = EC_WL_ROW_TOO_BIG;
        Stz* szErrMsg = "Error: %s; %s; %ldd bytes"_Stz;
        vSerialiseErrorResponse(eErrCode,
                                szErrMsg,
                                aEC2[eErrCode],
                                aErrorMsg[eErrCode],
                                WLP::PayloadObj::C_MAX_PAYLOAD_BYTES);
        return;
      }

      dScalingFactor *= C_ROWDATARESP_SCALE_DOWN;
      vTrace("RowDataResponse cannot fit %lld rows\n"_Stz, iRowsToSend);
      continue;
    }

    vTrace("RowDataResponse serialised %lld rows\n"_Stz, iRowsToSend);
    iStartingRow += iRowsToSend;

    if (iRowsToSend > iMaxRowsThatFit) {
      iMaxRowsThatFit = iRowsToSend;
    }

    // Slowly increase the scaling factor
    dScalingFactor += C_ROWDATARESP_SCALE_UP;
    if (dScalingFactor > 1.0) {
      dScalingFactor = 1.0;
    }

    m_pStmtCtx->vAddAffectedRows(iRowsToSend);
  }
}

/*----------------------------------------------------------------------------*/
/* Send row data to the client                                                */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vRowData(RowBuffer* pRowBuffer)
{
  ASSERT(m_pStmtCtx);
  ASSERT(m_pStmtCtx->pStreamDef());

  ASSERT(bCheckState({API_METADATA_RECEIVED, API_DATA_RECEIVED}));
  vNextState(API_DATA_RECEIVED);

  if (m_pStmtCtx->bGotError()) {
    return;
  }

  StreamDef* pDef = m_pStmtCtx->pStreamDef();
  RowPix* pRowPix = pDef->pRowPix();

  // Work out how many rows from this RowBuffer to serialise
  i8 iStartingRow = 0;
  d8 dScalingFactor = 1.0;  // if the serialised form doesn't fit in a single payload buffer
  i8 iRowsInBuf = pRowBuffer->nRows();
  i8 iMaxRowsThatFit = 0; // how many did we sucessfully fit in a RowDataResponse?

  // We might need to send multiple RowDataResponses for this RowBuffer.
  // This can be due to a limit of rows per response
  while(iStartingRow < iRowsInBuf) {
    std::vector<com::rapidsdata::wirelineprotocol::messages::v5::Row> xRowList;

    i8 iRowsToSend = nRowsToSend(iRowsInBuf - iStartingRow);
    if (iRowsToSend <= 0) {
      break;
    }

    // If we are sending a smaller number of rows then don't scale the quantity.
    // We don't want the last partial filled RowDataResponse to be split into
    // many more smaller ones.
    if (iMaxRowsThatFit == 0 || iRowsToSend > iMaxRowsThatFit) {
      iRowsToSend = (i8) (iRowsToSend * dScalingFactor + 0.5);
      if (iRowsToSend == 0) {
        iRowsToSend = 1;
      }

    } else {
      // We're sending less rows than we have previously fit. If these don't
      // fit then we ought to restart the scaling once again.
      dScalingFactor = 1.0;
      iMaxRowsThatFit = 0;
    }


    for (i8 i_Row = iStartingRow; i_Row < iStartingRow + iRowsToSend; i_Row++) {
      Row* pRow = pRowBuffer->pRowAddress(i_Row);
      vCreateAndAddRow(&xRowList, pRow, pRowPix);
    }

    ASSERT(xRowList.size() == iRowsToSend);

    RowDataResponse xRowDataResponse;
    xRowDataResponse.__set_rows(xRowList);

    // Serialise and send. Ensure that the message is < 1MB.
    try {
      vSerialiseAndSendMsg(&xRowDataResponse, MessageTypes::type::RowDataResponse, false);

    } catch (WLP::BufferFullException& e) {

      // Special case where we cannot serialise a single row.
      if (dScalingFactor < 1.0 && iRowsToSend == 1) {
        m_pStmtCtx->vGotError(true);
        eEC eErrCode = EC_WL_ROW_TOO_BIG;
        Stz* szErrMsg = "Error: %s; %s; %ldd bytes"_Stz;
        vSerialiseErrorResponse(eErrCode,
                                szErrMsg,
                                aEC2[eErrCode],
                                aErrorMsg[eErrCode],
                                WLP::PayloadObj::C_MAX_PAYLOAD_BYTES);
        return;
      }

      dScalingFactor *= C_ROWDATARESP_SCALE_DOWN;
      vTrace("RowDataResponse cannot fit %lld rows\n"_Stz, iRowsToSend);
      continue;
    }

    vTrace("RowDataResponse serialised %lld rows\n"_Stz, iRowsToSend);
    iStartingRow += iRowsToSend;

    if (iRowsToSend > iMaxRowsThatFit) {
      iMaxRowsThatFit = iRowsToSend;
    }

    // Slowly increase the scaling factor
    dScalingFactor += C_ROWDATARESP_SCALE_UP;
    if (dScalingFactor > 1.0) {
      dScalingFactor = 1.0;
    }

    m_pStmtCtx->vAddAffectedRows(iRowsToSend);
  }
}

/*----------------------------------------------------------------------------*/
/* Send a QueryExecuteResponse, update the state & clean up the current query */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vQueryComplete()
{
  ASSERT(bCheckState( { QUERY_BEGUN,
                        API_CHILD_JOB_EXITED } ));
  vNextState(QUERY_COMPLETED);

  u8 uQueryEndNanos = Hardware::uReadClock();
  double dQueryTime = (double) (uQueryEndNanos - m_oQueryCtx.uQueryStartNanos()) / C_1B;

  QueryExecuteResponse xResponse;
  xResponse.executionTime = dQueryTime;
  xResponse.numStatementsParsed    = m_oQueryCtx.nStmtParsed();
  xResponse.numStatementsSucceeded = m_oQueryCtx.nStmtSucceeded();
  xResponse.numStatementsFailed    = m_oQueryCtx.nStmtFailed();

  vSerialiseAndSendMsg(&xResponse, MessageTypes::type::QueryExecuteResponse);

  // Doing this after changing the state to QUERY_COMPLETED will delete m_pCurrentMsg
  m_oQueryCtx.vReset();

  vNextState(READY);
}

/*----------------------------------------------------------------------------*/
/* Processed parsed tasks and send a QueryReceiptResponse back to the client  */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vQueryReceipt(ENGINE::TaskList* pTaskList)
{
  ASSERT(bCheckState( {QUERY_BEGUN} ));

  QueryReceiptResponse xQueryReceipt;
  std::vector<StatementReceipt> xStatementReceipts;

  i8 nStatements = 0;

  pTaskList->vGrabLock();
  Task* pTask = pTaskList->pPeekTask();
  pTaskList->vDropLock();

  while (pTask != nullptr) {
    ++nStatements;

    Stz* szCommand = pTask->pJob()->szCmd();
    s256 sJobId;
    sJobId.vSprint("%lld"_Stz, pTask->pJob()->tJobId().iAsI8());

    StatementReceipt xStatementReceipt{};
    xStatementReceipt.__set_statementString(szCommand->azChar());
    xStatementReceipt.__set_statementId(sJobId.azChar());
    xStatementReceipt.__set_statementKey(12345);    // TODO: set this appropriately

    xStatementReceipts.push_back(xStatementReceipt);

    // TODO: Register the statement ID and statement key

    pTask = pTask->pNextItem();
  }

  xQueryReceipt.__set_statementReceipts(xStatementReceipts);
  vSerialiseAndSendMsg(&xQueryReceipt, MessageTypes::QueryReceiptResponse);
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
/* Throws a WLP::BufferFullException if the message doesn't fit and           */
/* bHandleBufferFull is false. Otherwise it serialises an ErrorMsg & closes   */
/* the socket.                                                                */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vSerialiseAndSendMsg(apache::thrift::TBase* pMsg, i8 iMsgType, bool bHandleBufferFull)
{
  MessageTypes::type eMsgType = (MessageTypes::type) iMsgType;

  WLP::PayloadObj* pPayload = new WLP::PayloadObj();

  // Serialise the error message to a payload
  m_pTransport->vUseBuffer(pPayload->pcBuf(), WLP::PayloadObj::C_MAX_PAYLOAD_BYTES);
  WLP::WlpHeader* pHeader = m_pTransport->pAllocHeader(eMsgType);

  try {
    pMsg->write(m_pProtocol.get());

  } catch (WLP::BufferFullException& e) {

    m_pTransport->vRemoveBuffer();
    delete pPayload;

    if (!bHandleBufferFull) {
      // We expect this exception to be caught elsewhere.
      throw;
    }

    // Handle this situation by sending an error message and causing the client socket
    // to be closed.
    m_eProtocolError = EC_WL_MSG_TOO_BIG; // This causes the socket closure.
    s2K sBuf;
    sBuf.vSprint("Error: %s; %s;"_Stz, aEC2[m_eProtocolError], aState[m_eProtocolError]);

    ErrorResponse oErrorResponse;
    oErrorResponse.__set_errorString(sBuf.azChar());
    oErrorResponse.__set_errorCode(m_eProtocolError);

    // Recurse, but importantly, if this fails then we are sweet out of luck.
    try {
      vSerialiseAndSendMsg(&oErrorResponse, MessageTypes::ErrorResponse, false);

    } catch (WLP::BufferFullException& e) {
      vTrace("Unable to serialise a simple ErrorResponse about a buffer being full.\n"_Stz);
      // Client will be disconnected anyway.
    }
    return;
  }

  // Message size needs to be written into the header of the message
  // as well as the payload object.
  i8 iMsgSize = m_pTransport->iBytesWritten();
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
  /*07*/ static StateList aNextStates_QueryBegun         = { API_STATEMENT_TO_SUBMIT, QUERY_COMPLETED, DISCONNECTED };
  /*08*/ static StateList aNextStates_ApiStmtToSubmit    = { API_STATEMENT_SUBMITTED, API_CHILD_JOB_EXITED, DISCONNECTED };
  /*08*/ static StateList aNextStates_ApiStmtSubmitted   = { API_METADATA_RECEIVED, API_STATEMENT_COMPLETED, API_CHILD_JOB_EXITED };
  /*09*/ static StateList aNextStates_ApiMetadataRcvd    = { API_DATA_RECEIVED, API_STATEMENT_COMPLETED, API_CHILD_JOB_EXITED };
  /*10*/ static StateList aNextStates_ApiDataRcvd        = { API_DATA_RECEIVED, API_STATEMENT_COMPLETED, API_CHILD_JOB_EXITED };
  /*11*/ static StateList aNextStates_ApiStmtCompleted   = { API_CHILD_JOB_EXITED };
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
    /*08*/ case API_STATEMENT_TO_SUBMIT: eValidNextStates = aNextStates_ApiStmtToSubmit;     break;
    /*09*/ case API_STATEMENT_SUBMITTED: eValidNextStates = aNextStates_ApiStmtSubmitted;    break;
    /*10*/ case API_METADATA_RECEIVED:   eValidNextStates = aNextStates_ApiMetadataRcvd;     break;
    /*11*/ case API_DATA_RECEIVED:       eValidNextStates = aNextStates_ApiDataRcvd;         break;
    /*12*/ case API_STATEMENT_COMPLETED: eValidNextStates = aNextStates_ApiStmtCompleted;    break;
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
    vTrace("Wireline client is making an invalid transition from state %s to %s\n"_Stz,
           aState[m_eProtocolState], aState[eNextState]);
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
void AdaptorWL::vProcessProtocolCompatibleRequest()
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
void AdaptorWL::vProcessDisconnectRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::DisconnectRequest);

  // Nothing to do in this message. We set m_eProtocolError to cause the socket
  // to be closed.

  m_eProtocolError = EC_WL_SOCKET_CLOSED; // Cause the socket to be closed.
  vNextState(DISCONNECTED);
}

/*----------------------------------------------------------------------------*/
/* Processes a PingRequest message                                            */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vProcessPingRequest()
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
void AdaptorWL::vProcessClearTextTunnelRequest()
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
void AdaptorWL::vProcessPlainAuthenticationRequest()
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
void AdaptorWL::vProcessKerberosAuthenticationRequest()
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
void AdaptorWL::vProcessGssNegotiationMsg()
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
void AdaptorWL::vProcessSessionChangeRequest()
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
void AdaptorWL::vProcessQueryExecuteRequest()
{
  ASSERT(m_pCurrentMsg);
  ASSERT(m_iCurrentMsgType == MessageTypes::type::QueryExecuteRequest);
  ASSERT(!m_pStmtCtx);
  ASSERT(m_oQueryCtx.nStmtParsed() == 0);
  ASSERT(m_oQueryCtx.nStmtSucceeded() == 0);
  ASSERT(m_oQueryCtx.nStmtFailed() == 0);

  if (!bCheckState({READY})) { return; }

  QueryExecuteRequest* pMsg = dynamic_cast<QueryExecuteRequest*>(m_pCurrentMsg);
  if (!pMsg) {
    vErrorDynamicCast(to_string(MessageTypes::type::QueryExecuteRequest).c_str());
    return;
  }

  // Sanity check the parameters in the message
  if (pMsg->rowsPerRowDataResponse < 0) {
    // Sending this error before the QueryReceiptResponse ends the dialog
    Stz* szMsg = "Error: %s; %s; Message QueryExecuteRequest field rowsPerRowDataResponse cannot be negative. Current value: %lld"_Stz;
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    vSerialiseErrorResponse(m_eProtocolError,
                            szMsg,
                            aEC2[m_eProtocolError],
                            aState[m_eProtocolError],
                            pMsg->rowsPerRowDataResponse);
    vNextState(READY);
    return;
  }

  if (pMsg->maxRowsToReturn < -1) {
    // Sending this error before the QueryReceiptResponse ends the dialog
    Stz* szMsg = "Error: %s; %s; Message QueryExecuteRequest field maxRowsToReturn cannot be less than -1. Current value: %lld"_Stz;
    m_eProtocolError = EC_WL_PROTOCOL_ERROR;
    vSerialiseErrorResponse(m_eProtocolError,
                            szMsg,
                            aEC2[m_eProtocolError],
                            aState[m_eProtocolError],
                            pMsg->rowsPerRowDataResponse);
    vNextState(READY);
    return;
  }

  vNextState(QUERY_BEGUN);

  // Move the current QueryExecuteRequest to the QueryContext object
  // so that we can keep it until the query is completed.
  m_oQueryCtx.vInit(pMsg);
  m_pCurrentMsg = nullptr;
}

/*----------------------------------------------------------------------------*/
/* Processes a QueryProgressRequest message                                   */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vProcessQueryProgressRequest()
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
void AdaptorWL::vProcessSessionMonitoringRequest()
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
void AdaptorWL::vProcessParseStatementRequest()
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
void AdaptorWL::vProcessStatementBindAndExecuteRequest()
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
void AdaptorWL::vProcessClosePreparedStatementRequest()
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

  Stz* szErrMsg = "Error: %s; %s; Current protocol state is %s. Expected to be in one of the following states [%s]"_Stz;
  const eEC eErrType = EC_WL_PROTOCOL_ERROR;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aEC2[eErrType],
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
  Stz* szErrMsg = "Error: %s; %s; Unable to cast decoded message to type %s"_Stz;
  const eEC eErrType = EC_WL_INTERNAL_ERROR;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aEC2[eErrType],
                          aErrorMsg[eErrType],
                          pcTargetType);
}

/*----------------------------------------------------------------------------*/
/* Internal error doing protocol state transition                             */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorStateTransition(Stz* szFromState, Stz* szToState)
{
  Stz* szErrMsg = "Error: %s; %s; Invalid state transition from state %s to state %s"_Stz;
  const eEC eErrType = EC_WL_PROTOCOL_ERROR;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aEC2[eErrType],
                          aErrorMsg[eErrType],
                          szFromState,
                          szToState);
}

/*----------------------------------------------------------------------------*/
/* Wireline message is not supported yet                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorMessageNotSupported()
{
  Stz* szErrMsg = "Error: %s; %s; %s"_Stz;
  const eEC eErrType = EC_WL_MSG_NOT_SUPPORTED;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aEC2[eErrType],
                          aErrorMsg[eErrType],
                          to_string((MessageTypes::type)m_iCurrentMsgType).c_str());
}

/*----------------------------------------------------------------------------*/
/* Wireline feature is not supported yet                                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vErrorFeatureNotSupported(Stz* szFeature)
{
  Stz* szErrMsg = "Error: %s; %s; %s"_Stz;
  const eEC eErrType = EC_WL_FEATURE_NOT_SUPPORTED;

  vSerialiseErrorResponse(eErrType,
                          szErrMsg,
                          aEC2[eErrType],
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
/* Convert an eCT to a wireline data type (DataType::type)                    */
/* We return as an i8 because we can't forward declare this enum in the hpp.  */
/*----------------------------------------------------------------------------*/
i8 AdaptorWL::eToWLType(eCT eColType)
{
  switch (eColType) {
    case CT_NONE:         return -1;
    case CT_BOOLEAN:      return DataType::BOOLEAN;
    case CT_CHAR:         return DataType::VARCHAR;
    case CT_DATE:         return DataType::DATE;
    case CT_DECIMAL:      return DataType::DECIMAL;
    case CT_FLOAT:        return DataType::FLOAT;
    case CT_INTEGER:      return DataType::BIGINT;
    case CT_INTERVAL_DT:  return DataType::INTERVAL_DAY_TO_SECOND;  // TODO: Needs to change and be a specific interval type
    case CT_INTERVAL_YM:  return DataType::INTERVAL_YEAR_TO_MONTH;  // TODO: Needs to change and be a specific interval type
    case CT_TIME:         return DataType::TIME;
    case CT_TIMESTAMP:    return DataType::TIMESTAMP;
    case CT_VARCHAR:      return DataType::VARCHAR;
    case CT_UNKNOWN:      return DataType::NULL;
    case CT_ENUMS:        return -1;
    default:              NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Work out how many rows to send in the RowDataResponse                      */
/*----------------------------------------------------------------------------*/
i8 AdaptorWL::nRowsToSend(i8 iRowsInBuf)
{
  i8 iMaxRows         = m_oQueryCtx.iMaxRowsToReturn();
  i8 iRowsAlreadySent = m_pStmtCtx->nAffectedRows();
  i8 iRowsPerResponse = m_oQueryCtx.iRowsPerRowDataResponse();
  i8 iRowsToSend      = iRowsInBuf;

  // Work out how many rows to send in this RowDataResponse
  if (iMaxRows != -1) {
    iRowsToSend = iMaxRows - iRowsAlreadySent;

    if (iRowsToSend > iRowsInBuf) {
      iRowsToSend = iRowsInBuf;
    }
  }

  if (iRowsPerResponse > 0 && iRowsToSend > iRowsPerResponse) {
    iRowsToSend = iRowsPerResponse;
  }

  return iRowsToSend;
}

/*----------------------------------------------------------------------------*/
/* Work out how many rows to send in the RowDataResponse                      */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vCreateAndAddRow(std::vector<com::rapidsdata::wirelineprotocol::messages::v5::Row>* pRowList,
                                 Row* pRow,
                                 RowPix* pRowPix)
{
  typedef com::rapidsdata::wirelineprotocol::messages::v5::Row RowWL;
  typedef com::rapidsdata::wirelineprotocol::messages::v5::ColumnValue ColumnWL;
  typedef com::rapidsdata::wirelineprotocol::messages::v5::Date DateWL;
  typedef com::rapidsdata::wirelineprotocol::messages::v5::Time TimeWL;
  typedef com::rapidsdata::wirelineprotocol::messages::v5::Timestamp TimestampWL;
  typedef com::rapidsdata::wirelineprotocol::messages::v5::Interval IntervalWL;

  RowWL xRowWL;
  std::vector<ColumnWL> xColumns;

  for (i8 i_Col = 0;
       i_Col < pRowPix->m_nColumns - pRowPix->m_nExtraCols;
       i_Col++) {

    ColumnWL    xColumnWL;
    DateWL      xDateWL;
    TimeWL      xTimeWL;
    TimestampWL xTimestampWL;
    IntervalWL  xIntervalWL;
    Dtm         wDtm;
    Bcd         xBcd;
    SP_Bcd      spBcd;

    eDM eDatum = (eDM) pRowPix->m_xaColPix[i_Col].m_uDatum;

    // Null check with special case for strings and decimals (why?)
    bool bIsNull = false;
    switch (eDatum) {
      case DM_VCH:
      case DM_BCD:
        if (pRow->bIsColNull(i_Col)) {
          bIsNull = true;
        }
        break;

      default:
        if (pRow->iColVal_int(i_Col) == C_DATUM_NULL) {
          bIsNull = true;
        }
        break;
    }

    if (bIsNull) {
      xColumnWL.__set_isNull(true);
      continue;
    }

    // Not null value
    i8 iVal = pRow->iColVal_int(i_Col);

    switch (eDatum) {
      case DM_BCD:
        xBcd = iVal;
        xColumnWL.__set_bigDecimalVal(spBcd.sz(xBcd.T.m_iMantissa,xBcd.T.m_iScale)->azChar()); // Fix
        break;

      case DM_BOL:
        xColumnWL.__set_booleanVal(iVal);
        break;

      case DM_DTE:
        wDtm = iVal;
        xDateWL.__set_year(wDtm.m_wDte.iYear());
        xDateWL.__set_month(wDtm.m_wDte.iMonth());
        xDateWL.__set_day(wDtm.m_wDte.iDay());
        xColumnWL.__set_dateVal(xDateWL);
        break;

      case DM_FLT:
        xColumnWL.__set_doubleVal(*(d8*)&iVal);
        break;

      case DM_INT:
        xColumnWL.__set_bigintVal(iVal);
        break;

      case DM_IVD:  // day-time
        // TODO: We actually need to set the specific interval type and then only set the fields that matter.
        wDtm = iVal;
        xIntervalWL.__set_type(IntervalType::type::DAY_TO_SECOND);
        xIntervalWL.__set_isNegative(wDtm.m_wIvl.T.m_xIvl.m_bIsNeg);
        xIntervalWL.__set_day(wDtm.m_wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M));
        xIntervalWL.__set_hour(wDtm.m_wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros   % (C_SECS_PER_DAY * C_1M) / (C_SECS_PER_HR * C_1M));
        xIntervalWL.__set_minute(wDtm.m_wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR * C_1M)  / (C_SECS_PER_MIN * C_1M));
        xIntervalWL.__set_second(wDtm.m_wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M) / C_1M);
        xIntervalWL.__set_nanoseconds((wDtm.m_wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M) * C_1K);
        xColumnWL.__set_intervalVal(xIntervalWL);
        break;

      case DM_IVY:  // year-month
        // TODO: We actually need to set the specific interval type and then only set the fields that matter.
        wDtm = iVal;
        xIntervalWL.__set_type(IntervalType::type::YEAR_TO_MONTH);
        xIntervalWL.__set_isNegative(wDtm.m_wIvl.T.m_xIvl.m_bIsNeg);
        xIntervalWL.__set_year(wDtm.m_wIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12);
        xIntervalWL.__set_month(wDtm.m_wIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths % 12);
        xColumnWL.__set_intervalVal(xIntervalWL);
        break;

      case DM_TME:
        wDtm = iVal;
        xTimeWL.__set_hour(wDtm.m_wTme.iHour());
        xTimeWL.__set_minute(wDtm.m_wTme.iMin());
        xTimeWL.__set_second(wDtm.m_wTme.iSec());
        xTimeWL.__set_nanoseconds(wDtm.m_wTme.m_uMicro * C_1K);
        xColumnWL.__set_timeVal(xTimeWL);
        break;

      case DM_TSP:
        wDtm = iVal;
        xTimestampWL.__set_year(wDtm.m_wTsp.iYear());
        xTimestampWL.__set_month(wDtm.m_wTsp.iMonth());
        xTimestampWL.__set_day(wDtm.m_wTsp.iDay());
        xTimestampWL.__set_hour(wDtm.m_wTsp.iHour());
        xTimestampWL.__set_minute(wDtm.m_wTsp.iMin());
        xTimestampWL.__set_second(wDtm.m_wTsp.iSec());
        xTimestampWL.__set_nanoseconds(wDtm.m_wTsp.iMicro() * C_1K);
        xColumnWL.__set_timestampVal(xTimestampWL);
        break;

      case DM_VCH:
        xColumnWL.__set_stringVal(pRow->szColVal_ofs(i_Col)->azChar());
        break;

      case DM_UNK:
        xColumnWL.__set_isNull(true);
        break;

      default:
        NEVER_REACH
    }

    xColumns.push_back(xColumnWL);
  }

  // Add the columns to the row, and the row to the rowlist
  xRowWL.__set_columns(xColumns);
  pRowList->push_back(xRowWL);
}



/*----------------------------------------------------------------------------*/
/* Helper to convert an internal AffectedVerb enum to the wireline version.   */
/*----------------------------------------------------------------------------*/
com::rapidsdata::wirelineprotocol::messages::v5::AffectedVerb::type
eToWLAffectedVerb(WLP::StatementContext::eAffectedVerb eVerb) {

  switch (eVerb) {
    case WLP::StatementContext::NONE:      return AffectedVerb::type::NONE;
    case WLP::StatementContext::SELECTED:  return AffectedVerb::type::SELECTED;
    case WLP::StatementContext::INSERTED:  return AffectedVerb::type::INSERTED;
    case WLP::StatementContext::UPDATED:   return AffectedVerb::type::UPDATED;
    case WLP::StatementContext::DELETED:   return AffectedVerb::type::DELETED;
    default: NEVER_REACH;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

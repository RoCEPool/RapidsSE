/*============================================================================*/
/* AdaptorWL.hpp -- Wire Line Adaptor                                         */
/*============================================================================*/
#pragma once

#include <vector>
#include "BASE.h"

#include "ApiHelper.hpp"
#include "PayloadObj.hpp"
#include "QueryContext.hpp"

// Forward declarations (outside namespace STORE scope)
namespace WLP { class BufferTransport; }
namespace apache { namespace thrift { class TBase; }}
namespace apache { namespace thrift { namespace transport { class TTransport; }}}
namespace apache { namespace thrift { namespace protocol { template <typename TYPE> class TCompactProtocolT; }}}

namespace com { namespace rapidsdata { namespace wirelineprotocol { namespace messages { namespace v5 {
  struct MessageTypes;
  struct DataType;
  class QueryExecuteRequest;
  class Row;
}}}}}

namespace STORE
{
  // Forward declarations (inside namespace STORE scope)
  typedef ListExo_t<CN_PAYLOADLIST, WLP::PayloadObj> PayloadList;

  class AdaptorWL : public Object
                  , public ApiHelper
  {
  private:
    // There is a state transition diagram that describes the valid transitions between
    // these states on the Sharepoint fileserver under the Wire Line Protocol directory.
    enum eState {
      /* 00 */ NONE = 0,
      /* 01 */ CONNECTED,
      /* 02 */ PROTOCOL_COMPATIBLE,
      /* 03 */ TUNNELED,
      /* 04 */ KERBEROS_NEGOTIATING,
      /* 05 */ AUTHENTICATED,
      /* 06 */ READY,
      /* 07 */ QUERY_BEGUN,
      /* 08 */ API_STATEMENT_TO_SUBMIT,
      /* 09 */ API_STATEMENT_SUBMITTED,
      /* 10 */ API_METADATA_RECEIVED,
      /* 11 */ API_DATA_RECEIVED,
      /* 12 */ API_STATEMENT_COMPLETED,
      /* 13 */ API_CHILD_JOB_EXITED,
      /* 14 */ QUERY_COMPLETED,
      /* 15 */ DISCONNECTED,
      /* 16 */ NENUMS
    };

    static Stz* aState[];

    // Current State enum
    apache::thrift::TBase* m_pCurrentMsg;
    i8           m_iCurrentMsgType;
    PayloadList* m_pPayloadSendList;
    eEC          m_eProtocolError;
    eState       m_eProtocolState;

    WLP::QueryContext m_oQueryCtx;
    WLP::StatementContext* m_pStmtCtx;

    // For thrift serialisation and deserialisation
    std::shared_ptr<WLP::BufferTransport> m_pTransport;
    std::shared_ptr<apache::thrift::protocol::TCompactProtocolT<apache::thrift::transport::TTransport>> m_pProtocol;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    AdaptorWL();
   ~AdaptorWL() noexcept(false);
  
    static void vRepair(Object* pObject);

    void vDecodePayload(WLP::PayloadObj* pPayload);
    void vProcessWLMsg();
    eEC  eProtocolError();

    bool bHasPayloadToSend();
    WLP::PayloadObj* pNextPayloadToSend();

    Stz* szStatement();
    bool bHasStatement();

    bool bHasQuery();   // TODO
    Stz* szQuery();     // TODO

    // State-change handlers called from sen code
    void vStatementToSubmit(Task* pNextTask);    // TODO
    void vQueryComplete();        // TODO

    void vStatementSubmitted();
    void vStatementParsingFailed(eEC eErrCode, Stz* szErrMsg);
    void vStatementSubmissionFailed(eEC eErrCode);
    void vCompilerError(InfoSet* pInfoSet);
    void vExecutionError(InfoSet* pInfoSet);
    void vChildJobExited();
    bool bIsChildJobActive();
    void vResultMetadata(StreamDef* pStreamDef);
    void vRowData(CrwBuffer* pCrwBuffer);
    void vRowData(RowBuffer* pRowBuffer);

  private:

    Footer* aFooter();
    void vDeserialiseMsg(i8 iMessageType);
    void vCleanupCurrentMsg();
    void vSerialiseErrorResponse(eEC eErrorCode, Stz* sFormat, ...);
    void vSerialiseAndSendMsg(apache::thrift::TBase* pMsg, i8 iMsgType, bool bHandleBufferFull = true);
    void vNextState(eState eNextState);

    // Wireline message handlers
    void vProcessProtocolCompatibleRequest();
    void vProcessDisconnectRequest();
    void vProcessPingRequest();
    void vProcessClearTextTunnelRequest();
    void vProcessPlainAuthenticationRequest();
    void vProcessKerberosAuthenticationRequest();
    void vProcessGssNegotiationMsg();
    void vProcessSessionChangeRequest();
    void vProcessQueryExecuteRequest();
    void vProcessQueryProgressRequest();
    void vProcessSessionMonitoringRequest();
    void vProcessParseStatementRequest();
    void vProcessStatementBindAndExecuteRequest();
    void vProcessClosePreparedStatementRequest();


    // Errors
    bool bCheckState(std::initializer_list<eState> eProtocolStates);
    void vErrorDynamicCast(const char* pcTargetType);
    void vErrorStateTransition(Stz* szFromState, Stz* szToState);
    void vErrorMessageNotSupported();
    void vErrorFeatureNotSupported(Stz* szFeature);

    // Helpers
    bool bQueryInProgress();
    void vSendQueryReceipt();
    void vFinishQueryDialog();
    i8   eToWLType(eCT eColType);
    i8   nRowsToSend(i8 iRowsInBuf);
    void vCreateAndAddRow(
        std::vector<com::rapidsdata::wirelineprotocol::messages::v5::Row>* pRowList,
        Row* pRow,
        RowPix* pRowPix);
  };

  AdaptorWL* _pAdaptorWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

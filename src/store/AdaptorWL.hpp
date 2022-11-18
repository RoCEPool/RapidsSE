/*============================================================================*/
/* AdaptorWL.hpp -- Wire Line Adaptor                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ApiHelper.hpp"
#include "PayloadObj.hpp"

// Forward declarations (outside namespace STORE scope)
namespace WLP { class BufferTransport; }
namespace apache { namespace thrift { class TBase; }}
namespace apache { namespace thrift { namespace transport { class TTransport; }}}
namespace apache { namespace thrift { namespace protocol { template <typename TYPE> class TCompactProtocolT; }}}

namespace com { namespace rapidsdata { namespace wirelineprotocol { namespace messages { namespace v5 {
  struct MessageTypes;
  struct DataType;
  class QueryExecuteRequest;
}}}}}

namespace STORE
{
  // Forward declarations (inside namespace STORE scope)
  typedef ListExo_t<CN_PAYLOADLIST, WLP::PayloadObj> PayloadList;

  // Internal structure/classes used
  struct QueryContext {
    com::rapidsdata::wirelineprotocol::messages::v5::QueryExecuteRequest* m_pQueryExecuteRequest;
    i8  m_iStmtCrs;
    i8  m_nStmtParsed;
    i8  m_nStmtSucceeded;
    i8  m_nStmtFailed;
    i8  m_iQueryStartNanos;

    QueryContext();
    ~QueryContext() noexcept(false);

    void vResetContext();
  };

  struct StatementContext {
    s48K  m_sStmtBuf;
    bool  m_bGotStmtError;

    StatementContext();
    ~StatementContext();

    void vResetContext();
  };

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

    QueryContext     m_xQueryCtx;
    StatementContext m_xStmtCtx;

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
    void vResetStmtCtx();

    // State-change handlers called from sen code
    void vStatementSubmitted();
    void vStatementSubmissionFailed(eEC eErrCode);
    void vCompilerError(InfoSet* pInfoSet);
    void vExecutionError(InfoSet* pInfoSet);
    void vChildJobExited();
    bool bIsChildJobActive();
    void vResultMetadata(StreamDef* pStreamDef);

  private:

    Footer* aFooter();
    void vDeserialiseMsg(i8 iMessageType);
    void vCleanupCurrentMsg();
    void vSerialiseErrorResponse(eEC eErrorCode, Stz* sFormat, ...);
    void vSerialiseAndSendMsg(apache::thrift::TBase* pMsg, i8 iMsgType);
    void vNextState(eState eNextState);

    // Wireline message handlers
    void processProtocolCompatibleRequest();
    void processDisconnectRequest();
    void processPingRequest();
    void processClearTextTunnelRequest();
    void processPlainAuthenticationRequest();
    void processKerberosAuthenticationRequest();
    void processGssNegotiationMsg();
    void processSessionChangeRequest();
    void processQueryExecuteRequest();
    void processQueryProgressRequest();
    void processSessionMonitoringRequest();
    void processParseStatementRequest();
    void processStatementBindAndExecuteRequest();
    void processClosePreparedStatementRequest();


    // Errors
    bool bCheckState(std::initializer_list<eState> eProtocolStates);
    void vErrorDynamicCast(const char* pcTargetType);
    void vErrorStateTransition(Stz* szFromState, Stz* szToState);
    void vErrorMessageNotSupported();
    void vErrorFeatureNotSupported(Stz* szFeature);

    // Helpers
    bool bQueryInProgress();
    bool bExtractNextStatement();
    void vSendQueryReceipt();
    void vFinishQueryDialog();
    i8   eToWLType(eCT eColType);
  };

  AdaptorWL* _pAdaptorWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

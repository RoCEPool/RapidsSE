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
}}}}}

namespace STORE
{
  // Forward declarations (inside namespace STORE scope)
  typedef ListExo_t<CN_PAYLOADLIST, WLP::PayloadObj> PayloadList;


  class AdaptorWL : public Object
                  , public ApiHelper
  {
  private:
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
      /* 13 */ QUERY_COMPLETED,
      /* 14 */ DISCONNECTED,
      /* 15 */ NENUMS
    };

    static Stz* aState[];

    // Current State enum
    apache::thrift::TBase* m_pCurrentMsg;
    i8           m_iCurrentMsgType;
    PayloadList* m_pPayloadSendList;
    eEC          m_eProtocolError;
    eState       m_eProtocolState;

    s48K         m_sSmtBuf;
    i8           m_iSmtCrs;

    i8           m_nSmtParsed;
    i8           m_nSmtSucceeded;
    i8           m_nSmtFailed;
    i8           m_iQueryStartNanos;

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
    void vClearStatement();

    void vStatementSubmitted();
    void vStatementSubmitted(eEC eErrCode);

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
    void vFinishQueryDialog();
  };

  AdaptorWL* _pAdaptorWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

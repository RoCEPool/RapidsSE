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
      NONE = 0,
      CONNECTED,
      PROTOCOL_COMPATIBLE,
      TUNNELED,
      AUTHENTICATED,
      READY,
      PROCESSING_MSG,
      WAITING_FOR_API,
      DISCONNECTED,
      NENUMS
    };

    static Stz* aState[];

    // Current State enum
    apache::thrift::TBase* m_pCurrentMsg;
    i8     m_iCurrentMsgType;
    PayloadList* m_pPayloadSendList;
    bool   m_bContinueProcessingMsg;
    eEC    m_eProtocolError;
    eState m_eProtocolState;

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
    bool bKeepProcessingMsg();
    eEC  eProtocolError();
    bool bHasPayloadToSend();
    WLP::PayloadObj* pNextPayloadToSend();

  private:

    Footer* aFooter();
    void vDeserialiseMsg(i8 iMessageType);
    void vCleanupCurrentMsg();
    void vSerialiseErrorResponse(eEC eErrorCode, Stz* sFormat, ...);
    void vSerialiseMsg(apache::thrift::TBase* pMsg, i8 iMsgType);
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

  };

  AdaptorWL* _pAdaptorWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AdaptorWL.hpp -- Wire Line Adaptor                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ApiHelper.hpp"
#include "PayloadObj.hpp"

namespace STORE
{
  typedef ListExo_t<CN_PAYLOADLIST, WLP::PayloadObj> PayloadList;

  class AdaptorWL : public Object
                  , public ApiHelper
  {
  private:
    // Current State
    // Current decoded message
    PayloadList* m_pPayloadSendList;
    bool m_bContinueProcessingMsg;
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    AdaptorWL(void);
   ~AdaptorWL(void) noexcept(false);
  
    static void vRepair(Object* pObject);

    void vDecodePayload(WLP::PayloadObj* pPayload);
    void vProcessWLMsg(void);
    bool bKeepProcessingMsg(void);
    bool bHasPayloadToSend(void);
    WLP::PayloadObj* pNextPayloadToSend(void);

  private:
  
    Footer* aFooter(void);

  };

  AdaptorWL* _pAdaptorWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

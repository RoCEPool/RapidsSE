/*============================================================================*/
/* SEinterface.hpp - se interface                                             */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class SEinterface : public Object
  {
  public:
    Dictionary m_xDictionary;

    Queue* m_pRequestQueue;
    Queue* m_pResponseQueue;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEinterface(void);
    ~SEinterface(void);
    
    Dictionary* pDictionary(void);
    
    void vDump(const char* aTitle);
    
    void vInit(void);
    void vAppendRequest(s& sRequest);
    bool bPopResponse(s& sResponse);
    
    void   vRequestQueue(Queue* pRequestQueue);
    Queue* pRequestQueue(void);
    
    void   vResponseQueue(Queue* pResponseQueue);
    Queue* pResponseQueue(void);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

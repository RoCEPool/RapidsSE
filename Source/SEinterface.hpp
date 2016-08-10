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
    i8         m_iPid;           // process which created this seitf
    Dictionary m_xDictionary;    // private dictionary

    Queue*     m_pRequestQueue;  // request queue
    Queue*     m_pResponseQueue; // reponse queue
    
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

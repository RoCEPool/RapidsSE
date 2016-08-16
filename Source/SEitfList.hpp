/*============================================================================*/
/* SessionList.hpp - Session List                                             */
/*============================================================================*/

#pragma once

#include "Interface.h"

namespace Interface {
    
  class SessionList : public Object,
                      public List_t<SEitf>
  {
  private:
    
  public:
    void* operator new(size_t, SessionList* pSessionList);
    void  operator delete(void*);
    
    SessionList(void);
    ~SessionList(void);
    
    SEitf* pAllocSEitf(void);
    void     vFreeSEitf(SEitf* pSEitf);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
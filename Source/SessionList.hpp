/*============================================================================*/
/* SessionSey.hpp - Session Set                                               */
/*============================================================================*/

#pragma once

#include "Sessions.h"

namespace Sessions {
    
  class SessionList : public Object,
                      public List_t<Session>
  {
  private:
    
  public:
    void* operator new(size_t, SessionList* pSessionList);
    void  operator delete(void*);
    
    SessionList(void);
    ~SessionList(void);
    
    Session* pAllocSession(void);
    void     vFreeSession(Session* pSession);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
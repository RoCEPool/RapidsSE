/*============================================================================*/
/* Session.hpp - Session                                                      */
/*============================================================================*/

#pragma once

#include "Sessions.h"

namespace Sessions {
    
  class Session : public Object,
                  public ListNode_t<Session>
  {
  private:
    User*  m_pUser;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Session(void);
    
    void vDump(void);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
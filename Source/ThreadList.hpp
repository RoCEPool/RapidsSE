/*============================================================================*/
/* ThreadList.hpp - Thread Set                                                */
/*============================================================================*/

#pragma once

#include "Threading.h"

namespace Threading {
    
  class ThreadList : public Object,
                     public List_t<Thread>
  {
  private:
    
  public:
    void* operator new(size_t, ThreadList* pThreadList);
    void  operator delete(void*);
    
    ThreadList(void);
    ~ThreadList(void);
    
    Thread* pAllocThread(void);
    void    vFreeThread(Thread* pThread);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
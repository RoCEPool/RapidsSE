/*============================================================================*/
/* ThreadList.hpp - Thread Set                                                */
/*============================================================================*/

#pragma once

#include "Threads.h"

namespace Threads {
    
  class ThreadList : public Object,
                     public List_t<Thread>
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, ThreadList* pThreadList);
    void  operator delete(void*);
    
    ThreadList(void);
    ~ThreadList(void);
    
    Thread* pAllocThread(void);
    void    vFreeThread(Thread* pThread);
    
    Footer* pFooter(void);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
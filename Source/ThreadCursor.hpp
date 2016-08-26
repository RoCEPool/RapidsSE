/*============================================================================*/
/* ThreadCursor.hpp -- Cursor over Threads in ThreadList                      */
/*============================================================================*/

#pragma once

#include "Threading.h"

namespace Threading
{
  class ThreadCursor : public Object,
                       public ListCursor_t<Thread>
  {
  private:
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, ThreadCursor* pThreadCursor);
    void  operator delete(void*);
    
    ThreadCursor(List_t<Thread>* pThreadList);
    ~ThreadCursor(void);
    
    void vInit(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
/*============================================================================*/
/* ThreadCursor.hpp -- Cursor over Threads in ThreadList                      */
/*============================================================================*/
#pragma once

namespace Threads
{
  class ThreadCursor : public Object,
                       public ListCursor_t<Thread>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, ThreadCursor* pThreadCursor);
    void  operator delete(void*);
    
    ThreadCursor(List_t<Thread>* pThreadList);
    ~ThreadCursor(void);
    
    void vInit(void);
    
    Footer* aFooter(void);
  };
  
  ThreadCursor* _pThreadCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

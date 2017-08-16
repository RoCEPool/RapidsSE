/*============================================================================*/
/* ThreadQueue.hpp -- Thread Queue                                            */
/*============================================================================*/
#pragma once

namespace Threads
{    
  class ThreadQueue : public Object,
                      public List_t<Thread>
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, ThreadQueue* pThreadQueue);
    void  operator delete(void*);
    
    ThreadQueue(void);
    ~ThreadQueue(void);
    
    Thread* pAllocThread(void);
    void    vFreeThread(Thread* pThread);
    
    Footer* aFooter(void);
  };
    
  ThreadQueue* _pThreadQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

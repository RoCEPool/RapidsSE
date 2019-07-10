/*============================================================================*/
/* ThreadQueue.hpp -- queue of (idle) threads                                 */
/*============================================================================*/
#pragma once

namespace Threads
{
  class ThreadQueue : public ListNL_t<Thread>
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ThreadQueue(void);
    ~ThreadQueue(void) noexcept(false);
    
    void vAppendThread(Thread* pThread);
    void vDropLock(void);
    void vGrabLock(void);
    bool bGrabLock(void);
    bool bIsLocked(void);
    Thread* pPeekThread(void);
    Thread* pPopThread(void);
    i8   nQueuedThreads(void);
  
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  ThreadQueue* _pThreadQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

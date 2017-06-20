/*============================================================================*/
/* Semaphore.hpp - a simple semaphore                                         */
/*============================================================================*/
#pragma once

namespace System
{
  class Semaphore : public Object
  {
  public:
    bool             m_bLocked;  // true if mutex is locked (debugging)
    pthread_mutex_t  m_xMutex;   // mutex variable
    pthread_cond_t   m_xCond;    // condition variable
    
    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void operator delete(void* pPosixThreadAddress);

    Semaphore(void);
    ~Semaphore(void);
  
    void vCondBroadcast(void);
    void vCondSignal(void);
    void vCondWait(void);
    void vCondTimedWait(i8 iNanoSeconds);
    bool bMutexLock(void);
    void vMutexLock(void);
    void vMutexUnlock(void);
  
    void vSignalThread(void);
  
    Footer* aFooter(void);
  };
  
  Semaphore* _pSemaphore(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

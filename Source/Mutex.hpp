/*============================================================================*/
/* Mutex.hpp -- Pthread Mutex (& cond variable                                */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Mutex : public Resource
  {
  public:
    bool                m_bLocked;    // true if mutex is locked (debugging)
  
    pthread_mutex_t     m_xMutex;     // mutex variable
    pthread_mutexattr_t m_xMutexAttr; // mutex shared attribute
  
    pthread_cond_t      m_xCond;      // condition variable
    pthread_condattr_t  m_xCondAttr;  // condition shared attribute

    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void* operator new(size_t, Mutex* pMutex);
    void operator delete(void* pMutexAddress);

    Mutex(void);
    ~Mutex(void) noexcept(false);
  
    static void vMonitorPortrait(s48K& sPortrait);
  
    void vCondBroadcast(void);
    void vCondSignal(void);
    void vCondWaitIndefinite(void);
    void vCondWaitUsec(i8 iMicroSeconds);
    bool bMutexIsLocked(void);
    bool bMutexLock(void);
    void vMutexLock(void);
    void vMutexUnlock(void);
  
    Footer* aFooter(void);
  };
  
  Mutex* _pMutex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

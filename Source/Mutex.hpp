/*============================================================================*/
/* Mutex.hpp -- Mutex (posix pthread mutex and condition variable)            */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Mutex : public Resource
  {
  public:
    eMK     m_eKind;   // kind of mutex
    Focal*  m_pFor;    // mutex is for a task or a thread
  
    bool    m_bLocked; // true if mutex is currently locked (debugging)
  
    pthread_mutex_t     m_xMutex;     // posix mutex variable
    pthread_mutexattr_t m_xMutexAttr; // posix mutex attribute
  
    pthread_cond_t      m_xCond;      // posix condition variable
    pthread_condattr_t  m_xCondAttr;  // posix condition attribute
  
    Atomic m_ixBroadcast;
    Atomic m_ixBusy;
    Atomic m_ixSignal;
    Atomic m_ixSignaled;
    Atomic m_ixWaitIndef;
    Atomic m_ixWaitTimed;
    Atomic m_ixLock;
    Atomic m_ixUnlock;
    Atomic m_ixTimeout;

    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void* operator new(size_t, Mutex* pMutex);
    void operator delete(void* pMutexAddress);

    Mutex(eMK eSuperKind, Focal* pFor, ReqTuple* pReqTuple);
   ~Mutex(void) noexcept(false);
  
    static void vMonitorPortrait(s48K& sPortrait);
  
    void vCondBroadcast(void);
    void vCondSignal(void);
    void vCondWaitIndefinite(void);
    void vCondWaitUsec(i8 iMicroSeconds);
    void vFocalOutline(s4K& sOutline);
    bool bMutexIsLocked(void);
    bool bMutexLock(void);
    void vMutexLock(Object* pByWhom);
    void vMutexUnlock(void);
  
    Footer* aFooter(void);
  };
  
  Mutex* _pMutex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

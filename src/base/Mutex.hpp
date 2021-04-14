/*============================================================================*/
/* Mutex.hpp -- Mutex (posix pthread mutex and condition variable)            */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Mutex : public Object
  {
  public:
    Object*    m_pIsFor;
    Loggable*  m_pLockedBy; // by whom is the mutex locked?
    Loggable*  m_pWaitedBy; // for whom is the mutex being waited on?

    pthread_mutex_t     m_xMutex;     // posix mutex variable
    pthread_mutexattr_t m_xMutexAttr; // posix mutex attribute
  
    pthread_cond_t      m_xCond;      // posix condition variable
    pthread_condattr_t  m_xCondAttr;  // posix condition attribute
  
    Atomic m_ixBroadcast;
    Atomic m_ixBusy;
    Atomic m_ixLock;
    Atomic m_ixLocked;
    Atomic m_ixSignal;
    Atomic m_ixSignaled;
    Atomic m_ixWaitIndef;
    Atomic m_ixWaitTimed;
    Atomic m_ixUnlock;
    Atomic m_ixTimedout;

    // must be last
    Footer m_xFooter;
  
    Mutex(Object* bIsFor);
   ~Mutex(void) noexcept(false);
  
    void vBroadcast(void);
    bool bIsLocked(void);
    bool bIsLockedByCaller(Loggable* pLockedBy);
    bool bLock(Loggable* pLockFor);
    void vLock(Loggable* pLockedBy);
    void vPause(Loggable* pLockFor, i8 iMicroSeconds);
    void vSignal(Loggable* pLockedBy);
    void vUnlock(Loggable* pLockedBy);
    void vWait(Loggable* pLockedBy);
  
    void vOutline(s4K& sOutline);
  
    Footer* aFooter(void);
  };
  
  Mutex* _pMutex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

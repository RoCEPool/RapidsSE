/*============================================================================*/
/* Mutex.hpp -- Mutex (posix pthread mutex and condition variable)            */
/*============================================================================*/
#pragma once

#include "eMK.h"

namespace Resources
{
  class Mutex : public Object
  {
  public:
    eMK     m_eMutex;    // kind of mutex
  
    bool    m_bObserve;  // observe this mutex?
    Focal*  m_pLockedBy; // by whom is the mutex locked?
    Focal*  m_pWaitedBy; // for whom is the mutex being waited on?

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
  
    Mutex(eMK eKind, Focal* pIsFor);
   ~Mutex(void) noexcept(false);
  
    static void vMonitorPortrait(s48K& sPortrait);
  
    void vBroadcast(void);
    bool bIsLocked(void);
    bool bIsLockedByCaller(Focal* pLockedBy);
    bool bLock(Focal* pLockFor);
    void vLock(Focal* pLockedBy);
    bool bObserve(void);
    void vObserve(bool bObserve);
    void vPause(Focal* pLockFor, i8 iMicroSeconds);
    void vSignal(Focal* pLockedBy);
    void vUnlock(Focal* pLockedBy);
    void vWait(Focal* pLockedBy);
  
    void vFocalOutline(s4K& sOutline);
  
    Footer* aFooter(void);
  };
  
  Mutex* _pMutex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

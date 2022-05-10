/*============================================================================*/
/* Mutex.hpp -- Mutex (posix pthread mutex and condition variable)            */
/*============================================================================*/
#pragma once

#include "eMX.h"

namespace BASE
{
  class Mutex : public Object
  {
  public:
    Object*  m_pIsFor;
    Object*  m_pLockedBy; // by whom is the mutex locked?
    Object*  m_pWaitedBy; // for whom is the mutex being waited on?

    eMX m_eMutex;
  
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
  
  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Mutex(Object* bIsFor);
   ~Mutex(void) noexcept(false);
  
    eMX  eMutex(void);
    void vMutex(eMX eMutex); // temp

    void vBroadcast(void);
    bool bIsLocked(void);
    bool bIsLockedByCaller(Object* pLockedBy);
    bool bLock(Object* pLockFor);
    void vLock(Object* pLockedBy);
    void vPause(Object* pLockFor, i8 iMicroSeconds);
    void vSignal(Object* pLockedBy);
    void vUnlock(Object* pLockedBy);
    void vWait(Object* pLockedBy);
  
    void vOutline(s4K& sOutline);
  
    Footer* aFooter(void);
  };
  
  Mutex* _pMutex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

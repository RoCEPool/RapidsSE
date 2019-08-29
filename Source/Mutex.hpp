/*============================================================================*/
/* Mutex.hpp -- Pthread Mutex (& cond variable)                               */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Mutex : public Resource
  {
  public:
    eMK     m_eKind;  // kind of mutex
    i8      m_iSuperId;
    Object* m_pFor;    // mutex is for a task or a threadx
  
    bool m_bLocked;    // true if mutex is currently locked (debugging)
  
    pthread_mutex_t     m_xMutex;     // posix mutex variable
    pthread_mutexattr_t m_xMutexAttr; // posix mutex attribute
  
    pthread_cond_t      m_xCond;      // posix condition variable
    pthread_condattr_t  m_xCondAttr;  // posix condition attribute

    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void* operator new(size_t, Mutex* pMutex);
    void operator delete(void* pMutexAddress);

    Mutex(eMK eSuperKind, Object* pFor, i8 i_SuperId, ReqTuple* pReqTuple);
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

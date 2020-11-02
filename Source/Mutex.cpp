/*============================================================================*/
/* Mutex.cpp -- Mutex (posix pthread mutex and condition variable)            */
/*============================================================================*/

#include "Locking.h"


Atomic gixMutex;
Atomic gixMutex_err;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Mutex* Locking::_pMutex(void* aAddr)
{
  Mutex* pMutex = reinterpret_cast<Mutex*>(aAddr);
  
  RASSERT_TYPE(pMutex,CN_MUTEX);
  
  return pMutex;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Mutex::Mutex(eMK eKind, Focal* pIsFor)
: Object(CN_MUTEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  m_eMutex    = eKind;
  
  m_bObserve  = false;
  
  m_pLockedBy = nullptr;
  m_pWaitedBy = nullptr;

  // m_ixBroadcast;
  // m_ixSignal;
  // m_ixWaitIndef;
  // m_ixWaitTimed;
  // m_ixLocks;
  // m_ixUnlocks;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // initialise attribute to mutex
  MemLib::vZero(&m_xMutexAttr,sizeof(m_xMutexAttr),CN_MUTEX);
  i8 iResult = pthread_mutexattr_init(&m_xMutexAttr);
  RASSERT(! iResult);
  
  // set attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xMutexAttr,PTHREAD_PROCESS_SHARED);
  RASSERT(! iResult);

  // initialize the mutex
  MemLib::vZero(&m_xMutex,sizeof(m_xMutex),CN_MUTEX);
  iResult = pthread_mutex_init(&m_xMutex, &m_xMutexAttr);
  RASSERT(! iResult);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // initialise attribute to condition
  MemLib::vZero(&m_xCondAttr,sizeof(m_xCondAttr),CN_MUTEX);
  iResult = pthread_condattr_init(&m_xCondAttr);
  RASSERT(! iResult);

  // set attribute to be shared
  iResult = pthread_condattr_setpshared(&m_xCondAttr, PTHREAD_PROCESS_SHARED);
  RASSERT(! iResult);

  // initialize the mutex
  MemLib::vZero(&m_xCond,sizeof(m_xCond),CN_MUTEX);
  iResult = pthread_cond_init(&m_xCond, &m_xCondAttr);
  RASSERT(! iResult);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nInits++;
#endif
  
  _checkInitialised_(CN_MUTEX);
    
  ++ gixMutex;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Mutex::~Mutex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //RASSERT(! m_pLockedBy);
  //RASSERT(! m_pWaitedBy);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iResult;
  
  iResult = pthread_mutex_destroy(&m_xMutex);
  RASSERT(! iResult);
  iResult = pthread_mutexattr_destroy(&m_xMutexAttr);
  RASSERT(! iResult);

  iResult = pthread_cond_destroy(&m_xCond);
  RASSERT(! iResult);
  iResult = pthread_condattr_destroy(&m_xCondAttr);
  RASSERT(! iResult);

    
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nDestroys++;
#endif
  
  -- gixMutex;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get monotor portrait of semaphore activity                                 */
/*----------------------------------------------------------------------------*/
void Mutex::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();
  
  sPortrait.vSprint("Mutex: broadcast %lld destroys %lld inits %lld "
                    "signals %lld waits %lld timedwaits %lld "
                    "unblocks %lld timeouts %lld\n"_Stz,
                    pStats->Mutex.m_nBroadcasts,
                    pStats->Mutex.m_nDestroys,
                    pStats->Mutex.m_nInits,
                    pStats->Mutex.m_nSignals,
                    pStats->Mutex.m_nWaits,
                    pStats->Mutex.m_nTimedWaits,
                    pStats->Mutex.m_nUnblocks,
                    pStats->Mutex.m_nTimeOuts
                    );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void Mutex::vBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nBroadcasts++;
#endif
  
  ++ m_ixBroadcast;

  // mutex does not have to be locked (double-check this)
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_cond_broadcast(&m_xCond);                 // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success
  if (0 == iRetVal)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred

  ++ gixMutex_err;
}

/*----------------------------------------------------------------------------*/
/* test if a mutex is locked                                                  */
/*----------------------------------------------------------------------------*/
bool Mutex::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  // m_bLocked is indeterminate

  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_mutex_trylock(&m_xMutex);                 // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success: busy -- mutex was already locked, not locked by caller
  if (EBUSY == iRetVal)
    return true;

  // success: was not busy -- mutex was not already locked, now locked by caller
  if (0 == iRetVal) {
    vUnlock(nullptr);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred

  ++ gixMutex_err;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test if a mutex is locked                                                  */
/*----------------------------------------------------------------------------*/
bool Mutex::bIsLockedByCaller(Focal* pLockedby)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  if (! bIsLocked())
    return false;
  
  if (pLockedby != m_pLockedBy)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to lock this mutex by calling thread                               */
/*----------------------------------------------------------------------------*/
bool Mutex::bLock(Focal* pLockFor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! The calling thread cannot already have the mutex
  //!
  // some way to test this???
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // DO NOT TOUCH m_pLockedBy
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_mutex_trylock(&m_xMutex);                 // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  
  // success: locked -- mutex is now locked by calling thread
  
  if (0 == iRetVal) {
    RASSERT(! m_pLockedBy);
    m_pLockedBy = pLockFor;

    ++ m_ixLock;
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // success: busy -- mutex has not been locked by calling thread
  
  if (EBUSY == iRetVal) {
    ++ m_ixBusy;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred
  
  ++ gixMutex_err;
  return false;
}

/*----------------------------------------------------------------------------*/
/* lock this mutex for given focal object                          (blocking) */
/*----------------------------------------------------------------------------*/
void Mutex::vLock(Focal* pLockFor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! The calling thread cannot already have the mutex
  //!
  
  // some way to test this???
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ++ m_ixLock;
  
  // DO NOT TOUCH m_pLockedBy
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_mutex_lock(&m_xMutex);                        // BLOCKING
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  
  // success -- mutex is now locked by calling thread
  
  if (0 == iRetVal) {
    RASSERT(! m_pLockedBy);
    m_pLockedBy = pLockFor;
    
    ++ m_ixLocked;
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred
  
  ++ gixMutex_err;
}

/*----------------------------------------------------------------------------*/
/* get observe                                                                */
/*----------------------------------------------------------------------------*/
bool Mutex::bObserve(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  return m_bObserve;
}

/*----------------------------------------------------------------------------*/
/* set observe                                                                */
/*----------------------------------------------------------------------------*/
void Mutex::vObserve(bool bObserve)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  m_bObserve = bObserve;
}

/*----------------------------------------------------------------------------*/
/* calling thread pauses for limited time to be signalled     (blocking call) */
/*----------------------------------------------------------------------------*/
void Mutex::vPause(Focal* pLockedBy, i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! This mutex must be locked by the calling thread.  While waiting the
  //! mutex is dropped. The mutex is reacquired by calling thread when the
  //! mutex is signaled.
  //!
  ASSERT(pLockedBy == m_pLockedBy);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //! #001 this code should be in a common library
  // get current secs.usec since unix epoch
  Dtm wIvl;
  
  wIvl.m_wIvl = IvlLib::wGetCurrIvl();
  
  // add the wait for period
  wIvl = IvlLib::wAdd(wIvl.m_wIvl,iMicroSeconds);
  
  // create a absolute time spec
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = wIvl.m_wIvl.m_uSecond;
  xTimeSpec.tv_nsec = wIvl.m_wIvl.m_uMicro * C_1K;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nTimedWaits++;
#endif
  
  ++ m_ixWaitTimed;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // clear tracking of the mutex lock state
  m_pLockedBy = nullptr;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);// BLOCKING
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  
  // success: signaled -- this thread has been signaled and now has mutex lock
  
  if (0 == iRetVal) {
    RASSERT(! m_pLockedBy);
    m_pLockedBy = pLockedBy;
    
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Mutex.m_nUnblocks++;
#endif
    
    ++ m_ixSignaled;
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // success: timedout -- call has timed-out; thread has reacquired mutex lock
  
  if (ETIMEDOUT == iRetVal) {
    RASSERT(! m_pLockedBy);
    m_pLockedBy = pLockedBy;

#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Mutex.m_nTimeOuts++;
#endif
    
    ++ m_ixTimedout;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred
  
  ++ gixMutex_err;
}

/*----------------------------------------------------------------------------*/
/* signal the condition variable of this mutex                                */
/*----------------------------------------------------------------------------*/
void Mutex::vSignal(Focal* pLockedBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  if (m_bObserve) {
    vDebugPoint();
  }
  
  //!
  //! Calling thread must have locked the mutex
  //!
  ASSERT(pLockedBy == m_pLockedBy);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nSignals++;
#endif
  
  ++ m_ixSignal;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_cond_signal(&m_xCond);                    // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success -- mutex signaled; blocked thread will resume when lock is dropped
  
  //! this mutex needs to be unlocked by caller so waiting thread can proceed
  
  if (0 == iRetVal) {
    ASSERT(pLockedBy == m_pLockedBy);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred

  ++ gixMutex_err;

}

/*----------------------------------------------------------------------------*/
/* unlock this mutex currently locked by given focal object                   */
/*----------------------------------------------------------------------------*/
void Mutex::vUnlock(Focal* pLockedBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! The mutex must be locked by the calling thread
  //!
  RASSERT(pLockedBy == m_pLockedBy);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // clear tracking of the mutex lock state
  m_pLockedBy = nullptr;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_mutex_unlock(&m_xMutex);                  // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  
  // DO NOT TOUCH m_pLockedBy
  
  // success -- mutex has been unlocked by calling thread
  
  if (0 == iRetVal) {
    ++ m_ixUnlock;
        
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred
  
  ++ gixMutex_err;
}

/*----------------------------------------------------------------------------*/
/* calling thread waits indefinitely to be signaled           (blocking call) */
/*----------------------------------------------------------------------------*/
void Mutex::vWait(Focal* pLockedBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! Calling thread must have locked the mutex.  Mutex will be unlocked and the
  //! thread will block atomically, waiting on the cond varable.
  //!
  //! When signalled the thread will automatically reaquire the mutex lock.
  //! The mutex must be explicitly unlocked by the caller.
  //!
  ASSERT(pLockedBy == m_pLockedBy);
  ASSERT(! m_pWaitedBy);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nWaits++;
#endif
  
  ++ m_ixWaitIndef;

  // clear tracking of the mutex lock state
  m_pWaitedBy = m_pLockedBy;
  m_pLockedBy = nullptr;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 iRetVal = pthread_cond_wait(&m_xCond,&m_xMutex);                // BLOCKING
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nUnblocks++;
#endif
  
  // success -- this thread has been signaled and now has the mutex lock

  if (0 == iRetVal) {
    m_pLockedBy = m_pWaitedBy;
    m_pWaitedBy = nullptr;

    ++ m_ixSignaled;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- some error has occurred

  ++ gixMutex_err;
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Mutex::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Mutex)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

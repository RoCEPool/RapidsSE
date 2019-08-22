/*============================================================================*/
/* Mutex.cpp -- Pthread Mutex (& cond variable)                               */
/*============================================================================*/

#include "Resources.h"
#include "AM.hpp"

Atomic gixMutex;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Mutex* Resources::_pMutex(void* aAddr)
{
  Mutex* pMutex = reinterpret_cast<Mutex*>(aAddr);
  
  RASSERT_TYPE(pMutex,CN_MUTEX);
  
  return pMutex;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Mutex::operator new(size_t)
{
  void* pMutex = HeapMem::pAllocate(CN_MUTEX);
  
  return pMutex;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Mutex::operator new(size_t, Mutex* pMutex)
{
  return pMutex;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Mutex::operator delete(void* pMutexAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pMutexAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Mutex::Mutex()
: Resource(CN_MUTEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  m_bLocked = false;

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
  
  if (AM::bAttached()) AM::pResourceTracker()->vIncMutexOpen();
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nInits++;
#endif
  
  _checkInitialised_(CN_MUTEX);
  
  ++ gixMutex;
  AM::vLogRes(this,RA_MTX_CONSTRUCTOR,
              "Mutex constructed : %p (%lld mutexes exist)"_Stz,this,* gixMutex);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Mutex::~Mutex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  i8 iResult;
  
  iResult = pthread_mutex_destroy(&m_xMutex);
  RASSERT(! iResult);
  iResult = pthread_mutexattr_destroy(&m_xMutexAttr);
  RASSERT(! iResult);

  iResult = pthread_cond_destroy(&m_xCond);
  RASSERT(! iResult);
  iResult = pthread_condattr_destroy(&m_xCondAttr);
  RASSERT(! iResult);

  if (AM::bAttached()) AM::pResourceTracker()->vDecMutexOpen();
    
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nDestroys++;
#endif
  
  -- gixMutex;
  AM::vLogRes(this,RA_MTX_DESTRUCTOR,
              "Mutex destructed : %p (%lld mutexes exist)"_Stz,this,* gixMutex);
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
void Mutex::vCondBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nBroadcasts++;
#endif
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_cond_broadcast(&m_xCond);                   // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success
  if (0 == rCode)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure
  
  vWarn("pthread_cond_broadcast() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* signal the condition variable of this mutex                                */
/*----------------------------------------------------------------------------*/
void Mutex::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! Calling thread must have the mutex
  //!
  ASSERT(m_bLocked);
  ASSERT(bMutexIsLocked());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nSignals++;
#endif
  
  AM::vLogRes(this,RA_MTX_SIGNAL,
              "Mutex signaled : %p"_Stz,this);
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_cond_signal(&m_xCond);                      // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success -- condition variable of this mutex has been signaled
  
  if (0 == rCode) {
    ASSERT(m_bLocked);
    ASSERT(bMutexIsLocked());
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure

  vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* calling thread waits to be signalled (blocking call)                       */
/*----------------------------------------------------------------------------*/
void Mutex::vCondWaitIndefinite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! Calling thread must have the mutex.  Mutex will be unlocked and the
  //! thread will block atomically, waiting on the cond varable.
  //!
  //! When signalled the thread will automatically reaquire the mutex lock.
  //! The mutex must be explicitly unlocked by the caller.
  //!
  ASSERT(m_bLocked);
  ASSERT(bMutexIsLocked());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nWaits++;
#endif
  
  AM::vLogRes(this,RA_MTX_WAIT_INDEFINITE,
              "Mutex wait : %p"_Stz,this);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // clear tracking of the mutex lock state
  m_bLocked = false;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);                  // blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nUnblocks++;
#endif
  
  AM::vLogRes(this,RA_NONE,
              "Mutex unblocked : %p"_Stz,this);

  // success

  if (0 == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* calling thread waits to be signalled (time limited blocking call)          */
/*----------------------------------------------------------------------------*/
void Mutex::vCondWaitUsec(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! This mutex must be locked by the calling thread.  While waiting the
  //! mutex is dropped
  //!
  RASSERT(m_bLocked);
  RASSERT(bMutexIsLocked());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get current secs.usec since unix epoch
  Dtm wIvl = IvlLib::wGetCurrIvl();
  
  // add the wait for period
  wIvl = IvlLib::wAdd(wIvl,iMicroSeconds);
  
  // create a absolute time spec
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = wIvl.xIvl.m_uSecond;
  xTimeSpec.tv_nsec = wIvl.xIvl.m_uMicro * C_1K;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Mutex.m_nTimedWaits++;
#endif
  
  AM::vLogRes(this,RA_MTX_WAIT_TIMED,
              "Mutex timedwait : %p %lldus"_Stz,this,iMicroSeconds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // clear tracking of the mutex lock state
  m_bLocked = false;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);  // blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Mutex.m_nUnblocks++;
#endif
    
    AM::vLogRes(this,RA_MTX_SIGNALED,
                "Mutex timed unblock : %p %lldus"_Stz,this,iMicroSeconds);
    m_bLocked = true;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timeout
  
  if (ETIMEDOUT == rCode) {
    RASSERT(! m_bLocked);
    
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Mutex.m_nTimeOuts++;
#endif
    
   AM::vLogRes(this,RA_MTX_TIMEOUT,
               "Mutex timeout : %p %lldus"_Stz,this,iMicroSeconds);
   m_bLocked = true;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* test if a mutex is locked (non-blocking)                                   */
/*----------------------------------------------------------------------------*/
bool Mutex::bMutexIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_mutex_trylock(&m_xMutex);                   // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  
  // busy -- mutex was already locked
  if (EBUSY == rCode)
    return true;
  
  // not busy -- mutex was not already locked
  if (0 == rCode)
    vMutexUnlock();
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* try and lock this mutex by calling thread (non-blocking)                   */
/*----------------------------------------------------------------------------*/
bool Mutex::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! The calling thread cannot already have the mutex
  //!
  // DO NOT TOUCH m_bLocked
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_mutex_trylock(&m_xMutex);                   // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success -- mutex is now locked by calling thread
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    AM::vLogRes(this,RA_MTX_LOCKED,
                "Mutex locked : %p "_Stz,this);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // busy -- mutex has not been locked by calling thread
  
  if (EBUSY == rCode)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- mutex has not been locked by calling thread
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* lock this mutex by calling thread (blocking)                               */
/*----------------------------------------------------------------------------*/
void Mutex::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! The calling thread cannot already have the mutex
  //!
  // DO NOT TOUCH m_bLocked
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_mutex_lock(&m_xMutex);                     // blocking call
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // success -- mutex is now locked by calling thread
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    AM::vLogRes(this,RA_MTX_UNLOCKED,
                "Mutex unlocked : %p "_Stz,this);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- mutex has not been locked by calling thread
  
  vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* unlock this mutex currently locked by calling thread (non-blocking)        */
/*----------------------------------------------------------------------------*/
void Mutex::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //!
  //! The mutex must be locked by the calling thread
  //!
  RASSERT(m_bLocked);
  RASSERT(bMutexIsLocked());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // clear tracking of the mutex lock state
  m_bLocked = false;
  
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  i8 rCode = pthread_mutex_unlock(&m_xMutex);                    // non-blocking
  //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  // DO NOT TOUCH m_bLocked

  // success -- mutex has been unlocked by calling thread
  
  if (0 == rCode)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // failure -- mutex has not been unlocked by calling thread
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Mutex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

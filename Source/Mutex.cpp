/*============================================================================*/
/* Mutex.hpp -- Pthread Mutex (& cond variable                                */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern pthread_key_t giTLS_ThreadKey;

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Mutex* System::_pMutex(void* aAddr)
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
: Object(CN_MUTEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  m_bLocked = false;

  // initialise attribute to mutex
  MemLib::vZero(&m_xMutexAttr,sizeof(m_xMutexAttr),CN_MUTEX);
  i8 iResult = pthread_mutexattr_init(&m_xMutexAttr);
  ASSERT( ! iResult);
  
  // set attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);

  // initialize the mutex
  MemLib::vZero(&m_xMutex,sizeof(m_xMutex),CN_MUTEX);
  pthread_mutex_init(&m_xMutex, &m_xMutexAttr);

  // initialise attribute to condition
  MemLib::vZero(&m_xCondAttr,sizeof(m_xCondAttr),CN_MUTEX);
  pthread_condattr_init(&m_xCondAttr);
  
  // set attribute to be shared
  pthread_condattr_setpshared(&m_xCondAttr, PTHREAD_PROCESS_SHARED);
  
  // initialize the mutex
  MemLib::vZero(&m_xCond,sizeof(m_xCond),CN_MUTEX);
  pthread_cond_init(&m_xCond, &m_xCondAttr);
  
  _checkInitialised_(CN_MUTEX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Mutex::~Mutex(void) noexcept(false)
{
  pthread_mutex_destroy(&m_xMutex);
  pthread_mutexattr_destroy(&m_xMutexAttr);
  
  pthread_cond_destroy(&m_xCond);
  pthread_condattr_destroy(&m_xCondAttr);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get monotor portrait of semaphore activity                                 */
/*----------------------------------------------------------------------------*/
void Mutex::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  sPortrait.vSprint("Mutex: broadcast %lld signals %lld "
                                "waits %lld timedwaits %lld "
                                "unblocks %lld timeouts %lld\n"_Stz,
                    pStats->Mutex.m_nBroadcasts,
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
  
  i8 rCode = pthread_cond_broadcast(&m_xCond);
  
  gpAttachMap->m_oStatistics.Mutex.m_nBroadcasts++;
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_broadcast() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void Mutex::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //
  // **calling thread must have the mutex**
  //
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  ASSERT(bMutexIsLocked());
  
  i8 rCode = pthread_cond_signal(&m_xCond);
  
  gpAttachMap->m_oStatistics.Mutex.m_nSignals++;

  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (blocking call)                               */
/*----------------------------------------------------------------------------*/
void Mutex::vCondWait_Mutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  //
  // Calling thread must have the mutex.  Mutex will be unlocked and the
  // thread will block atomically, waiting on the cond varable.
  //
  // When signalled the thread will automatically reaquire the mutex lock.
  // The mutex must be explicitly unlocked by the caller.
  //
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  ASSERT(bMutexIsLocked());
  
  // clear this threads tracking of the mutex
  m_bLocked = false;
  
  gpAttachMap->m_oStatistics.Mutex.m_nWaits++;
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
  gpAttachMap->m_oStatistics.Mutex.m_nUnblocks++;

  if (0 == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (time limited blocking call)                  */
/*----------------------------------------------------------------------------*/
void Mutex::vCondTimedWait(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  RASSERT(bMutexIsLocked());

  Since oSince;
  
  oSince.vNow();
  
  oSince.vAdd(iMicroSeconds);
  
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = oSince.m_iSeconds;
  xTimeSpec.tv_nsec = oSince.m_iMicroseconds * 1000;
  
  // clear this threads tracking of the mutex
  m_bLocked = false;
  
  gpAttachMap->m_oStatistics.Mutex.m_nTimedWaits++;
  
  // this is a time limited blocking call
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    gpAttachMap->m_oStatistics.Mutex.m_nUnblocks++;
    m_bLocked = true;
    return;
  }
  if (ETIMEDOUT == rCode) {
    RASSERT(! m_bLocked);
    gpAttachMap->m_oStatistics.Mutex.m_nTimeOuts++;
   m_bLocked = true;
    return;
  }

  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* test if a threads mutex is locked (non-blocking)                           */
/*----------------------------------------------------------------------------*/
bool Mutex::bMutexIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  // this is a non-blocking call
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (EBUSY == rCode)
    return true;
  
  if (0 == rCode)
    vMutexUnlock();
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool Mutex::boLockMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  // the calling thread cannot already have the mutex
  
  // DO NOT TOUCH m_bLocked
  
  // this is a non-blocking call
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    return true;
  }
  
  if (EBUSY == rCode)
    return false;
  
  vWarn("pthread_mutex_trylock() error; rCode=%lld\n"_Stz,rCode);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void Mutex::voLockMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  
  // the calling thread cannot already have the mutex
  
  // DO NOT TOUCH m_bLocked
  
  // this is a blocking call
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  //vTrace("oLockMutexLock %p\n"_Stz,this);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex (non-blocking)                                      */
/*----------------------------------------------------------------------------*/
void Mutex::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  RASSERT(bMutexIsLocked());
  
  //vTrace("MutexUnlock %p\n"_Stz,this);
  
  // clear this threads tracking of the mutex
  m_bLocked = false;
  
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  
  // DO NOT TOUCH m_bLocked
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void Mutex::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  ASSERT(bMutexIsLocked());
  
  vCondSignal();
  vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* signal a thread (non-locking; should not be calling thread)                */
/*----------------------------------------------------------------------------*/
void Mutex::vSignalThread_NL(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTEX,_FOOTER_);
  
  vCondSignal();
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

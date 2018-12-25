/*============================================================================*/
/* Semaphore.cpp - a simple semaphore                                         */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Semaphore* System::_pSemaphore(void* aAddr)
{
  Semaphore* pSemaphore = reinterpret_cast<Semaphore*>(aAddr);
  
  RASSERT_TYPE(pSemaphore,CN_SEMAPHORE);
  
  return pSemaphore;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Semaphore::operator new(size_t)
{
  void* pSemaphore = HeapMem::pAllocate(CN_SEMAPHORE);
  
  return pSemaphore;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Semaphore::operator new(size_t, Semaphore* pSemaphore)
{
  return pSemaphore;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Semaphore::operator delete(void* pSemaphoreAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pSemaphoreAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Semaphore::Semaphore()
: Object(CN_SEMAPHORE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  m_bLocked = false;

  // initialise attribute to mutex
  MemLib::vZero(&m_xMutexAttr,sizeof(m_xMutexAttr),CN_SEMAPHORE);
  i8 iResult = pthread_mutexattr_init(&m_xMutexAttr);
  ASSERT( ! iResult);
  
  // set attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);

  // initialize the mutex
  MemLib::vZero(&m_xMutex,sizeof(m_xMutex),CN_SEMAPHORE);
  pthread_mutex_init(&m_xMutex, &m_xMutexAttr);

  // initialise attribute to condition
  MemLib::vZero(&m_xCondAttr,sizeof(m_xCondAttr),CN_SEMAPHORE);
  pthread_condattr_init(&m_xCondAttr);
  
  // set attribute to be shared
  pthread_condattr_setpshared(&m_xCondAttr, PTHREAD_PROCESS_SHARED);
  
  // initialize the mutex
  MemLib::vZero(&m_xCond,sizeof(m_xCond),CN_SEMAPHORE);
  pthread_cond_init(&m_xCond, &m_xCondAttr);
  
  _checkInitialised_(CN_SEMAPHORE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Semaphore::~Semaphore(void) noexcept(false)
{
  pthread_mutex_destroy(&m_xMutex);
  pthread_mutexattr_destroy(&m_xMutexAttr);
  
  pthread_cond_destroy(&m_xCond);
  pthread_condattr_destroy(&m_xCondAttr);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void Semaphore::vCondBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  i8 rCode = pthread_cond_broadcast(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_broadcast() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void Semaphore::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  //
  // **calling thread must have the mutex**
  //
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  ASSERT(bMutexIsLocked());
  
  i8 rCode = pthread_cond_signal(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (blocking call)                               */
/*----------------------------------------------------------------------------*/
void Semaphore::vCondWait_Mutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
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
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
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
void Semaphore::vCondTimedWait(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  RASSERT(bMutexIsLocked());

  Since oSince;
  
  oSince.vSet();
  
  oSince.vAdd(iMicroSeconds);
  
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = oSince.m_iSeconds;
  xTimeSpec.tv_nsec = oSince.m_iMicroseconds * 1000;
  
  // clear this threads tracking of the mutex
  m_bLocked = false;
  
  // this is a time limited blocking call
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);
  
  if (0 == rCode || ETIMEDOUT == rCode) {
    RASSERT(! m_bLocked);
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* test if a threads mutex is locked (non-blocking)                           */
/*----------------------------------------------------------------------------*/
bool Semaphore::bMutexIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
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
bool Semaphore::boLockMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
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
void Semaphore::voLockMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  
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
void Semaphore::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
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
void Semaphore::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  ASSERT(bMutexIsLocked());
  vCondSignal();
  vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* signal a thread (non-locking; should not be calling thread)                */
/*----------------------------------------------------------------------------*/
void Semaphore::vSignalThread_NL(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  vCondSignal();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Semaphore::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

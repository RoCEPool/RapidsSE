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
  
  RASSERT( !pSemaphore || CN_SEMAPHORE == pSemaphore->eClass() );
  
  return pSemaphore;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Semaphore::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pSemaphore = Heap::pAllocate(CN_SEMAPHORE);
  
  return pSemaphore;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Semaphore::operator new(size_t iSize, Semaphore* pSemaphore)
{
  RASSERT(iSize);
  return pSemaphore;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Semaphore::operator delete(void* pSemaphoreAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pSemaphoreAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Semaphore::Semaphore()
: Object(CN_SEMAPHORE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  m_bLocked = false;
  m_pLockingThread = nullptr;

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
Semaphore::~Semaphore(void)
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
//#define _OLD_RELEASE

#ifdef _OLD_RELEASE
void Semaphore::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  //
  // **calling thread must have the mutex**
  //
  
//  PosixThread* pCallingThread =
//  static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
//  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  //RASSERT(m_pLockingThread);
  //RASSERT(pCallingThread == m_pLockingThread);
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
  
//  PosixThread* pCallingThread =
//  static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
//  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  //RASSERT(m_pLockingThread);
  //RASSERT(pCallingThread == m_pLockingThread);
  ASSERT(bMutexIsLocked());
  
  // clear this threads tracking of the mutex
  //m_pLockingThread = nullptr;
  m_bLocked = false;
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    //RASSERT(! m_pLockingThread);
    //m_pLockingThread = pCallingThread;
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
  
//  PosixThread* pCallingThread =
//  static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
//  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  //RASSERT(m_pLockingThread);
  //RASSERT(pCallingThread == m_pLockingThread);
  RASSERT(bMutexIsLocked());
  
  Since xSince;
  
  xSince.vSet();
  
  xSince.vAdd(iMicroSeconds);
  
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = xSince.m_iSeconds;
  xTimeSpec.tv_nsec = xSince.m_iMicroseconds * 1000;
  
  // clear this threads tracking of the mutex
  //m_pLockingThread = nullptr;
  m_bLocked = false;
  
  // this is a time limited blocking call
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);
  
  if (0 == rCode || ETIMEDOUT == rCode) {
    RASSERT(! m_bLocked);
    //RASSERT(! m_pLockingThread);
    //m_pLockingThread = pCallingThread;
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
bool Semaphore::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
//  PosixThread* pCallingThread =
//  static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
//  RASSERT(pCallingThread);
  
  // the calling thread cannot already have the mutex
  //RASSERT(pCallingThread != m_pLockingThread);
  
  // DO NOT TOUCH m_bLocked or m_pLockingThread
  
  // this is a non-blocking call
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    //RASSERT(! m_pLockingThread);
    //m_pLockingThread = pCallingThread;
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
void Semaphore::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
//  PosixThread* pCallingThread =
//  static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
//  RASSERT(pCallingThread);
  
  // the calling thread cannot already have the mutex
  //RASSERT(pCallingThread != m_pLockingThread);
  
  // DO NOT TOUCH m_bLocked or m_pLockingThread
  
  // this is a blocking call
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  //vTrace("MutexLock %p\n"_Stz,this);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    //RASSERT(! m_pLockingThread);
    //m_pLockingThread = pCallingThread;
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
  
//  PosixThread* pCallingThread =
//  static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
//  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  //RASSERT(m_pLockingThread);
  //RASSERT(pCallingThread == m_pLockingThread);
  RASSERT(bMutexIsLocked());
  
  //vTrace("MutexUnlock %p\n"_Stz,this);
  
  // clear this threads tracking of the mutex
  //m_pLockingThread = nullptr;
  m_bLocked = false;
  
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  
  // DO NOT TOUCH m_bLocked or m_pLockingThread
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

#else
void Semaphore::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  //
  // **calling thread must have the mutex**
  //
  
  PosixThread* pCallingThread =
    static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  RASSERT(m_pLockingThread);
  RASSERT(pCallingThread == m_pLockingThread);
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
  
  PosixThread* pCallingThread =
    static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  ASSERT(m_bLocked);
  RASSERT(m_pLockingThread);
  RASSERT(pCallingThread == m_pLockingThread);
  ASSERT(bMutexIsLocked());
  
  // clear this threads tracking of the mutex
  m_pLockingThread = nullptr;
  m_bLocked = false;
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    RASSERT(! m_pLockingThread);
    m_pLockingThread = pCallingThread;
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
  
  PosixThread* pCallingThread =
    static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  RASSERT(m_pLockingThread);
  RASSERT(pCallingThread == m_pLockingThread);
  RASSERT(bMutexIsLocked());

  Since xSince;
  
  xSince.vSet();
  
  xSince.vAdd(iMicroSeconds);
  
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = xSince.m_iSeconds;
  xTimeSpec.tv_nsec = xSince.m_iMicroseconds * 1000;
  
  // clear this threads tracking of the mutex
  m_pLockingThread = nullptr;
  m_bLocked = false;
  
  // this is a time limited blocking call
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);
  
  if (0 == rCode || ETIMEDOUT == rCode) {
    RASSERT(! m_bLocked);
    RASSERT(! m_pLockingThread);
    m_pLockingThread = pCallingThread;
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
bool Semaphore::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  PosixThread* pCallingThread =
    static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  RASSERT(pCallingThread);
  
  // the calling thread cannot already have the mutex
  RASSERT(pCallingThread != m_pLockingThread);
  
  // DO NOT TOUCH m_bLocked or m_pLockingThread
  
  // this is a non-blocking call
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    RASSERT(! m_pLockingThread);
    m_pLockingThread = pCallingThread;
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
void Semaphore::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  PosixThread* pCallingThread =
    static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  RASSERT(pCallingThread);
  
  // the calling thread cannot already have the mutex
  RASSERT(pCallingThread != m_pLockingThread);
  
  // DO NOT TOUCH m_bLocked or m_pLockingThread
  
  // this is a blocking call
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  //vTrace("MutexLock %p\n"_Stz,this);
  
  if (0 == rCode) {
    RASSERT(! m_bLocked);
    RASSERT(! m_pLockingThread);
    m_pLockingThread = pCallingThread;
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
  
  PosixThread* pCallingThread =
    static_cast<PosixThread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  RASSERT(pCallingThread);
  
  // the mutex should be locked by the calling thread
  RASSERT(m_bLocked);
  RASSERT(m_pLockingThread);
  RASSERT(pCallingThread == m_pLockingThread);
  RASSERT(bMutexIsLocked());
  
  //vTrace("MutexUnlock %p\n"_Stz,this);
  
  // clear this threads tracking of the mutex
  m_pLockingThread = nullptr;
  m_bLocked = false;
  
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  
  // DO NOT TOUCH m_bLocked or m_pLockingThread
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}
#endif

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

/*============================================================================*/
/* Semaphore.cpp - a simple semaphore                                         */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

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
  
  m_bLocked        = false;

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
void Semaphore::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  //
  // **calling thread must have the mutex**
  //
  
  i8 rCode = pthread_cond_signal(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* condition wait (thread to wait will be calling thread)                     */
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
  
  ASSERT(m_bLocked);
  
  m_bLocked = false;
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (should be calling thread)                    */
/*----------------------------------------------------------------------------*/
void Semaphore::vCondTimedWait(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  Since xSince;
  
  xSince.vSet();
  
  xSince.vAdd(iMicroSeconds);
  
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = xSince.m_iSeconds;
  xTimeSpec.tv_nsec = xSince.m_iMicroseconds * 1000;
  
  ASSERT(m_bLocked);
  
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);
  
  if (0 == rCode || ETIMEDOUT == rCode)
    return;
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void Semaphore::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  // this is a blocking call
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  //vTrace("MutexLock %p\n"_Stz,this);
  
  if (0 == rCode) {
    ASSERT( ! m_bLocked );
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool Semaphore::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = true;
    return true;
  }
  
  if (EBUSY == rCode)
    return false;
  
  vWarn("pthread_mutex_trylock() error; rCode=%lld\n"_Stz,rCode);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex                                                     */
/*----------------------------------------------------------------------------*/
void Semaphore::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  //vTrace("MutexUnlock %p\n"_Stz,this);
  
  ASSERT( m_bLocked );
  m_bLocked = false;
  
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  
  if (0 == rCode) {
    //m_bLocked = false;
    return;
  }
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void Semaphore::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEMAPHORE,_FOOTER_);
  
  vMutexLock();
  vCondSignal();
  vMutexUnlock();
}

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

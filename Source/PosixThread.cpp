/*============================================================================*/
/* PosixThread - a posix thread                                               */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

Atomic gzThreadOpenCount;

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PosixThread* System::_pPosixThread(void* aAddr)
{
  PosixThread* pPosixThread = reinterpret_cast<PosixThread*>(aAddr);
  
  RASSERT( !pPosixThread || CN_POSIXTHREAD == pPosixThread->eClass() );
  
  return pPosixThread;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* PosixThread::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pPosixThread = Heap::pAllocate(CN_POSIXTHREAD);
  
  return pPosixThread;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void PosixThread::operator delete(void* pPosixThreadAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pPosixThreadAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PosixThread::PosixThread()
: Object(CN_POSIXTHREAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_iOpSysThreadId = 0;
  m_iPosixThreadId = 0;
  m_bLocked        = false;

  MemLib::vZero(&m_xMutex,sizeof(m_xMutex));
  pthread_mutex_init(&m_xMutex,NULL);

  MemLib::vZero(&m_xCond,sizeof(m_xCond));
  pthread_cond_init(&m_xCond,NULL);
  
  _checkInitialised_(CN_POSIXTHREAD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PosixThread::~PosixThread(void)
{
  pthread_mutex_destroy(&m_xMutex);
  pthread_cond_destroy(&m_xCond);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bind os thread to a cpu                                                    */
/*----------------------------------------------------------------------------*/
bool PosixThread::bBindThread(i8 iCpu)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  i8 iCpuCount = Hardware::iCpuCount();
  
  if (iCpu > iCpuCount) {
    vWarn("Unable to bind to Cpu %lld on a system having %lld cores!\n"_Str, iCpu, iCpuCount);
    return false;
  }
  
  #ifdef __APPLE__
  
    thread_port_t mach_thread;

    vWarn("binding to core %d\n"_Str, (int)iCpu);
  
    thread_affinity_policy_data_t policy = { (int)iCpu };
    mach_thread = pthread_mach_thread_np(m_iPosixThreadId);
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&policy, 1);
    return true;

  #else
  /*
    i8 iResult;
  
    cpu_set_t xAffinity;
  
    CPU_ZERO(&xAffinity);
    CPU_SET(iCpu, &xAffinity);
  
    ASSERT(pthread_self() == m_iPosixThreadId);
  
    iResult = pthread_setafffinity_np(m_iPosixThreadId, sizeof(cpu_set_t), &xAffinity);
    if (iResult == -1) {
      vWarn("Error in pthread_setafffinity_np() : %lld\n"_Str, errno);
      
    }
  */
    return true;
    #endif
}


/*----------------------------------------------------------------------------*/
/* get os thread id                                                           */
/*----------------------------------------------------------------------------*/
void PosixThread::vSetOSThreadId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  i8 iOpSysThreadId = syscall(SYS_thread_selfid);
#else
  i8 iOpSysThreadId = syscall(SYS_gettid);
#endif
  
  if (-1 == iOpSysThreadId)
    vWarn("syscall(SYS_gettid) error; errno=%d\n"_Str,errno);
  else
    m_iOpSysThreadId = iOpSysThreadId;
}
  
/*----------------------------------------------------------------------------*/
/* create thread, passing it function to execute                              */
/*----------------------------------------------------------------------------*/
bool PosixThread::bSpawnThread(void* pFun(void*), void* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  // we shouldnt have a thread yet
  ASSERT( ! m_iPosixThreadId );
  
  i8 rCode = pthread_create(&m_iPosixThreadId,
                             nullptr,
                             pFun,
                             pArg
                             );
  
  if (rCode) {
    vWarn("pthread_create() error; rCode=%lld\n"_Str,rCode);
    return false;
  }
  
  ++gzThreadOpenCount;
  
  // this is important otherwise resources are not released
  pthread_detach(m_iPosixThreadId);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void PosixThread::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  vMutexLock();
  vCondSignal();
  vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* get cpu of this thread                                                     */
/*----------------------------------------------------------------------------*/
i8 PosixThread::iThreadCpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  vWarn("Get thread Cpu affinity not implemented!\n"_Str);
  return 0;
  
#else
  /*
  i8 iResult;
  
  cpu_set_t xAffinity;
  
  CPU_ZERO(&xAffinity);
  
  ASSERT(pthread_self() == m_iPosixThreadId);
  
  iResult = pthread_getafffinity_np(m_iPosixThreadId, sizeof(cpu_set_t), &xAffinity);
  if (iResult == -1) {
    vWarn("Error in pthread_getafffinity_np() : %lld\n"_Str, errno);
  }
  
  i8 cpu_set_size = static_cast<i8>(sizeof(cpu_set_t));
  
  for (i8 i = 0; i < cpu_set_size; i++) {
    if (CPU_ISSET(i, &xAffinity)
      return i;
    }
   */
  return 0;
        
#endif
      
}
      
/*----------------------------------------------------------------------------*/
/* thread notifies its exiting                                                */
/*----------------------------------------------------------------------------*/
void PosixThread::vThreadExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  --gzThreadOpenCount;
}
        
/*----------------------------------------------------------------------------*/
/* get thread priority                                                        */
/*----------------------------------------------------------------------------*/
i8 PosixThread::iThreadPriority(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPosixThreadId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  return xSched.sched_priority;
  
#else
  //vWarn("Get thread priority not implemented!\n");
  return 0;
#endif

}
        
/*----------------------------------------------------------------------------*/
/* boost thread priority                                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vThreadBoost(i8 nPoints)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPosixThreadId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  xSched.sched_priority += (int)nPoints;
  iError = pthread_setschedparam(m_iPosixThreadId,iPolicy,&xSched);
  ASSERT(! iError);
  
#else
  ASSERT(nPoints >= 0);
  //vWarn("Boost thread priority not implemented!\n");
#endif

}
        
/*----------------------------------------------------------------------------*/
/* set thread priority                                                        */
/*----------------------------------------------------------------------------*/
void PosixThread::vThreadPriority(i8 iPriority)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPosixThreadId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  xSched.sched_priority = (int)iPriority;
  iError = pthread_setschedparam(m_iPosixThreadId,iPolicy,&xSched);
  ASSERT(! iError);
  
#else
  ASSERT(iPriority >= 0);
  //vWarn("Set thread priority not implemented!\n");
#endif

}
        
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  i8 rCode = pthread_cond_broadcast(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_broadcast() error; rCode=%lld\n"_Str,rCode);
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  i8 rCode = pthread_cond_signal(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_signal() error; rCode=%lld\n"_Str,rCode);
}

/*----------------------------------------------------------------------------*/
/* condition wait (thread to wait will be calling thread)                     */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  //
  // Calling thread must have the mutex.  Mutex will be unlocked and the
  // thread will block atomically, waiting on the cond varable.
  //
  // When signalled the thread will automatically reaquire the mutex lock.
  // The mutex must be explicitly unlocked by the caller.
  //
  
  ASSERT(m_bLocked);
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Str,rCode);
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (should be calling thread)                    */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondTimedWait(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
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
  
  vWarn("pthread_cond_wait() error; rCode=%lld\n"_Str,rCode);
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void PosixThread::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Str,rCode);
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool PosixThread::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = true;
    return true;
  }
  
  if (EBUSY == rCode)
    return false;
  
  vWarn("pthread_mutex_trylock() error; rCode=%lld\n"_Str,rCode);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex                                                     */
/*----------------------------------------------------------------------------*/
void PosixThread::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = false;
    return;
  }
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Str,rCode);
}

/*----------------------------------------------------------------------------*/
/* set value in tls slot                                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vTlsSet(pthread_key_t iTlsKey, void* pArg)
{
  pthread_setspecific(iTlsKey,pArg);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sleep for given number of microseconds                                     */
/*----------------------------------------------------------------------------*/
void PosixThread::vSleepMicro(i8 nMicroSeconds)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nMicroSeconds >= C_1M) {
    xReq.tv_nsec = (nMicroSeconds % C_1M) * C_1K;
    xReq.tv_sec  = DIV(nMicroSeconds,C_1M);
  }
  else {
    xReq.tv_nsec = nMicroSeconds * C_1K;
    xReq.tv_sec  = 0;
  }
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PosixThread::vSleepMicro error; errno=%lld\n"_Str,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of milliseconds                                     */
/*----------------------------------------------------------------------------*/
void PosixThread::vSleepMilli(i8 nMilliSeconds)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nMilliSeconds >= C_1K) {
    xReq.tv_nsec = (nMilliSeconds % C_1K) * C_1M;
    xReq.tv_sec  = DIV(nMilliSeconds,C_1K);
  }
  else {
    xReq.tv_nsec = nMilliSeconds * C_1M;
    xReq.tv_sec  = 0;
  }
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PosixThread::vSleepMilli error; errno=%lld\n"_Str,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of nanoseconds                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vSleepNano(i8 nNanoSeconds)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nNanoSeconds >= C_1B) {
    xReq.tv_nsec = nNanoSeconds % C_1B;
    xReq.tv_sec  = DIV(nNanoSeconds,C_1B);
  }
  else {
    xReq.tv_nsec = nNanoSeconds;
    xReq.tv_sec  = 0;
  }
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PosixThread::vSleepNano error; errno=%lld\n"_Str,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of seconds                                          */
/*----------------------------------------------------------------------------*/
void PosixThread::vSleepSec(i8 nSeconds)
{
  sleep((unsigned)nSeconds);
}

/*----------------------------------------------------------------------------*/
/* create a tls slot                                                          */
/*----------------------------------------------------------------------------*/
bool PosixThread::bTlsCreate(pthread_key_t* pTlsKey)
{
  i8 iResult = pthread_key_create(pTlsKey,nullptr);
  
  if (0 == iResult)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get value from tls slot                                                    */
/*----------------------------------------------------------------------------*/
void* PosixThread::pTlsGet(pthread_key_t iTlsKey)
{
  void* pVal = pthread_getspecific(iTlsKey);
  
  return pVal;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PosixThread::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* PosixThread - a posix thread                                               */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PosixThread::PosixThread()
{
  m_iOpSysThreadId = 0;
  m_iPosixThreadId = 0;
  m_bLocked        = false;

  pthread_mutex_init(&m_xMutex,NULL);
  pthread_cond_init(&m_xCond,NULL);
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
/* bind os thread to a cpu                                                    */
/*----------------------------------------------------------------------------*/
bool PosixThread::bBindThread(i8 iCpu)
{
  
  i8 iCpuCount = System::iCpuCount();
  
  #ifdef __APPLE__
  
    thread_port_t mach_thread;
  
    if (iCpu > iCpuCount) {
        vWarn("Unable to bind to Cpu %lld on a system having %lld cores!", iCpu, iCpuCount);
      return false;
    }
    printf("binding to core %d\n", (int)iCpu);
  
    thread_affinity_policy_data_t policy = { (int)iCpu };
    mach_thread = pthread_mach_thread_np(m_iPosixThreadId);
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&policy, 1);
      return true;

    #else
      vWarn("Cpu Binding not implemented on LINUX!");
      return true;
    #endif
}

/*----------------------------------------------------------------------------*/
/* get cpu this of this thread                                                */
/*----------------------------------------------------------------------------*/
i8 PosixThread::iThreadCpu(void)
{
  vWarn("Getting thread Cpu affinity not implemented!");
  return 0;
}


/*----------------------------------------------------------------------------*/
/* get os thread id                                                           */
/*----------------------------------------------------------------------------*/
void PosixThread::vSetOSThreadId(void)
{
#ifdef __APPLE__
  i8 iOpSysThreadId = syscall(SYS_thread_selfid);
#else
  i8 iOpSysThreadId = syscall(SYS_gettid);
#endif
  
  if (-1 == iOpSysThreadId)
    vWarn("syscall(SYS_gettid) error; errno=%d",errno);
  else
    m_iOpSysThreadId = iOpSysThreadId;
}
  
/*----------------------------------------------------------------------------*/
/* create thread, passing it function to execute                              */
/*----------------------------------------------------------------------------*/
bool PosixThread::bSpawnThread(void* pFun(void*), void* pArg)
{
  // we shouldnt have a thread yet
  ASSERT( ! m_iPosixThreadId );
  
  i8 rCode = pthread_create(&m_iPosixThreadId,
                             nullptr,
                             pFun,
                             pArg
                             );
  
  if (rCode) {
    vWarn("pthread_create() error; rCode=%lld",rCode);
    return false;
  }
  
  // this is important otherwise resources are not released
  pthread_detach(m_iPosixThreadId);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void PosixThread::vSignalThread(void)
{
  vMutexLock();
  vCondSignal();
  vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondBroadcast(void)
{
  i8 rCode = pthread_cond_broadcast(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_broadcast() error; rCode=%lld",rCode);
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondSignal(void)
{
  i8 rCode = pthread_cond_signal(&m_xCond);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_signal() error; rCode=%lld",rCode);
}

/*----------------------------------------------------------------------------*/
/* condition wait (thread to wait will be calling thread)                     */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondWait(void)
{
  ASSERT(m_bLocked);
  
  i8 rCode = pthread_cond_wait(&m_xCond,&m_xMutex);
  
  if (0 == rCode)
    return;
  
  vWarn("pthread_cond_wait() error; rCode=%lld",rCode);
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (should be calling thread)                    */
/*----------------------------------------------------------------------------*/
void PosixThread::vCondTimedWait(i8 iNanoSeconds)
{
  Clock xClock;
  
  xClock.vAdd(iNanoSeconds);
  
  struct timespec xTimeSpec;
  xTimeSpec.tv_sec  = xClock.m_iSeconds;
  xTimeSpec.tv_nsec = xClock.m_iMicro * 1000;
  
  ASSERT(m_bLocked);
  
  i8 rCode = pthread_cond_timedwait(&m_xCond,&m_xMutex,&xTimeSpec);
  
  if (0 == rCode || ETIMEDOUT == rCode)
    return;
  
  vWarn("pthread_cond_wait() error; rCode=%lld",rCode);
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void PosixThread::vMutexLock(void)
{
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = true;
    return;
  }
  
  vWarn("pthread_mutex_lock() error; rCode=%lld",rCode);
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool PosixThread::bMutexLock(void)
{
  i8 rCode = pthread_mutex_trylock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = true;
    return true;
  }
  
  if (EBUSY == rCode)
    return false;
  
  vWarn("pthread_mutex_trylock() error; rCode=%lld",rCode);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex                                                     */
/*----------------------------------------------------------------------------*/
void PosixThread::vMutexUnlock(void)
{
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  
  if (0 == rCode) {
    m_bLocked = false;
    return;
  }
  
  vWarn("pthread_mutex_unlock() error; rCode=%lld",rCode);
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
/* set value in tls slot                                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vTlsSet(pthread_key_t iTlsKey, void* pArg)
{
  pthread_setspecific(iTlsKey,pArg);
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

/*----------------------------------------------------------------------------*/
/* sleep for given number of nanoseconds                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vNanoSleep(i8 nNanoSeconds)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nNanoSeconds >= C_1Bn) {
    xReq.tv_nsec = nNanoSeconds % C_1Bn;
    xReq.tv_sec  = DIV(nNanoSeconds,C_1Bn);
  }
  else {
    xReq.tv_nsec = nNanoSeconds;
    xReq.tv_sec  = 0;
  }
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("nanosleep() error; errno=%lld",errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of seconds                                          */
/*----------------------------------------------------------------------------*/
void PosixThread::vSecSleep(i8 nSeconds)
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
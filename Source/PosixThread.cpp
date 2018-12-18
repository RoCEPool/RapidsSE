/*============================================================================*/
/* PosixThread - a posix thread                                               */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

#ifdef __APPLE__
  #include <mach/mach_time.h>
  #include <mach/thread_policy.h>
#endif

extern ResourceTracker gxResourceTracker;

extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PosixThread* System::_pPosixThread(void* aAddr)
{
  PosixThread* pPosixThread = reinterpret_cast<PosixThread*>(aAddr);
  
  RASSERT_TYPE(pPosixThread,CN_POSIXTHREAD);
  
  return pPosixThread;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* PosixThread::operator new(size_t)
{
  void* pPosixThread = HeapMem::pAllocate(CN_POSIXTHREAD);
  
  return pPosixThread;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void PosixThread::operator delete(void* pPosixThreadAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pPosixThreadAddress);
  
  HeapMem::vFree(pObject);
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
  
  _checkInitialised_(CN_POSIXTHREAD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PosixThread::~PosixThread(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bind thread to a core                                                      */
/*----------------------------------------------------------------------------*/
bool PosixThread::bBindThread(i8 i_Core)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
//
// refer: https://developer.apple.com/library/archive/releasenotes/Performance/RN-AffinityAPI/
//
  
//  i8 iCoreCount = Hardware::iCoreCount();
//  if (i_Core > iCoreCount) {
//    vWarn("Unable to bind to core %lld on a system having %lld cores!\n"_Stz,
//          i_Core, iCoreCount);
//    return false;
//  }
  
#ifdef __APPLE__
  int iReturn = 0;
  thread_affinity_policy_data_t xPolicy = { (int)i_Core };
  thread_port_t uMachThread = pthread_mach_thread_np(m_iPosixThreadId);

  vTrace("binding thread with posix id %d to core %d\n"_Stz,
         m_iPosixThreadId, (int)i_Core);

//  mach_msg_type_number_t policy_infoCnt = 1;
//  boolean_t get_default;
//  
//  iReturn =
//    thread_policy_get(uMachThread,
//                      THREAD_AFFINITY_POLICY,
//                      (thread_policy_t)&xPolicy,
//                      &policy_infoCnt,
//                      &get_default
//                      );

  iReturn =
    thread_policy_set(uMachThread,
                      THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&xPolicy,
                      1 //mach_msg_type_number_t
                      );
  
  RASSERT(iReturn == 0);
  return true;

#else

//  i8 iResult;
//
//  cpu_set_t xAffinity;
//
//  CPU_ZERO(&xAffinity);
//  CPU_SET(iCpu, &xAffinity);
//
//  ASSERT(pthread_self() == m_iPosixThreadId);
//
//  iResult = pthread_setafffinity_np(m_iPosixThreadId,
//                                    sizeof(cpu_set_t), &xAffinity);
//  if (iResult == -1) {
//    vWarn("Error in pthread_setafffinity_np() : %lld\n"_Stz, errno);
//  }

  return true;
#endif
}

/*----------------------------------------------------------------------------*/
/* set standard scheduling policy for thread                                  */
/*----------------------------------------------------------------------------*/
void PosixThread::vDisableRealtime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());

  struct thread_standard_policy xStandardPolicy;

  i8 iReturn = thread_policy_set(uThreadPort,
                                 THREAD_STANDARD_POLICY,
                                 (thread_policy_t)&xStandardPolicy,
                                 THREAD_STANDARD_POLICY_COUNT);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("PosixThread::vDisableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }

#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* set realtime scheduling policy for thread                                  */
/*----------------------------------------------------------------------------*/
void PosixThread::vEnableRealtime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iSchedulerPeriod_us  = OpSys::iSchedulerPeriod_us();
  i8 iKernelTicksPerSec   = C_1M / iSchedulerPeriod_us;

  i8 iOverTheNextPeriod     = iKernelTicksPerSec * 10; //8000;    // 60s
  i8 iThreadRequiresSoLong  = iOverTheNextPeriod / 1;     // 30s
  i8 iMustCompleteIn        = iThreadRequiresSoLong * 2;     // 30s
  
  bool bPreemptible = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  struct thread_time_constraint_policy xRealtimePolicy;
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());
  
  xRealtimePolicy.period      = (int)iOverTheNextPeriod;
  xRealtimePolicy.computation = (int)iThreadRequiresSoLong;
  xRealtimePolicy.constraint  = (int)iMustCompleteIn;
  xRealtimePolicy.preemptible = bPreemptible;
  
  i8 iReturn = thread_policy_set(uThreadPort,
                                 THREAD_TIME_CONSTRAINT_POLICY,
                                 (thread_policy_t)&xRealtimePolicy,
                                 THREAD_TIME_CONSTRAINT_POLICY_COUNT);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("PosixThread::vEnableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }

#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void PosixThread::vEnableRealtime_2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  i8 iReturn = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  mach_timebase_info_data_t xTimebase;
  iReturn = mach_timebase_info(&xTimebase);
  if (iReturn != KERN_SUCCESS) {
    vTrace("Warning: Couldn't get timebase.\n"_Stz);
    return;
  }
  
  // The number of nanoseconds per tick is:
  vTrace("nanoseconds per tick: %lld\n"_Stz,
         xTimebase.numer / xTimebase.denom);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iOverTheNextPeriod     = 5000 * 1000 * xTimebase.denom / xTimebase.numer;
  i8 iThreadRequiresSoLong  = 5000 * 1000 * xTimebase.denom / xTimebase.numer;
  i8 iMustCompleteIn        = 5000 * 1000 * xTimebase.denom / xTimebase.numer;
  
  bool bPreemptible = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  struct thread_time_constraint_policy xRealtimePolicy;
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());
  
  xRealtimePolicy.period      = (int)iOverTheNextPeriod;
  xRealtimePolicy.computation = (int)iThreadRequiresSoLong;
  xRealtimePolicy.constraint  = (int)iMustCompleteIn;
  xRealtimePolicy.preemptible = bPreemptible;
  
  iReturn = thread_policy_set(uThreadPort,
                              THREAD_TIME_CONSTRAINT_POLICY,
                              (thread_policy_t)&xRealtimePolicy,
                              THREAD_TIME_CONSTRAINT_POLICY_COUNT);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("PosixThread::vEnableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }
  
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* get current thread info                                                    */
/*----------------------------------------------------------------------------*/
void PosixThread::vGetInfo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  i8 iReturn = 0;
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());
  struct thread_basic_info BasicInfo;
  mach_msg_type_number_t  Count = THREAD_BASIC_INFO_COUNT;

  iReturn = thread_info(uThreadPort,
                        THREAD_BASIC_INFO,
                        (thread_info_t)&BasicInfo,
                        &Count);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("PosixThread::vGetInfo: thread_info() failed %lld.\n"_Stz,
           iReturn);
    return;
  }


#else
  
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get current thread policy                                                  */
/*----------------------------------------------------------------------------*/
void PosixThread::vGetPolicy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__

  i8 iReturn = 0;

  thread_affinity_policy_data_t current_policy;
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());

  mach_msg_type_number_t policy_count = THREAD_AFFINITY_POLICY_COUNT;

  boolean_t get_default = FALSE;

  iReturn = thread_policy_get(uThreadPort,
                              THREAD_AFFINITY_POLICY,
                              (thread_policy_t)&current_policy,
                              &policy_count,
                              &get_default);
#else

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
    vWarn("syscall(SYS_gettid) error; errno=%d\n"_Stz,errno);
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
    vWarn("pthread_create() error; rCode=%lld\n"_Stz,rCode);
    return false;
  }
  
  gxResourceTracker.vIncThreadOpen();

  // this is important otherwise resources are not released
  pthread_detach(m_iPosixThreadId);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get cpu of this thread                                                     */
/*----------------------------------------------------------------------------*/
i8 PosixThread::iThreadCpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  vWarn("Get thread Cpu affinity not implemented!\n"_Stz);
  return 0;
  
#else
  /*
  i8 iResult;
  
  cpu_set_t xAffinity;
  
  CPU_ZERO(&xAffinity);
  
  ASSERT(pthread_self() == m_iPosixThreadId);
  
  iResult = pthread_getafffinity_np(m_iPosixThreadId, sizeof(cpu_set_t), &xAffinity);
  if (iResult == -1) {
    vWarn("Error in pthread_getafffinity_np() : %lld\n"_Stz, errno);
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
  
  gxResourceTracker.vDecThreadOpen();
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for C++ thread                                                        */
/*----------------------------------------------------------------------------*/
bool PosixThread::bIsCppThread(void)
{
  if (-1 != (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for Java thread                                                       */
/*----------------------------------------------------------------------------*/
bool PosixThread::bIsJavaThread(void)
{
  if (-1 == (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

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
    vWarn("PosixThread::vSleepMicro error; errno=%lld\n"_Stz,errno);
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
    vWarn("PosixThread::vSleepMilli error; errno=%lld\n"_Stz,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of nanoseconds                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vSleepNano(i8 nNanoSeconds)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nNanoSeconds >= C_1G) {
    xReq.tv_nsec = nNanoSeconds % C_1G;
    xReq.tv_sec  = DIV(nNanoSeconds,C_1G);
  }
  else {
    xReq.tv_nsec = nNanoSeconds;
    xReq.tv_sec  = 0;
  }
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PosixThread::vSleepNano error; errno=%lld\n"_Stz,errno);
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
/* get stack base address                                                     */
/*----------------------------------------------------------------------------*/
void* PosixThread::aStackBase(void)
{
  void* aAddress;
  
  pthread_t tThread = pthread_self();
  
#ifdef __APPLE__
  
  aAddress = pthread_get_stackaddr_np(tThread);
  
#elif __LINUX__
  
  size_t iStackSize;
  pthread_attr_t attr;
  
  i8 rCode = pthread_getattr_np(tThread, &attr);
  if (0 != rCode)
    vWarn("pthread_getattr_np() error; rCode=%lld\n"_Stz,rCode);
  
  rCode = pthread_attr_getstack(&attr,
                            &aAddress, &iStackSize);
  if (0 != rCode)
    vWarn("pthread_attr_getstack() error; rCode=%lld\n"_Stz,rCode);
  
#else
#error "Unknown Operating System"
#endif
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* get current stack pointer                                                  */
/*----------------------------------------------------------------------------*/
void* PosixThread::aStackPointer(void)
{
  void* aPointer = &aPointer;
  
  return aPointer;
}

/*----------------------------------------------------------------------------*/
/* get stack size                                                             */
/*----------------------------------------------------------------------------*/
i8 PosixThread::iStackSize(void)
{
  pthread_t tThread = pthread_self();
  i8 iSize;
  
#ifdef __APPLE__
  
  iSize = pthread_get_stacksize_np(tThread);
  
#elif __LINUX__
  
  pthread_attr_t attr;
  void* aAddress;
  size_t iStackSize;
  
  i8 rCode = pthread_getattr_np(tThread, &attr);
  if (0 != rCode)
    vWarn("pthread_getattr_np() error; rCode=%lld\n"_Stz,rCode);
  
  rCode = pthread_attr_getstack(&attr,
                                   &aAddress, &iStackSize);
  if (0 != rCode)
    vWarn("pthread_attr_getstack() error; rCode=%lld\n"_Stz,rCode);
  
  iSize = (i8) iStackSize;
  
#else
#error "Unknown Operating System"
#endif
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* create a tls slot                                                          */
/*----------------------------------------------------------------------------*/
bool PosixThread::bTlsCreate(pthread_key_t* pTlsKey)
{
  i8 iResult = pthread_key_create(pTlsKey,nullptr);
  
  if (0 == iResult) {
    RASSERT(-1 != (int)*pTlsKey);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get value from tls slot                                                    */
/*----------------------------------------------------------------------------*/
void* PosixThread::pTlsGet(pthread_key_t iTlsKey)
{
  RASSERT(-1 != (int)iTlsKey);
  
  void* pVal = pthread_getspecific(iTlsKey);
  
  return pVal;
}

/*----------------------------------------------------------------------------*/
/* set value in tls slot                                                      */
/*----------------------------------------------------------------------------*/
void PosixThread::vTlsSet(pthread_key_t iTlsKey, void* pArg)
{
  RASSERT(-1 != (int)iTlsKey);
  
  pthread_setspecific(iTlsKey,pArg);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PosixThread::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

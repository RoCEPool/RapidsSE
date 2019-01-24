/*============================================================================*/
/* Pthread - a posix thread                                               */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

#ifdef __APPLE__
  #include <mach/mach_time.h>
  #include <mach/thread_policy.h>
#endif

extern AttachMap* gpAttachMap;

extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Pthread* System::_pPthread(void* aAddr)
{
  Pthread* pPthread = reinterpret_cast<Pthread*>(aAddr);
  
  RASSERT_TYPE(pPthread,CN_PTHREAD);
  
  return pPthread;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Pthread::operator new(size_t)
{
  void* pPthread = HeapMem::pAllocate(CN_PTHREAD);
  
  return pPthread;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Pthread::operator delete(void* pPthreadAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pPthreadAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pthread::Pthread()
: Object(CN_PTHREAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
  m_iOpSysThreadId = 0;
  m_iPthreadId = 0;
  
  _checkInitialised_(CN_PTHREAD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pthread::~Pthread(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set standard scheduling policy for thread                                  */
/*----------------------------------------------------------------------------*/
void Pthread::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  sPortrait.vSprint("Pthread: sleeps %lld\n"_Stz,
                    pStats->Thread.m_nSleeps);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bind thread to a core                                                      */
/*----------------------------------------------------------------------------*/
bool Pthread::bBindThread(i8 i_Core)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
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
  thread_port_t uMachThread = pthread_mach_thread_np(m_iPthreadId);

  vTrace("binding thread with posix id %d to core %d\n"_Stz,
         m_iPthreadId, (int)i_Core);

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
//  ASSERT(pthread_self() == m_iPthreadId);
//
//  iResult = pthread_setafffinity_np(m_iPthreadId,
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
void Pthread::vDisableRealtime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());

  struct thread_standard_policy xStandardPolicy;

  i8 iReturn = thread_policy_set(uThreadPort,
                                 THREAD_STANDARD_POLICY,
                                 (thread_policy_t)&xStandardPolicy,
                                 THREAD_STANDARD_POLICY_COUNT);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("Pthread::vDisableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }

#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* set realtime scheduling policy for thread                                  */
/*----------------------------------------------------------------------------*/
void Pthread::vEnableRealtime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
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
    vTrace("Pthread::vEnableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }

#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Pthread::vEnableRealtime_2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  i8 iReturn = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  mach_timebase_info_data_t xBase;
  iReturn = mach_timebase_info(&xBase);
  if (iReturn != KERN_SUCCESS) {
    vTrace("Warning: Couldn't get timebase.\n"_Stz);
    return;
  }
  
  // The number of nanoseconds per tick is:
//  vTrace("nanoseconds per tick: %lld\n"_Stz,
//         xTimebase.numer / xTimebase.denom);

//  uint64_t nsec = C_1M;
//  uint64_t absunit;
//  nanoseconds_to_absolutetime(nsec, &absunit);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iOverTheNextPeriod     = 20 * C_1M * xBase.denom / xBase.numer;
  i8 iThreadRequiresSoLong  = 10 * C_1M * xBase.denom / xBase.numer;
  i8 iMustCompleteIn        = 10 * C_1M * xBase.denom / xBase.numer;
  
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
    vTrace("Pthread::vEnableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }
  
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* get current thread info                                                    */
/*----------------------------------------------------------------------------*/
i8 Pthread::iGetInfo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
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
    vTrace("Pthread::vGetInfo: thread_info() failed %lld.\n"_Stz,
           iReturn);
    return -1;
  }

  //vTrace("Current Policy: %lld\n"_Stz,BasicInfo.policy);
  
  return BasicInfo.policy;

#else
  
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get current thread policy                                                  */
/*----------------------------------------------------------------------------*/
void Pthread::vGetPolicy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
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
void Pthread::vSetOSThreadId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
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
bool Pthread::bSpawnThread(void* pFun(void*), void* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
  // we shouldnt have a thread yet
  ASSERT( ! m_iPthreadId );
  
  i8 rCode = pthread_create(&m_iPthreadId,
                             nullptr,
                             pFun,
                             pArg
                             );
  
  if (rCode) {
    vWarn("pthread_create() error; rCode=%lld\n"_Stz,rCode);
    return false;
  }
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vIncThreadOpen();

  // this is important otherwise resources are not released
  pthread_detach(m_iPthreadId);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get cpu of this thread                                                     */
/*----------------------------------------------------------------------------*/
i8 Pthread::iThreadCpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  vWarn("Get thread Cpu affinity not implemented!\n"_Stz);
  return 0;
  
#else
  /*
  i8 iResult;
  
  cpu_set_t xAffinity;
  
  CPU_ZERO(&xAffinity);
  
  ASSERT(pthread_self() == m_iPthreadId);
  
  iResult = pthread_getafffinity_np(m_iPthreadId, sizeof(cpu_set_t), &xAffinity);
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
void Pthread::vThreadExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vDecThreadOpen();
}
        
/*----------------------------------------------------------------------------*/
/* get thread priority                                                        */
/*----------------------------------------------------------------------------*/
i8 Pthread::iThreadPriority(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPthreadId,&iPolicy,&xSched);
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
void Pthread::vThreadBoost(i8 nPoints)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPthreadId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  xSched.sched_priority += (int)nPoints;
  iError = pthread_setschedparam(m_iPthreadId,iPolicy,&xSched);
  ASSERT(! iError);
  
#else
  ASSERT(nPoints >= 0);
  //vWarn("Boost thread priority not implemented!\n");
#endif

}
        
/*----------------------------------------------------------------------------*/
/* set thread priority                                                        */
/*----------------------------------------------------------------------------*/
void Pthread::vThreadPriority(i8 iPriority)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD,_FOOTER_);
  
#ifdef __APPLE__
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPthreadId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  xSched.sched_priority = (int)iPriority;
  iError = pthread_setschedparam(m_iPthreadId,iPolicy,&xSched);
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
bool Pthread::bIsCppThread(void)
{
  if (-1 != (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for Java thread                                                       */
/*----------------------------------------------------------------------------*/
bool Pthread::bIsJavaThread(void)
{
  if (-1 == (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of microseconds                                     */
/*----------------------------------------------------------------------------*/
void Pthread::vSleepMicro(i8 nMicroSeconds, eCN eCallingClass)
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
  
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMicroSeconds * C_1K;

  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("Pthread::vSleepMicro error; errno=%lld\n"_Stz,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of milliseconds                                     */
/*----------------------------------------------------------------------------*/
void Pthread::vSleepMilli(i8 nMilliSeconds, eCN eCallingClass)
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
  
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMilliSeconds * C_1M;

  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("Pthread::vSleepMilli error; errno=%lld\n"_Stz,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of nanoseconds                                      */
/*----------------------------------------------------------------------------*/
void Pthread::vSleepNano(i8 nNanoSeconds, eCN eCallingClass)
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
  
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nNanoSeconds;

  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("Pthread::vSleepNano error; errno=%lld\n"_Stz,errno);
  }
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of seconds                                          */
/*----------------------------------------------------------------------------*/
void Pthread::vSleepSec(i8 nSeconds, eCN eCallingClass)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nSeconds * C_1G;

  sleep((unsigned)nSeconds);
}

/*----------------------------------------------------------------------------*/
/* sleeper statistics                                                         */
/*----------------------------------------------------------------------------*/
void Pthread::vSleepers(void)
{
  s32 sUnitMag;
  s32 sTimeMag;

  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
  Magnitudes::vUnitMag(pStats->Thread.m_nSleeps,sUnitMag);
  vTrace("Pthread::vSleep() total %s\n"_Stz,sUnitMag.aStz());
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Thread.m_iaSleeps[i_Class]) {
      Magnitudes::vUnitMag(pStats->Thread.m_iaSleeps[i_Class],   sUnitMag);
      Magnitudes::vTimeMag(pStats->Thread.m_iaDuration[i_Class], sTimeMag);
      vTrace("  %s: %s times for duration of %s\n"_Stz,
             aCN[i_Class],sUnitMag.aStz(),sTimeMag.aStz());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get stack base address                                                     */
/*----------------------------------------------------------------------------*/
void* Pthread::aStackBase(void)
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
void* Pthread::aStackPointer(void)
{
  void* aPointer = &aPointer;
  
  return aPointer;
}

/*----------------------------------------------------------------------------*/
/* get stack size                                                             */
/*----------------------------------------------------------------------------*/
i8 Pthread::iStackSize(void)
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
bool Pthread::bTlsCreate(pthread_key_t* aTlsKey)
{
  i8 iResult = pthread_key_create(aTlsKey,nullptr);
  
  if (0 == iResult) {
    RASSERT(-1 != (int)*aTlsKey);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get value from tls slot                                                    */
/*----------------------------------------------------------------------------*/
void* Pthread::pTlsGet(pthread_key_t iTlsKey)
{
  RASSERT(-1 != (int)iTlsKey);
  
  void* pVal = pthread_getspecific(iTlsKey);
  
  return pVal;
}

/*----------------------------------------------------------------------------*/
/* set value in tls slot                                                      */
/*----------------------------------------------------------------------------*/
void Pthread::vTlsSet(pthread_key_t iTlsKey, void* pArg)
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
Footer* Pthread::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTHREAD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

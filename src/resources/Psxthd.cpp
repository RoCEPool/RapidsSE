/*============================================================================*/
/* PsxThd - a posix thread                                                    */
/*============================================================================*/

#include "Resources.h"


Atomic gixPsxThd;

extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PsxThd* Resources::_pPsxThd(void* aAddr)
{
  PsxThd* pPsxThd = reinterpret_cast<PsxThd*>(aAddr);
  
  RASSERT_TYPE(pPsxThd,CN_PSXTHD);
  
  return pPsxThd;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* PsxThd::operator new(size_t)
{
  void* pPsxThd = HeapMem::pAllocate(CN_PSXTHD);
  
  return pPsxThd;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void PsxThd::operator delete(void* pPsxThdAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pPsxThdAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PsxThd::PsxThd()
: Resource(CN_PSXTHD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
  m_iOsThdId = 0;
  m_iPsxThdId = 0;
  
  m_aStackBase   = nullptr;
  m_iStackSize   = 0;

  _checkInitialised_(CN_PSXTHD);

  ++ gixPsxThd;
  AM::vLogRes(SL_INF,this,RA_THD_CONSTRUCT,
              "PsxThd construct : %p (%lld pthreads exist)"_Stz, //!format-LogRes
              this,* gixPsxThd);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PsxThd::~PsxThd(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
  -- gixPsxThd;
  AM::vLogRes(SL_INF,this,RA_THD_DESTRUCT,
              "PsxThd destruct : %p (%lld pthreads exist)"_Stz, //!format-LogRes
              this,* gixPsxThd);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stack base address                                                     */
/*----------------------------------------------------------------------------*/
void* PsxThd::aGetStackBase(void)
{
  void* aAddress;
  
  pthread_t tThread = pthread_self();
  
#ifdef WITH_APPLE
  
  aAddress = pthread_get_stackaddr_np(tThread);
  
#elif defined(WITH_LINUX)
  
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
#error "Unknown Operating Library"
#endif
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* get current stack pointer                                                  */
/*----------------------------------------------------------------------------*/
void* PsxThd::aGetStackPointer(void)
{
  void* aPointer = &aPointer;
  
  return aPointer;
}

/*----------------------------------------------------------------------------*/
/* get stack size                                                             */
/*----------------------------------------------------------------------------*/
i8 PsxThd::iGetStackSize(void)
{
  pthread_t tThread = pthread_self();
  i8 iSize;
  
#ifdef WITH_APPLE
  
  iSize = pthread_get_stacksize_np(tThread);
  
#elif defined(WITH_LINUX)
  
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
#error "Unknown Operating Library"
#endif
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* test for C++ thread                                                        */
/*----------------------------------------------------------------------------*/
bool PsxThd::bIsCppThread(void)
{
  if (C_UNKNOWN != (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test for Java thread                                                       */
/*----------------------------------------------------------------------------*/
bool PsxThd::bIsJavaThread(void)
{
  if (C_UNKNOWN == (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* mark thread as not being an SE thread                                      */
/*----------------------------------------------------------------------------*/
void PsxThd::vMarkNonSeThread(void)
{
  PsxThd::vTlsSet(giTLS_ThreadKey,(void*)-1);
}

/*----------------------------------------------------------------------------*/
/* set standard scheduling policy for thread                                  */
/*----------------------------------------------------------------------------*/
void PsxThd::vMonitorPortrait(s48K& sPortrait)
{
  s32 sUnitMag;
  s32 sTimeMag;

  Statistics* pStats = AM::pStatistics();
  
  sPortrait.vSprint("PsxThd: sleeps %lld"_Stz,
                    pStats->Thread.m_nSleeps);
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Thread.m_iaSleeps[i_Class]) {
      MagLib::vUnitMag(pStats->Thread.m_iaSleeps[i_Class],   sUnitMag);
      MagLib::vTimeMag(pStats->Thread.m_iaDuration[i_Class], sTimeMag);
      sPortrait.vSprintCat(" %s %s/%s"_Stz,
             aCN[i_Class],sUnitMag.szStz(),sTimeMag.szStz());
    }
  }
  sPortrait.vSprintCat("\n"_Stz);

}
/*----------------------------------------------------------------------------*/
/* sleeper statistics                                                         */
/*----------------------------------------------------------------------------*/
void PsxThd::vSleepers(void)
{
  s32 sUnitMag;
  s32 sTimeMag;

  Statistics* pStats = AM::pStatistics();
  
  MagLib::vUnitMag(pStats->Thread.m_nSleeps,sUnitMag);
  vTrace("PsxThd::vSleep() total %s\n"_Stz,sUnitMag.szStz());
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Thread.m_iaSleeps[i_Class]) {
      MagLib::vUnitMag(pStats->Thread.m_iaSleeps[i_Class],   sUnitMag);
      MagLib::vTimeMag(pStats->Thread.m_iaDuration[i_Class], sTimeMag);
      vTrace("  %s: %s times for duration of %s\n"_Stz,
             aCN[i_Class],sUnitMag.szStz(),sTimeMag.szStz());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* create thread, passing it function to execute                              */
/*----------------------------------------------------------------------------*/
i8 PsxThd::iSpawnPosixThread(void* aFun(void*), void* aArg)
{
  _static_
    
  pthread_t iThreadId;
  
  i8 rCode = pthread_create(&iThreadId,nullptr,aFun,aArg);
  
  if (rCode) {
    vWarn("pthread_create() error; rCode=%lld\n"_Stz,rCode);
    return -1;
  }
  
  // this is important otherwise resources are not released
  pthread_detach(iThreadId);
  
  return (i8)iThreadId;
}

/*----------------------------------------------------------------------------*/
/* create a tls slot                                                          */
/*----------------------------------------------------------------------------*/
bool PsxThd::bTlsCreate(pthread_key_t* aTlsKey)
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
void* PsxThd::pTlsGet(pthread_key_t iTlsKey)
{
  RASSERT(-1 != (int)iTlsKey);
  
  void* pVal = pthread_getspecific(iTlsKey);
  
  return pVal;
}

/*----------------------------------------------------------------------------*/
/* set value in tls slot                                                      */
/*----------------------------------------------------------------------------*/
void PsxThd::vTlsSet(pthread_key_t iTlsKey, void* pArg)
{
  RASSERT(-1 != (int)iTlsKey);
  
  pthread_setspecific(iTlsKey,pArg);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bind thread to a core                                                      */
/*----------------------------------------------------------------------------*/
bool PsxThd::bBindThread(i8 i_Core)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
//
// refer: https://developer.apple.com/library/archive/releasenotes/Performance/RN-AffinityAPI/
//
  
//  i8 iCoreCount = Hardware::iCoreCount();
//  if (i_Core > iCoreCount) {
//    vWarn("Unable to bind to core %lld on a system having %lld cores!\n"_Stz,
//          i_Core, iCoreCount);
//    return false;
//  }
  
#ifdef WITH_APPLE
  int iReturn = 0;
  thread_affinity_policy_data_t xPolicy = { (int)i_Core };
  thread_port_t uMachThread = pthread_mach_thread_np(m_iPsxThdId);

  vTrace("binding thread with posix id %d to core %d\n"_Stz,
         m_iPsxThdId, (int)i_Core);

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
//  ASSERT(pthread_self() == m_iPsxThdId);
//
//  iResult = pthread_setafffinity_np(m_iPsxThdId,
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
void PsxThd::vDisableRealtime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());

  struct thread_standard_policy xStandardPolicy;

  i8 iReturn = thread_policy_set(uThreadPort,
                                 THREAD_STANDARD_POLICY,
                                 (thread_policy_t)&xStandardPolicy,
                                 THREAD_STANDARD_POLICY_COUNT);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("PsxThd::vDisableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }

#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* set realtime scheduling policy for thread                                  */
/*----------------------------------------------------------------------------*/
void PsxThd::vEnableRealtime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE

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
    vTrace("PsxThd::vEnableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }

#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void PsxThd::vEnableRealtime_2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  
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
    vTrace("PsxThd::vEnableRealtime: thread_policy_set() failed %lld.\n"_Stz,
           iReturn);
    return;
  }
  
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void PsxThd::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* get cpu and sleep info                                                     */
/*----------------------------------------------------------------------------*/
bool PsxThd::bGetCpu(i8& iUser, i8& iSystem, i8& iUsage, i8& iSleep)
{
  
#ifdef WITH_APPLE
  
  thread_port_t uThreadPort = pthread_mach_thread_np(m_iPsxThdId);  
  thread_extended_info_data_t xInfoExtended;
  mach_msg_type_number_t iCount = THREAD_EXTENDED_INFO_COUNT;

  int iResult = thread_info(uThreadPort,
                            THREAD_EXTENDED_INFO,
                            (thread_info_t)&xInfoExtended,
                            &iCount);
  if (iResult != KERN_SUCCESS)
    return false;
  
  iUser   = xInfoExtended.pth_user_time;
  iSystem = xInfoExtended.pth_system_time;
  iUsage  = xInfoExtended.pth_cpu_usage;
  iSleep  = xInfoExtended.pth_sleep_time;
  
#else
  
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get current thread info                                                    */
/*----------------------------------------------------------------------------*/
i8 PsxThd::iGetInfo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  
  i8 iReturn = 0;
  
  thread_port_t uThreadPort = pthread_mach_thread_np(pthread_self());
  struct thread_basic_info BasicInfo;
  mach_msg_type_number_t  Count = THREAD_BASIC_INFO_COUNT;

  iReturn = thread_info(uThreadPort,
                        THREAD_BASIC_INFO,
                        (thread_info_t)&BasicInfo,
                        &Count);
  
  if (iReturn != KERN_SUCCESS) {
    vTrace("PsxThd::vGetInfo: thread_info() failed %lld.\n"_Stz,
           iReturn);
    return -1;
  }

  //vTrace("Current Policy: %lld\n"_Stz,BasicInfo.policy);
  
  return BasicInfo.policy;

#else
  return 0;
  
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get current thread policy                                                  */
/*----------------------------------------------------------------------------*/
void PsxThd::vGetPolicy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE

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
void PsxThd::vSetOSThreadId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  i8 iOpSysThreadId = syscall(SYS_thread_selfid);
#else
  i8 iOpSysThreadId = syscall(SYS_gettid);
#endif
  
  if (-1 == iOpSysThreadId)
    vWarn("syscall(SYS_gettid) error; errno=%d\n"_Stz,errno);
  else
    m_iOsThdId = iOpSysThreadId;
}


////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* sleep indefinite                                                           */
/*----------------------------------------------------------------------------*/
void PsxThd::vSleepInfinite(void)
{
  AM::vLogRes(SL_INF,this,RA_THD_SLEEP_INDEF,
              "Thead sleeps indefinitely"_Stz); //!format-LogRes
  for (;;)
    sleep(C_1B);
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of milliseconds                                     */
/*----------------------------------------------------------------------------*/
void PsxThd::vSleepMsec(i8 nMilliSeconds, eCN eCallingClass)
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
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMilliSeconds * C_1M;
#endif
  
  AM::vLogRes(SL_INF,this,RA_THD_SLEEP,
              "PsxThd sleeps for %lldms via nanosleep()"_Stz, //!format-LogRes
              nMilliSeconds);
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PsxThd::vSleepMsec error; errno=%lld\n"_Stz,errno);
  }
  
  AM::vLogRes(SL_INF,this,RA_THD_AWAKE,
              "PsxThd is awake"_Stz); //!format-LogRes
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of nanoseconds                                      */
/*----------------------------------------------------------------------------*/
void PsxThd::vSleepNsec(i8 nNanoSeconds, eCN eCallingClass)
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
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nNanoSeconds;
#endif
  
  AM::vLogRes(SL_INF,this,RA_THD_SLEEP,
              "PsxThd sleeps for %lldns via nanosleep()"_Stz, //!format-LogRes
              nNanoSeconds);
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PsxThd::vSleepNsec error; errno=%lld\n"_Stz,errno);
  }
  AM::vLogRes(SL_INF,this,RA_THD_AWAKE,
              "PsxThd is awake"_Stz); //!format-LogRes
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of seconds                                          */
/*----------------------------------------------------------------------------*/
void PsxThd::vSleepSsec(i8 nSeconds, eCN eCallingClass)
{
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nSeconds * C_1G;
#endif
  
  AM::vLogRes(SL_INF,this,RA_THD_SLEEP,
              "Pthead sleeps for %llds via sleep()"_Stz, //!format-LogRes
              nSeconds);
  
  sleep((unsigned)nSeconds);
  
  AM::vLogRes(SL_INF,this,RA_THD_AWAKE,
              "PsxThd is awake"_Stz); //!format-LogRes
}

/*----------------------------------------------------------------------------*/
/* sleep for given number of microseconds                                     */
/*----------------------------------------------------------------------------*/
void PsxThd::vSleepUsec(i8 nMicroSeconds, eCN eCallingClass)
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
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMicroSeconds * C_1K;
#endif
  
  AM::vLogRes(SL_INF,this,RA_THD_SLEEP,
              "PsxThd sleeps for %lldus via nanosleep()"_Stz, //!format-LogRes
              nMicroSeconds);
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("PsxThd::vSleepMicro error; errno=%lld\n"_Stz,errno);
  }
  
  AM::vLogRes(SL_INF,this,RA_THD_AWAKE,
              "PsxThd is awake"_Stz); //!format-LogRes
}


  
/*----------------------------------------------------------------------------*/
/* create thread, passing it function to execute                              */
/*----------------------------------------------------------------------------*/
bool PsxThd::bSpawnThread(void* pFun(void*), void* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
  // we shouldnt have a thread yet
  ASSERT( ! m_iPsxThdId );
  
  i8 rCode = pthread_create(&m_iPsxThdId,
                             nullptr,
                             pFun,
                             pArg
                             );
  
  if (rCode) {
    vWarn("pthread_create() error; rCode=%lld\n"_Stz,rCode);
    return false;
  }
  
  if (AM::bAttached()) AM::pResourceTracker()->vIncThreadOpen();
  
  AM::vLogRes(SL_INF,this,RA_THD_SPAWNED,
              "Thread spawned : %lld"_Stz, //!format-LogRes
              m_iPsxThdId);

  // this is important otherwise resources are not released
  pthread_detach(m_iPsxThdId);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get cpu of this thread                                                     */
/*----------------------------------------------------------------------------*/
i8 PsxThd::iThreadCpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  
  vWarn("Get thread Cpu affinity not implemented!\n"_Stz);
  return 0;
  
#else
  /*
  i8 iResult;
  
  cpu_set_t xAffinity;
  
  CPU_ZERO(&xAffinity);
  
  ASSERT(pthread_self() == m_iPsxThdId);
  
  iResult = pthread_getafffinity_np(m_iPsxThdId, sizeof(cpu_set_t), &xAffinity);
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
/* thread has begun executing                                                 */
/*----------------------------------------------------------------------------*/
void PsxThd::vThreadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
  m_aStackBase = PsxThd::aGetStackBase();
  m_iStackSize = PsxThd::iGetStackSize();

}

/*----------------------------------------------------------------------------*/
/* thread notifies its exiting                                                */
/*----------------------------------------------------------------------------*/
void PsxThd::vThreadExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
  if (AM::bAttached()) AM::pResourceTracker()->vDecThreadOpen();
}
        
/*----------------------------------------------------------------------------*/
/* get thread priority                                                        */
/*----------------------------------------------------------------------------*/
i8 PsxThd::iThreadPriority(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPsxThdId,&iPolicy,&xSched);
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
void PsxThd::vThreadBoost(i8 nPoints)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPsxThdId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  xSched.sched_priority += (int)nPoints;
  iError = pthread_setschedparam(m_iPsxThdId,iPolicy,&xSched);
  ASSERT(! iError);
  
#else
  ASSERT(nPoints >= 0);
  //vWarn("Boost thread priority not implemented!\n");
#endif

}
        
/*----------------------------------------------------------------------------*/
/* set thread priority                                                        */
/*----------------------------------------------------------------------------*/
void PsxThd::vThreadPriority(i8 iPriority)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PSXTHD,_FOOTER_);
  
#ifdef WITH_APPLE
  int iPolicy;
  struct sched_param xSched;
  
  i8 iError = pthread_getschedparam(m_iPsxThdId,&iPolicy,&xSched);
  ASSERT(! iError);
  
  xSched.sched_priority = (int)iPriority;
  iError = pthread_setschedparam(m_iPsxThdId,iPolicy,&xSched);
  ASSERT(! iError);
  
#else
  ASSERT(iPriority >= 0);
  //vWarn("Set thread priority not implemented!\n");
#endif

}
        

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(PsxThd);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* PsxThd.hpp - posix thread                                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  static const i8 C_THREAD_BOOST = 8;
  
  class PsxThd : public Resource
  {
  public:
    pthread_t m_iPsxThdId;  // posix thread id
  
    void*     m_aStackBase; // stack base address
    i8        m_iStackSize; // stack size

    i8        m_iOsThdId;   // thread id as known by os
  
    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void operator delete(void* pPsxThdAddress);

    PsxThd(void);
   ~PsxThd(void) noexcept(false);
  
    static void* aGetStackBase(void);
    static void* aGetStackPointer(void);
    static i8    iGetStackSize(void);
  
    static bool  bIsCppThread(void);
    static bool  bIsJavaThread(void);

    static void  vMarkNonSeThread(void);
    static void  vMonitorPortrait(s48K& sPortrait);

    static void  vSleepers(void);

    static bool  bTlsCreate(pthread_key_t* aTlsKey);
    static void* pTlsGet(pthread_key_t iTlsKey);
    static void  vTlsSet(pthread_key_t iTlsKey, void* pArg);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    bool bBindThread(i8 i_Core);
    void vDisableRealtime(void);
    void vEnableRealtime(void);
    void vEnableRealtime_2(void);
    void vFocalOutline(s4K& sOutline);
    bool bGetCpu(i8& iUser, i8& iSystem, i8& iUsage, i8& iSleep);
    i8   iGetInfo(void);
    void vGetPolicy(void);
    void vSetOSThreadId(void);
    bool bSpawnThread(void* pFun(void*), void* pArg);
  
    void vSleepInfinite(void);
    void vSleepMsec(i8 nMilliSeconds, eCN eCallingClass);
    void vSleepNsec(i8 nNanoSeconds, eCN eCallingClass);
    void vSleepSsec(i8 nSeconds, eCN eCallingClass);
    void vSleepUsec(i8 nMicroSeconds, eCN eCallingClass);

    i8   iThreadCpu(void);
    void vThreadEntry(void);
    void vThreadExit(void);
    i8   iThreadPriority(void);
    void vThreadBoost(i8 nPoints);
    void vThreadPriority(i8 iPriority);
  
  
 
    Footer* aFooter(void);
  };
  
  PsxThd* _pPsxThd(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

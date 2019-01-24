/*============================================================================*/
/* Pthread.hpp - posix thread                                                 */
/*============================================================================*/
#pragma once

namespace System
{
  static const i8 C_THREAD_BOOST = 8;
  
  class Pthread : public Object
  {
  public:
    i8        m_iOpSysThreadId; // thread id as known by os
    pthread_t m_iPthreadId; // posix thread id
  
    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void operator delete(void* pPthreadAddress);

    Pthread(void);
    ~Pthread(void) noexcept(false);
  
    static void vMonitorPortrait(s48K& sPortrait);
  
    bool bBindThread(i8 i_Core);
    void vDisableRealtime(void);
    void vEnableRealtime(void);
    void vEnableRealtime_2(void);
    i8   iGetInfo(void);
    void vGetPolicy(void);
    void vSetOSThreadId(void);
    bool bSpawnThread(void* pFun(void*), void* pArg);
    i8   iThreadCpu(void);
    void vThreadExit(void);
    i8   iThreadPriority(void);
    void vThreadBoost(i8 nPoints);
    void vThreadPriority(i8 iPriority);
    
    //---------------------------------------------------------------------------
  
    static bool  bIsCppThread(void);
    static bool  bIsJavaThread(void);
  
    static void  vSleepMicro(i8 nMicroSeconds, eCN eCallingClass);
    static void  vSleepMilli(i8 nMilliSeconds, eCN eCallingClass);
    static void  vSleepNano(i8 nNanoSeconds, eCN eCallingClass);
    static void  vSleepSec(i8 nSeconds, eCN eCallingClass);
    static void  vSleepers(void);
    static void* aStackBase(void);
    static void* aStackPointer(void);
    static i8    iStackSize(void);
    static bool  bTlsCreate(pthread_key_t* aTlsKey);
    static void* pTlsGet(pthread_key_t iTlsKey);
    static void  vTlsSet(pthread_key_t iTlsKey, void* pArg);
  
    Footer* aFooter(void);
  };
  
  Pthread* _pPthread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

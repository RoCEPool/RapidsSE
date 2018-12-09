/*============================================================================*/
/* PosixThread.hpp - posix thread                                             */
/*============================================================================*/
#pragma once

namespace System
{
  static const i8 C_THREAD_BOOST = 8;
  
  class PosixThread : public Object
  {
  public:
    i8        m_iOpSysThreadId; // thread id as known by os
    pthread_t m_iPosixThreadId; // posix thread id
  
    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void operator delete(void* pPosixThreadAddress);

    PosixThread(void);
    ~PosixThread(void) noexcept(false);
    
    bool bBindThread(i8 i_Core);
    void vDisableRealtime(void);
    void vEnableRealtime(void);
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
  
    static void  vSleepMicro(i8 nMicroSeconds);
    static void  vSleepMilli(i8 nMilliSeconds);
    static void  vSleepNano(i8 nNanoSeconds);
    static void  vSleepSec(i8 nSeconds);
    static void* aStackBase(void);
    static void* aStackPointer(void);
    static i8    iStackSize(void);
    static bool  bTlsCreate(pthread_key_t* pTlsKey);
    static void* pTlsGet(pthread_key_t iTlsKey);
    static void  vTlsSet(pthread_key_t iTlsKey, void* pArg);
  
    Footer* aFooter(void);
  };
  
  PosixThread* _pPosixThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* OSthread.hpp - an os thread                                                */
/*============================================================================*/
#pragma once

#include "Platform.h"

namespace Platform
{
  static const i8 C_THREAD_BOOST = 8;
  
  class PosixThread
  {
  public:
    i8               m_iOpSysThreadId; // thread id as known by os
    pthread_t        m_iPosixThreadId; // posix thread id
    bool             m_bLocked;        // true if mutex is locked
    pthread_mutex_t  m_xMutex;         // mutex variable
    pthread_cond_t   m_xCond;          // condition variable
    
    PosixThread(void);
    ~PosixThread(void);
    
    bool bBindThread(i8 iCpu);
    void vSetOSThreadId(void);
    void vSignalThread(void);
    bool bSpawnThread(void* pFun(void*), void* pArg);
    i8   iThreadCpu(void);
    void vThreadExit(void);
    
    i8   iThreadPriority(void);
    void vThreadBoost(i8 nPoints);
    void vThreadPriority(i8 iPriority);
    
    //---------------------------------------------------------------------------

    void  vCondBroadcast(void);
    void  vCondSignal(void);
    void  vCondWait(void);
    void  vCondTimedWait(i8 iNanoSeconds);
    bool  bMutexLock(void);
    void  vMutexLock(void);
    void  vMutexUnlock(void);
    void  vTlsSet(pthread_key_t iTlsKey, void* pArg);
    
    //---------------------------------------------------------------------------
    
    static void vSleepMicro(i8 nMicroSeconds);
    static void vSleepMilli(i8 nMilliSeconds);
    static void vSleepNano(i8 nNanoSeconds);
    static void vSleepSec(i8 nSeconds);
    static bool bTlsCreate(pthread_key_t* pTlsKey);
    static void* pTlsGet(pthread_key_t iTlsKey);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
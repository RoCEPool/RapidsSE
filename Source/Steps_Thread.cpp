/*============================================================================*/
/* Steps_Thread.cpp - thread step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Thread(void)
{
  vAddStep("thread::disableRealtime"_Stz, &Step::vThreadDisableRealtime);
  vAddStep("thread::enableRealtime"_Stz,  &Step::vThreadEnableRealtime);

  vAddStep("thread::sleepMsec"_Stz,       &Step::vThreadSleepMsec);
  vAddStep("thread::sleepNsec"_Stz,       &Step::vThreadSleepNsec);
  vAddStep("thread::sleepSsec"_Stz,       &Step::vThreadSleepSsec);
  vAddStep("thread::sleepUsec"_Stz,       &Step::vThreadSleepUsec);
  
  vAddStep("thread.bind"_Stz,             &Step::vThreadBind);
  vAddStep("thread.cpu"_Stz,              &Step::vThreadCpu);
  vAddStep("thread.dedicated@"_Stz,       &Step::vThreadDedicatedFetch);
  vAddStep("thread.dedicated!"_Stz,       &Step::vThreadDedicatedStore);
  vAddStep("thread.dump"_Stz,             &Step::vThreadDump);
  vAddStep("thread.id"_Stz,               &Step::vThreadId);
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Step::vThreadBind(void)
{
  ENTRY();
  
  
  i8 iCpu = m_pDataStack->iPopInt(__func__);
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Step::vThreadCpu(void)
{
  ENTRY();
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated@ -- fetch m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedFetch(void)
{
  ENTRY();
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pThread->m_eDedicated);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated! -- store m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedStore(void)
{
  ENTRY();
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  i8 iDedicated = m_pDataStack->iPopAsInt(__func__);

  pThread->m_eDedicated = (eCN)iDedicated;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.disableRealtime -- disable realtime                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDisableRealtime(void)
{
  ENTRY();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vDisableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDump(void)
{
  ENTRY();
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  pThread->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.enableRealtime -- enable realtime                                   */
/*----------------------------------------------------------------------------*/
void Step::vThreadEnableRealtime(void)
{
  ENTRY();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vEnableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Step::vThreadId(void)
{
  ENTRY();
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pThread->m_pPsxThd->m_iOsThdId);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* thread::sleepMsec -- sleep for n milliseconds                              */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepMsec(void)
{
  ENTRY();
  
  i8 nMilliseconds = m_pDataStack->iPopInt(__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepMsec(nMilliseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepNsec -- sleep for n nanoseconds                               */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepNsec(void)
{
  ENTRY();
  
  i8 nNanoseconds = m_pDataStack->iPopInt(__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepNsec(nNanoseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleepSsec -- sleep for n seconds                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepSsec(void)
{
  ENTRY();
  
  i8 nSeconds = m_pDataStack->iPopInt(__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepSsec(nSeconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepUsec -- sleep for n microseconds                              */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepUsec(void)
{
  ENTRY();
  
  i8 nMicroseconds = m_pDataStack->iPopInt(__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepUsec(nMicroseconds,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

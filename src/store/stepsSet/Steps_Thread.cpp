/*============================================================================*/
/* Steps_Thread.cpp - thread step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


#include "PsxThd.hpp"  //?

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
  vAddStep("thread.faultCurrent!"_Stz,    &Step::vThreadFaultCurrentStore);
  vAddStep("thread.faultInjection!"_Stz,  &Step::vThreadFaultInjectionStore);
  vAddStep("thread.faultPoint!"_Stz,      &Step::vThreadFaultPointStore);
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
  
  Thread* pThread = Thread::pThisTLSThread();

  m_pDataStack->vPushInt(pThread->eDedicated());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated! -- store m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedStore(void)
{
  ENTRY();
  
  Thread* pThread = Thread::pThisTLSThread();

  i8 iDedicated = m_pDataStack->iPopAsInt(__func__);

  pThread->vDedicated((eCN)iDedicated);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.disableRealtime -- disable realtime                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDisableRealtime(void)
{
  ENTRY();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->pPsxThd()->vDisableRealtime();
  
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
  
  pThread->pPsxThd()->vEnableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.faultCurrent! -- store faultCurrent                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadFaultCurrentStore(void)
{
  ENTRY();

  i8 iFaultCurrent = m_pDataStack->iPopInt(__func__);
  UNUSED(iFaultCurrent);
#ifdef WITH_OOM_FAULTINJECTION
  PsxThd::vTlsSet(giTLS_CurrentPoint,(void*)iFaultCurrent);
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.faultInjection! -- store faultInjection                             */
/*----------------------------------------------------------------------------*/
void Step::vThreadFaultInjectionStore(void)
{
  ENTRY();

  i8 iFaultInjection = m_pDataStack->iPopBol(__func__);
  UNUSED(iFaultInjection);
#ifdef WITH_OOM_FAULTINJECTION
  PsxThd::vTlsSet(gbTLS_FaultInjection,(void*)iFaultInjection);
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.faultPoint! -- store faultPoint                                     */
/*----------------------------------------------------------------------------*/
void Step::vThreadFaultPointStore(void)
{
  ENTRY();

  i8 iFaultPoint = m_pDataStack->iPopInt(__func__);
  UNUSED(iFaultPoint);
#ifdef WITH_OOM_FAULTINJECTION
  PsxThd::vTlsSet(giTLS_FaultPoint,(void*)iFaultPoint);
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Step::vThreadId(void)
{
  ENTRY();
  
  Thread* pThread = _pThread(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pThread->pPsxThd()->m_iOsThdId);
  
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
  
#ifdef WITH_DEDICATED_SLEEP
  RASSERT_FATAL(CN_NONE != pThread->eDedicated());
#endif
  
  pThread->pPsxThd()->vSleepMsec(nMilliseconds,CN_WORD);
  
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
  
#ifdef WITH_DEDICATED_SLEEP
  RASSERT_FATAL(CN_NONE != pThread->eDedicated());
#endif
  
  pThread->pPsxThd()->vSleepNsec(nNanoseconds,CN_WORD);
  
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
  
#ifdef WITH_DEDICATED_SLEEP
  RASSERT_FATAL(CN_NONE != pThread->eDedicated());
#endif
  
  pThread->pPsxThd()->vSleepSsec(nSeconds,CN_WORD);
  
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
  
#ifdef WITH_DEDICATED_SLEEP
  RASSERT_FATAL(CN_NONE != pThread->eDedicated());
#endif
  
  pThread->pPsxThd()->vSleepUsec(nMicroseconds,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

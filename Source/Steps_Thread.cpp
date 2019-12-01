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
  ENTRY(); // ENTRY("vThreadBind"_Stz);
  
  i8 iCpu;
  Object* pObject;
  
  m_pDataStack->vPopInt(iCpu,__func__);
  
  m_pDataStack->vPopPtr(pObject,__func__);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Step::vThreadCpu(void)
{
  ENTRY(); // ENTRY("vThreadCpu"_Stz);
  
  Object* pObject;
  
  m_pDataStack->vPopPtr(pObject,__func__);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  m_pDataStack->vPushInt(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated@ -- fetch m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedFetch(void)
{
  ENTRY(); // ENTRY("vThreadDedicatedFetch"_Stz);
  
  Object* pThreadObject;
  m_pDataStack->vPopPtr(pThreadObject,__func__);
  Thread* pThread = _pThread(pThreadObject);
  
  m_pDataStack->vPushInt(pThread->m_eDedicated);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated! -- store m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedStore(void)
{
  ENTRY(); // ENTRY("vThreadDedicatedStore"_Stz);
  
  Object* pThreadObject;
  m_pDataStack->vPopPtr(pThreadObject,__func__);
  Thread* pThread = _pThread(pThreadObject);
  
  i8 iDedicated;
  if ( ! m_pDataStack->bPopAsInt(iDedicated) ) THROW_DSTACK();

  pThread->m_eDedicated = (eCN)iDedicated;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.disableRealtime -- disable realtime                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDisableRealtime(void)
{
  ENTRY(); // ENTRY("vThreadDisableRealtime"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vDisableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDump(void)
{
  ENTRY(); // ENTRY("vThreadDump"_Stz);
  
  Object* pObject;
  
  m_pDataStack->vPopPtr(pObject,__func__);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.enableRealtime -- enable realtime                                   */
/*----------------------------------------------------------------------------*/
void Step::vThreadEnableRealtime(void)
{
  ENTRY(); // ENTRY("vThreadEnableRealtime"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vEnableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Step::vThreadId(void)
{
  ENTRY(); // ENTRY("vThreadId"_Stz);
  
  Object* pObject;
  
  m_pDataStack->vPopPtr(pObject,__func__);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  m_pDataStack->vPushInt(pThread->m_pPsxThd->m_iOsThdId);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* thread::sleepMsec -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepMsec(void)
{
  ENTRY(); // ENTRY("vThreadSleepMsec"_Stz);
  
  i8 nMilliseconds;
  
  m_pDataStack->vPopInt(nMilliseconds,__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepMsec(nMilliseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepNsec -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepNsec(void)
{
  ENTRY(); // ENTRY("vThreadSleepNsec"_Stz);
  
  i8 nNanoseconds;
  
  m_pDataStack->vPopInt(nNanoseconds,__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepNsec(nNanoseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleepSsec -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepSsec(void)
{
  ENTRY(); // ENTRY("vThreadSleepSsec"_Stz);
  
  i8 nSeconds;
  
  m_pDataStack->vPopInt(nSeconds,__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepSsec(nSeconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepUsec -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepUsec(void)
{
  ENTRY(); // ENTRY("vThreadSleepUsec"_Stz);
  
  i8 nMicroseconds;
  
  m_pDataStack->vPopInt(nMicroseconds,__func__);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPsxThd->vSleepUsec(nMicroseconds,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

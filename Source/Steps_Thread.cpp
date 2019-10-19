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
ENTRY("vThreadBind"_Stz);
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! m_pDataStack->bPop(iCpu) )
    THROW_DSTACK();
  
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
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
ENTRY("vThreadCpu"_Stz);
  
  Object* pObject;
  
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  m_pDataStack->vPushI8(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated@ -- fetch m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedFetch(void)
{
  ENTRY("vThreadDedicatedFetch"_Stz);
  
  Object* pThreadObject;
  if ( ! m_pDataStack->bPop(pThreadObject) )
    THROW_DSTACK();
  Thread* pThread = _pThread(pThreadObject);
  
  m_pDataStack->vPushI8(pThread->m_eDedicated);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated! -- store m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedStore(void)
{
  ENTRY("vThreadDedicatedStore"_Stz);
  
  Object* pThreadObject;
  if ( ! m_pDataStack->bPop(pThreadObject) )
    THROW_DSTACK();
  Thread* pThread = _pThread(pThreadObject);
  
  i8 iDedicated;
  if ( ! m_pDataStack->bPopAsInt(iDedicated) )
    THROW_DSTACK();

  pThread->m_eDedicated = (eCN)iDedicated;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.disableRealtime -- disable realtime                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDisableRealtime(void)
{
  ENTRY("vThreadDisableRealtime"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vDisableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDump(void)
{
ENTRY("vThreadDump"_Stz);
  
  Object* pObject;
  
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
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
  ENTRY("vThreadEnableRealtime"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vEnableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Step::vThreadId(void)
{
ENTRY("vThreadId"_Stz);
  
  Object* pObject;
  
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  m_pDataStack->vPushI8(pThread->m_pPthread->m_iOpSysThreadId);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* thread::sleepMsec -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepMsec(void)
{
ENTRY("vThreadSleepMsec"_Stz);
  
  i8 nMilliseconds;
  
  if ( ! m_pDataStack->bPop(nMilliseconds) )
    THROW_DSTACK();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepMsec(nMilliseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepNsec -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepNsec(void)
{
  ENTRY("vThreadSleepNsec"_Stz);
  
  i8 nNanoseconds;
  
  if ( ! m_pDataStack->bPop(nNanoseconds) )
    THROW_DSTACK();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepNsec(nNanoseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleepSsec -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepSsec(void)
{
  ENTRY("vThreadSleepSsec"_Stz);
  
  i8 nSeconds;
  
  if ( ! m_pDataStack->bPop(nSeconds) )
    THROW_DSTACK();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepSsec(nSeconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepUsec -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepUsec(void)
{
ENTRY("vThreadSleepUsec"_Stz);
  
  i8 nMicroseconds;
  
  if ( ! m_pDataStack->bPop(nMicroseconds) )
    THROW_DSTACK();
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepUsec(nMicroseconds,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

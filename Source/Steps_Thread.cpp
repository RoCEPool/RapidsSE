/*============================================================================*/
/* Steps_Thread.cpp - thread step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  vAddStep("thread.condSignal"_Stz,       &Step::vThreadCondSignal);
  vAddStep("thread.condWait"_Stz,         &Step::vThreadCondWait);
  vAddStep("thread.cpu"_Stz,              &Step::vThreadCpu);
  vAddStep("thread.dedicated@"_Stz,       &Step::vThreadDedicatedFetch);
  vAddStep("thread.dedicated!"_Stz,       &Step::vThreadDedicatedStore);
  vAddStep("thread.dump"_Stz,             &Step::vThreadDump);
  vAddStep("thread.id"_Stz,               &Step::vThreadId);
  vAddStep("thread.mutexLock"_Stz,        &Step::vThreadMutexLock);
  vAddStep("thread.mutexUnlock"_Stz,      &Step::vThreadMutexUnlock);
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Step::vThreadBind(MP_Task mpTask)
{
ENTRY("vThreadBind"_Stz);
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(iCpu) )
    THROW_INVALID_DSTACK("thread.bind"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.bind"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condSignal -- signal condition on threads mutex                     */
/*----------------------------------------------------------------------------*/
void Step::vThreadCondSignal(MP_Task mpTask)
{
 ENTRY("vThreadCondSignal"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condSignal"_Stz);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pMutexThread->vCondSignal();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condWait -- wait for cond on threads mutex                          */
/*----------------------------------------------------------------------------*/
void Step::vThreadCondWait(MP_Task mpTask)
{
 ENTRY("vThreadCondWait"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condWait"_Stz);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pMutexThread->vCondWaitIndefinite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Step::vThreadCpu(MP_Task mpTask)
{
ENTRY("vThreadCpu"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.cpu"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  mpTask->m_pDataStack->vPushI8(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated@ -- fetch m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedFetch(MP_Task mpTask)
{
  ENTRY("vThreadDedicatedFetch"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.dedicated@"_Stz);
  Thread* pThread = _pThread(pThreadObject);
  
  mpTask->m_pDataStack->vPushI8(pThread->m_eDedicated);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dedicated! -- store m_eDedicated                                    */
/*----------------------------------------------------------------------------*/
void Step::vThreadDedicatedStore(MP_Task mpTask)
{
  ENTRY("vThreadDedicatedStore"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.dedicated!"_Stz);
  Thread* pThread = _pThread(pThreadObject);
  
  i8 iDedicated;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iDedicated) )
    THROW_INVALID_DSTACK("thread.dedicated!"_Stz);

  pThread->m_eDedicated = (eCN)iDedicated;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.disableRealtime -- disable realtime                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDisableRealtime(MP_Task mpTask)
{
  ENTRY("vThreadDisableRealtime"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vDisableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadDump(MP_Task mpTask)
{
ENTRY("vThreadDump"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.dump"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.enableRealtime -- enable realtime                                   */
/*----------------------------------------------------------------------------*/
void Step::vThreadEnableRealtime(MP_Task mpTask)
{
  ENTRY("vThreadEnableRealtime"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vEnableRealtime();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Step::vThreadId(MP_Task mpTask)
{
ENTRY("vThreadId"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.id"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  mpTask->m_pDataStack->vPushI8(pThread->m_pPthread->m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Step::vThreadMutexLock(MP_Task mpTask)
{
ENTRY("vThreadoLockMutexLock"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexLock"_Stz);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pMutexThread->vMutexLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexUnlock -- release a threads mutex lock                         */
/*----------------------------------------------------------------------------*/
void Step::vThreadMutexUnlock(MP_Task mpTask)
{
ENTRY("vThreadMutexUnlock"_Stz);

  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexUnlock"_Stz);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pMutexThread->vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepMsec -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepMsec(MP_Task mpTask)
{
ENTRY("vThreadSleepMsec"_Stz);
  
  i8 nMilliseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMilliseconds) )
    THROW_INVALID_DSTACK("sleepMsec"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepMsec(nMilliseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepNsec -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepNsec(MP_Task mpTask)
{
  ENTRY("vThreadSleepNsec"_Stz);
  
  i8 nNanoseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nNanoseconds) )
    THROW_INVALID_DSTACK("sleepNsec"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepNsec(nNanoseconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleepSsec -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepSsec(MP_Task mpTask)
{
  ENTRY("vThreadSleepSsec"_Stz);
  
  i8 nSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleepSsec"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepSsec(nSeconds,CN_WORD);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread::sleepUsec -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleepUsec(MP_Task mpTask)
{
ENTRY("vThreadSleepUsec"_Stz);
  
  i8 nMicroseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMicroseconds) )
    THROW_INVALID_DSTACK("sleepUsec"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPthread->vSleepUsec(nMicroseconds,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

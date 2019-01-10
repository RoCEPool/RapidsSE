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
  vAddStep("thread.bind"_Stz,            &Step::vThreadBind);
  vAddStep("thread.condSignal"_Stz,      &Step::vThreadCondSignal);
  vAddStep("thread.condWait"_Stz,        &Step::vThreadCondWait);
  vAddStep("thread.cpu"_Stz,             &Step::vThreadCpu);
  vAddStep("thread.dedicated@"_Stz,      &Step::vThreadDedicatedFetch);
  vAddStep("thread.dedicated!"_Stz,      &Step::vThreadDedicatedStore);
  vAddStep("thread::disableRealtime"_Stz, &Step::vThreadDisableRealtime);
  vAddStep("thread.dump"_Stz,            &Step::vThreadDump);
  vAddStep("threa::enableRealtime"_Stz,  &Step::vThreadEnableRealtime);
  vAddStep("thread.id"_Stz,              &Step::vThreadId);
  vAddStep("thread.muteoLock"_Stz,       &Step::vThreadoLockMutexLock);
  vAddStep("thread.mutexUnlock"_Stz,     &Step::vThreadMutexUnlock);
  vAddStep("thread.sleep_s"_Stz,         &Step::vThreadSleep_s);
  vAddStep("thread.sleep_ms"_Stz,        &Step::vThreadSleep_ms);
  vAddStep("thread.sleep_us"_Stz,        &Step::vThreadSleep_us);
  vAddStep("thread.sleep_ns"_Stz,        &Step::vThreadSleep_ns);
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
  
  pThread->m_pSemaphore->vCondSignal();
  
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
  
  pThread->m_pSemaphore->vCondWait_Mutex();
  
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
  
  pThread->m_pPosixThread->vDisableRealtime();
  
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
  
  pThread->m_pPosixThread->vEnableRealtime();
  
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
  
  mpTask->m_pDataStack->vPushI8(pThread->m_pPosixThread->m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.muteoLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Step::vThreadoLockMutexLock(MP_Task mpTask)
{
ENTRY("vThreadoLockMutexLock"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.muteoLock"_Stz);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pSemaphore->voLockMutexLock();

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
  
  pThread->m_pSemaphore->vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_s -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleep_s(MP_Task mpTask)
{
ENTRY("vThreadSleep_s"_Stz);
  
  i8 nSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleep_s"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPosixThread->vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleep_ms(MP_Task mpTask)
{
ENTRY("vThreadSleep_ms"_Stz);
  
  i8 nMilliseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMilliseconds) )
    THROW_INVALID_DSTACK("sleep_ms"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPosixThread->vSleepMilli(nMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleep_us(MP_Task mpTask)
{
ENTRY("vThreadSleep_us"_Stz);
  
  i8 nMicroseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMicroseconds) )
    THROW_INVALID_DSTACK("sleep_us"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPosixThread->vSleepMicro(nMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Step::vThreadSleep_ns(MP_Task mpTask)
{
ENTRY("vThreadSleep_ns"_Stz);
  
  i8 nNanoseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nNanoseconds) )
    THROW_INVALID_DSTACK("sleep_ns"_Stz);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  pThread->m_pPosixThread->vSleepNano(nNanoseconds);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

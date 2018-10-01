/*============================================================================*/
/* Steps_Thread.cpp - thread step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Thread(void)
{
  vAddStep("thread.bind"_Stz,            &vThreadBind,            false);
  vAddStep("thread.condSignal"_Stz,      &vThreadCondSignal,      false);
  vAddStep("thread.condWait"_Stz,        &vThreadCondWait,        false);
  vAddStep("thread.cpu"_Stz,             &vThreadCpu,             false);
  vAddStep("thread.dump"_Stz,            &vThreadDump,            false);
  vAddStep("thread.id"_Stz,              &vThreadId,              false);
  vAddStep("thread.mutexLock"_Stz,       &vThreadMutexLock,       false);
  vAddStep("thread.mutexUnlock"_Stz,     &vThreadMutexUnlock,     false);
  vAddStep("thread.sleep_s"_Stz,         &vThreadSleep_s,         false);
  vAddStep("thread.sleep_ms"_Stz,        &vThreadSleep_ms,        false);
  vAddStep("thread.sleep_us"_Stz,        &vThreadSleep_us,        false);
  vAddStep("thread.sleep_ns"_Stz,        &vThreadSleep_ns,        false);
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(MP_Task mpTask)
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
void Task::vThreadCondSignal(MP_Task mpTask)
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
void Task::vThreadCondWait(MP_Task mpTask)
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
void Task::vThreadCpu(MP_Task mpTask)
{
ENTRY("vThreadCpu"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.cpu"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  mpTask->m_pDataStack->vPushInt(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(MP_Task mpTask)
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
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadId(MP_Task mpTask)
{
ENTRY("vThreadId"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.id"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  mpTask->m_pDataStack->vPushInt(pThread->m_pPosixThread->m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexLock(MP_Task mpTask)
{
ENTRY("vThreadMutexLock"_Stz);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexLock"_Stz);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pSemaphore->vMutexLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexUnlock -- release a threads mutex lock                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexUnlock(MP_Task mpTask)
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
void Task::vThreadSleep_s(MP_Task mpTask)
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
void Task::vThreadSleep_ms(MP_Task mpTask)
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
void Task::vThreadSleep_us(MP_Task mpTask)
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
void Task::vThreadSleep_ns(MP_Task mpTask)
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

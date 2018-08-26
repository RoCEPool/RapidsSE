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
void Task::vAddPrimitives_Thread(void)
{
  vAddPrimitive("thread.bind"_Stz,            &vThreadBind,            false);
  vAddPrimitive("thread.condSignal"_Stz,      &vThreadCondSignal,      false);
  vAddPrimitive("thread.condWait"_Stz,        &vThreadCondWait,        false);
  vAddPrimitive("thread.cpu"_Stz,             &vThreadCpu,             false);
  vAddPrimitive("thread.dump"_Stz,            &vThreadDump,            false);
  vAddPrimitive("thread.id"_Stz,              &vThreadId,              false);
  vAddPrimitive("thread.kind@"_Stz,           &vThreadKindFetch,       false);
  vAddPrimitive("thread.kind!"_Stz,           &vThreadKindStore,       false);
  vAddPrimitive("thread.mutexLock"_Stz,       &vThreadMutexLock,       false);
  vAddPrimitive("thread.mutexUnlock"_Stz,     &vThreadMutexUnlock,     false);
  vAddPrimitive("thread.sleep_s"_Stz,         &vThreadSleep_s,         false);
  vAddPrimitive("thread.sleep_ms"_Stz,        &vThreadSleep_ms,        false);
  vAddPrimitive("thread.sleep_us"_Stz,        &vThreadSleep_us,        false);
  vAddPrimitive("thread.sleep_ns"_Stz,        &vThreadSleep_ns,        false);
  vAddPrimitive("thread.task@"_Stz,           &vThreadTaskFetch,       false);
  vAddPrimitive("thread.task!"_Stz,           &vThreadTaskStore,       false);
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
/* thread.kind@ -- fetch thread kind from thread which is on stack            */
/*----------------------------------------------------------------------------*/
void Task::vThreadKindFetch(MP_Task mpTask)
{
  ENTRY("vThreadKindFetch"_Stz);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.kind@"_Stz);
  Thread* pThread = _pThread(pThreadEmb);
  
  mpTask->m_pDataStack->vPushInt(pThread->m_eThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.kind! -- store thread kind into thread which is on stack            */
/*----------------------------------------------------------------------------*/
void Task::vThreadKindStore(MP_Task mpTask)
{
  ENTRY("vThreadKindStore"_Stz);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.kind@"_Stz);
  Thread* pThread = _pThread(pThreadEmb);
  
  i8 eThread;
  if ( ! mpTask->m_pDataStack->bPop(eThread) )
    THROW_INVALID_DSTACK("thread.kind!"_Stz);
  
  pThread->m_eThread = (eTH)eThread;
  
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

/*----------------------------------------------------------------------------*/
/* thread.task@ -- fetch current task from the thread which is on data stack  */
/*----------------------------------------------------------------------------*/
void Task::vThreadTaskFetch(MP_Task mpTask)
{
  ENTRY("vThreadTaskFetch"_Stz);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.task@"_Stz);
  Thread* pThread = _pThread(pThreadEmb);
  
  Task* pTask = *pThread->m_npTask;
  
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.task! -- store current task into the thread which is on data stack  */
/*----------------------------------------------------------------------------*/
void Task::vThreadTaskStore(MP_Task mpTask)
{
  ENTRY("vThreadTaskStore"_Stz);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.task@"_Stz);
  Thread* pThread = _pThread(pThreadEmb);
  
  Object* pTaskEmb;
  if ( ! mpTask->m_pDataStack->bPop(pTaskEmb) )
    THROW_INVALID_DSTACK("thread.task!"_Stz);
  Task* pTask = _pTask(pTaskEmb);
  
  pThread->m_npTask = pTask;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

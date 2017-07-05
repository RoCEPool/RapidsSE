/*============================================================================*/
/* Steps_Thread.cpp - thread step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Thread(void)
{
  vAddPrimitive("thread.append"_Str,          &vThreadAppend,          false);
  vAddPrimitive("thread.bind"_Str,            &vThreadBind,            false);
  vAddPrimitive("thread.condSignal"_Str,      &vThreadCondSignal,      false);
  vAddPrimitive("thread.condWait"_Str,        &vThreadCondWait,        false);
  vAddPrimitive("thread.cpu"_Str,             &vThreadCpu,             false);
  vAddPrimitive("thread.dump"_Str,            &vThreadDump,            false);
  vAddPrimitive("thread.init"_Str,            &vThreadInit,            false);
  vAddPrimitive("thread.id"_Str,              &vThreadId,              false);
  vAddPrimitive("thread.kind@"_Str,           &vThreadSchedulerFetch,  false);
  vAddPrimitive("thread.kind!"_Str,           &vThreadSchedulerStore,  false);
  vAddPrimitive("thread.mutexLock"_Str,       &vThreadMutexLock,       false);
  vAddPrimitive("thread.mutexUnlock"_Str,     &vThreadMutexUnlock,     false);
  vAddPrimitive("thread.name@"_Str,           &vThreadNameFetch,       false);
  vAddPrimitive("thread.name!"_Str,           &vThreadNameStore,       false);
  vAddPrimitive("thread.scheduler@"_Str,      &vThreadSchedulerFetch,  false);
  vAddPrimitive("thread.scheduler!"_Str,      &vThreadSchedulerStore,  false);
  vAddPrimitive("thread.taskset@"_Str,        &vThreadSfetch,          false);
  vAddPrimitive("thread.taskset!"_Str,        &vThreadSstore,          false);
  vAddPrimitive("thread.sleep_s"_Str,         &vThreadSleep_s,         false);
  vAddPrimitive("thread.sleep_ms"_Str,        &vThreadSleep_ms,        false);
  vAddPrimitive("thread.sleep_us"_Str,        &vThreadSleep_us,        false);
  vAddPrimitive("thread.sleep_ns"_Str,        &vThreadSleep_ns,        false);
  vAddPrimitive("thread.spawn_actor"_Str,     &vThreadSpawn_Actor,     false);
  vAddPrimitive("thread.spawn_partition"_Str, &vThreadSpawn_Partition, false);
  vAddPrimitive("thread.spawn_general"_Str,   &vThreadSpawn_General,   false);
  vAddPrimitive("thread.spawn_worker"_Str,    &vThreadSpawn_Worker,    false);
}

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(MP_Task mpTask)
{
ENTRY("vThreadAppend"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.append"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(MP_Task mpTask)
{
ENTRY("vThreadBind"_Str);
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(iCpu) )
    THROW_INVALID_DSTACK("thread.bind"_Str);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.bind"_Str);
  
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
 ENTRY("vThreadCondSignal"_Str);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condSignal"_Str);
  
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
 ENTRY("vThreadCondWait"_Str);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condWait"_Str);
  
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
ENTRY("vThreadCpu"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.cpu"_Str);
  
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
ENTRY("vThreadDump"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.dump"_Str);
  
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
ENTRY("vThreadId"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.id"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  mpTask->m_pDataStack->vPushInt(pThread->m_pPosixThread->m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(MP_Task mpTask)
{
ENTRY("vThreadInit"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.init"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  new(pThread) Thread;
  
  Word* pWordThread = Word::pToWord(pThread);
  
  s32 sName;
  sName = pWordThread->sName();
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.kind@ -- fetch thread kind from thread which is on stack            */
/*----------------------------------------------------------------------------*/
void Task::vThreadKindFetch(MP_Task mpTask)
{
  ENTRY("vThreadKindFetch"_Str);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.kind@"_Str);
  Thread* pThread = _pThread(pThreadEmb);
  
  mpTask->m_pDataStack->vPushInt(pThread->m_eThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.kind! -- store thread kind into thread which is on stack            */
/*----------------------------------------------------------------------------*/
void Task::vThreadKindStore(MP_Task mpTask)
{
  ENTRY("vThreadKindStore"_Str);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.kind@"_Str);
  Thread* pThread = _pThread(pThreadEmb);
  
  i8 eThread;
  if ( ! mpTask->m_pDataStack->bPop(eThread) )
    THROW_INVALID_DSTACK("thread.kind!"_Str);
  
  pThread->m_eThread = (eTH)eThread;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexLock(MP_Task mpTask)
{
ENTRY("vThreadMutexLock"_Str);
  
  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexLock"_Str);
  
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
ENTRY("vThreadMutexUnlock"_Str);

  Object* pThreadObject;
  if ( ! mpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexUnlock"_Str);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pSemaphore->vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(MP_Task mpTask)
{
ENTRY("vThreadNameFetch"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name@"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  mpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.scheduler@ -- fetch scheduler from given thread on stack            */
/*----------------------------------------------------------------------------*/
void Task::vThreadSchedulerFetch(MP_Task mpTask)
{
  ENTRY("vThreadSchedulerFetch"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.scheduler@"_Str);
  Thread* pThread = _pThread(pThreadEmb);
  
  Scheduler* pScheduler = pThread->m_pScheduler;
  
  Object* pSchedulerEmb = static_cast<Object*>(pScheduler);
  
  mpTask->m_pDataStack->vPushObject(pSchedulerEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.scheduler! -- store scheduler into given thread                     */
/*----------------------------------------------------------------------------*/
void Task::vThreadSchedulerStore(MP_Task mpTask)
{
  ENTRY("vThreadSchedulerStore"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.scheduler!"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pSchedulerEmb;
  if ( ! mpTask->m_pDataStack->bPop(pSchedulerEmb) )
    THROW_INVALID_DSTACK("thread.scheduler!"_Str);
  
  Scheduler* pScheduler = _pScheduler(pSchedulerEmb);
  
  pThread->m_pScheduler = pScheduler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(MP_Task mpTask)
{
ENTRY("vThreadNameStore"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name!"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWrk xNameWsp;
  if ( ! mpTask->m_pDataStack->bPop(xNameWsp) )
    THROW_INVALID_DSTACK("thread.name!"_Str);
  
  sName = _pStrVal(xNameWsp.pNestedValueSelf());
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(MP_Task mpTask)
{
  ENTRY("vThreadSfetch"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset@"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;
  
  mpTask->m_pDataStack->vPushObject(pTaskSetEmb);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(MP_Task mpTask)
{
ENTRY("vThreadSstore"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset!"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pTaskSetEmb) )
    THROW_INVALID_DSTACK("thread.taskset!"_Str);
  
  ASSERT(CN_TASKSET == pTaskSetEmb->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pTaskSetEmb);
  
  pThread->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_s -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_s(MP_Task mpTask)
{
ENTRY("vThreadSleep_s"_Str);
  
  i8 nSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleep_s"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ms(MP_Task mpTask)
{
ENTRY("vThreadSleep_ms"_Str);
  
  i8 nMilliseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMilliseconds) )
    THROW_INVALID_DSTACK("sleep_ms"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepMilli(nMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_us(MP_Task mpTask)
{
ENTRY("vThreadSleep_us"_Str);
  
  i8 nMicroseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMicroseconds) )
    THROW_INVALID_DSTACK("sleep_us"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepMicro(nMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ns(MP_Task mpTask)
{
ENTRY("vThreadSleep_ns"_Str);
  
  i8 nNanoseconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nNanoseconds) )
    THROW_INVALID_DSTACK("sleep_ns"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepNano(nNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_actor -- spwan os thread associated with this thread          */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Actor(MP_Task mpTask)
{
  ENTRY("vThreadSpawn_Actor"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_actor"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Actor();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_partition -- spwan os thread associated with this thread      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Partition(MP_Task mpTask)
{
ENTRY("vThreadSpawn_Partition"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_partition"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Partition();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_general -- spwan os thread associated with this thread        */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_General(MP_Task mpTask)
{
  ENTRY("vThreadSpawn_General"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_general"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_General();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_worker -- spwan os thread associated with this thread         */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Worker(MP_Task mpTask)
{
  ENTRY("vThreadSpawn_Worker"_Str);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_worker"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Actor();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Thread(void)
{
  vAddPrimitive("thread.actorModel@"_Stz,     &vThreadActorModelFetch,       false);
  vAddPrimitive("thread.actorModel!"_Stz,     &vThreadActorModelStore,       false);
  vAddPrimitive("thread.append"_Stz,          &vThreadAppend,          false);
  vAddPrimitive("thread.bind"_Stz,            &vThreadBind,            false);
  vAddPrimitive("thread.condSignal"_Stz,      &vThreadCondSignal,      false);
  vAddPrimitive("thread.condWait"_Stz,        &vThreadCondWait,        false);
  vAddPrimitive("thread.cpu"_Stz,             &vThreadCpu,             false);
  vAddPrimitive("thread.dump"_Stz,            &vThreadDump,            false);
  vAddPrimitive("thread.init"_Stz,            &vThreadInit,            false);
  vAddPrimitive("thread.id"_Stz,              &vThreadId,              false);
  vAddPrimitive("thread.kind@"_Stz,           &vThreadKindFetch,       false);
  vAddPrimitive("thread.kind!"_Stz,           &vThreadKindStore,       false);
  vAddPrimitive("thread.mutexLock"_Stz,       &vThreadMutexLock,       false);
  vAddPrimitive("thread.mutexUnlock"_Stz,     &vThreadMutexUnlock,     false);
  vAddPrimitive("thread.name@"_Stz,           &vThreadNameFetch,       false);
  vAddPrimitive("thread.name!"_Stz,           &vThreadNameStore,       false);
  vAddPrimitive("thread.scheduler@"_Stz,      &vThreadSchedulerFetch,  false);
  vAddPrimitive("thread.scheduler!"_Stz,      &vThreadSchedulerStore,  false);
  vAddPrimitive("thread.taskqueue@"_Stz,      &vThreadTaskQueueFetch,  false);
  vAddPrimitive("thread.taskqueue!"_Stz,      &vThreadTaskQueueStore,  false);
  vAddPrimitive("thread.taskset@"_Stz,        &vThreadSfetch,          false);
  vAddPrimitive("thread.taskset!"_Stz,        &vThreadSstore,          false);
  vAddPrimitive("thread.sleep_s"_Stz,         &vThreadSleep_s,         false);
  vAddPrimitive("thread.sleep_ms"_Stz,        &vThreadSleep_ms,        false);
  vAddPrimitive("thread.sleep_us"_Stz,        &vThreadSleep_us,        false);
  vAddPrimitive("thread.sleep_ns"_Stz,        &vThreadSleep_ns,        false);
  vAddPrimitive("thread.spawn_drone"_Stz,     &vThreadSpawn_Drone,     false);
  vAddPrimitive("thread.spawn_partition"_Stz, &vThreadSpawn_Partition, false);
  vAddPrimitive("thread.spawn_server"_Stz,    &vThreadSpawn_Server,    false);
  vAddPrimitive("thread.spawn_worker"_Stz,    &vThreadSpawn_Worker,    false);
  vAddPrimitive("thread.task@"_Stz,           &vThreadTaskFetch,       false);
  vAddPrimitive("thread.task!"_Stz,           &vThreadTaskStore,       false);
}

/*----------------------------------------------------------------------------*/
/* thread.actorModel@ -- fetch thread actor model from thread on stack        */
/*----------------------------------------------------------------------------*/
void Task::vThreadActorModelFetch(MP_Task mpTask)
{
  ENTRY("vThreadActorModelFetch"_Stz);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.actorModel@"_Stz);
  Thread* pThread = _pThread(pThreadEmb);
  
  mpTask->m_pDataStack->vPushInt(pThread->m_eActorModel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.actorModel! -- store thread kind into thread on stack               */
/*----------------------------------------------------------------------------*/
void Task::vThreadActorModelStore(MP_Task mpTask)
{
  ENTRY("vThreadActorModelStore"_Stz);
  
  Object* pThreadEmb;
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.actorModel@"_Stz);
  Thread* pThread = _pThread(pThreadEmb);
  
  i8 eActorModel;
  if ( ! mpTask->m_pDataStack->bPop(eActorModel) )
    THROW_INVALID_DSTACK("thread.actorModel!"_Stz);
  
  pThread->m_eActorModel = (eAC)eActorModel;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(MP_Task mpTask)
{
ENTRY("vThreadAppend"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.append"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
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
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(MP_Task mpTask)
{
ENTRY("vThreadInit"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.init"_Stz);
  
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
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(MP_Task mpTask)
{
ENTRY("vThreadNameFetch"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name@"_Stz);
  
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
  ENTRY("vThreadSchedulerFetch"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.scheduler@"_Stz);
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
  ENTRY("vThreadSchedulerStore"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.scheduler!"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pSchedulerEmb;
  if ( ! mpTask->m_pDataStack->bPop(pSchedulerEmb) )
    THROW_INVALID_DSTACK("thread.scheduler!"_Stz);
  
  Scheduler* pScheduler = _pScheduler(pSchedulerEmb);
  
  pThread->m_pScheduler = pScheduler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(MP_Task mpTask)
{
ENTRY("vThreadNameStore"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name!"_Stz);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWrk xNameWsp;
  if ( ! mpTask->m_pDataStack->bPop(xNameWsp) )
    THROW_INVALID_DSTACK("thread.name!"_Stz);
  
  sName = _pStrVal(xNameWsp.pNestedValueSelf());
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskqueue@ -- fetch taskqueue from thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadTaskQueueFetch(MP_Task mpTask)
{
  ENTRY("vThreadTaskQueueFetch"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskqueue@"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskList* pTaskQueue = pThread->m_pTaskQueue;
  
  Object* pTaskQueueEmb = static_cast<Object*>(pTaskQueue);
  
  mpTask->m_pDataStack->vPushObject(pTaskQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskqueue! -- store taskqueue into thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadTaskQueueStore(MP_Task mpTask)
{
  ENTRY("vThreadTaskQueueStore"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskqueue!"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskQueueEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pTaskQueueEmb) )
    THROW_INVALID_DSTACK("thread.taskqueue!"_Stz);
  
  ASSERT(CN_TASKLIST == pTaskQueueEmb->eClass());
  
  TaskList* pTaskQueue = static_cast<TaskList*>(pTaskQueueEmb);
  
  pThread->m_pTaskQueue = pTaskQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(MP_Task mpTask)
{
  ENTRY("vThreadSfetch"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset@"_Stz);
  
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
ENTRY("vThreadSstore"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset!"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pTaskSetEmb) )
    THROW_INVALID_DSTACK("thread.taskset!"_Stz);
  
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
ENTRY("vThreadSleep_s"_Stz);
  
  i8 nSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleep_s"_Stz);
  
  Thread* pThread = Thread::pThread();
  
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
  
  Thread* pThread = Thread::pThread();
  
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
  
  Thread* pThread = Thread::pThread();
  
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
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepNano(nNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_drone -- spawn os thread associated with this thread          */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Drone(MP_Task mpTask)
{
  ENTRY("vThreadSpawn_Drone"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_actor"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Drone();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_partition -- spawn os thread associated with this thread      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Partition(MP_Task mpTask)
{
ENTRY("vThreadSpawn_Partition"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_partition"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Partition();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_server -- spawn os thread associated with this thread         */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Server(MP_Task mpTask)
{
  ENTRY("vThreadSpawn_Server"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_server"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Server();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn_worker -- spawn os thread associated with this thread         */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn_Worker(MP_Task mpTask)
{
  ENTRY("vThreadSpawn_Worker"_Stz);
  
  Object* pThreadEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn_worker"_Stz);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn_Worker();
  
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

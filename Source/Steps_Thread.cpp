/*============================================================================*/
/* Steps_Thread.cpp - thread step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Thread(void)
{
  vAddPrimitive("thread.append"_Str,      &vThreadAppend,      false);
  vAddPrimitive("thread.bind"_Str,        &vThreadBind,        false);
  vAddPrimitive("thread.condSignal"_Str,  &vThreadCondSignal,  false);
  vAddPrimitive("thread.condWait"_Str,    &vThreadCondWait,    false);
  vAddPrimitive("thread.cpu"_Str,         &vThreadCpu,         false);
  vAddPrimitive("thread.dump"_Str,        &vThreadDump,        false);
  vAddPrimitive("thread.init"_Str,        &vThreadInit,        false);
  vAddPrimitive("thread.id"_Str,          &vThreadId,          false);
  vAddPrimitive("thread.mutexLock"_Str,   &vThreadMutexLock,   false);
  vAddPrimitive("thread.mutexUnlock"_Str, &vThreadMutexUnlock, false);
  vAddPrimitive("thread.name@"_Str,       &vThreadNameFetch,   false);
  vAddPrimitive("thread.name!"_Str,       &vThreadNameStore,   false);
  vAddPrimitive("thread.taskset@"_Str,    &vThreadSfetch,      false);
  vAddPrimitive("thread.taskset!"_Str,    &vThreadSstore,      false);
  vAddPrimitive("thread.sleep_s"_Str,     &vThreadSleep_s,     false);
  vAddPrimitive("thread.sleep_ms"_Str,    &vThreadSleep_ms,    false);
  vAddPrimitive("thread.sleep_us"_Str,    &vThreadSleep_us,    false);
  vAddPrimitive("thread.sleep_ns"_Str,    &vThreadSleep_ns,    false);
  vAddPrimitive("thread.spawn"_Str,       &vThreadSpawn,       false);
}

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(NNP_Task nnpTask)
{
ENTRY("vThreadAppend"_Str);
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.append"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(NNP_Task nnpTask)
{
ENTRY("vThreadBind"_Str);
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(iCpu) )
    THROW_INVALID_DSTACK("thread.bind"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.bind"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condSignal -- signal condition on threads mutex                     */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondSignal(NNP_Task nnpTask)
{
 ENTRY("vThreadCondSignal"_Str);
  
  Object* pThreadObject;
  if ( ! nnpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condSignal"_Str);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pPosixThread->vCondSignal();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condWait -- wait for cond on threads mutex                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondWait(NNP_Task nnpTask)
{
 ENTRY("vThreadCondWait"_Str);
  
  Object* pThreadObject;
  if ( ! nnpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condWait"_Str);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pPosixThread->vCondWait();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadCpu(NNP_Task nnpTask)
{
ENTRY("vThreadCpu"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.cpu"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_pDataStack->vPushInt(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(NNP_Task nnpTask)
{
ENTRY("vThreadDump"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.dump"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadId(NNP_Task nnpTask)
{
ENTRY("vThreadId"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.id"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_pDataStack->vPushInt(pThread->m_pPosixThread->m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(NNP_Task nnpTask)
{
ENTRY("vThreadInit"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
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
/* thread.mutexLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexLock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexLock"_Str);
  
  Object* pThreadObject;
  if ( ! nnpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexLock"_Str);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pPosixThread->vMutexLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexUnlock -- release a threads mutex lock                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexUnlock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexUnlock"_Str);

  Object* pThreadObject;
  if ( ! nnpTask->m_pDataStack->bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexUnlock"_Str);
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_pPosixThread->vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(NNP_Task nnpTask)
{
ENTRY("vThreadNameFetch"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name@"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  nnpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(NNP_Task nnpTask)
{
ENTRY("vThreadNameStore"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name!"_Str);
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWrk xNameWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xNameWsp) )
    THROW_INVALID_DSTACK("thread.name!"_Str);
  
  sName = _pStrVal(xNameWsp.pNestedValueSelf());
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(NNP_Task nnpTask)
{
ENTRY("vThreadSfetch"_Str);
 
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset@"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;

  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;

  nnpTask->m_pDataStack->vPushObject(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(NNP_Task nnpTask)
{
ENTRY("vThreadSstore"_Str);
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset!"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pTaskSetEmb) )
    THROW_INVALID_DSTACK("thread.taskset!"_Str);
  
  ASSERT(CN_TASKSET == pTaskSetEmb->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pTaskSetEmb);
  
  pThread->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_s -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_s(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_s"_Str);
  
  i8 nSeconds;
  
  if ( ! nnpTask->m_pDataStack->bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleep_s"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ms(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ms"_Str);
  
  i8 nMilliseconds;
  
  if ( ! nnpTask->m_pDataStack->bPop(nMilliseconds) )
    THROW_INVALID_DSTACK("sleep_ms"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepMilli(nMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_us(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_us"_Str);
  
  i8 nMicroseconds;
  
  if ( ! nnpTask->m_pDataStack->bPop(nMicroseconds) )
    THROW_INVALID_DSTACK("sleep_us"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepMicro(nMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ns(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ns"_Str);
  
  i8 nNanoseconds;
  
  if ( ! nnpTask->m_pDataStack->bPop(nNanoseconds) )
    THROW_INVALID_DSTACK("sleep_ns"_Str);
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_pPosixThread->vSleepNano(nNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn -- spwan os thread associated with this thread                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn(NNP_Task nnpTask)
{
ENTRY("vThreadSpawn"_Str);
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn"_Str);
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

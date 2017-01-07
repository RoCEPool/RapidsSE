/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("thread.append",      &vThreadAppend,      false);
  vAddPrimitive("thread.bind",        &vThreadBind,        false);
  vAddPrimitive("thread.condSignal",  &vThreadCondSignal,  false);
  vAddPrimitive("thread.condWait",    &vThreadCondWait,    false);
  vAddPrimitive("thread.cpu",         &vThreadCpu,         false);
  vAddPrimitive("thread.dump",        &vThreadDump,        false);
  vAddPrimitive("thread.init",        &vThreadInit,        false);
  vAddPrimitive("thread.id",          &vThreadId,          false);
  vAddPrimitive("thread.mutexLock",   &vThreadMutexLock,   false);
  vAddPrimitive("thread.mutexUnlock", &vThreadMutexUnlock, false);
  vAddPrimitive("thread.name@",       &vThreadNameFetch,   false);
  vAddPrimitive("thread.name!",       &vThreadNameStore,   false);
  vAddPrimitive("thread.taskset@",    &vThreadSfetch,      false);
  vAddPrimitive("thread.taskset!",    &vThreadSstore,      false);
  vAddPrimitive("thread.sleep_s",     &vThreadSleep_s,     false);
  vAddPrimitive("thread.sleep_ms",    &vThreadSleep_ms,    false);
  vAddPrimitive("thread.sleep_us",    &vThreadSleep_us,    false);
  vAddPrimitive("thread.sleep_ns",    &vThreadSleep_ns,    false);
  vAddPrimitive("thread.spawn",       &vThreadSpawn,       false);
}

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(NNP_Task nnpTask)
{
ENTRY("vThreadAppend");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.append");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(NNP_Task nnpTask)
{
ENTRY("vThreadBind");
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(iCpu) )
    THROW_INVALID_DSTACK("thread.bind");
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.bind");
  
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
 ENTRY("vThreadCondSignal");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condSignal");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vCondSignal();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condWait -- wait for cond on threads mutex                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondWait(NNP_Task nnpTask)
{
 ENTRY("vThreadCondWait");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condWait");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vCondWait();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadCpu(NNP_Task nnpTask)
{
ENTRY("vThreadCpu");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.cpu");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_xDataStack.vPush(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(NNP_Task nnpTask)
{
ENTRY("vThreadDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.dump");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadId(NNP_Task nnpTask)
{
ENTRY("vThreadId");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.id");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_xDataStack.vPush(pThread->m_xPosixThread.m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(NNP_Task nnpTask)
{
ENTRY("vThreadInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.init");
  
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
ENTRY("vThreadMutexLock");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexLock");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vMutexLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexUnlock -- release a threads mutex lock                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexUnlock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexUnlock");

  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexUnlock");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(NNP_Task nnpTask)
{
ENTRY("vThreadNameFetch");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name@");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  nnpTask->m_xDataStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(NNP_Task nnpTask)
{
ENTRY("vThreadNameStore");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name!");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWsp xNameWsp;
  if ( ! nnpTask->m_xDataStack.bPop(xNameWsp) )
    THROW_INVALID_DSTACK("thread.name!");
  
  sName = xNameWsp.sStr();
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(NNP_Task nnpTask)
{
ENTRY("vThreadSfetch");
 
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset@");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;

  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;

  nnpTask->m_xDataStack.vPush(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(NNP_Task nnpTask)
{
ENTRY("vThreadSstore");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset!");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pTaskSetEmb) )
    THROW_INVALID_DSTACK("thread.taskset!");
  
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
ENTRY("vThreadSleep_s");
  
  i8 nSeconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleep_s");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ms(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ms");
  
  i8 nMilliseconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nMilliseconds) )
    THROW_INVALID_DSTACK("sleep_ms");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMilli(nMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_us(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_us");
  
  i8 nMicroseconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nMicroseconds) )
    THROW_INVALID_DSTACK("sleep_us");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMicro(nMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ns(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ns");
  
  i8 nNanoseconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nNanoseconds) )
    THROW_INVALID_DSTACK("sleep_ns");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepNano(nNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn -- spwan os thread associated with this thread                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn(NNP_Task nnpTask)
{
ENTRY("vThreadSpawn");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern Scheduler*      gpScheduler;

extern bool            gbTracepoint;

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Task(void)
{
  vAddPrimitive("task.ack"_Str,       &vTaskAck,           false);
  vAddPrimitive("task.acks@"_Str,     &vTaskAcksFetch,     false);
  vAddPrimitive("task.acks!"_Str,     &vTaskAcksStore,     false);
  vAddPrimitive("task.comment@"_Str,  &vTaskCommentFetch,  false);
  vAddPrimitive("task.comment!"_Str,  &vTaskCommentStore,  false);
  vAddPrimitive("task.delete"_Str,    &vTaskDelete,        false);
  vAddPrimitive("task.dicStack"_Str,  &vTaskDicStack,      false);
  vAddPrimitive("task.dump"_Str,      &vTaskDump,          false);
  vAddPrimitive("task.init"_Str,      &vTaskInit,          false);
  vAddPrimitive("task.jobQueue@"_Str, &vTaskJobQueueFetch, false);
  vAddPrimitive("task.jobQueue!"_Str, &vTaskJobQueueStore, false);
  vAddPrimitive("task.name@"_Str,     &vTaskNameFetch,     false);
  vAddPrimitive("task.name!"_Str,     &vTaskNameStore,     false);
  vAddPrimitive("task::new"_Str,      &vTaskNew,           false);
  vAddPrimitive("task.parent@"_Str,   &vTaskParentFetch,   false);
  vAddPrimitive("task.parent!"_Str,   &vTaskParentStore,   false);
  vAddPrimitive("task.plan@"_Str,     &vTaskPlanFetch,     false);
  vAddPrimitive("task.plan!"_Str,     &vTaskPlanStore,     false);
  vAddPrimitive("task.request"_Str,   &vTaskRequest,       false);
  vAddPrimitive("task.seed"_Str,      &vTaskSeed,          false);
  vAddPrimitive("task.seitf@"_Str,    &vTaskSeitfFetch,    false);
  vAddPrimitive("task.seitf!"_Str,    &vTaskSeitfStore,    false);
  vAddPrimitive("task.socket@"_Str,   &vTaskSocketFetch,   false);
  vAddPrimitive("task.socket!"_Str,   &vTaskSocketStore,   false);
  vAddPrimitive("task.state@"_Str,    &vTaskStateFetch,    false);
  vAddPrimitive("task.state!"_Str,    &vTaskStateStore,    false);
  vAddPrimitive("task.taskset@"_Str,  &vTaskTaskSetFetch,  false);
  vAddPrimitive("task.taskset!"_Str,  &vTaskTaskSetStore,  false);
  vAddPrimitive("task::this"_Str,     &vTaskThis,          false);
  vAddPrimitive("task.thread"_Str,    &vTaskThread,        false);
  vAddPrimitive("task.wait"_Str,      &vTaskWait,          false);
  vAddPrimitive("task.yield"_Str,     &vTaskYield,         false);
}

/*----------------------------------------------------------------------------*/
/* ack -- acknowledge that task of this tasks completion                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskAck(MP_Task mpTask)
{
  ENTRY("vTaskAck"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iMore = --pThatTask->m_zAcks;
  
  if ( ! iMore ) {
    //    vTrace("Task::vTaskSignal: task %p %s resuming task %p %s\n"_Str,
    //           *mpTask,mpTask->m_sName.s(),
    //           pThatTask,pThatTask->m_sName.s());
    if (gbTracepoint)
      vTrace("[N%lld:%8s\\%lld %-11s] ack resumes %s\\%lld\n"_Str,
             gpGrid->iThisNode(),
             mpTask->m_sName.s(),
             *mpTask->m_zAcks,
             mpTask->m_sComment.s(),
             
             pThatTask->m_sName.s(),
             *pThatTask->m_zAcks);
    
    while (TS_WAITING_ACK != pThatTask->m_eState)
      PosixThread::vSleepMicro(10);
    
    //Thread* pThread = Thread::pThread();
    //pThread->m_pTaskSet->vAppend(pThatTask);
    
    gpScheduler->vEnqueueTask_Mutex(pThatTask);

  }
  else {
    //    vTrace("Task::vTaskSignal: task %p %s signalled task %p\\%lld %s\n"_Str,
    //           *mpTask,mpTask->m_sName.s(),
    //           pThatTask,iMore,pThatTask->m_sName.s());
//    vTrace("[N%lld:%8s\\%lld %-11s] ack'd %s\\%lld\n"_Str,
//           gpGrid->iThisNode(),
//           mpTask->m_sName.s(),
//           *mpTask->m_zAcks,
//           mpTask->m_sComment.s(),
//           
//           pThatTask->m_sName.s(),
//           iMore);
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks@ -- push current value of given tasks m_zAcks                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksFetch(MP_Task mpTask)
{
  ENTRY("vTaskAcksFetch"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks@"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  mpTask->m_pDataStack->vPushInt(*pGivenTask->m_zAcks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_zAcks                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(MP_Task mpTask)
{
  ENTRY("vTaskAcksStore"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 iInt;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iInt) )
    THROW_INVALID_DSTACK("task.acks!"_Str);
  
  ASSERT(0 == *pGivenTask->m_zAcks);

  pGivenTask->m_zAcks = iInt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment@ -- push comment of given task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentFetch(MP_Task mpTask)
{
  ENTRY("vTaskCommentFetch"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment@"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  mpTask->m_pDataStack->vPushS32(pGivenTask->m_sComment);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment! -- store comment into given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentStore(MP_Task mpTask)
{
  ENTRY("vTaskCommentStore"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment!"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  s32 sComment;
  if ( ! mpTask->m_pDataStack->bPop(sComment) )
    THROW_INVALID_DSTACK("task.comment!"_Str);
  
  pGivenTask->m_sComment = sComment;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.delete -- delete task on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskDelete(MP_Task mpTask)
{
  ENTRY("vTaskDelete"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Str);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  delete pThatTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskDicStack(MP_Task mpTask)
{
  ENTRY("vTaskDicStack"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Str);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_xPathStack;
  
  mpTask->m_pDataStack->vPushObject(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(MP_Task mpTask)
{
ENTRY("vTaskDump"_Str);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Str);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(MP_Task mpTask)
{
ENTRY("vTaskInit"_Str);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Str);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  new(pThatTask) Task;
  
  Word* pWordThread = Word::pToWord(pThatTask);
  
  s32 sName;
  sName = pWordThread->sName();
  
  pThatTask->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.jobQueue@ -- push job queue of given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskJobQueueFetch(MP_Task mpTask)
{
  ENTRY("vTaskJobQueueFetch"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.jobQueue@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Queue* pQueue = pThatTask->m_pJobQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  mpTask->m_pDataStack->vPushObject(pQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.jobQueue! -- store job quueue into given task                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskJobQueueStore(MP_Task mpTask)
{
  ENTRY("vTaskJobQueueStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.jobQueue!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pQueueEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pQueueEmb) )
    THROW_INVALID_DSTACK("task.jobQueue!"_Str);
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pJobQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(MP_Task mpTask)
{
ENTRY("vTaskNameFetch"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  mpTask->m_pDataStack->vPushS32(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(MP_Task mpTask)
{
ENTRY("vTaskNameStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("task.name!"_Str);
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::new -- create and seed a new task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(MP_Task mpTask)
{
ENTRY("vTaskNew"_Str);
  //                                   pParent   sSeed     nTaskArgs
  if ( ! mpTask->m_pDataStack->bGuard(CN_PTRVAL,CN_STRVAL,CN_INTVAL) )
    THROW_INVALID_DSTACK("task::new"_Str);
  
  Object* pParent;
  if ( ! mpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task::new"_Str);
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  s32 sSeed;
  if ( ! mpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task::new"_Str);
  
  i8 nTaskArgs;
  if ( ! mpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task::new"_Str);
  //ASSERT(! nTaskArgs);

  if ( ! mpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task::new"_Str);
  
  // create new task (will be automatically appended to task list)
  MP_Task nnpNewTask = new Task;
  
  nnpNewTask->m_pParentTask = pParentTask;
  nnpNewTask->vSeedWord(sSeed);
  
  // copy task arguments to new task (preserving stack order)
  ValWrk xValWrk;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    mpTask->m_pDataStack->vPeekValWrk(iTaskArg,xValWrk);
    nnpNewTask->m_pDataStack->vPushValWrk(xValWrk);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = mpTask->m_pDataStack->bPop(xValWrk);
    ASSERT(bPopped);
  }
  
  Task* pNewTask = *nnpNewTask;
  
  Object* pTask = pNewTask;
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent@ -- fetch parent of task on data stack                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentFetch(MP_Task mpTask)
{
  ENTRY("vTaskParentFetch"_Str);
  
  mpTask->m_pDataStack->vPushObject(mpTask->m_pParentTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent! -- store parent task into this task                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentStore(MP_Task mpTask)
{
  ENTRY("vTaskParentStore"_Str);
  
  Object* pParent;
  if ( ! mpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task.parent!"_Str);
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  mpTask->m_pParentTask = pParentTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan@ -- fetch plan of task onto data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanFetch(MP_Task mpTask)
{
  ENTRY("vTaskPlanFetch"_Str);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.plan@"_Str);
  Task* pThatTask = _pTask(pObject);
  
  Plan* pPlan = pThatTask->m_pPlan;
  
  Object* pPtrPlan= pPlan;
  
  mpTask->m_pDataStack->vPushObject(pPtrPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan! -- store plan on data stack into task on data stack             */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanStore(MP_Task mpTask)
{
  ENTRY("vTaskPlanStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.plan!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("task.plan!"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pThatTask->m_pPlan = pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.request -- get request value set from task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskRequest(MP_Task mpTask)
{
  ENTRY("vTaskRequest"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  mpTask->m_pDataStack->vPushValue(pThatTask->pRequest());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(MP_Task mpTask)
{
ENTRY("vTaskSeed"_Str);
  
//  //                                   sSeed     nTaskArgs
//  if ( ! mpTask->m_pDataStack->bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_DSTACK("task.seed"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! mpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  pThatTask->vSeedWord(sSeed);

  i8 nTaskArgs;
  if ( ! mpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  //ASSERT(! nTaskArgs);
  
  if ( ! mpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  
  // copy task arguments to new task (preserving stack order)
  ValWrk xValWrk;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    mpTask->m_pDataStack->vPeekValWrk(iTaskArg,xValWrk);
    pThatTask->m_pDataStack->vPushValWrk(xValWrk);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = mpTask->m_pDataStack->bPop(xValWrk);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf@ -- fetch SEitf of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfFetch(MP_Task mpTask)
{
  ENTRY("vTaskSeitfFetch"_Str);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.seitf@"_Str);
  Task* pThatTask = _pTask(pObject);
  
  SEitf* pSEitf = pThatTask->m_pSEitf;
  
  Object* pPtrSEitf= pSEitf;
  
  mpTask->m_pDataStack->vPushObject(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf! -- store SEitf on data stack into task on data stack           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfStore(MP_Task mpTask)
{
  ENTRY("vTaskSeitfStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seitf!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSEitf;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("task.seitf!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket@ -- fetch socket of task on data stack                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketFetch(MP_Task mpTask)
{
  ENTRY("vTaskSocketFetch"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Socket* pSocket = pThatTask->m_pSocket;
  
  Object* pSocketEmb = static_cast<Object*>(pSocket);
  
  mpTask->m_pDataStack->vPushObject(pSocketEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket! -- store socket on data stack into task                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketStore(MP_Task mpTask)
{
  ENTRY("vTaskSocketStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! mpTask->m_pDataStack->bPop(ptrSocket) )
    THROW_INVALID_DSTACK("task.socket!"_Str);
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(MP_Task mpTask)
{
ENTRY("vTaskStateFetch"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  mpTask->m_pDataStack->vPushInt((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(MP_Task mpTask)
{
ENTRY("vTaskStateStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! mpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("task.state!"_Str);
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset@ -- fetch taskset of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetFetch(MP_Task mpTask)
{
  ENTRY("vTaskTaskSetFetch"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  TaskSet* pTaskSet = pThatTask->m_pTaskSet;
  
  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);
  
  mpTask->m_pDataStack->vPushObject(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset! -- store taskset on data stack into task                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetStore(MP_Task mpTask)
{
  ENTRY("vTaskTaskSetStore"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrTaskSet;
  
  if ( ! mpTask->m_pDataStack->bPop(ptrTaskSet) )
    THROW_INVALID_DSTACK("task.taskset!"_Str);
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(ptrTaskSet);
  
  pThatTask->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(MP_Task mpTask)
{
ENTRY("vTaskThis"_Str);
  
  Object* pTask = *mpTask;
  
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(MP_Task mpTask)
{
  ENTRY("vTaskThread"_Str);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread"_Str);
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  mpTask->m_pDataStack->vPushObject(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskWait(MP_Task mpTask)
{
  ENTRY("vTaskWait"_Str);
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(MP_Task mpTask)
{
  ENTRY("vTaskYield"_Str);
  
  mpTask->vState(TS_YIELDED);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

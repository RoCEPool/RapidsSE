/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
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
  vAddPrimitive("task.dicStack"_Str,  &vTaskDicStack,      false);
  vAddPrimitive("task.dump"_Str,      &vTaskDump,          false);
  vAddPrimitive("task.init"_Str,      &vTaskInit,          false);
  vAddPrimitive("task.jobQueue@"_Str, &vTaskJobQueueFetch, false);
  vAddPrimitive("task.jobQueue!"_Str, &vTaskJobQueueStore, false);
  vAddPrimitive("task.name@"_Str,     &vTaskNameFetch,     false);
  vAddPrimitive("task.name!"_Str,     &vTaskNameStore,     false);
  vAddPrimitive("task.new"_Str,       &vTaskNew,           false);
  vAddPrimitive("task.parent@"_Str,   &vTaskParentFetch,   false);
  vAddPrimitive("task.parent!"_Str,   &vTaskParentStore,   false);
  vAddPrimitive("task.plan@"_Str,     &vTaskPlanFetch,     false);
  vAddPrimitive("task.plan!"_Str,     &vTaskPlanStore,     false);
  vAddPrimitive("task.seed"_Str,      &vTaskSeed,          false);
  vAddPrimitive("task.seitf@"_Str,    &vTaskSeitfFetch,    false);
  vAddPrimitive("task.seitf!"_Str,    &vTaskSeitfStore,    false);
  vAddPrimitive("task.socket@"_Str,   &vTaskSocketFetch,   false);
  vAddPrimitive("task.socket!"_Str,   &vTaskSocketStore,   false);
  vAddPrimitive("task.state@"_Str,    &vTaskStateFetch,    false);
  vAddPrimitive("task.state!"_Str,    &vTaskStateStore,    false);
  vAddPrimitive("task.taskset@"_Str,  &vTaskTaskSetFetch,  false);
  vAddPrimitive("task.taskset!"_Str,  &vTaskTaskSetStore,  false);
  vAddPrimitive("task.this"_Str,      &vTaskThis,          false);
  vAddPrimitive("task.thread"_Str,    &vTaskThread,        false);
  vAddPrimitive("task.wait"_Str,      &vTaskWait,          false);
  vAddPrimitive("task.yield"_Str,     &vTaskYield,         false);
}

/*----------------------------------------------------------------------------*/
/* ack -- acknowledge that task of this tasks completion                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskAck(NNP_Task nnpTask)
{
  ENTRY("vTaskAck"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iMore = --pThatTask->m_zAcks;
  
  if ( ! iMore ) {
    //    vTrace("Task::vTaskSignal: task %p %s resuming task %p %s\n"_Str,
    //           *nnpTask,nnpTask->m_sName.aStr(),
    //           pThatTask,pThatTask->m_sName.aStr());
    if (gxArguments.m_bTracepoint)
      vTrace("[N%lld:%8s\\%lld %-11s] ack resumes %s\\%lld\n"_Str,
             gpGrid->iThisNode(),
             nnpTask->m_sName.aStr(),
             *nnpTask->m_zAcks,
             nnpTask->m_sComment.aStr(),
             
             pThatTask->m_sName.aStr(),
             *pThatTask->m_zAcks);
    
    while (TS_WAITING_ACK != pThatTask->m_eState)
      PosixThread::vSleepMicro(10);
    
    //Thread* pThread = Thread::pThread();
    //pThread->m_pTaskSet->vAppend(pThatTask);
    pThatTask->m_pTaskSet->vAppend(pThatTask);
    
  }
  else {
    //    vTrace("Task::vTaskSignal: task %p %s signalled task %p\\%lld %s\n"_Str,
    //           *nnpTask,nnpTask->m_sName.aStr(),
    //           pThatTask,iMore,pThatTask->m_sName.aStr());
//    vTrace("[N%lld:%8s\\%lld %-11s] ack'd %s\\%lld\n"_Str,
//           gpGrid->iThisNode(),
//           nnpTask->m_sName.aStr(),
//           *nnpTask->m_zAcks,
//           nnpTask->m_sComment.aStr(),
//           
//           pThatTask->m_sName.aStr(),
//           iMore);
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks@ -- push current value of given tasks m_zAcks                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksFetch"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks@"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_pDataStack->vPush(*pGivenTask->m_zAcks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_zAcks                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksStore"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 iInt;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iInt) )
    THROW_INVALID_DSTACK("task.acks!"_Str);
  
  ASSERT(0 == *pGivenTask->m_zAcks);

  pGivenTask->m_zAcks = iInt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment@ -- push comment of given task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentFetch"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment@"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_pDataStack->vPush(pGivenTask->m_sComment);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment! -- store comment into given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentStore"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment!"_Str);
  Task* pGivenTask = _pTask(pPtrTask);
  
  s32 sComment;
  if ( ! nnpTask->m_pDataStack->bPop(sComment) )
    THROW_INVALID_DSTACK("task.comment!"_Str);
  
  pGivenTask->m_sComment = sComment;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskDicStack(NNP_Task nnpTask)
{
  ENTRY("vTaskDicStack"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Str);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_xPathStack;
  
  nnpTask->m_pDataStack->vPush(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(NNP_Task nnpTask)
{
ENTRY("vTaskDump"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Str);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(NNP_Task nnpTask)
{
ENTRY("vTaskInit"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
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
void Task::vTaskJobQueueFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskJobQueueFetch"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.jobQueue@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Queue* pQueue = pThatTask->m_pJobQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  nnpTask->m_pDataStack->vPush(pQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.jobQueue! -- store job quueue into given task                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskJobQueueStore(NNP_Task nnpTask)
{
  ENTRY("vTaskJobQueueStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.jobQueue!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pQueueEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pQueueEmb) )
    THROW_INVALID_DSTACK("task.jobQueue!"_Str);
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pJobQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(NNP_Task nnpTask)
{
ENTRY("vTaskNameFetch"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  nnpTask->m_pDataStack->vPush(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(NNP_Task nnpTask)
{
ENTRY("vTaskNameStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("task.name!"_Str);
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.new -- create and seed a new task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(NNP_Task nnpTask)
{
ENTRY("vTaskNew"_Str);
  //                                   pParent   sSeed     nTaskArgs
  if ( ! nnpTask->m_pDataStack->bGuard(CN_PTRVAL,CN_STRVAL,CN_INTVAL) )
    THROW_INVALID_DSTACK("task.new"_Str);
  
  Object* pParent;
  if ( ! nnpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task.new"_Str);
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  s32 sSeed;
  if ( ! nnpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.new"_Str);
  
  i8 nTaskArgs;
  if ( ! nnpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.new"_Str);
  //ASSERT(! nTaskArgs);

  if ( ! nnpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.new"_Str);
  
  // create new task (will be automatically appended to task list)
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->m_pParentTask = pParentTask;
  nnpNewTask->vSeedWord(sSeed);
  
  // copy task arguments to new task (preserving stack order)
  ValWsp xValWsp;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    nnpTask->m_pDataStack->vPeek(iTaskArg,&xValWsp);
    nnpNewTask->m_pDataStack->vPush(xValWsp);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = nnpTask->m_pDataStack->bPop(xValWsp);
    ASSERT(bPopped);
  }
  
  Task* pNewTask = *nnpNewTask;
  
  Object* pTask = pNewTask;
  nnpTask->m_pDataStack->vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent@ -- fetch parent of task on data stack                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskParentFetch"_Str);
  
  nnpTask->m_pDataStack->vPush(nnpTask->m_pParentTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent! -- store parent task into this task                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskParentStore"_Str);
  
  Object* pParent;
  if ( ! nnpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task.parent!"_Str);
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  nnpTask->m_pParentTask = pParentTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan@ -- fetch plan of task onto data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskPlanFetch"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.plan@"_Str);
  Task* pThatTask = _pTask(pObject);
  
  Plan* pPlan = pThatTask->m_pPlan;
  
  Object* pPtrPlan= pPlan;
  
  nnpTask->m_pDataStack->vPush(pPtrPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan! -- store plan on data stack into task on data stack             */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanStore(NNP_Task nnpTask)
{
  ENTRY("vTaskPlanStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.plan!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("task.plan!"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pThatTask->m_pPlan = pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(NNP_Task nnpTask)
{
ENTRY("vTaskSeed"_Str);
  
//  //                                   sSeed     nTaskArgs
//  if ( ! nnpTask->m_pDataStack->bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_DSTACK("task.seed"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! nnpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  pThatTask->vSeedWord(sSeed);

  i8 nTaskArgs;
  if ( ! nnpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  //ASSERT(! nTaskArgs);
  
  if ( ! nnpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Str);
  
  // copy task arguments to new task (preserving stack order)
  ValWsp xValWsp;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    nnpTask->m_pDataStack->vPeek(iTaskArg,&xValWsp);
    pThatTask->m_pDataStack->vPush(xValWsp);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = nnpTask->m_pDataStack->bPop(xValWsp);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf@ -- fetch SEitf of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskSeitfFetch"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.seitf@"_Str);
  Task* pThatTask = _pTask(pObject);
  
  SEitf* pSEitf = pThatTask->m_pSEitf;
  
  Object* pPtrSEitf= pSEitf;
  
  nnpTask->m_pDataStack->vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf! -- store SEitf on data stack into task on data stack           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfStore(NNP_Task nnpTask)
{
  ENTRY("vTaskSeitfStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seitf!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("task.seitf!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket@ -- fetch socket of task on data stack                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskSocketFetch"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Socket* pSocket = pThatTask->m_pSocket;
  
  Object* pSocketEmb = static_cast<Object*>(pSocket);
  
  nnpTask->m_pDataStack->vPush(pSocketEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket! -- store socket on data stack into task                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketStore(NNP_Task nnpTask)
{
  ENTRY("vTaskSocketStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! nnpTask->m_pDataStack->bPop(ptrSocket) )
    THROW_INVALID_DSTACK("task.socket!"_Str);
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(NNP_Task nnpTask)
{
ENTRY("vTaskStateFetch"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  nnpTask->m_pDataStack->vPush((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(NNP_Task nnpTask)
{
ENTRY("vTaskStateStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! nnpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("task.state!"_Str);
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset@ -- fetch taskset of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetFetch"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset@"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  TaskSet* pTaskSet = pThatTask->m_pTaskSet;
  
  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);
  
  nnpTask->m_pDataStack->vPush(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset! -- store taskset on data stack into task                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetStore(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetStore"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset!"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrTaskSet;
  
  if ( ! nnpTask->m_pDataStack->bPop(ptrTaskSet) )
    THROW_INVALID_DSTACK("task.taskset!"_Str);
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(ptrTaskSet);
  
  pThatTask->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(NNP_Task nnpTask)
{
ENTRY("vTaskThis"_Str);
  
  Object* pTask = *nnpTask;
  
  nnpTask->m_pDataStack->vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(NNP_Task nnpTask)
{
  ENTRY("vTaskThread"_Str);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread"_Str);
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  nnpTask->m_pDataStack->vPush(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskWait(NNP_Task nnpTask)
{
  ENTRY("vTaskWait"_Str);
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(NNP_Task nnpTask)
{
  ENTRY("vTaskYield"_Str);
  
  nnpTask->vState(TS_YIELDED);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

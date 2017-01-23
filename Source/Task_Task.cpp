/*============================================================================*/
/* Task_Task.cpp - task task group                                            */
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
  vAddPrimitive("task.ack",        &vTaskAck,          false);
  vAddPrimitive("task.acks@",      &vTaskAcksFetch,    false);
  vAddPrimitive("task.acks!",      &vTaskAcksStore,    false);
  vAddPrimitive("task.comment@",   &vTaskCommentFetch, false);
  vAddPrimitive("task.comment!",   &vTaskCommentStore, false);
  vAddPrimitive("task.dicStack",   &vTaskDicStack,     false);
  vAddPrimitive("task.dump",       &vTaskDump,         false);
  vAddPrimitive("task.init",       &vTaskInit,         false);
  vAddPrimitive("task.name@",      &vTaskNameFetch,    false);
  vAddPrimitive("task.name!",      &vTaskNameStore,    false);
  vAddPrimitive("task.new",        &vTaskNew,          false);
  vAddPrimitive("task.parent@",    &vTaskParentFetch,  false);
  vAddPrimitive("task.parent!",    &vTaskParentStore,  false);
  vAddPrimitive("task.queue@",     &vTaskQfetch,       false);
  vAddPrimitive("task.queue!",     &vTaskQstore,       false);
  vAddPrimitive("task.seed",       &vTaskSeed,         false);
  vAddPrimitive("task.seitf@",     &vTaskSeitfFetch,   false);
  vAddPrimitive("task.seitf!",     &vTaskSeitfStore,   false);
  vAddPrimitive("task.socket@",    &vTaskSocketFetch,  false);
  vAddPrimitive("task.socket!",    &vTaskSocketStore,  false);
  vAddPrimitive("task.state@",     &vTaskStateFetch,   false);
  vAddPrimitive("task.state!",     &vTaskStateStore,   false);
  vAddPrimitive("task.taskset@",   &vTaskTaskSetFetch, false);
  vAddPrimitive("task.taskset!",   &vTaskTaskSetStore, false);
  vAddPrimitive("task.this",       &vTaskThis,         false);
  vAddPrimitive("task.thread",     &vTaskThread,       false);
  vAddPrimitive("task.wait",       &vTaskWait,         false);
  vAddPrimitive("task.yield",      &vTaskYield,        false);
}

/*----------------------------------------------------------------------------*/
/* ack -- acknowledge that task of this tasks completion                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskAck(NNP_Task nnpTask)
{
  ENTRY("vTaskAck");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack");
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iMore = --pThatTask->m_zAcks;
  
  if ( ! iMore ) {
    //    vTrace("Task::vTaskSignal: task %p %s resuming task %p %s\n",
    //           *nnpTask,nnpTask->m_sName.a(),
    //           pThatTask,pThatTask->m_sName.a());
    if (gxArguments.m_bTracepoint)
      vTrace("[N%lld:%8s\\%lld %-11s] ack resumes %s\\%lld\n",
             gpGrid->iThisNode(),
             nnpTask->m_sName.a(),
             *nnpTask->m_zAcks,
             nnpTask->m_sComment.a(),
             
             pThatTask->m_sName.a(),
             *pThatTask->m_zAcks);
    
    while (TS_WAITING_ACK != pThatTask->m_eState)
      PosixThread::vSleepMicro(10);
    
    //Thread* pThread = Thread::pThread();
    //pThread->m_pTaskSet->vAppend(pThatTask);
    pThatTask->m_pTaskSet->vAppend(pThatTask);
    
  }
  else {
    //    vTrace("Task::vTaskSignal: task %p %s signalled task %p\\%lld %s\n",
    //           *nnpTask,nnpTask->m_sName.a(),
    //           pThatTask,iMore,pThatTask->m_sName.a());
    vTrace("[N%lld:%8s\\%lld %-11s] ack'd %s\\%lld\n",
           gpGrid->iThisNode(),
           nnpTask->m_sName.a(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.a(),
           
           pThatTask->m_sName.a(),
           iMore);
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks@ -- push current value of given tasks m_zAcks                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks@");
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_pDataStack->vPush(*pGivenTask->m_zAcks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_zAcks                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksStore");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!");
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 iInt;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iInt) )
    THROW_INVALID_DSTACK("task.acks!");
  
  ASSERT(0 == *pGivenTask->m_zAcks);

  pGivenTask->m_zAcks = iInt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment@ -- push comment of given task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment@");
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_pDataStack->vPush(pGivenTask->m_sComment);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment! -- store comment into given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentStore");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment!");
  Task* pGivenTask = _pTask(pPtrTask);
  
  s32 sComment;
  if ( ! nnpTask->m_pDataStack->bPop(sComment) )
    THROW_INVALID_DSTACK("task.comment!");
  
  pGivenTask->m_sComment = sComment;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskDicStack(NNP_Task nnpTask)
{
  ENTRY("vTaskDicStack");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init");
  
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
ENTRY("vTaskDump");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump");
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(NNP_Task nnpTask)
{
ENTRY("vTaskInit");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init");
  
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
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(NNP_Task nnpTask)
{
ENTRY("vTaskNameFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@");
  Task* pThatTask = _pTask(pTaskObject);
  
  nnpTask->m_pDataStack->vPush(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(NNP_Task nnpTask)
{
ENTRY("vTaskNameStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!");
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("task.name!");
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.new -- create and seed a new task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(NNP_Task nnpTask)
{
ENTRY("vTaskNew");
  //                                   pParent   sSeed     nTaskArgs
  if ( ! nnpTask->m_pDataStack->bGuard(CN_PTRVAL,CN_STRVAL,CN_INTVAL) )
    THROW_INVALID_DSTACK("task.new");
  
  Object* pParent;
  if ( ! nnpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task.new");
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  s32 sSeed;
  if ( ! nnpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.new");
  char* aSeedWord = sSeed.a();
  
  i8 nTaskArgs;
  if ( ! nnpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.new");
  //ASSERT(! nTaskArgs);

  if ( ! nnpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.new");
  
  // create new task (will be automatically appended to task list)
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->m_pParentTask = pParentTask;
  nnpNewTask->vSeedWord(aSeedWord);
  
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
  ENTRY("vTaskParentFetch");
  
  nnpTask->m_pDataStack->vPush(nnpTask->m_pParentTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent! -- store parent task into this task                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskParentStore");
  
  Object* pParent;
  if ( ! nnpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task.parent!");
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  nnpTask->m_pParentTask = pParentTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue@ -- fetch queue of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQfetch(NNP_Task nnpTask)
{
ENTRY("vTaskQfetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.queue@");
  Task* pThatTask = _pTask(pTaskObject);
  
  Queue* pQueue = pThatTask->m_pQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  nnpTask->m_pDataStack->vPush(pQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue! -- store queue on data stack into task on data stack           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQstore(NNP_Task nnpTask)
{
ENTRY("vTaskQstore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.queue!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pQueueEmb;
  
  if ( ! nnpTask->m_pDataStack->bPop(pQueueEmb) )
    THROW_INVALID_DSTACK("task.queue!");
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(NNP_Task nnpTask)
{
ENTRY("vTaskSeed");
  
//  //                                   sSeed     nTaskArgs
//  if ( ! nnpTask->m_pDataStack->bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_DSTACK("task.seed");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed");
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! nnpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.seed");
  pThatTask->vSeedWord(sSeed.a());

  i8 nTaskArgs;
  if ( ! nnpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed");
  //ASSERT(! nTaskArgs);
  
  if ( ! nnpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed");
  
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
  ENTRY("vTaskSeitfFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.seitf@");
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
  ENTRY("vTaskSeitfStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seitf!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("task.seitf!");
  
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
  ENTRY("vTaskSocketFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket@");
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
  ENTRY("vTaskSocketStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! nnpTask->m_pDataStack->bPop(ptrSocket) )
    THROW_INVALID_DSTACK("task.socket!");
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(NNP_Task nnpTask)
{
ENTRY("vTaskStateFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@");
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
ENTRY("vTaskStateStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!");
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! nnpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("task.state!");
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset@ -- fetch taskset of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset@");
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
  ENTRY("vTaskTaskSetStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrTaskSet;
  
  if ( ! nnpTask->m_pDataStack->bPop(ptrTaskSet) )
    THROW_INVALID_DSTACK("task.taskset!");
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(ptrTaskSet);
  
  pThatTask->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(NNP_Task nnpTask)
{
ENTRY("vTaskThis");
  
  Object* pTask = *nnpTask;
  
  nnpTask->m_pDataStack->vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(NNP_Task nnpTask)
{
  ENTRY("vTaskThread");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread");
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
  ENTRY("vTaskWait");
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(NNP_Task nnpTask)
{
  ENTRY("vTaskYield");
  
  nnpTask->vState(TS_YIELDED);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

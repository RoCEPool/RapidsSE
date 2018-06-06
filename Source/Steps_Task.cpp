/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Task(void)
{
  vAddPrimitive("task.ack"_Stz,       &vTaskAck,           false);
  vAddPrimitive("task.acks@"_Stz,     &vTaskAcksFetch,     false);
  vAddPrimitive("task.acks!"_Stz,     &vTaskAcksStore,     false);
  vAddPrimitive("task.actorModel@"_Stz, &vTaskActorModelFetch, false);
  vAddPrimitive("task.actorModel!"_Stz, &vTaskActorModelStore, false);
  vAddPrimitive("task::breakPoint"_Stz, &vTaskBreakPoint,  false);
  vAddPrimitive("task.comment@"_Stz,  &vTaskCommentFetch,  false);
  vAddPrimitive("task.comment!"_Stz,  &vTaskCommentStore,  false);
  vAddPrimitive("task.delete"_Stz,    &vTaskDelete,        false);
  
  vAddPrimitive("task.destructorTrace@"_Stz,  &vTaskDestructorTraceFetch, false);
  vAddPrimitive("task.destructorTrace!"_Stz,  &vTaskDestructorTraceStore, false);
  
  vAddPrimitive("task.dicStack"_Stz,  &vTaskDicStack,      false);
  vAddPrimitive("task.dump"_Stz,      &vTaskDump,          false);
  vAddPrimitive("task.dropLock"_Stz,  &vTaskDropLock,      false);
  vAddPrimitive("task.grabLock"_Stz,  &vTaskGrabLock,      false);
  vAddPrimitive("task.init"_Stz,      &vTaskInit,          false);
  vAddPrimitive("task.jobQueue@"_Stz, &vTaskJobQueueFetch, false);
  vAddPrimitive("task.jobQueue!"_Stz, &vTaskJobQueueStore, false);
  vAddPrimitive("task.kind@"_Stz,     &vTaskKindFetch,     false);
  vAddPrimitive("task.kind!"_Stz,     &vTaskKindStore,     false);
  vAddPrimitive("task.name@"_Stz,     &vTaskNameFetch,     false);
  vAddPrimitive("task.name!"_Stz,     &vTaskNameStore,     false);
  vAddPrimitive("task::new"_Stz,      &vTaskNew,           false);
  vAddPrimitive("task.parent@"_Stz,   &vTaskParentFetch,   false);
  vAddPrimitive("task.parent!"_Stz,   &vTaskParentStore,   false);
  vAddPrimitive("task.partition@"_Stz,&vTaskPartitionFetch,false);
  vAddPrimitive("task.partition!"_Stz,&vTaskPartitionStore,false);
  vAddPrimitive("task.plan@"_Stz,     &vTaskPlanFetch,     false);
  vAddPrimitive("task.plan!"_Stz,     &vTaskPlanStore,     false);
  vAddPrimitive("task.request"_Stz,   &vTaskRequest,       false);
  vAddPrimitive("task.seed"_Stz,      &vTaskSeed,          false);
  vAddPrimitive("task.seitf@"_Stz,    &vTaskSeitfFetch,    false);
  vAddPrimitive("task.seitf!"_Stz,    &vTaskSeitfStore,    false);
  vAddPrimitive("task.sleep"_Stz,     &vTaskSleep,         false);
  vAddPrimitive("task.socket@"_Stz,   &vTaskSocketFetch,   false);
  vAddPrimitive("task.socket!"_Stz,   &vTaskSocketStore,   false);
  vAddPrimitive("task.state@"_Stz,    &vTaskStateFetch,    false);
  vAddPrimitive("task.state!"_Stz,    &vTaskStateStore,    false);
  vAddPrimitive("task.taskset@"_Stz,  &vTaskTaskSetFetch,  false);
  vAddPrimitive("task.taskset!"_Stz,  &vTaskTaskSetStore,  false);
  vAddPrimitive("task::this"_Stz,     &vTaskThis,          false);
  vAddPrimitive("task.thread"_Stz,    &vTaskThread,        false);
  vAddPrimitive("task.wait"_Stz,      &vTaskWait,          false);
  vAddPrimitive("task.yield"_Stz,     &vTaskYield,         false);
}

/*----------------------------------------------------------------------------*/
/* ack -- acknowledge that task of this tasks completion                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskAck(MP_Task mpTask)
{
  ENTRY("vTaskAck"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  if (pThatTask->bR35model()) {
    
    Handlers::vAckTaskHandler(pThatTask);
    
    EXIT();
    return;
  }
  
  i8 iMore = --pThatTask->m_zAcks;
  
  if (iMore) {
    //    vTrace("Task::vTaskSignal: task %p %s signalled task %p\\%lld %s\n"_Stz,
    //           *mpTask,mpTask->m_sName.aChar(),
    //           pThatTask,iMore,pThatTask->m_sName.aChar());
    //    vTrace("[N%lld:%8s\\%lld %-11s] ack'd %s\\%lld\n"_Stz,
    //           gpAttachMap->m_pGrid->iThisNode(),
    //           mpTask->m_sName.aChar(),
    //           *mpTask->m_zAcks,
    //           mpTask->m_sComment.aChar(),
    //
    //           pThatTask->m_sName.aChar(),
    //           iMore);
    EXIT();
    return;
  }
  
  
  //    vTrace("Task::vTaskSignal: task %p %s resuming task %p %s\n"_Stz,
  //           *mpTask,mpTask->m_sName.aChar(),
  //           pThatTask,pThatTask->m_sName.aChar());
  if (gpAttachMap->m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] ack resumes %s\\%lld\n"_Stz,
           gpAttachMap->m_pGrid->iThisNode(),
           mpTask->m_sName.aChar(),
           *mpTask->m_zAcks,
           mpTask->m_sComment.aChar(),
           
           pThatTask->m_sName.aChar(),
           *pThatTask->m_zAcks);
  
  while (TS_WAITING_ACK != pThatTask->m_eState)
    PosixThread::vSleepMicro(10);
  
  if (pThatTask->eActorModel() == AC_SERVER) {
    gpAttachMap->m_pServerScheduler->vEnqueueTask_Mutex(pThatTask);
  }
  else if (pThatTask->eActorModel() == AC_PARTITION) {
    gpAttachMap->m_pPartitionScheduler->vEnqueueTask_Mutex(pThatTask->m_iPartition,pThatTask);
  }
  else {
    Thread* pThread = pThatTask->pThread();
    pThread->m_pTaskSet->vAppend(pThatTask);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks@ -- push current value of given tasks m_zAcks                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksFetch(MP_Task mpTask)
{
  ENTRY("vTaskAcksFetch"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks@"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  mpTask->m_pDataStack->vPushInt(*pGivenTask->m_zAcks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_zAcks                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(MP_Task mpTask)
{
  ENTRY("vTaskAcksStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 nAcksRequired;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nAcksRequired) )
    THROW_INVALID_DSTACK("task.acks!"_Stz);
  
  ASSERT(0 == *pGivenTask->m_zAcks);

  if (pGivenTask->bR35model()) {
    RASSERT(pGivenTask->bIsLocked());
  }
  
  pGivenTask->m_zAcks = nAcksRequired;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.actorModel@ -- push actor model of given task                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskActorModelFetch(MP_Task mpTask)
{
  ENTRY("vTaskActorModelFetch"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.actorModel@"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  mpTask->m_pDataStack->vPushInt(pGivenTask->m_eActorModel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.actorModel! -- store actor model into given task                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskActorModelStore(MP_Task mpTask)
{
  ENTRY("vTaskActorModelStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.actorModel!"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 eActorModel;
  if ( ! mpTask->m_pDataStack->bPop(eActorModel) )
    THROW_INVALID_DSTACK("task.actorModel!"_Stz);
  
  pGivenTask->m_eActorModel = (eAC)eActorModel;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.breakPoint -- break point so we can examine task & thread state       */
/*----------------------------------------------------------------------------*/
void Task::vTaskBreakPoint(MP_Task mpTask)
{
  ENTRY("vTaskBreakPoint"_Stz);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment@ -- push comment of given task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentFetch(MP_Task mpTask)
{
  ENTRY("vTaskCommentFetch"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment@"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  mpTask->m_pDataStack->vPushS32(pGivenTask->m_sComment);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment! -- store comment into given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentStore(MP_Task mpTask)
{
  ENTRY("vTaskCommentStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment!"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  s32 sComment;
  if ( ! mpTask->m_pDataStack->bPop(sComment) )
    THROW_INVALID_DSTACK("task.comment!"_Stz);
  
  pGivenTask->m_sComment = sComment;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.delete -- delete task on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskDelete(MP_Task mpTask)
{
  ENTRY("vTaskDelete"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.delete"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  delete pThatTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.destructorTrace@ -- fetch destructor trace                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskDestructorTraceFetch(MP_Task mpTask)
{
  ENTRY("vTaskDestructorTraceFetch"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.destructorTrace@"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  mpTask->m_pDataStack->vPushInt(pThatTask->bDestructorTrace());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.destructorTrace! -- store destructor trace                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskDestructorTraceStore(MP_Task mpTask)
{
  ENTRY("vTaskDestructorTraceStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.destructorTrace!"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  i8 iDestructorTrace;
  if ( ! mpTask->m_pDataStack->bPop(iDestructorTrace) )
    THROW_INVALID_DSTACK("task.destructorTrace!"_Stz);
  
  pThatTask->vDestructorTrace(iDestructorTrace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskDicStack(MP_Task mpTask)
{
  ENTRY("vTaskDicStack"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_xPathStack;
  
  mpTask->m_pDataStack->vPushObject(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dropLock -- drop lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskDropLock(MP_Task mpTask)
{
  ENTRY("vTaskDrop"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(MP_Task mpTask)
{
ENTRY("vTaskDump"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.grabLock -- grab lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskGrabLock(MP_Task mpTask)
{
  ENTRY("vTaskGrab"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vGrabLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(MP_Task mpTask)
{
ENTRY("vTaskInit"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Stz);
  
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
/* task.kind@ -- push the kind of given task                                  */
/*----------------------------------------------------------------------------*/
void Task::vTaskKindFetch(MP_Task mpTask)
{
  ENTRY("vTaskKindFetch"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.kind@"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  mpTask->m_pDataStack->vPushInt(pGivenTask->m_eTaskKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.kind! -- store task kind into given task                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskKindStore(MP_Task mpTask)
{
  ENTRY("vTaskKindStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.kind!"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 eTaskKind;
  if ( ! mpTask->m_pDataStack->bPop(eTaskKind) )
    THROW_INVALID_DSTACK("task.kind!"_Stz);
  
  pGivenTask->m_eTaskKind = (eTK)eTaskKind;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.jobQueue@ -- push job queue of given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskJobQueueFetch(MP_Task mpTask)
{
  ENTRY("vTaskJobQueueFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.jobQueue@"_Stz);
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
  ENTRY("vTaskJobQueueStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.jobQueue!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pQueueEmb;
  
  if ( ! mpTask->m_pDataStack->bPop(pQueueEmb) )
    THROW_INVALID_DSTACK("task.jobQueue!"_Stz);
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pJobQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(MP_Task mpTask)
{
ENTRY("vTaskNameFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  mpTask->m_pDataStack->vPushS32(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(MP_Task mpTask)
{
ENTRY("vTaskNameStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("task.name!"_Stz);
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::new -- create and seed a new task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(MP_Task mpTask)
{
ENTRY("vTaskNew"_Stz);

  if ( ! mpTask->m_pDataStack->bGuard(4) )
    THROW_INVALID_DSTACK("task::new"_Stz);
  
  i8 eKind;
  if ( ! mpTask->m_pDataStack->bPop(eKind) )
    THROW_INVALID_DSTACK("task::new"_Stz);  //ASSERT(! nTaskArgs);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("task::new"_Stz);
  
  i8 eModel;
  if ( ! mpTask->m_pDataStack->bPop(eModel) )
    THROW_INVALID_DSTACK("task::new"_Stz);  //ASSERT(! nTaskArgs);
  
  Object* pParent;
  if ( ! mpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task::new"_Stz);
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  s32 sSeed;
  if ( ! mpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task::new"_Stz);
  
  i8 nTaskArgs;
  if ( ! mpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task::new"_Stz);
  //ASSERT(! nTaskArgs);

  if ( ! mpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task::new"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify spawn arguments
  
  // if we are a parent spawning a child there must be at least 1 ack remaining
  if (AC_R35 == eModel && pParentTask) {
    RASSERT(*pParentTask->m_zAcks);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create new task (will be automatically appended to task list)
  
  MP_Task nnpNewTask = new Task;
  
  nnpNewTask->m_sName = sName;
  nnpNewTask->m_eTaskKind = (eTK)eKind;
  nnpNewTask->m_eActorModel = (eAC)eModel;
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
  ENTRY("vTaskParentFetch"_Stz);
  
  mpTask->m_pDataStack->vPushObject(mpTask->m_pParentTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent! -- store parent task into this task                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentStore(MP_Task mpTask)
{
  ENTRY("vTaskParentStore"_Stz);
  
  Object* pParent;
  if ( ! mpTask->m_pDataStack->bPop(pParent) )
    THROW_INVALID_DSTACK("task.parent!"_Stz);
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  mpTask->m_pParentTask = pParentTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.partition@ -- fetch partition of task                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskPartitionFetch(MP_Task mpTask)
{
  ENTRY("vTaskPartitionFetch"_Stz);
  
  mpTask->m_pDataStack->vPushInt(mpTask->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.partition@ -- store partition of task                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskPartitionStore(MP_Task mpTask)
{
  ENTRY("vTaskPartitionStore"_Stz);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("task.partition@"_Stz);
  
  mpTask->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan@ -- fetch plan of task onto data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanFetch(MP_Task mpTask)
{
  ENTRY("vTaskPlanFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.plan@"_Stz);
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
  ENTRY("vTaskPlanStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.plan!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("task.plan!"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pThatTask->m_pPlan = pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.request -- get request value set from task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskRequest(MP_Task mpTask)
{
  ENTRY("vTaskRequest"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  mpTask->m_pDataStack->vPushValue(pThatTask->pRequest());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(MP_Task mpTask)
{
ENTRY("vTaskSeed"_Stz);
  
//  //                                   sSeed     nTaskArgs
//  if ( ! mpTask->m_pDataStack->bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_DSTACK("task.seed"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! mpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  pThatTask->vSeedWord(sSeed);

  i8 nTaskArgs;
  if ( ! mpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  //ASSERT(! nTaskArgs);
  
  if ( ! mpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  
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
  ENTRY("vTaskSeitfFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.seitf@"_Stz);
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
  ENTRY("vTaskSeitfStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seitf!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSEitf;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("task.seitf!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sleep -- this task sleeps                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskSleep(MP_Task mpTask)
{
  ENTRY("vTaskSleep"_Stz);
  
  i8 nMicroSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMicroSeconds) )
    THROW_INVALID_DSTACK("sleep_s"_Stz);
  
  mpTask->vSleep(nMicroSeconds);
  mpTask->vState(TS_SLEEPING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket@ -- fetch socket of task on data stack                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketFetch(MP_Task mpTask)
{
  ENTRY("vTaskSocketFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket@"_Stz);
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
  ENTRY("vTaskSocketStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! mpTask->m_pDataStack->bPop(ptrSocket) )
    THROW_INVALID_DSTACK("task.socket!"_Stz);
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(MP_Task mpTask)
{
ENTRY("vTaskStateFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@"_Stz);
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
ENTRY("vTaskStateStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! mpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("task.state!"_Stz);
  
  // an r35 task being set to waiting state must be locked
  if (pThatTask->bR35model() && TS_WAITING_ACK == iState) {
    RASSERT(pThatTask->bIsLocked());
  }
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset@ -- fetch taskset of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetFetch(MP_Task mpTask)
{
  ENTRY("vTaskTaskSetFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset@"_Stz);
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
  ENTRY("vTaskTaskSetStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrTaskSet;
  
  if ( ! mpTask->m_pDataStack->bPop(ptrTaskSet) )
    THROW_INVALID_DSTACK("task.taskset!"_Stz);
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(ptrTaskSet);
  
  pThatTask->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(MP_Task mpTask)
{
ENTRY("vTaskThis"_Stz);
  
  Object* pTask = *mpTask;
  
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(MP_Task mpTask)
{
  ENTRY("vTaskThread"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread"_Stz);
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  mpTask->m_pDataStack->vPushObject(pThread);
  ASSERT(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskWait(MP_Task mpTask)
{
  ENTRY("vTaskWait"_Stz);
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(MP_Task mpTask)
{
  ENTRY("vTaskYield"_Stz);
  
  mpTask->vState(TS_YIELDED);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

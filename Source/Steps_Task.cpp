/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Task(void)
{
  vAddStep("task::breakPoint"_Stz, &Step::vTaskBreakPoint);
  vAddStep("task::this"_Stz,       &Step::vTaskThis);

  vAddStep("task.ack"_Stz,       &Step::vTaskAck);
  vAddStep("task.acks!"_Stz,     &Step::vTaskAcksStore);
  vAddStep("task.dicStack"_Stz,  &Step::vTaskDicStack);
  vAddStep("task.dump"_Stz,      &Step::vTaskDump);
  vAddStep("task.dropLock"_Stz,  &Step::vTaskDropLock);
  vAddStep("task.grabLock"_Stz,  &Step::vTaskGrabLock);
  vAddStep("task.isAckSet"_Stz,  &Step::vTaskIsAckSet);
  vAddStep("task.job@"_Stz,      &Step::vTaskJobFetch);
  vAddStep("task.job!"_Stz,      &Step::vTaskJobStore);
  vAddStep("task.name@"_Stz,     &Step::vTaskNameFetch);
  vAddStep("task.partition@"_Stz,&Step::vTaskPartitionFetch);
  vAddStep("task.partition!"_Stz,&Step::vTaskPartitionStore);
  vAddStep("task.seed"_Stz,      &Step::vTaskSeed);
  vAddStep("task.session@"_Stz,  &Step::vTaskSessionFetch);
  vAddStep("task.session!"_Stz,  &Step::vTaskSessionStore);
  vAddStep("task.sleepUsec"_Stz, &Step::vTaskSleepUsec);
  vAddStep("task.send"_Stz,      &Step::vTaskSend);
  vAddStep("task.sendNode@"_Stz, &Step::vTaskSendNodeFetch);
  vAddStep("task.sendNode!"_Stz, &Step::vTaskSendNodeStore);
  vAddStep("task.state@"_Stz,    &Step::vTaskStateFetch);
  vAddStep("task.state!"_Stz,    &Step::vTaskStateStore);
  vAddStep("task.thread"_Stz,    &Step::vTaskThread);
  vAddStep("task.wait"_Stz,      &Step::vTaskWait);
  vAddStep("task.yield"_Stz,     &Step::vTaskYield);
}

/*----------------------------------------------------------------------------*/
/* task.ack -- acknowledge that task of this tasks completion                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskAck(MP_Task mpTask)
{
  ENTRY("vTaskAck"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Stz);
  Task* pTaskBeingAcked = _pTask(pTaskObject);
  
  bool bWasQueued = Handlers::bAckTask(pTaskBeingAcked,*mpTask);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (bWasQueued)
    gpAttachMap->m_oStatistics.Task.m_nAcks++;
#endif
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_oAcksRemaining                */
/*----------------------------------------------------------------------------*/
void Step::vTaskAcksStore(MP_Task mpTask)
{
  ENTRY("vTaskAcksStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 nAcksRequired;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nAcksRequired) )
    THROW_INVALID_DSTACK("task.acks!"_Stz);
  
  ASSERT(0 == pGivenTask->m_oAcksRemaining.iPrintOnlyValue());

  RASSERT(pGivenTask->bIsLocked());
  
  Job* pJob = pGivenTask->m_pJob;
  
  if (pJob && pJob->m_bMarkActivity) {
    ActivityMap::vMarkJob(pJob,"Setting acks: %s %p to %lld"_Stz,
                       aCN[pJob->eClass()],
                       pJob,
                       nAcksRequired);
    gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob->m_iJobId);
  }
  
  ActionSequence::vAdd(pJob,AN_ACKS_REQUIRED,nAcksRequired);
  
  pGivenTask->m_oAcksRemaining.vSetRemaining(nAcksRequired);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.breakPoint -- break point so we can examine task & thread state       */
/*----------------------------------------------------------------------------*/
void Step::vTaskBreakPoint(MP_Task mpTask)
{
  ENTRY("vTaskBreakPoint"_Stz);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDicStack(MP_Task mpTask)
{
  ENTRY("vTaskDicStack"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_oDicStack;
  
  mpTask->m_pDataStack->vPushPointer(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dropLock -- drop lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskDropLock(MP_Task mpTask)
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
void Step::vTaskDump(MP_Task mpTask)
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
void Step::vTaskGrabLock(MP_Task mpTask)
{
  ENTRY("vTaskGrab"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.grabLock"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vGrabLock();
  ActionSequence::vAdd(mpTask->m_pJob,AN_TASK_LOCKED);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.isAckSet -- is task ack set?                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsAckSet(MP_Task mpTask)
{
  ENTRY("vTaskIsAckSet"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.isAckSet"_Stz);
  Task* pTaskBeingAcked = _pTask(pTaskObject);
  
  i8 iIsAckSet = pTaskBeingAcked->m_oAcksRemaining.bIsSet();
  Job* pJob = pTaskBeingAcked->m_pJob;
  
  if (pJob && pJob->m_bMarkActivity) {
    ActivityMap::vMarkJob(pJob,"Task %p: IsAckSet %lld"_Stz,
                       pTaskBeingAcked,
                       iIsAckSet);
  }
  
  mpTask->m_pDataStack->vPushI8(iIsAckSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job@ -- push job of given task                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobFetch(MP_Task mpTask)
{
  ENTRY("vTaskJobFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.job@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Job* pJob = pThatTask->m_pJob;
  
  mpTask->m_pDataStack->vPushPointer(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job! -- store job into given task                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobStore(MP_Task mpTask)
{
  ENTRY("vTaskJobStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.job!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pJobObject;
  if ( ! mpTask->m_pDataStack->bPop(pJobObject) )
    THROW_INVALID_DSTACK("task.job!"_Stz);
  Job* pJob = _pJob(pJobObject);
  
  pThatTask->m_pJob = pJob;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Step::vTaskNameFetch(MP_Task mpTask)
{
ENTRY("vTaskNameFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  s256 sName((Stz*)pThatTask->aJobName());
  
  mpTask->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.partition@ -- fetch partition of task                                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskPartitionFetch(MP_Task mpTask)
{
  ENTRY("vTaskPartitionFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  mpTask->m_pDataStack->vPushI8(pThatTask->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.partition! -- store partition of task                                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskPartitionStore(MP_Task mpTask)
{
  ENTRY("vTaskPartitionStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("task.partition!"_Stz);
  
  pThatTask->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeed(MP_Task mpTask)
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


  i8 nTaskArgs;
  if ( ! mpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  //ASSERT(! nTaskArgs);
  
  if ( ! mpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  
  // copy task arguments to given task (preserving stack order)
  GenVar xVariable;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    mpTask->m_pDataStack->vPeekValue(iTaskArg,xVariable);
    pThatTask->m_pDataStack->vPushGenVar(xVariable);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = mpTask->m_pDataStack->bPop(xVariable);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.session@ -- fetch session of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Step::vTaskSessionFetch(MP_Task mpTask)
{
  ENTRY("vTaskSessionFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.session@"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  Session* pSession = pThatTask->m_pSession;
  
  mpTask->m_pDataStack->vPushPointer(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.session! -- store session on data stack into task on data stack       */
/*----------------------------------------------------------------------------*/
void Step::vTaskSessionStore(MP_Task mpTask)
{
  ENTRY("vTaskSessionStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.session!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSession;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrSession) )
    THROW_INVALID_DSTACK("task.session!"_Stz);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(! pSession || CN_SESSION == pSession->eClass());
  
  pThatTask->m_pSession = pSession;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.send -- this task sends                                               */
/*----------------------------------------------------------------------------*/
void Step::vTaskSend(MP_Task mpTask)
{
  ENTRY("vTaskSend"_Stz);
  
  mpTask->vState(TK_SEND);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode@ -- fetch send node of task on data stack                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSendNodeFetch(MP_Task mpTask)
{
  ENTRY("vTaskSendNodeFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.sendNode@"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  Node* pSendNode = pThatTask->m_pSendNode;
  
  Object* pPtrSendNode = pSendNode;
  
  mpTask->m_pDataStack->vPushPointer(pPtrSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode! -- store send node on data stack into task on data stack    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSendNodeStore(MP_Task mpTask)
{
  ENTRY("vTaskSendNodeStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.sendNode!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSendNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSendNode) )
    THROW_INVALID_DSTACK("task.sendNode!"_Stz);
  Node* pSendNode = _pNode(pPtrSendNode);
  
  pThatTask->m_pSendNode = pSendNode;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sleepUs -- this task sleeps for given microseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepUsec(MP_Task mpTask)
{
  ENTRY("vTaskSleepUsec"_Stz);
  
  i8 nMicroSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMicroSeconds) )
    THROW_INVALID_DSTACK("task.sleepUsec"_Stz);
  
  ActionSequence::vAdd(mpTask->m_pJob,AN_TASK_SLEEP,nMicroSeconds);
  
  mpTask->vSleepUsec(nMicroSeconds);
  mpTask->vState(TK_SLEEPING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskStateFetch(MP_Task mpTask)
{
ENTRY("vTaskStateFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  eTK eState = pThatTask->m_eState;
  
  mpTask->m_pDataStack->vPushI8((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskStateStore(MP_Task mpTask)
{
ENTRY("vTaskStateStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! mpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("task.state!"_Stz);
  
  // a task being set to waiting state must be locked
  if (TK_WAITING == iState) {
    //RASSERT(pThatTask->bIsLocked());
  }
  
  if (mpTask->m_pJob)                              //! why can there be no job??
    ActionSequence::vAdd(mpTask->m_pJob,AN_TASK_STATE_SET,iState);
  
  pThatTask->m_eState = (eTK)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Step::vTaskThis(MP_Task mpTask)
{
ENTRY("vTaskThis"_Stz);
  
  Object* pTask = *mpTask;
  
  mpTask->m_pDataStack->vPushPointer(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskThread(MP_Task mpTask)
{
  ENTRY("vTaskThread"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread"_Stz);
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  mpTask->m_pDataStack->vPushPointer(pThread);
  ASSERT(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskWait(MP_Task mpTask)
{
  ENTRY("vTaskWait"_Stz);
  
  ActionSequence::vAdd(mpTask->m_pJob,AN_TASK_WAIT);
  
  mpTask->vState(TK_WAITING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield(MP_Task mpTask)
{
  ENTRY("vTaskYield"_Stz);
  
  ActionSequence::vAdd(mpTask->m_pJob,AN_TASK_YIELD);
  
  mpTask->vState(TK_YIELDED);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Task(void)
{
  vAddStep("task::breakPoint"_Stz, &Step::vTaskBreakPoint);
  vAddStep("task::delete"_Stz,     &Step::vTaskDelete);
  vAddStep("task::sleepMsec"_Stz,  &Step::vTaskSleepMsec);
  vAddStep("task::sleepSsec"_Stz,  &Step::vTaskSleepSsec);
  vAddStep("task::sleepUsec"_Stz,  &Step::vTaskSleepUsec);
  vAddStep("task::this"_Stz,       &Step::vTaskThis);
  vAddStep("task::wait"_Stz,       &Step::vTaskWait);
  vAddStep("task::yield"_Stz,      &Step::vTaskYield);

  vAddStep("task.ack"_Stz,         &Step::vTaskAck);
  vAddStep("task.acks!"_Stz,       &Step::vTaskAcksStore);
  vAddStep("task.dicStack"_Stz,    &Step::vTaskDicStack);
  vAddStep("task.dump"_Stz,        &Step::vTaskDump);
  vAddStep("task.dropLock"_Stz,    &Step::vTaskDropLock);
  vAddStep("task.grabLock"_Stz,    &Step::vTaskGrabLock);
  vAddStep("task.isAckSet"_Stz,    &Step::vTaskIsAckSet);
  vAddStep("task.job@"_Stz,        &Step::vTaskJobFetch);
  vAddStep("task.job!"_Stz,        &Step::vTaskJobStore);
  vAddStep("task.name@"_Stz,       &Step::vTaskNameFetch);
  vAddStep("task.seed"_Stz,        &Step::vTaskSeed);
  vAddStep("task.session@"_Stz,    &Step::vTaskSessionFetch);
  vAddStep("task.session!"_Stz,    &Step::vTaskSessionStore);
  vAddStep("task.sendNode@"_Stz,   &Step::vTaskSendNodeFetch);
  vAddStep("task.sendNode!"_Stz,   &Step::vTaskSendNodeStore);
  vAddStep("task.state@"_Stz,      &Step::vTaskStateFetch);
  vAddStep("task.thread"_Stz,      &Step::vTaskThread);
}

/*----------------------------------------------------------------------------*/
/* task.ack -- acknowledge that task of this tasks completion                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskAck(void)
{
  ENTRY("vTaskAck"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pTaskWaiting = _pTask(pTaskObject);
  
  bool bWasQueued = Scheduler::bAckWaitingTask(pTaskWaiting,m_pTask);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (bWasQueued)
    gpAttachMap->m_oStatistics.Task.m_nAcks++;
#endif
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_oAcksRmg                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskAcksStore(void)
{
  ENTRY("vTaskAcksStore"_Stz);
  
  Object* pPtrTask;
  if ( ! m_pDataStack->bPopObject(pPtrTask) )
    THROW_DSTACK();
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 nAcksRequired;
  if ( ! m_pDataStack->bPopAsInt(nAcksRequired) )
    THROW_DSTACK();
  
  ASSERT(0 == pGivenTask->m_oAcksRmg.iPrintOnlyValue());

  RASSERT(pGivenTask->bIsLocked());
  
  Job* pJob = pGivenTask->m_pJob;
  
  if (pJob && pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Setting acks: %s %p to %lld"_Stz,
                       aCN[pJob->eClass()],
                       pJob,
                       nAcksRequired);
    AM::pOriginatingJobs()->vLogJobs(pJob->m_iUniqueId);
  }
  
  Scheduler::vTaskRequiresAcks(pGivenTask,nAcksRequired);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.breakPoint -- break point so we can examine task & thread state       */
/*----------------------------------------------------------------------------*/
void Step::vTaskBreakPoint(void)
{
  ENTRY("vTaskBreakPoint"_Stz);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDicStack(void)
{
  ENTRY("vTaskDicStack"_Stz);
  
  Object* pPtrTask;
  if ( ! m_pDataStack->bPopObject(pPtrTask) )
    THROW_DSTACK();
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_pForth->m_oDicStack;
  
  m_pDataStack->vPushPointer(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dropLock -- drop lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskDropLock(void)
{
  ENTRY("vTaskDrop"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pObject);
  
  Scheduler::vDropTaskLock(pThatTask);
  //pThatTask->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskDump(void)
{
ENTRY("vTaskDump"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::delete -- remove task from scheduler and delete                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDelete(void)
{
  ENTRY("vTaskDelete"_Stz);
  
  m_pTask->vUnlinkJob();
  
  m_pTask->vTransition(TR_DELETE);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.grabLock -- grab lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskGrabLock(void)
{
  ENTRY("vTaskGrab"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pObject);
  
  Scheduler::vGrabTaskLock(pThatTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.isAckSet -- is task ack set?                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsAckSet(void)
{
  ENTRY("vTaskIsAckSet"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pTaskBeingAcked = _pTask(pTaskObject);
  
  i8 iIsAckSet = pTaskBeingAcked->m_oAcksRmg.bIsSet();
  Job* pJob = pTaskBeingAcked->m_pJob;
  
  if (pJob && pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Task %p: IsAckSet %lld"_Stz,
                       pTaskBeingAcked,
                       iIsAckSet);
  }
  
  m_pDataStack->vPushI8(iIsAckSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job@ -- push job of given task                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobFetch(void)
{
  ENTRY("vTaskJobFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  Job* pJob = pThatTask->m_pJob;
  
  m_pDataStack->vPushPointer(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job! -- store job into given task                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobStore(void)
{
  ENTRY("vTaskJobStore"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pJobObject;
  if ( ! m_pDataStack->bPopObject(pJobObject) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobObject);
  
  pThatTask->m_pJob = pJob;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Step::vTaskNameFetch(void)
{
ENTRY("vTaskNameFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  s256 sName((Stz*)pThatTask->szJobName());
  
  m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeed(void)
{
ENTRY("vTaskSeed"_Stz);
  
//  //                                   sSeed     nTaskArgs
//  if ( ! m_pDataStack->bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_DSTACK();
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! m_pDataStack->bPopS32(sSeed) )
    THROW_DSTACK();


  i8 nTaskArgs;
  if ( ! m_pDataStack->bPopI8(nTaskArgs) )
    THROW_DSTACK();
  //ASSERT(! nTaskArgs);
  
  if ( ! m_pDataStack->bGuard(nTaskArgs) )
    THROW_DSTACK();
  
  // copy task arguments to given task (preserving stack order)
  GenVar xVariable;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    m_pDataStack->vPeekValue(iTaskArg,xVariable);
    pThatTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = m_pDataStack->bPopGenVar(xVariable);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.session@ -- fetch session of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Step::vTaskSessionFetch(void)
{
  ENTRY("vTaskSessionFetch"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pObject);
  
  Session* pSession = pThatTask->m_pSession;
  
  m_pDataStack->vPushPointer(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.session! -- store session on data stack into task on data stack       */
/*----------------------------------------------------------------------------*/
void Step::vTaskSessionStore(void)
{
  ENTRY("vTaskSessionStore"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSession;
  
  if ( ! m_pDataStack->bPopObject(pPtrSession) )
    THROW_DSTACK();
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(! pSession || CN_SESSION == pSession->eClass());
  
  pThatTask->m_pSession = pSession;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode@ -- fetch send node of task on data stack                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSendNodeFetch(void)
{
  ENTRY("vTaskSendNodeFetch"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pObject);
  
  Node* pSendNode = pThatTask->pSendNode();
  
  Object* pPtrSendNode = pSendNode;
  
  m_pDataStack->vPushPointer(pPtrSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode! -- store send node on data stack into task on data stack    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSendNodeStore(void)
{
  ENTRY("vTaskSendNodeStore"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSendNode;
  if ( ! m_pDataStack->bPopObject(pPtrSendNode) )
    THROW_DSTACK();
  Node* pSendNode = _pNode(pPtrSendNode);
  
  pThatTask->vSendNode(pSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepMsec -- this task sleeps for given milliseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepMsec(void)
{
  ENTRY("vTaskSleepMsec"_Stz);
  
  i8 nMilliSeconds;
  
  if ( ! m_pDataStack->bPopI8(nMilliSeconds) )
    THROW_DSTACK();
  
  i8 nMicroSeconds = nMilliSeconds * C_1K;
  
  m_pTask->vSleepUsec(nMicroSeconds);
  m_pTask->vTransition(TR_SLEEP);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepSsec -- this task sleeps for given seconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepSsec(void)
{
  ENTRY("vTaskSleepSsec"_Stz);
  
  i8 nSeconds;
  
  if ( ! m_pDataStack->bPopI8(nSeconds) )
    THROW_DSTACK();
  
  i8 nMicroSeconds = nSeconds * C_1M;
  
  m_pTask->vSleepUsec(nMicroSeconds);
  m_pTask->vTransition(TR_SLEEP);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepUsec -- this task sleeps for given microseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepUsec(void)
{
  ENTRY("vTaskSleepUsec"_Stz);
  
  i8 nMicroSeconds;
  
  if ( ! m_pDataStack->bPopI8(nMicroSeconds) )
    THROW_DSTACK();
  
  m_pTask->vSleepUsec(nMicroSeconds);
  m_pTask->vTransition(TR_SLEEP);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskStateFetch(void)
{
ENTRY("vTaskStateFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  m_pDataStack->vPushI8((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::this -- push this task                                               */
/*----------------------------------------------------------------------------*/
void Step::vTaskThis(void)
{
ENTRY("vTaskThis"_Stz);
  
  Object* pTask = m_pTask;
  
  m_pDataStack->vPushPointer(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskThread(void)
{
  ENTRY("vTaskThread"_Stz);
  
  Object* pTaskObject;
  if ( ! m_pDataStack->bPopObject(pTaskObject) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  m_pDataStack->vPushPointer(pThread);
  ASSERT(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskWait(void)
{
  ENTRY("vTaskWait"_Stz);
  
  m_pTask->vTransition(TR_WAIT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::yield -- this task yields                                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield(void)
{
  ENTRY("vTaskYield"_Stz);
    
  m_pTask->vTransition(TR_YIELD);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

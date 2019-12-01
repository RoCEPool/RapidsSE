/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Task(void)
{
  vAddStep("task::breakPoint"_Stz,    &Step::vTaskBreakPoint);
  vAddStep("task::delete"_Stz,        &Step::vTaskDelete);
  vAddStep("task::doneFinally"_Stz,   &Step::vTaskDoneFinally);
  vAddStep("task::dump"_Stz,          &Step::vTaskDump);
  vAddStep("task::seedInitially"_Stz, &Step::vTaskSeedInitially);
  vAddStep("task::sleepMsec"_Stz,     &Step::vTaskSleepMsec);
  vAddStep("task::sleepSsec"_Stz,     &Step::vTaskSleepSsec);
  vAddStep("task::sleepUsec"_Stz,     &Step::vTaskSleepUsec);
  vAddStep("task::this"_Stz,          &Step::vTaskThis);
  vAddStep("task::wait"_Stz,          &Step::vTaskWait);
  vAddStep("task::yield"_Stz,         &Step::vTaskYield);

  vAddStep("task.ack"_Stz,         &Step::vTaskAck);
  vAddStep("task.acks!"_Stz,       &Step::vTaskAcksStore);
  vAddStep("task.dicStack"_Stz,    &Step::vTaskDicStack);
  vAddStep("task.dropLock"_Stz,    &Step::vTaskDropLock);
  vAddStep("task.grabLock"_Stz,    &Step::vTaskGrabLock);
  vAddStep("task.isAckSet"_Stz,    &Step::vTaskIsAckSet);
  vAddStep("task.job@"_Stz,        &Step::vTaskJobFetch);
  vAddStep("task.job!"_Stz,        &Step::vTaskJobStore);
  vAddStep("task.name@"_Stz,       &Step::vTaskNameFetch);
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
  ENTRY(); // ENTRY("vTaskAck"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
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
  ENTRY(); // ENTRY("vTaskAcksStore"_Stz);
  
  Object* pPtrTask;
  m_pDataStack->vPopPtr(pPtrTask,__func__);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 nAcksRequired;
  if ( ! m_pDataStack->bPopAsInt(nAcksRequired) ) THROW_DSTACK();
  
  ASSERT(0 == pGivenTask->m_oAcksRmg.iPrintOnlyValue());

  RASSERT(pGivenTask->bIsLocked());
  
  Job* pJob = pGivenTask->m_pJob;
  
  if (pJob && pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Setting acks: %s %p to %lld"_Stz,
                       aCN[pJob->eClass()],
                       pJob,
                       nAcksRequired);
    AM::pOriginatingJobs()->vLogJobs(pJob->m_iFocalId);
  }
  
  Scheduler::vTaskRequiresAcks(pGivenTask,nAcksRequired);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.breakPoint -- break point so we can examine task & thread state       */
/*----------------------------------------------------------------------------*/
void Step::vTaskBreakPoint(void)
{
  ENTRY(); // ENTRY("vTaskBreakPoint"_Stz);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDicStack(void)
{
  ENTRY(); // ENTRY("vTaskDicStack"_Stz);
  
  Object* pPtrTask;
  m_pDataStack->vPopPtr(pPtrTask,__func__);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_pForth->m_oDicStack;
  
  m_pDataStack->vPushPtr(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::doneFinally -- set done finally as been executed                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskDoneFinally(void)
{
  ENTRY(); // ENTRY("vTaskDoneFinally"_Stz);
  
  m_bDoneFinally = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dropLock -- drop lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskDropLock(void)
{
  ENTRY(); // ENTRY("vTaskDrop"_Stz);
  
  Object* pObject;
  m_pDataStack->vPopPtr(pObject,__func__);
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
  ENTRY(); // ENTRY("vTaskDump"_Stz);
  
  m_pTask->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::delete -- remove task from scheduler and delete                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDelete(void)
{
  ENTRY(); // ENTRY("vTaskDelete"_Stz);
  
  m_pTask->vUnlinkJob();
  
  m_pTask->vSchedulerAct(SA_DELETE);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.grabLock -- grab lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskGrabLock(void)
{
  ENTRY(); // ENTRY("vTaskGrab"_Stz);
  
  Object* pObject;
  m_pDataStack->vPopPtr(pObject,__func__);
  Task* pThatTask = _pTask(pObject);
  
  Scheduler::vGrabTaskLock(pThatTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.isAckSet -- is task ack set?                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsAckSet(void)
{
  ENTRY(); // ENTRY("vTaskIsAckSet"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pTaskBeingAcked = _pTask(pTaskObject);
  
  i8 iIsAckSet = pTaskBeingAcked->m_oAcksRmg.bIsSet();
  Job* pJob = pTaskBeingAcked->m_pJob;
  
  if (pJob && pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Task %p: IsAckSet %lld"_Stz,
                       pTaskBeingAcked,
                       iIsAckSet);
  }
  
  m_pDataStack->vPushInt(iIsAckSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job@ -- push job of given task                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobFetch(void)
{
  ENTRY(); // ENTRY("vTaskJobFetch"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pThatTask = _pTask(pTaskObject);
  
  Job* pJob = pThatTask->m_pJob;
  
  m_pDataStack->vPushPtr(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job! -- store job into given task                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobStore(void)
{
  ENTRY(); // ENTRY("vTaskJobStore"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pJobObject;
  m_pDataStack->vPopPtr(pJobObject,__func__);
  Job* pJob = _pJob(pJobObject);
  
  pThatTask->m_pJob = pJob;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Step::vTaskNameFetch(void)
{
  ENTRY(); // ENTRY("vTaskNameFetch"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pThatTask = _pTask(pTaskObject);
  
  s256 sName((Stz*)pThatTask->szJobName());
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::seedInitially -- set seed initially as been executed                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeedInitially(void)
{
  ENTRY(); // ENTRY("vTaskSeedInitially"_Stz);
  
  m_bSeedInitially = true;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode@ -- fetch send node of task on data stack                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSendNodeFetch(void)
{
  ENTRY(); // ENTRY("vTaskSendNodeFetch"_Stz);
  
  Object* pObject;
  m_pDataStack->vPopPtr(pObject,__func__);
  Task* pThatTask = _pTask(pObject);
  
  Node* pSendNode = pThatTask->pSendNode();
  
  Object* pPtrSendNode = pSendNode;
  
  m_pDataStack->vPushPtr(pPtrSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode! -- store send node on data stack into task on data stack    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSendNodeStore(void)
{
  ENTRY(); // ENTRY("vTaskSendNodeStore"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSendNode;
  m_pDataStack->vPopPtr(pPtrSendNode,__func__);
  Node* pSendNode = _pNode(pPtrSendNode);
  
  pThatTask->vSendNode(pSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepMsec -- this task sleeps for given milliseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepMsec(void)
{
  ENTRY(); // ENTRY("vTaskSleepMsec"_Stz);
  
  i8 nMilliSeconds;
  m_pDataStack->vPopInt(nMilliSeconds,__func__);
  
  i8 nMicroSeconds = nMilliSeconds * C_1K;
  
  m_pTask->vSleepUsec(nMicroSeconds);
  m_pTask->vSchedulerAct(SA_SLEEP);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepSsec -- this task sleeps for given seconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepSsec(void)
{
  ENTRY(); // ENTRY("vTaskSleepSsec"_Stz);
  
  i8 nSeconds;
  
  m_pDataStack->vPopInt(nSeconds,__func__);
  i8 nMicroSeconds = nSeconds * C_1M;
  
  m_pTask->vSleepUsec(nMicroSeconds);
  m_pTask->vSchedulerAct(SA_SLEEP);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepUsec -- this task sleeps for given microseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepUsec(void)
{
  ENTRY(); // ENTRY("vTaskSleepUsec"_Stz);
  
  i8 nMicroSeconds;
  m_pDataStack->vPopInt(nMicroSeconds,__func__);
  
  m_pTask->vSleepUsec(nMicroSeconds);
  m_pTask->vSchedulerAct(SA_SLEEP);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskStateFetch(void)
{
  ENTRY(); // ENTRY("vTaskStateFetch"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  m_pDataStack->vPushInt((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::this -- push this task                                               */
/*----------------------------------------------------------------------------*/
void Step::vTaskThis(void)
{
  ENTRY(); // ENTRY("vTaskThis"_Stz);
  
  Object* pTask = m_pTask;
  
  m_pDataStack->vPushPtr(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskThread(void)
{
  ENTRY(); // ENTRY("vTaskThread"_Stz);
  
  Object* pTaskObject;
  m_pDataStack->vPopPtr(pTaskObject,__func__);
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  m_pDataStack->vPushPtr(pThread);
  ASSERT(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskWait(void)
{
  ENTRY(); // ENTRY("vTaskWait"_Stz);
  
  m_pTask->vSchedulerAct(SA_WAIT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::yield -- this task yields                                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield(void)
{
  ENTRY(); // ENTRY("vTaskYield"_Stz);
    
  m_pTask->vSchedulerAct(SA_YIELD);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

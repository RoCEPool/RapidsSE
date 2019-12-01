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
  
  Task* pTaskWaiting = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pGivenTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  i8 nAcksRequired = m_pDataStack->iPopAsInt(__func__);
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  Scheduler::vGrabTaskLock(pThatTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.isAckSet -- is task ack set?                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsAckSet(void)
{
  ENTRY(); // ENTRY("vTaskIsAckSet"_Stz);
  
  Task* pTaskBeingAcked = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  pThatTask->m_pJob = pJob;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Step::vTaskNameFetch(void)
{
  ENTRY(); // ENTRY("vTaskNameFetch"_Stz);
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  Node* pSendNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  pThatTask->vSendNode(pSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepMsec -- this task sleeps for given milliseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepMsec(void)
{
  ENTRY(); // ENTRY("vTaskSleepMsec"_Stz);
  
  i8 nMilliSeconds = m_pDataStack->iPopInt(__func__);
  
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
  
  i8 nSeconds = m_pDataStack->iPopInt(__func__);
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
  
  i8 nMicroSeconds = m_pDataStack->iPopInt(__func__);
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));

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

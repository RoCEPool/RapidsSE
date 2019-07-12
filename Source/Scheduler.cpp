/*============================================================================*/
/* Scheduler.cpp -- Thread/Task Event Scheduler                               */
/*============================================================================*/

#include "Scheduling.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

////////////////////////////////////////////////////////////////////////////////
//                                task                                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* ack task handler                                                           */
/*----------------------------------------------------------------------------*/
bool Scheduler::bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  //
  // this handler is called from Task::vTaskAck()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Job* pJobBeingAcked = pTaskBeingAcked->pJob();
  Job* pJobAcking = pTaskAcking->pJob();
  
  if (pJobAcking->m_bMarkActivity || pJobBeingAcked->m_bMarkActivity) {
    ActivityMap::vMarkJob(pJobBeingAcked,
                       "pTaskBeingAcked:%p Job %s "_Stz,
                       pTaskBeingAcked,
                       aCN[pJobBeingAcked->eClass()]
                      );
    ActivityMap::vMarkJob(pJobAcking,
                       "pTaskAcking:%p Job %s "_Stz,
                       pTaskAcking,
                       aCN[pJobAcking->eClass()]
                       );
  }
  
  ActionSequence::vAdd(pTaskAcking->pJob(),
                       AN_ACK_TASK,
                       pTaskBeingAcked->pJob());
  
  bool bAmDecider = pTaskBeingAcked->bAckAndDecide();
  
  if (bAmDecider) {
    if (pJobBeingAcked->m_bMarkActivity || pJobAcking->m_bMarkActivity)
      ActivityMap::vMarkJob(pJobAcking,
                            "Decider %s acks %s"_Stz,
                            aCN[pJobAcking->eClass()],
                            aCN[pJobBeingAcked->eClass()]
                            );
  }

  if ( (pJobBeingAcked && JT_NONE != pJobBeingAcked->eJobTrace()) ||
       (pJobAcking && JT_NONE != pJobAcking->eJobTrace())  ) {
    vTrace("Ack of %s task %p by %s task %p ; %s\n"_Stz,
           pJobBeingAcked ? aCN[pJobBeingAcked->m_pJobClass->m_eClass] : "<no job>"_Stz,
           pTaskBeingAcked,
           pJobAcking ? aCN[pJobAcking->m_pJobClass->m_eClass] : "<no job>"_Stz,
           pJobAcking,
           bAmDecider ? "decider" : "not decider");
  }
  
  if (bAmDecider) {
    ActionSequence::vAdd(pTaskAcking->pJob(),
                         AN_AM_DECIDER,
                         pTaskBeingAcked->pJob());
    
    vScheduleAckedTask(pTaskBeingAcked);
  }
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* append task to executing set                                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vAppendExecutingSet(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pTask->vState(TS_EXECUTING);
  
  pQueues->vAddToExecutingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* task has beeb spawned                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vSpawnedTask(Task* pTask)
{
  //
  // this handler is called from session::bSpawnJob()
  // we want to run given task. task should be ready to run
  //
  
  vScheduleSpawnedTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* task has beeb spawned                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vSpawnedTaskDedicated(Task* pTask)
{
  //
  // this handler is called from session::bSpawnJobDedicated()
  // we want to run given task with deciated thread.
  // task should be ready to run
  //
  
  pTask->vState(TS_READY);
  
  Thread* pThread = new Thread;
  pThread->m_pTask = pTask;
  pThread->m_eDedicated = pTask->pJob()->eClass();
  
  pThread->vSpawnPthread_dedicated(pTask);
}

/*----------------------------------------------------------------------------*/
//void Scheduler::vSpawnedTaskDedicated(Task* pTask)
//{
//  //
//  // this handler is called from session::bSpawnJob()
//  // we want to run given task. task should be ready to run
//  //
//
//  Queues* pQueues = gpAttachMap->m_pQueues;
//
//  pQueues->vScheduleSpawnedTask(pTask);
//}

/*----------------------------------------------------------------------------*/
/* task has returned                                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskReturn(Task** ppTask)
{
  //
  // this handler is called from Thread::vLoop()
  // after a task returns to the thread
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Task* pTask = *ppTask;
  *ppTask = nullptr;
  
  // first remove this task from the running queue
  pQueues->vRemoveFromExecutingSet(pTask);
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_EXITED:   vScheduleExitedTask(pTask);    break;
    case TS_SLEEPING: vScheduleSleepingTask(pTask);  break;
    case TS_WAITING:  vScheduleWaitingTask(pTask);   break;
    case TS_YIELDED:  vScheduleYieldedTask(pTask);   break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // should never come to thread handler for these states
      
    case TS_EXECUTING: ASSERT(0);
      
    default:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                thread                                      //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* append idle thread onto dedicated thread vector                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddDedicatedThread(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAddDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto general thread vector                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddGeneralThread(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAddGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* check tasks in the sleeping queue                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vCheckSleeping(void)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  while (Task* pWokenTask = pQueues->pPopSleepingQueue())
    vScheduleWokenTask(pWokenTask);
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vIdleThread(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vAppendIdleThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* pause till general threads are all idle                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vPauseForGeneralThreads(void)
{
  Queues* pQueues = gpAttachMap->m_pQueues;

  // pause till all general threads have all begun
  while (*gpAttachMap->m_ixThreads != gpAttachMap->Arg.m_nThreads)
    Pthread::vSleepMsec(10,CN_NONE);
  
  RASSERT(pQueues->m_pGeneralThreadVec->nItems() == gpAttachMap->Arg.m_nThreads);
  
  // pause till all general threads are in idle state
  for (i8 i_Thread = 0; i_Thread < pQueues->m_pGeneralThreadVec->nItems(); i_Thread++) {
    
    Thread* pThread = pQueues->m_pGeneralThreadVec->pGet(i_Thread);
    
    while (TD_IDLE != pThread->m_eThreadState)
      Pthread::vSleepMsec(10,CN_QUEUES);
  }
  
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from thread vector                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemoveDedicatedThread(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vRemoveDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from thread vector                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemoveGeneralThread(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vRemoveGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* calling thread waits for task                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vWaitForTask(Thread* pThread)
{
  pThread->m_eThreadState = TD_IDLE;
  vIdleThread(pThread);
  pThread->m_pMutexThread->vCondWaitIndefinite();
  pThread->m_eThreadState = TD_RUNNING;
}



////////////////////////////////////////////////////////////////////////////////
//                                private                                     //
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* schedule a fully acked task                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleAckedTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Scheduling acked: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  //ActionSequence::vAdd(pTask->pJob(),AN_TASK_ACKED);
  
  pTask->vGrabLock();
  
  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
  
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_WAITING_SET == pTask->eQueue());
  
  pTask->vDropLock();
  
  pQueues->vRemoveFromWaitingSet(pTask);
  
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vState(TS_READY);
  
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule an exited task                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleExitedTask(Task* pTask)
{
  RASSERT(TS_EXITED == pTask->eState());
  
  ASSERT(! pTask->bWordBody());
  
  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* schedule a ready to execute task                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleReadyTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(TS_READY == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule ready: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
#ifdef WITH_EVENT_MODEL
  Thread* pIdleThread = pQueues->pPopIdleThread();
  
  if (pIdleThread) {
    ActionSequence::vAdd(pTask->pJob(),AN_THREAD_EXECUTE,pIdleThread->iThreadNo());
    
    pIdleThread->m_pTask = pTask;
    
    pIdleThread->m_nAssignedTasks++;
    
    pIdleThread->m_pMutexThread->vMutexLock();
    pIdleThread->m_pMutexThread->vCondSignal();
    pIdleThread->m_pMutexThread->vMutexUnlock();
    
    return;
  }
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
    pQueues->vAddToReadyQueue(pTask);
  else
    pQueues->vAddToReadyQueueLP(pTask);
#else
  Thread* pIdleThread = pQueues->pFindIdleThread();
  
  if (pIdleThread) {
    ActionSequence::vAdd(pTask->pJob(),AN_THREAD_EXECUTE,pIdleThread->iThreadNo());
    pIdleThread->vExecuteTask(pTask);
    
    return;
  }
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
    pQueues->vAddToReadyQueue(pTask);
  else
    pQueues->vAddToReadyQueueLP(pTask);
#endif
}

/*----------------------------------------------------------------------------*/
/* schedule sleeping task                                                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleSleepingTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule sleeping: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  // is the task still actually sleeping
  if (! pTask->bWakeupNow()) {
    pQueues->vAddToSleepingQueue(pTask);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupTask();
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  pCallingThread->vReExecuteTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule a newly spawned task                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleSpawnedTask(Task* pTask)
{
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vState(TS_READY);
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMarkInfo("Schedule spawned: %s %p slot[%lld]"_Stz,
                             aCN[pTask->pJob()->eClass()],
                             pTask->pJob(),
                             pTask->pJob()->m_i_Slot);
    else
      ActivityMap::vMarkJob(pTask->pJob(),
                            "Schedule spawned: %s %p slot[%lld]"_Stz,
                            aCN[pTask->pJob()->eClass()],
                            pTask->pJob(),
                            pTask->pJob()->m_i_Slot);
  }
  
  //vTrace("Spawned Task %p\n"_Stz,pTask);
  
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule ready waiting task                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleWaitingTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;

  RASSERT(TS_WAITING == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMarkInfo(
                             "Schedule waiting: %s %p"_Stz,
                             aCN[pTask->pJob()->eClass()],
                             pTask->pJob());
    else
      ActivityMap::vMarkJob(pTask->pJob(),
                            "Schedule waiting: %s %p"_Stz,
                            aCN[pTask->pJob()->eClass()],
                            pTask->pJob());
  }
  
  pQueues->vAddToWaitingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule woken task                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleWokenTask(Task* pTask)
{
  pTask->vState(TS_READY);
  
  //vTrace("Spawned Task %p\n"_Stz,pTask);
  
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule yielded task                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleYieldedTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMarkInfo(
                             "Schedule yielded: %s %p"_Stz,
                             aCN[pTask->pJob()->eClass()],
                             pTask->pJob());
    else
      ActivityMap::vMarkJob(pTask->pJob(),
                            "Schedule yielded: %s %p"_Stz,
                            aCN[pTask->pJob()->eClass()],
                            pTask->pJob());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // non-partitioned task yield
  
  
  // - A general thread has executed a task which retured TS_YIELDED.
  //   If the ready queue is empty then this general thread can
  //     continue to execute the yield task.
  //   Otherwise we add the task to the ready queue
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL) {
    pQueues->m_pReadyQueue->vGrabLock();
    i8 iLengthReadyQueue = pQueues->nTasksReady();
    if (0 == iLengthReadyQueue) {
      pQueues->m_pReadyQueue->vDropLock();
      pCallingThread->vReExecuteTask(pTask);
    }
    else {
      pTask->vState(TS_READY);
      pQueues->vAddToReadyQueue_NL(pTask);
      pQueues->m_pReadyQueue->vDropLock();
    }
  }
  else {
    pQueues->m_pReadyQueueLP->vGrabLock();
    pTask->vState(TS_READY);
    pQueues->vAddToReadyQueueLP_NL(pTask);
    pQueues->m_pReadyQueueLP->vDropLock();
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
/* ack a waiting task                                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  //
  // called from Step::vTaskAck()
  //
  
  RASSERT(TS_EXECUTING == pTaskAcking->eState());
  RASSERT(pTaskBeingAcked->iAcksRmg_assert_only_() > 0);
  
  Job* pJobBeingAcked = pTaskBeingAcked->pJob();
  Job* pJobAcking     = pTaskAcking->pJob();
  
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bAmDecider) {
    ActionSequence::vAdd(pTaskAcking->pJob(),
                         AN_AM_DECIDER,
                         pTaskBeingAcked->pJob());
    
    RASSERT(! pTaskBeingAcked->iAcksRmg_assert_only_());
    vQueue_AckedTask(pTaskBeingAcked);
  }
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* dedicated task has been spawned.  spawn a dedicated thread for it          */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskDedicated(Task* pTask)
{
  //
  // called from Api::bSpawnJobDedicated()
  //
  
  ASSERT(TS_NONE == pTask->eState());
  pTask->vState(TS_READY);
  
  Thread* pThread = new Thread;
  pThread->m_pTask = pTask;
  pThread->m_eDedicated = pTask->pJob()->eClass();
  
  pThread->vSpawnPthread_dedicated(pTask);
}

/*----------------------------------------------------------------------------*/
/* general task has been spawned.  add it to queues (ready queue)             */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  
  RASSERT(TS_NONE == pTask->eState());
  pTask->vState(TS_READY);

  vQueue_NewTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* task has returned from task loop                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskReturn(Task** ppTask)
{
  //
  // called from Thread::vLoop_general()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Task* pTask = *ppTask;
  *ppTask = nullptr;
  
  // first remove this task from the running queue
  pQueues->vRemoveFromExecutingSet(pTask);
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_EXITED:   vQueue_ExitedTask(pTask);    break;
    case TS_SLEEPING: vQueue_SleepingTask(pTask);  break;
    case TS_WAITING:  vQueue_WaitingTask(pTask);   break;
    case TS_YIELDED:  vQueue_YieldedTask(pTask);   break;
      
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
/* append given idle thread onto dedicated thread vector                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddDedicatedThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(CN_NONE != pThread->m_eDedicated);
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAddDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* append given idle thread onto general thread vector                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddGeneralThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAddGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* assign task to dedicated thread                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vAssignDedicated(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  // vBeginThread() will have been called
  ASSERT(TD_RUNNING == pThread->m_eThreadState);
  ASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  Pthread::vTlsSet(giTLS_TaskKey,pThread->m_pTask);
  ASSERT(Pthread::pTlsGet(giTLS_TaskKey));
  
  RASSERT(TS_READY == pThread->m_pTask->eState());
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pThread->m_pTask->vThread(pThread);
  
  // add this task to the executing task queue
  vQueue_ExecutingTask(pThread->m_pTask);
}

/*----------------------------------------------------------------------------*/
/* begin a new thread                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vBeginThread(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated() and Thread::vLoop_general()
  //
  
  // thread has mutex while it is running
  pThread->m_eThreadState = TD_RUNNING;
  pThread->m_pMutexThread->vMutexLock();
  
  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
}

/*----------------------------------------------------------------------------*/
/* check to see if tasks in the sleeping queue can be woken                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vCheckSleeping(void)
{
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  while (Task* pWokenTask = pQueues->pPopSleepingQueue())
    vQueue_WokenTask(pWokenTask);
}

/*----------------------------------------------------------------------------*/
/* end of thread loop                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vEndThread(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  pThread->m_pMutexThread->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* pause till general threads are all idle                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vPauseForGeneralThreads(void)
{
  //
  // called from Engine::vStartSystem()
  //
  
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
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vRemoveDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from thread vector                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemoveGeneralThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vRemoveGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* calling thread waits for task to be assigned                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vWaitForTaskAssignment(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  while (! pThread->m_pTask) {
    pThread->m_eThreadState = TD_IDLE;
    vQueue_IdleThread(pThread);
    pThread->m_pMutexThread->vCondWaitIndefinite();
    pThread->m_eThreadState = TD_RUNNING;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pThread->m_eThreadState = TD_RUNNING;
  ASSERT(pThread->m_pTask);
  
  Pthread::vTlsSet(giTLS_TaskKey,pThread->m_pTask);
  ASSERT(Pthread::pTlsGet(giTLS_TaskKey));
  
  RASSERT(TS_READY   == pThread->m_pTask->eState() ||
          TS_YIELDED == pThread->m_pTask->eState());
  pThread->m_pTask->vState(TS_EXECUTING);
  pThread->m_pTask->vThread(pThread);
  
  // add this task to the executing task queue
  vQueue_ExecutingTask(pThread->m_pTask);
}



////////////////////////////////////////////////////////////////////////////////
//                                private                                     //
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* attempt to pop an idle thread for given task                               */
/*----------------------------------------------------------------------------*/
Thread* Scheduler::pPop_IdleThread(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Thread* pIdleThread = pQueues->pPopIdleThread();
  
  if (! pIdleThread)
    return nullptr;
  
  ActionSequence::vAdd(pTask->pJob(),AN_THREAD_EXECUTE,pIdleThread->iThreadNo());
  
  pIdleThread->m_pTask = pTask;
  
  pIdleThread->m_nAssignedTasks++;
  
  pIdleThread->m_pMutexThread->vMutexLock();
  pIdleThread->m_pMutexThread->vCondSignal();
  pIdleThread->m_pMutexThread->vMutexUnlock();
  
  return pIdleThread;
}

/*----------------------------------------------------------------------------*/
/* schedule a fully acked task                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_AckedTask(Task* pTask)
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
  
  pTask->vDropLock();

  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_WAITING_SET == pTask->eQueue());
  RASSERT(0 == pTask->iAcksRmg_assert_only_());
  
  vRemove_WaitingTask(pTask);
  
  pTask->vState(TS_READY);
  vQueue_ReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* append task to executing set                                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_ExecutingTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pTask->vState(TS_EXECUTING);
  
  pQueues->vAddToExecutingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* delete an exited task                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_ExitedTask(Task* pTask)
{
  RASSERT(TS_EXITED == pTask->eState());
  
  ASSERT(! pTask->bWordBody());
  
  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_IdleThread(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vAppendIdleThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* schedule a newly spawned task                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_NewTask(Task* pTask)
{
  RASSERT(QK_NONE == pTask->eQueue());
  
  
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
  
  pTask->vState(TS_READY);
  vQueue_ReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule a ready to execute task                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_ReadyTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(TS_READY == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule ready: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
#ifdef WITH_EVENT_MODEL
  Thread* pIdleThread = pPop_IdleThread(pTask);
  
  if (pIdleThread)
    return;
  
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
void Scheduler::vQueue_SleepingTask(Task* pTask)
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
/* schedule waiting task                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_WaitingTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;

  // task must be in the waiting state
  RASSERT(TS_WAITING == pTask->eState());
  
  // task must be locked
  RASSERT(pTask->bIsLocked());

  // activity
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
  
  // add task to waiting set
  pQueues->vAddToWaitingSet(pTask);
  
  // drop task lock
  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* schedule woken task                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_WokenTask(Task* pTask)
{
  RASSERT(TS_READY == pTask->eState());
  
  vQueue_ReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule yielded task                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_YieldedTask(Task* pTask)
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

/*----------------------------------------------------------------------------*/
/* remove a fully acked task from waiting set                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemove_WaitingTask(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(0 == pTask->iAcksRmg_assert_only_());

  RASSERT(QK_WAITING_SET == pTask->eQueue());
  pQueues->vRemoveFromWaitingSet(pTask);
  RASSERT(QK_NONE == pTask->eQueue());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

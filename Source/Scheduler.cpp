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
  
  if (! bAmDecider)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ActionSequence::vAdd(pTaskAcking->pJob(),
                       AN_AM_DECIDER,
                       pTaskBeingAcked->pJob());
  
  RASSERT(! pTaskBeingAcked->iAcksRmg_assert_only_());
  
  //vQueue_AckedTask(pTaskBeingAcked);
  
  if (pTaskBeingAcked->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTaskBeingAcked->pJob(),
                          "Scheduling acked: %s %p"_Stz,
                          aCN[pTaskBeingAcked->pJob()->eClass()],
                          pTaskBeingAcked->pJob());
  
  //ActionSequence::vAdd(pTask->pJob(),AN_TASK_ACKED);
  
  pTaskBeingAcked->vGrabLock();
  
  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
  
  pTaskBeingAcked->vDropLock();
  
  vRemove_WaitingSet(pTaskBeingAcked);
  
  pTaskBeingAcked->vState(TS_READY);
  vScheduleReadyTask(pTaskBeingAcked);

  return true;
}

/*----------------------------------------------------------------------------*/
/* add new dedicated task to queues and spawn a dedicated thread for it       */
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
/* add new general task to queues                                             */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  
  RASSERT(TS_NONE == pTask->eState());
  pTask->vState(TS_READY);

  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* check to see if tasks in the sleeping queue can be woken                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vCheckSleepingTasks(void)
{
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  while (Task* pWokenTask = pQueues->pPopSleepingQueue()) {
    pWokenTask->vState(TS_READY);
    vScheduleReadyTask(pWokenTask);
  }
}

/*----------------------------------------------------------------------------*/
/* schedule a ready to execute task                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleReadyTask(Task* pTask)
{
  //Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(TS_READY == pTask->eState());
  RASSERT(QK_NONE  == pTask->eQueue());

  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule ready: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  if (bAssign_IdleThread(pTask))
    return;
  
  vTask_Ready(pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                                thread                                      //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* append given idle thread onto dedicated thread vector                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddThreadDedicated(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(CN_NONE != pThread->m_eDedicated);
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAddThreadDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* append given idle thread onto general thread vector                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddThreadGeneral(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAddThreadGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* assign task to dedicated thread                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vAssignDedicatedThread(Thread* pThread)
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
  vTask_Executing(pThread->m_pTask);
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
void Scheduler::vPauseTillGeneralThreads(void)
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
/* task has returned from task loop                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vReturnTaskLoop(Task** ppTask)
{
  //
  // called from Thread::vLoop_general()
  //
  
  // disconnect / de-assign task from thread
  Task* pTask = *ppTask;
  *ppTask = nullptr;
  
  // first remove this task from the executing set
  vRemove_ExecutingSet(pTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_EXITED:    vTask_Exited(pTask);   break;
    case TS_SLEEPING:  vTask_Sleeping(pTask); break;
    case TS_WAITING:   vTask_Waiting(pTask);  break;
    case TS_YIELDED:   vTask_Yielded(pTask);  break;
      
      // should never come to thread handler for these states
      
    case TS_EXECUTING: ASSERT(0);
    case TS_READY:     ASSERT(0);
    case TS_NONE:      ASSERT(0);
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* calling thread waits for task to be assigned                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vWaitForTaskAssignment(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  vThread_Idle(pThread);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  ASSERT(TD_RUNNING == pThread->m_eThreadState);
  ASSERT(pThread->m_pTask);
  RASSERT(TS_READY   == pThread->m_pTask->eState() ||
          TS_YIELDED == pThread->m_pTask->eState());

  Pthread::vTlsSet(giTLS_TaskKey,pThread->m_pTask);
  ASSERT(Pthread::pTlsGet(giTLS_TaskKey));
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pThread->m_pTask->vThread(pThread);
  
  // add this task to the executing task queue
  vTask_Executing(pThread->m_pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                                private                                     //
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* attempt to pop an idle thread and assign given task to it                  */
/*----------------------------------------------------------------------------*/
bool Scheduler::bAssign_IdleThread(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Thread* pIdleThread = pQueues->pPopIdleThread();
  
  if (! pIdleThread)
    return false;
  
  ActionSequence::vAdd(pTask->pJob(),AN_THREAD_EXECUTE,pIdleThread->iThreadNo());
  
  pIdleThread->m_pTask = pTask;
  
  pIdleThread->m_nAssignedTasks++;
  
  pIdleThread->m_pMutexThread->vMutexLock();
  pIdleThread->m_pMutexThread->vCondSignal();
  pIdleThread->m_pMutexThread->vMutexUnlock();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* deassign task from thread                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vDeassign_Thread(Task* pTask, Thread* pThread)
{
  
}

/*----------------------------------------------------------------------------*/
/* remove given task from executing set                                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemove_ExecutingSet(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vRemoveFromExecutingSet(pTask);
  RASSERT(QK_NONE == pTask->eQueue());
}

/*----------------------------------------------------------------------------*/
/* remove given fully acked task from waiting set                             */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemove_WaitingSet(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(0 == pTask->iAcksRmg_assert_only_());

  RASSERT(QK_WAITING_SET == pTask->eQueue());
  pQueues->vRemoveFromWaitingSet(pTask);
  RASSERT(QK_NONE == pTask->eQueue());
}

/*----------------------------------------------------------------------------*/
/* task transitions to exited                                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Exited(Task* pTask)
{
  RASSERT(TS_EXITED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  ASSERT(! pTask->bWordBody());
  
  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* task transitions to executing                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Executing(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pTask->vState(TS_EXECUTING);
  
  pQueues->vAddToExecutingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* task transitions to ready                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Ready(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pTask->vState(TS_READY);
  ASSERT(QK_NONE == pTask->eQueue());

  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
    pQueues->vAddToReadyQueue(pTask);
  else
    pQueues->vAddToReadyQueueLP(pTask);
}

/*----------------------------------------------------------------------------*/
/* task transitions to sleeping                                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Sleeping(Task* pTask)
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
/* task transitions to waiting                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Waiting(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // task must be in the waiting state and locked
  RASSERT(TS_WAITING == pTask->eState());
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
/* task transitions to yielded                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Yielded(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // task must be in the yielded state
  RASSERT(TS_YIELDED == pTask->eState());
  
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
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_Idle(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  while (! pThread->m_pTask) {
    pThread->m_eThreadState = TD_IDLE;
    pQueues->vAppendIdleThread(pThread);
    pThread->m_pMutexThread->vCondWaitIndefinite();
    pThread->m_eThreadState = TD_RUNNING;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

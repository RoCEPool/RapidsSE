/*============================================================================*/
/* Scheduler.cpp -- Thread/Task Event Scheduler                               */
/*============================================================================*/

#include "Scheduling.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;


////////////////////////////////////////////////////////////////////////////////
//                             task interface                                 //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* ack a waiting task                                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  //
  // called from Step::vTaskAck()
  //
  
  bool bAmDecider = bTask_AckWaiting(pTaskBeingAcked,pTaskAcking);
  
  if (! bAmDecider)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(! pTaskBeingAcked->iAcksRmg_assert_only_());
  
  ActionSequence::vAdd(pTaskAcking->pJob(),
                       AN_AM_DECIDER,
                       pTaskBeingAcked->pJob());
  
  vTask_ResumeAck(pTaskBeingAcked);

  return true;
}

/*----------------------------------------------------------------------------*/
/* add new dedicated task into system (spawn a dedicated thread for it)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskDedicated(Task* pTask)
{
  //
  // called from Api::bSpawnJobDedicated()
  //
  
  Thread* pThread = new Thread;

  vTask_SpawnNew(pTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vThread_DedicatedTask(pThread,pTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  
  vTask_SpawnNew(pTask);

  Scheduler::vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* task has returned from task loop                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vReturnTaskLoop(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  Task* pTask = pThread->m_pTask;
  
  // disconnect / de-assign task from thread
  vThread_DeassignRunning(pThread);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // remove task from the executing set
  gpAttachMap->m_pQueues->vRemoveFromExecutingSet(pTask);
  
  // add task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_EXITED:    vTask_Exit(pTask);   break;
    case TS_SLEEPING:  vTask_Sleep(pTask);  break;
    case TS_WAITING:   vTask_Wait(pTask);   break;
    case TS_YIELDED:   vTask_Yield(pTask);  break;
      
      // should never come here for these states
    case TS_EXECUTING: ASSERT(0);
    case TS_READY:     ASSERT(0);
    case TS_NONE:      ASSERT(0);
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* schedule a ready to execute task                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleReadyTask(Task* pTask)
{
  RASSERT(TS_READY == pTask->eState());

  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule ready: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  if (bThread_AssignIdle(pTask))
    return;
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
    vQueue_AppendReady(pTask);
  else
    vQueue_AppendReadyLP(pTask);
}

/*----------------------------------------------------------------------------*/
/* check to see if tasks in the sleeping queue can be woken                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vWakeSleepingTasks(void)
{
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  vTask_WakeSleeping();
}


////////////////////////////////////////////////////////////////////////////////
//                             thread interface                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* append new thread onto dedicated thread vector                             */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddThreadDedicated(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  RASSERT(CN_NONE != pThread->m_eDedicated);
  
  vThread_AddDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* append new thread onto general thread vector                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddThreadGeneral(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  vThread_AddGeneral(pThread);
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
  
  // put task into thread local storage
  Pthread::vTlsSet(giTLS_TaskKey,pThread->m_pTask);
  ASSERT(Pthread::pTlsGet(giTLS_TaskKey));
  
  RASSERT(TS_READY == pThread->m_pTask->eState());
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pThread->m_pTask->vThread(pThread);
  
  // add this task to the executing task set
  vQueue_AddExecuting(pThread->m_pTask);
}

/*----------------------------------------------------------------------------*/
/* begin a new thread                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vBeginThread(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated() and Thread::vLoop_general()
  //
  
  vThread_Beginning(pThread);
}

/*----------------------------------------------------------------------------*/
/* end of thread loop                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vEndThread(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  vThread_Ending(pThread);
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  
  vThread_WaitAssignment(pThread);
  
  vThread_Running(pThread);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ASSERT(TD_RUNNING == pThread->m_eThreadState);
  ASSERT(pThread->m_pTask);
  RASSERT(TS_READY   == pThread->m_pTask->eState() ||
          TS_YIELDED == pThread->m_pTask->eState());

  // put task into thread local storage
  Pthread::vTlsSet(giTLS_TaskKey,pThread->m_pTask);
  ASSERT(Pthread::pTlsGet(giTLS_TaskKey));
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pThread->m_pTask->vThread(pThread);
  
  // add this task to the executing task set
  vQueue_AddExecuting(pThread->m_pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                            implementation                                  //
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* ack a waiting task                                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bTask_AckWaiting(Task* pTaskBeingAcked, Task* pTaskAcking)
{
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

  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* task is to exit                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Exit(Task* pTask)
{
  RASSERT(TS_EXITED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  
  ASSERT(! pTask->bWordBody());
  
  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* resume an acked task (that had been waiting)                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_ResumeAck(Task* pTask)
{
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Scheduling acked: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  pTask->vGrabLock();
  
  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
  
  pTask->vDropLock();
  
  gpAttachMap->m_pQueues->vRemoveFromWaitingSet(pTask);
  
  pTask->vState(TS_READY);
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* task transitions to sleeping                                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Sleep(Task* pTask)
{
  RASSERT(TS_SLEEPING == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule sleeping: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  // is the task still actually sleeping
  if (! pTask->bWakeupNow()) {
    vQueue_InsertSleeping(pTask);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupTask();
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  pCallingThread->vReExecuteTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* task transitions to new                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_SpawnNew(Task* pTask)
{
  ASSERT(TS_NONE == pTask->eState());
  
  pTask->vState(TS_READY);
}

/*----------------------------------------------------------------------------*/
/* task is to wait for acks                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wait(Task* pTask)
{
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add task to waiting set
  vQueue_AddWaiting(pTask);
  
  // drop task lock
  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* wake any sleeping tasks whose wakeup time has arrived                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_WakeSleeping(void)
{
  while (Task* pWokenTask = gpAttachMap->m_pQueues->pPopSleepingQueue()) {
    RASSERT(QK_NONE  == pWokenTask->eQueue());
    pWokenTask->vState(TS_READY);
    Scheduler::vScheduleReadyTask(pWokenTask);
  }
}

/*----------------------------------------------------------------------------*/
/* task has yielded                                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Yield(Task* pTask)
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



////////////////////////////////////////////////////////////////////////////////
//                          implementation for threads                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add thread to dedicated thread set                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddDedicated(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  pQueues->vAddThreadDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* add thread to general thread set                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneral(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  pQueues->vAddThreadGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AppendIdle(Thread* pThread)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  
  pQueues->vAppendIdleThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* attempt to pop an idle thread and assign given task to it                  */
/*----------------------------------------------------------------------------*/
bool Scheduler::bThread_AssignIdle(Task* pTask)
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
void Scheduler::vThread_DeassignRunning(Thread* pThread)
{
  pThread->m_pTask = nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread is beginning                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_Beginning(Thread* pThread)
{
  // thread has mutex while it is running
  pThread->m_eThreadState = TD_RUNNING;
  pThread->m_pMutexThread->vMutexLock();
  
  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
}

/*----------------------------------------------------------------------------*/
/* dedicate thread to task                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_DedicatedTask(Thread* pThread, Task* pTask)
{
  pThread->m_pTask = pTask;
  pThread->m_eDedicated = pTask->pJob()->eClass();
  
  pThread->vSpawnPthread_dedicated();
}

/*----------------------------------------------------------------------------*/
/* thread is ending                                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_Ending(Thread* pThread)
{
  pThread->m_pMutexThread->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* thread is running                                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_Running(Thread* pThread)
{
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  RASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  while (! pThread->m_pTask) {
    pThread->m_eThreadState = TD_IDLE;
    vThread_AppendIdle(pThread);
    pThread->m_pMutexThread->vCondWaitIndefinite();
    pThread->m_eThreadState = TD_RUNNING;
  }
}


////////////////////////////////////////////////////////////////////////////////
//                               support for queues                           //
////////////////////////////////////////////////////////////////////////////////
//! MOVE TO QUEUES

/*----------------------------------------------------------------------------*/
/* add task to executing set                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_AddExecuting(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pTask->vState(TS_EXECUTING);
  pQueues->vAddToExecutingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to waiting set                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_AddWaiting(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // task must be locked and in the waiting state
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_WAITING == pTask->eState());

  // add task to waiting set
  pQueues->vAddToWaitingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* append task to ready queue (normal priority)                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_AppendReady(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vAddToReadyQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* append task to ready queue (low-priority)                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_AppendReadyLP(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vAddToReadyQueueLP(pTask);
}

/*----------------------------------------------------------------------------*/
/* insert task into sleeping queue                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueue_InsertSleeping(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vAddToSleepingQueue(pTask);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

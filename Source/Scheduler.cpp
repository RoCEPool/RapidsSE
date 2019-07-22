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
/* ack a waiting task (returns true if task is resumed)                       */
/*----------------------------------------------------------------------------*/
bool Scheduler::bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  //
  // called from Step::vTaskAck()
  //
  
  bool bAmDecider = bTask_Ack(pTaskBeingAcked,pTaskAcking);
  
  if (! bAmDecider)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTask_Resume(pTaskBeingAcked);

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

  vTask_Ready(pTask);
  
  vThread_DedicatedTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system and schedule it as ready                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  
  vTask_Ready(pTask);
  
  Scheduler::vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* op task is to bide till operand arrives                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vBideTask(Task* pTask)
{
  //
  // called from Op::pPopOperand()
  //
  
  vTask_Bide(pTask);
}

/*----------------------------------------------------------------------------*/
/* task has returned from task loop (action depends on transition request)    */
/*----------------------------------------------------------------------------*/
void Scheduler::vReturnTaskLoop(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  Task* pTask = pThread->m_pTask;
  
  // disconnect / de-assign task from thread
  vThread_Deassign(pThread);
  
  // remove task from the executing set
  gpAttachMap->m_pQueues->vRemoveExecutingSet(pTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add task to another queue depending on its transition request
  switch (pTask->eTransition()) {
      
    case TR_BIDE:   vTask_Bide(pTask);   break;
    case TR_EXIT:   vTask_Exit(pTask);   break;
    case TR_SLEEP:  vTask_Sleep(pTask);  break;
    case TR_WAIT:   vTask_Wait(pTask);   break;
    case TR_YIELD:  vTask_Yield(pTask);  break;
      
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
  RASSERT(QK_NONE == pTask->eQueue());
  
  // attempt to assign a thread to the task
  if (bThread_AssignIdle(pTask))
    return;
  
  // append task to a ready queue
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
    gpAttachMap->m_pQueues->vAppendReadyQueue(pTask);
  else
    gpAttachMap->m_pQueues->vAppendReadyQueueLP(pTask);
}

/*----------------------------------------------------------------------------*/
/* trigger biding op task that operand has arrived                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTriggerTask(Task* pTask)
{
  //
  // called from Op::vAppendIn()
  //
  
  vTask_Trigger(pTask);
}

/*----------------------------------------------------------------------------*/
/* check to see if any tasks in the sleeping queue can be woken               */
/*----------------------------------------------------------------------------*/
void Scheduler::vWakeSleepingTasks(void)
{
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  vTask_Wakeup();
}


////////////////////////////////////////////////////////////////////////////////
//                             thread interface                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* append new thread onto dedicated thread vector                             */
/*----------------------------------------------------------------------------*/
void Scheduler::vThreadAddDedicated(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_AddDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* append new thread onto general thread vector                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vThreadAddGeneral(Thread* pThread)
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

  vThread_AssignDedicated(pThread);
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
/* pause till all general threads are all idle (at system startup)            */
/*----------------------------------------------------------------------------*/
void Scheduler::vPauseTillGeneralThreads(void)
{
  //
  // called from Engine::vStartSystem()
  //
  
  vThread_PauseGeneral();
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from dedicated thread vector                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vThreadRemoveDedicated(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_RemoveDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from general thread vector                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThreadRemoveGeneral(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  vThread_RemoveGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* calling thread waits for task to be assigned                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vWaitForTaskAssignment(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  // thread waits for task assignement
  vThread_WaitAssignment(pThread);
  
  // task was assigned to this thread
  Task* pTask = pThread->m_pTask;
  vTask_Thread(pTask,pThread);

  // task enters execute state
  pTask->vTransition(TR_EXECUTE);
  vTask_Execute(pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                          implementation for tasks                          //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* ack a waiting task                                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bTask_Ack(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  // note: the task may not yet be waiting.  there is an indefinite period
  //       between when the task grabs its lock + sets the number of acks and
  //       when it enters the waiting state and drops the lock
  
  RASSERT(pTaskBeingAcked->iAcksRmg_assert_only_() > 0);
  
  RASSERT(TS_EXECUTING == pTaskAcking->eState());
  
//  Job* pJobBeingAcked = pTaskBeingAcked->pJob();
//  Job* pJobAcking     = pTaskAcking->pJob();
//
//  if (pJobAcking->m_bMarkActivity || pJobBeingAcked->m_bMarkActivity) {
//    ActivityLog::vMarkJob(pJobBeingAcked,
//                          "pTaskBeingAcked:%p Job %s "_Stz,
//                          pTaskBeingAcked,
//                          aCN[pJobBeingAcked->eClass()]
//                          );
//    ActivityLog::vMarkJob(pJobAcking,
//                          "pTaskAcking:%p Job %s "_Stz,
//                          pTaskAcking,
//                          aCN[pJobAcking->eClass()]
//                          );
//  }
//
//  ActionSequence::vAdd(pTaskAcking->pJob(),
//                       AN_ACK_TASK,
//                       pTaskBeingAcked->pJob());
  

  
  
  // decrement the acks remaining
  bool bAmDecider = pTaskBeingAcked->bAcksRmgDec();
  
//  if (bAmDecider)
//    ActionSequence::vAdd(pTaskAcking->pJob(),
//                         AN_AM_DECIDER,
//                         pTaskBeingAcked->pJob());
//
//  if (bAmDecider) {
//    if (pJobBeingAcked->m_bMarkActivity || pJobAcking->m_bMarkActivity)
//      ActivityLog::vMarkJob(pJobAcking,
//                            "Decider %s acks %s"_Stz,
//                            aCN[pJobAcking->eClass()],
//                            aCN[pJobBeingAcked->eClass()]
//                            );
//  }
//  
//  if ( (pJobBeingAcked && JT_NONE != pJobBeingAcked->eJobTrace()) ||
//      (pJobAcking && JT_NONE != pJobAcking->eJobTrace())  ) {
//    vTrace("Ack of %s task %p by %s task %p ; %s\n"_Stz,
//           pJobBeingAcked ? aCN[pJobBeingAcked->m_pJobClass->m_eClass] : "<no job>"_Stz,
//           pTaskBeingAcked,
//           pJobAcking ? aCN[pJobAcking->m_pJobClass->m_eClass] : "<no job>"_Stz,
//           pJobAcking,
//           bAmDecider ? "decider" : "not decider");
//  }

  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* op-task is to bide till operand arrives                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Bide(Task* pTask)
{
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(TR_BIDE == pTask->eTransition());
  RASSERT(QK_NONE == pTask->eQueue());
  
  RASSERT(pTask->bIsLocked());
  
  pTask->vState(TS_BIDING);
  pTask->vTransition(TR_NONE);
  
  // add task to biding set
  gpAttachMap->m_pQueues->vAddBidingSet(pTask);
  
  // **do not touch task** (other than to drop lock)
  
  // drop task lock
  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* task transitions to executing                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Execute(Task* pTask)
{
  RASSERT(TS_READY   == pTask->eState());
  RASSERT(TR_EXECUTE == pTask->eTransition());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vState(TS_EXECUTING);
  pTask->vTransition(TR_NONE);

  gpAttachMap->m_pQueues->vAddExecutingSet(pTask);
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* task transitions to exited and is deleted                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Exit(Task* pTask)
{
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(TR_EXIT == pTask->eTransition());
  RASSERT(QK_NONE == pTask->eQueue());
  ASSERT(! pTask->bWordBody());

  pTask->vState(TS_EXITED);
  pTask->vTransition(TR_NONE);

  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* task transitions to none                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_None(Task* pTask)
{
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vState(TS_NONE);
}

/*----------------------------------------------------------------------------*/
/* task transitions to ready                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Ready(Task* pTask)
{
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vState(TS_READY);
}

/*----------------------------------------------------------------------------*/
/* resume a fully acked task (that had been waiting)                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Resume(Task* pTask)
{
  RASSERT(! pTask->iAcksRmg_assert_only_());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Scheduling acked: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  // cycle the task lock to ensure task has fully entered wait state
  pTask->vGrabLock();
  pTask->vDropLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_WAITING_SET == pTask->eQueue());

  gpAttachMap->m_pQueues->vRemoveWaitingSet(pTask);
  
  pTask->vState(TS_READY);
  Scheduler::vScheduleReadyTask(pTask);
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* task transitions to sleeping                                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Sleep(Task* pTask)
{
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(TR_SLEEP == pTask->eTransition());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vTransition(TR_NONE);

  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Schedule sleeping: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  // is the task still actually sleeping
  if (! pTask->bWakeupNow()) {
    pTask->vState(TS_SLEEPING);
    gpAttachMap->m_pQueues->vAddSleepingQueue(pTask);
    
    // **do not touch task**

    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupClear();
  
  pTask->vTransition(TR_READY);
  vTask_Ready(pTask);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  vThread_ReExecuteTask(pThread,pTask);
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* task has been assigned the given thread                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Thread(Task* pTask, Thread* pThread)
{
  RASSERT(TS_READY   == pTask->eState());
  
  ASSERT(TD_ASSIGNED == pThread->m_eThreadState);
  ASSERT(pTask == pThread->m_pTask);
  
  pTask->vThread(pThread);
  
  // ok to touch task
}

/*----------------------------------------------------------------------------*/
/* op task has an operand arriving into its queue while its dormant           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Trigger(Task* pTask)
{
  RASSERT(TS_BIDING == pTask->eState());
  RASSERT(TR_NONE == pTask->eTransition());
  RASSERT(QK_BIDING_SET == pTask->eQueue());
  ASSERT(pTask->bIsLocked());
  
  gpAttachMap->m_pQueues->vRemoveBidingSet(pTask);
  
  pTask->vState(TS_READY);
  pTask->vDropLock();
  
  Scheduler::vScheduleReadyTask(pTask);
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* task is to wait for acks                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wait(Task* pTask)
{
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(TR_WAIT == pTask->eTransition());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(pTask->bIsLocked());

  pTask->vState(TS_WAITING);
  pTask->vTransition(TR_NONE);
  
  // activity
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityLog::vMarkInfo(
                             "Schedule waiting: %s %p"_Stz,
                             aCN[pTask->pJob()->eClass()],
                             pTask->pJob());
    else
      ActivityLog::vMarkJob(pTask->pJob(),
                            "Schedule waiting: %s %p"_Stz,
                            aCN[pTask->pJob()->eClass()],
                            pTask->pJob());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add task to waiting set
  gpAttachMap->m_pQueues->vAddWaitingSet(pTask);
  
  // **do not touch task** (other than to drop lock)

  // drop task lock
  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* task is to be woken                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wake(Task* pTask)
{
  RASSERT(TS_SLEEPING == pTask->eState());
  RASSERT(QK_NONE  == pTask->eQueue());
  
  vTask_Ready(pTask);
  
  Scheduler::vScheduleReadyTask(pTask);
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* wakeup any sleeping tasks whose wakeup time has arrived                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wakeup(void)
{
  while (Task* pTask = gpAttachMap->m_pQueues->pPopSleepingQueue())
    vTask_Wake(pTask);
    // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* task has yielded                                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Yield(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // task must be in the yielded state
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(TR_YIELD == pTask->eTransition());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vState(TS_READY);
  pTask->vTransition(TR_NONE);
  
  Thread* pThread = Thread::pThisTLSThread();
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityLog::vMarkInfo(
                             "Schedule yielded: %s %p"_Stz,
                             aCN[pTask->pJob()->eClass()],
                             pTask->pJob());
    else
      ActivityLog::vMarkJob(pTask->pJob(),
                            "Schedule yielded: %s %p"_Stz,
                            aCN[pTask->pJob()->eClass()],
                            pTask->pJob());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // - A general thread has executed a task which retured TR_YIELD.
  //   If the ready queue is empty then this general thread can
  //     continue to execute the yield task.
  //   Otherwise we add the task to the ready queue
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL) {
    i8 iLengthReadyQueue = pQueues->nTasksReady();
    if (0 == iLengthReadyQueue) {
      vThread_ReExecuteTask(pThread,pTask);
      // **do not touch task**
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTask_Ready(pTask);
  Scheduler::vScheduleReadyTask(pTask);
  // **do not touch task**

//  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL) {
//    gpAttachMap->m_pQueues->vAppendReadyQueue(pTask);
//  }
//  else {
//    gpAttachMap->m_pQueues->vAppendReadyQueueLP(pTask);
//  }
}



////////////////////////////////////////////////////////////////////////////////
//                          implementation for threads                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add thread to dedicated thread set                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddDedicated(Thread* pThread)
{
  RASSERT(CN_NONE != pThread->m_eDedicated);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  pQueues->vThreadAddDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* add thread to general thread set                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneral(Thread* pThread)
{
  RASSERT(CN_NONE == pThread->m_eDedicated);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  pQueues->vThreadAddGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AppendIdle(Thread* pThread)
{
  RASSERT(! pThread->m_pTask);
  RASSERT(CN_NONE == pThread->m_eDedicated);

  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  pQueues->vThreadAppendIdle(pThread);
}

/*----------------------------------------------------------------------------*/
/* assign task to dedicated thread                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AssignDedicated(Thread* pThread)
{
  ASSERT(TD_ASSIGNED == pThread->m_eThreadState);
  ASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  RASSERT(pThread->m_pTask);
  RASSERT(TS_READY == pThread->m_pTask->eState());
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pThread->m_pTask->vThread(pThread);
  
  // add this task to the executing task set
  gpAttachMap->m_pQueues->vAddExecutingSet(pThread->m_pTask);
}

/*----------------------------------------------------------------------------*/
/* attempt to pop an idle thread and assign given task to it                  */
/*----------------------------------------------------------------------------*/
bool Scheduler::bThread_AssignIdle(Task* pTask)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Thread* pThread = pQueues->pThreadPopIdle();
  
  if (! pThread)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //ActionSequence::vAdd(pTask->pJob(),AN_THREAD_EXECUTE,pIdleThread->iThreadNo());
  
  pThread->m_pTask = pTask;
  pThread->m_nAssignedTasks++;
  
  pThread->m_eThreadState = TD_ASSIGNED;

  pThread->m_pMutexThread->vMutexLock();
  pThread->m_pMutexThread->vCondSignal();
  pThread->m_pMutexThread->vMutexUnlock();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* deassign task from thread                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_Deassign(Thread* pThread)
{
  pThread->m_pTask = nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread is beginning                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_Beginning(Thread* pThread)
{
  pThread->vThisTLSThread();
  
  // thread has mutex while it is running
  pThread->m_eThreadState = TD_ASSIGNED;
  pThread->m_pMutexThread->vMutexLock();
  
  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
}

/*----------------------------------------------------------------------------*/
/* dedicate thread to task                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_DedicatedTask(Task* pTask)
{
  Thread* pThread = new Thread;

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
/* pause till general threads are all idle                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_PauseGeneral(void)
{
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
/* re-execute same task (tried to sleep or yield but could continue)          */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ReExecuteTask(Thread* pThread, Task* pTask)
{
  // task should be ready or yielded
  RASSERT(TS_READY == pTask->eState());
  //pTask->vState(TS_EXECUTING);
  
  // thread should be idle
  RASSERT(TD_ASSIGNED == pThread->m_eThreadState);
  
  // thread should have no task
  RASSERT(! pThread->m_pTask);
  
  // set task to be executed
  pThread->m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from thread vector                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveDedicated(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vThreadRemoveDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from thread vector                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveGeneral(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vThreadRemoveGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  ASSERT(TD_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  while (! pThread->m_pTask) {
    pThread->m_eThreadState = TD_IDLE;
    vThread_AppendIdle(pThread);
    pThread->m_pMutexThread->vCondWaitIndefinite();
  }

  RASSERT(TD_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pTask);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

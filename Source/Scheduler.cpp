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
/* ack a waiting task (returns true if task was resumed)                      */
/*----------------------------------------------------------------------------*/
bool Scheduler::bAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking)
{
  //
  // called from Step::vTaskAck()
  //
  
  bool bAmDecider = bTask_Ack(pTaskWaiting,pTaskAcking);
  
  if (! bAmDecider)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTask_Resume(pTaskWaiting);

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

  vThread_SpawnDedicated(pTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system and schedule it                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  
  vTask_Schedule(pTask);
}

/*----------------------------------------------------------------------------*/
/* execute dedicated task                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vExecuteDedicatedTask(Task* pTask, Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  vThread_ExecuteDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* task has returned from task loop, action depends on transition request     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskLoopTransition(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  Task* pTask = pThread->m_pTask;
  
  // remove task from the executing set
  vTask_Desist(pTask);

  // disconnect / de-assign task from calling thread
  vThread_UnassignTask(pThread);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // transition task to different state/queue depending on transition request
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
/* trigger biding op task that operand has arrived                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTriggerBidingTask(Task* pTask)
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
void Scheduler::vAddDedicatedThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_AddDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* append new thread onto general thread vector                               */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddGeneralThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  vThread_AddGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* beginning of thread loop                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vBeginThreadLoop(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated() and Thread::vLoop_general()
  //
  
  vThread_BeginLoop(pThread);
}

/*----------------------------------------------------------------------------*/
/* end of thread loop                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vEndThreadLoop(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  vThread_EndLoop(pThread);
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
void Scheduler::vRemoveDedicatedThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_RemoveDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from general thread vector                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemoveGeneralThread(Thread* pThread)
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
  vTask_Execute(pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                          implementation for tasks                          //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* change any task state to TS_ABLE (able to be executed)                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Able(Task* pTask)
{
  // note: task can be in any current state
  
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  pTask->vState(TS_ABLE);
}

/*----------------------------------------------------------------------------*/
/* ack a waiting task                                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bTask_Ack(Task* pTaskWaiting, Task* pTaskAcking)
{
  // note: the task may not yet be waiting.  there is an indefinite period
  //       between when the task grabs its lock + sets the number of acks and
  //       when it enters the waiting state and drops the lock
  
  RASSERT(pTaskWaiting->iAcksRmg_assert_only_() > 0);
  
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // decrement the acks remaining
  bool bAmDecider = pTaskWaiting->bAcksRmgDec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
/* locked desisted op-task transitions to TS_BIDING and added to biding set   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Bide(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_BIDE == pTask->eTransition());
  
  pTask->vTransitionClear();
  pTask->vState(TS_BIDING);
  
  gpAttachMap->m_pQueues->vAddBidingSet(pTask);
  // **do not touch task** (other than to drop lock)
  
  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* remove executing task from executing set and change state to TS_DESISTED   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Desist(Task* pTask)
{
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(QK_EXECUTING_SET == pTask->eQueue());
  RASSERT(TR_NONE != pTask->eTransition());

  gpAttachMap->m_pQueues->vRemoveExecutingSet(pTask);
  
  pTask->vState(TS_DESISTED);
}

/*----------------------------------------------------------------------------*/
/* change able-to-run task to TS_EXECUTING and add to executing set           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Execute(Task* pTask)
{
  RASSERT(TS_ABLE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  pTask->vState(TS_EXECUTING);

  gpAttachMap->m_pQueues->vAddExecutingSet(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* desisted task transitions to TS_EXITED and is deleted                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Exit(Task* pTask)
{
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_EXIT == pTask->eTransition());

  pTask->vTransitionClear();
  pTask->vState(TS_EXITED);

  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* change able-to-run task state to TS_READY and append to a ready queue      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Ready(Task* pTask)
{
  RASSERT(TS_ABLE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  pTask->vState(TS_READY);

  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
    gpAttachMap->m_pQueues->vAppendReadyQueue(pTask);
  else
    gpAttachMap->m_pQueues->vAppendReadyQueueLP(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* fully acked task is removed from waiting set,                              */
/*   transitions to TS_ABLE and scheduled                                     */
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
  RASSERT(TR_NONE == pTask->eTransition());

  gpAttachMap->m_pQueues->vRemoveWaitingSet(pTask);
  
  vTask_Able(pTask);
  vTask_Schedule(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* attempt to assign a thread to able-to-run task, else add to ready queue    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Schedule(Task* pTask)
{
  RASSERT(TS_ABLE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  if (bThread_AssignIdle(pTask))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTask_Ready(pTask);
}

/*----------------------------------------------------------------------------*/
/* transition desisted task to TS_SLEEPING and add to sleeping queue          */
/*   (if wakeup time has already arrived, calling thread re-executes task)    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Sleep(Task* pTask)
{
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_SLEEP == pTask->eTransition());

  pTask->vTransitionClear();
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Schedule sleeping: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  // is the task to actually sleep?
  if (! pTask->bWakeupNow()) {
    pTask->vState(TS_SLEEPING);
    gpAttachMap->m_pQueues->vAddSleepingQueue(pTask);
    // **do not touch task**
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupClear();
  
  Thread* pThread = Thread::pThisTLSThread();
  vThread_ContinueSame(pThread,pTask);
  
  vTask_Able(pTask);
  vTask_Execute(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* able-to-run task has been assigned the given thread                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Thread(Task* pTask, Thread* pThread)
{
  RASSERT(TS_ABLE == pTask->eState());
  
  RASSERT(TD_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pTask == pThread->m_pTask);
  
  pTask->vThread(pThread);
  // ok to touch task
}

/*----------------------------------------------------------------------------*/
/* locked biding op-task is removed from biding set,                          */
/*   transitions to TS_ABLE, unlocked and scheduled                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Trigger(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_BIDING == pTask->eState());
  RASSERT(QK_BIDING_SET == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  gpAttachMap->m_pQueues->vRemoveBidingSet(pTask);
  
  pTask->vDropLock();
  
  vTask_Able(pTask);
  vTask_Schedule(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* transition locked desisted task to TS_WAITING and add to waiting set       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wait(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_WAIT == pTask->eTransition());

  pTask->vTransitionClear();
  pTask->vState(TS_WAITING);
  
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
  
  gpAttachMap->m_pQueues->vAddWaitingSet(pTask);
  // **do not touch task** (other than to drop lock)

  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* wakeup any sleeping tasks whose wakeup time has arrived                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wakeup(void)
{
  while (Task* pTask = gpAttachMap->m_pQueues->pPopSleepingQueue()) {
    vTask_Able(pTask);
    vTask_Schedule(pTask);
    // **do not touch task**
  }
}

/*----------------------------------------------------------------------------*/
/* desisted task has yielded and is scheduled                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Yield(Task* pTask)
{
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_YIELD == pTask->eTransition());

  pTask->vTransitionClear();

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
  // no other ready to run tasks, so thread continues executing the task
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL) {
    i8 iLengthReadyQueue = gpAttachMap->m_pQueues->nTasksReady();
    if (0 == iLengthReadyQueue) {
      vTask_Able(pTask);
      vTask_Execute(pTask);
      vThread_ContinueSame(pThread,pTask);
      // **do not touch task**
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other ready to run tasks, task is scheduled

  vTask_Able(pTask);
  vTask_Schedule(pTask);
  // **do not touch task**
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
  pQueues->vAddDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* add thread to general thread set                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneral(Thread* pThread)
{
  RASSERT(CN_NONE == pThread->m_eDedicated);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pThread->m_eThreadState = TD_IDLE;
  pQueues->vAddGeneralThread(pThread);
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
/* attempt to pop an idle thread and assign given task to it                  */
/*----------------------------------------------------------------------------*/
bool Scheduler::bThread_AssignIdle(Task* pTask)
{
  RASSERT(TS_ABLE == pTask->eState());
  
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
/* thread is beginning                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_BeginLoop(Thread* pThread)
{
  pThread->vThisTLSThread();
  
  // thread has mutex while it is running
  pThread->m_eThreadState = TD_UNASSIGNED;
  pThread->m_pMutexThread->vMutexLock();
  
  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
}

/*----------------------------------------------------------------------------*/
/* continue execute same task (tried to sleep or yield but could continue)    */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ContinueSame(Thread* pThread, Task* pTask)
{
  RASSERT(TS_EXECUTING == pTask->eState());
  
  RASSERT(TD_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(! pThread->m_pTask);
  
  pThread->m_pTask = pTask;
  pThread->m_eThreadState = TD_ASSIGNED;
}

/*----------------------------------------------------------------------------*/
/* thread is ending                                                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_EndLoop(Thread* pThread)
{
  pThread->m_pMutexThread->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* assign task to dedicated thread                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ExecuteDedicated(Thread* pThread)
{
  ASSERT(TD_UNASSIGNED == pThread->m_eThreadState);
  ASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  RASSERT(pThread->m_pTask);
  RASSERT(TS_ABLE  == pThread->m_pTask->eState());
  
  pThread->m_pTask->vThread(pThread);
  
  pThread->m_pTask->vState(TS_EXECUTING);
  gpAttachMap->m_pQueues->vAddExecutingSet(pThread->m_pTask);
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
/* remove dedicated thread from thread vector                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveDedicated(Thread* pThread)
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
void Scheduler::vThread_RemoveGeneral(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->vRemoveGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* spawn dedicate thread for given task                                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_SpawnDedicated(Task* pTask)
{
  RASSERT(TS_ABLE == pTask->eState());
  
  Thread* pThread = new Thread;
  
  pThread->m_pTask = pTask;
  pThread->m_eDedicated = pTask->pJob()->eClass();
  
  pThread->vSpawnPthread_dedicated();
}

/*----------------------------------------------------------------------------*/
/* deassign task from thread                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_UnassignTask(Thread* pThread)
{
  pThread->m_pTask = nullptr;
  pThread->m_eThreadState = TD_UNASSIGNED;
}

/*----------------------------------------------------------------------------*/
/* append idle thread onto idle thread queue                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  ASSERT(TD_UNASSIGNED == pThread->m_eThreadState);
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

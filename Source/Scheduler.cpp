/*============================================================================*/
/* Scheduler.cpp -- Job/Task/Thread Scheduler Mark II                         */
/*============================================================================*/

#include "Threads.h"

// /////////////////////////////////////////////////////////////////////////////
//                             task transition                                //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* task has returned from task loop, action depends on transition request     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskLoopTransition(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move task out of executing queue
  
  Task* pTask = pThread->m_pTask;
  RASSERT(pThread == pTask->pvThread_DONT_DEREF());
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(QK_EXECUTING_SET == pTask->eQueue());

  // remove task from the executing set
  
  Queues* pQueues = AM::pQueues();
  pQueues->vRemoveExecutingSet(pTask);
  RASSERT(QK_NONE == pTask->eQueue());
  pTask->vState(TS_DESISTED);

  // disconnect / de-assign task from calling thread
  vThread_UnassignTask(pThread);
  RASSERT(nullptr == pTask->pvThread_DONT_DEREF());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // transition task to different state+queue depending on transition request
  
  switch (pTask->eSchedulerAct()) {
      
    case SA_BIDE_FOR_OPERAND:  vTaskTransit_BideForOperand(pTask);   break;
    case SA_DELETE_THIS_TASK:  vTaskTransit_DeleteThisTask(pTask);   break;
    case SA_SIGNAL_COMPLETION: vTaskTransit_SignalCompletion(pTask); break;
    case SA_SLEEP_SOME_TIME:   vTaskTransit_SleepSomeTime(pTask);    break;
    case SA_WAIT_FOR_RESUME:   vTaskTransit_WaitForResume(pTask);    break;
    case SA_YIELD_FOR_OTHERS:  vTaskTransit_YieldForOthers(pTask);   break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* op-task with empty operand queue transitions to TS_BIDING and              */
/*   is added to biding set                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskTransit_BideForOperand(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(SA_BIDE_FOR_OPERAND == pTask->eSchedulerAct());
  pTask->vSchedulerActClear();

  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_BIDE,
              "Transition task to TS_BIDING; add to BidingSet"_Stz); //!format-LogTsk

  Queues* pQueues = AM::pQueues();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vState(TS_BIDING);
  pQueues->vAddBidingSet(pTask);
  
  /** do not touch task **/
  
  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* desisted task transitions to TS_COMPLETED state and is deleted             */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskTransit_DeleteThisTask(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(SA_DELETE_THIS_TASK == pTask->eSchedulerAct());
  pTask->vSchedulerActClear();
  
  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_EXIT,
              "Task to TS_COMPLETED state; and deleted"_Stz); //!format-LogTsk
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vState(TS_COMPLETED);
  
  /** do not touch task, leave task locked **/
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* task has completed; signal the thread waiting on the tasks mutex           */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskTransit_SignalCompletion(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(SA_SIGNAL_COMPLETION == pTask->eSchedulerAct());
  pTask->vSchedulerActClear();

  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SLEEP,
              "Add desisted task to SleepingQueue"_Stz); //!format-LogTsk
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vState(TS_COMPLETED);
  
  pTask->vMutexLock();
  pTask->vMutexSignal();
  pTask->vMutexUnlock();

  /**do not touch task, leave locked. task is reponsibility of signaled thread **/
}

/*----------------------------------------------------------------------------*/
/* transition desisted task to TS_SLEEPING state and add to sleeping queue    */
/*   (if wakeup time has already arrived, calling thread re-executes task)    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskTransit_SleepSomeTime(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(SA_SLEEP_SOME_TIME == pTask->eSchedulerAct());
  pTask->vSchedulerActClear();
  
  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SLEEP,
              "Add desisted task to SleepingQueue"_Stz); //!format-LogTsk
  
  Queues* pQueues = AM::pQueues();
  Thread* pThread = Thread::pThisTLSThread();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  // is the task to actually sleep?
  if (! pTask->bWakeupNow()) {
    pTask->vState(TS_SLEEPING);
    pQueues->vAddSleepingQueue(pTask);
    /** do not touch task **/
    pTask->vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupClear();
  
  pTask->vState(TS_EXECUTING);
  pQueues->vAddExecutingSet(pTask);
  vThread_ContinueSame(pThread,pTask);
  /** do not touch task **/
  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* add task to waiting set                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskTransit_WaitForResume(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(SA_WAIT_FOR_RESUME == pTask->eSchedulerAct());
  pTask->vSchedulerActClear();
  
  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_WAIT,
              "Task moved to WaitingSet"_Stz); //!format-LogTsk
  
  Queues* pQueues = AM::pQueues();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vState(TS_WAITING);
  pQueues->vAddWaitingSet(pTask);
  
  /** do not touch task **/

  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* desisted task has yielded and is scheduled                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskTransit_YieldForOthers(Task* pTask)
{
  RASSERT(pTask->bIsLocked());
  RASSERT(SA_YIELD_FOR_OTHERS == pTask->eSchedulerAct());
  pTask->vSchedulerActClear();

  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_YIELD,
              "Yield desisted task; continue executing or schedule it"_Stz); //!format-LogTsk

  Queues* pQueues = AM::pQueues();
  Thread* pThread = Thread::pThisTLSThread();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  // no other ready to run tasks, so thread continues executing the task
  
  if (pTask->pJob()->aJobClass()->m_ePriority == PY_NORMAL) {
    i8 iLengthReadyQueue = pQueues->nTasksReady();
    if (0 == iLengthReadyQueue) {
      pTask->vState(TS_EXECUTING);
      pQueues->vAddExecutingSet(pTask);
      vThread_ContinueSame(pThread,pTask);
      
      /** do not touch task **/

      pTask->vDropTaskLock();
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other ready to run tasks, task is scheduled
  
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(SA_NONE == pTask->eSchedulerAct());
  
  vScheduleTask(pTask);
  
  /** do not touch task **/

  pTask->vDropTaskLock();
}


// /////////////////////////////////////////////////////////////////////////////
//                               cross task                                   //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* ack a waiting task (returns true if task was resumed)                      */
/*----------------------------------------------------------------------------*/
bool Scheduler::bTaskCross_AckWaiting(Task* pTaskWaiting, Task* pTaskAcking)
{
  //
  // called from Step::vTaskAck()
  //
    
  RASSERT(pTaskWaiting->iAcksRmg_assert_only_() > 0);
  
  RASSERT(TS_EXECUTING == pTaskAcking->eState());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // decrement the acks remaining
  bool bAmDecider = pTaskWaiting->bAcksRmgDec();
    
  if (! bAmDecider)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(! pTaskWaiting->iAcksRmg_assert_only_());
  
  // cycle the task lock to ensure task has fully entered wait state
  pTaskWaiting->vGrabTaskLock();
  pTaskWaiting->vDropTaskLock();
    
  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
  RASSERT(TS_WAITING == pTaskWaiting->eState());
  RASSERT(QK_WAITING_SET == pTaskWaiting->eQueue());
  RASSERT(SA_NONE == pTaskWaiting->eSchedulerAct());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveWaitingSet(pTaskWaiting);
    
  RASSERT(TS_DESISTED == pTaskWaiting->eState());
  RASSERT(QK_NONE == pTaskWaiting->eQueue());
  RASSERT(SA_NONE == pTaskWaiting->eSchedulerAct());
  
  vScheduleTask(pTaskWaiting);
  // **do not touch task**

  return true;
}

/*----------------------------------------------------------------------------*/
/* trigger op-task that operand has arrived while it is biding                */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_TriggerBiding(Task* pTaskBiding, Task* pTriggeringTask)
{
  //
  // called from Op::vAppendOperand()
  //
  
  RASSERT(pTaskBiding->bIsLocked());
  RASSERT(TS_BIDING == pTaskBiding->eState());
  RASSERT(QK_BIDING_SET == pTaskBiding->eQueue());
  RASSERT(SA_NONE == pTaskBiding->eSchedulerAct());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveBidingSet(pTaskBiding);
  
  pTaskBiding->vDropTaskLock();
  
  RASSERT(TS_DESISTED == pTaskBiding->eState());
  RASSERT(QK_NONE == pTaskBiding->eQueue());
  RASSERT(SA_NONE == pTaskBiding->eSchedulerAct());
  
  vScheduleTask(pTaskBiding);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* check to see if any tasks in the sleeping queue can be woken               */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_WakeSleepers(Task* pWakerTask)
{
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  Queues* pQueues = AM::pQueues();
  
  while (Task* pTask = pQueues->pPopSleepingQueue()) {
        
    RASSERT(TS_DESISTED == pTask->eState());
    RASSERT(QK_NONE == pTask->eQueue());
    RASSERT(SA_NONE == pTask->eSchedulerAct());
    
    vScheduleTask(pTask);
    // **do not touch task**
  }
}

// /////////////////////////////////////////////////////////////////////////////
//                                task api                                    //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add new dedicated task into system (spawn a dedicated thread for it)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddDedicated(Task* pTask)
{
  //
  // called from Api::bSpawnJobDedicated()
  //
  
  pTask->vState(TS_DESISTED);
  vThread_SpawnDedicated(pTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(SA_NONE == pTask->eSchedulerAct());
  
  pTask->vState(TS_DESISTED);

  vScheduleTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* set dedicated task to be executed by dedicated thread                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_ExecuteDedicated(Task* pTask, Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  vThread_ExecuteDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* set number of acks required by (soon to be) waiting task                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_RequiredAcks(Task* pTask, i8 nAcksRequired)
{
  //
  // called from ???
  //
  
  pTask->vAcksRmg(nAcksRequired);
}



// /////////////////////////////////////////////////////////////////////////////
//                              job interface                                 //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add job to originating list                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_AddOriginating(Job* pJob)
{
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pOriginatingJobs->vGrabLock();
  pQueues->m_pOriginatingJobs->vAddJob(pJob);
  pQueues->m_pOriginatingJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_ORIGIN,
              "Job added to OriginatingJobs"_Stz); //!format-LogJob
}

/*----------------------------------------------------------------------------*/
/* add job to participating list                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_AddParticipating(Job* pJob)
{
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pParticipatingJobs->vGrabLock();
  pQueues->m_pParticipatingJobs->vAddJob(pJob);
  pQueues->m_pParticipatingJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_PARTG,
              "Job added to ParticipatingJobs"_Stz); //!format-LogJob
  
}

/*----------------------------------------------------------------------------*/
/* dump originating job list                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_DumpOriginating(void)
{
  Queues* pQueues = AM::pQueues();
  
  AM::vLogQue(SL_INF,pQueues,QA_QUE_ORIGINATORS,
              "OriginatingJobs list:"_Stz); //!format-LogQue

  pQueues->m_pOriginatingJobs->vGrabLock();
  pQueues->m_pOriginatingJobs->vContentDump();
  pQueues->m_pOriginatingJobs->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump participating job list                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_DumpParticipating(void)
{
  Queues* pQueues = AM::pQueues();
  
  AM::vLogQue(SL_INF,pQueues,QA_QUE_PARTICPANTS,
              "ParticipatingJobs list:"_Stz); //!format-LogQue
  
  pQueues->m_pParticipatingJobs->vGrabLock();
  pQueues->m_pParticipatingJobs->vContentDump();
  pQueues->m_pParticipatingJobs->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* find job in originating list                                               */
/*----------------------------------------------------------------------------*/
Job* Scheduler::pJob_FindOriginating(i8 iJobId)
{
  Queues* pQueues = AM::pQueues();
  
  AM::vLogQue(SL_INF,pQueues,QA_QUE_FIND_ORIG,
              "Find job %lld in OriginatingJobs"_Stz, //!format-LogQue
              iJobId);
  
  Job* pJob = pQueues->m_pOriginatingJobs->pFindJob(iJobId);
  
  if (pJob)
    AM::vLogQue(SL_INF,pQueues,QA_QUE_FOUND_ORIG,
                "found job %lld in OriginatingJobs"_Stz, //!format-LogQue
                iJobId);
  else
    AM::vLogQue(SL_INF,pQueues,QA_QUE_NOT_ORIG,
                "did not find job %lld in OriginatingJobs"_Stz, //!format-LogQue
                iJobId);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find job in participating list                                             */
/*----------------------------------------------------------------------------*/
Job* Scheduler::pJob_FindParticipating(i8 iJobId)
{
  Queues* pQueues = AM::pQueues();

  AM::vLogQue(SL_INF,pQueues,QA_QUE_FIND_PARTG,
              "find job %lld in ParticipatingJobs"_Stz, //!format-LogQue
              iJobId);
  
  Job* pJob = pQueues->m_pParticipatingJobs->pFindJob(iJobId);
  
  if (pJob)
    AM::vLogQue(SL_INF,pQueues,QA_QUE_FOUND_PARTG,
                "found job %lld in ParticipatingJobs"_Stz, //!format-LogQue
                iJobId);
  else
    AM::vLogQue(SL_INF,pQueues,QA_QUE_NOT_PARTG,
                "did not find job %lld in ParticipatingJobs"_Stz, //!format-LogQue
                iJobId);

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* remove job from originating list                                           */
/*----------------------------------------------------------------------------*/
bool Scheduler::bJob_RemoveOriginating(Job* pJob, bool bDump)
{
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pOriginatingJobs->vGrabLock();
  
  if (bDump)
    pQueues->m_pOriginatingJobs->vContentDump();
  
  bool bRemoved = pQueues->m_pOriginatingJobs->bRemoveJob(pJob);
  
  pQueues->m_pOriginatingJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_ORIGIN,
              "Job removed from OriginatingJobs"_Stz); //!format-LogJob
  
  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/* remove job from participating list                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bJob_RemoveParticipating(Job* pJob, bool bDump)
{
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pParticipatingJobs->vGrabLock();
  
  if (bDump)
    pQueues->m_pParticipatingJobs->vContentDump();

  bool bRemoved = pQueues->m_pParticipatingJobs->bRemoveJob(pJob);
  
  pQueues->m_pParticipatingJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_PARTG,
              "Job fremoved from ParticipatingJobs"_Stz); //!format-LogJob
  
  return bRemoved;
}




////////////////////////////////////////////////////////////////////////////////
//                             thread interface                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into dedicated thread vector                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddDedicatedThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_AddDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into general thread vector                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneralThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  vThread_AddGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* beginning of thread loop                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_BeginThreadLoop(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated() and Thread::vLoop_general()
  //
  
  vThread_BeginLoop(pThread);
}

/*----------------------------------------------------------------------------*/
/* end of thread loop                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_EndThreadLoop(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  vThread_EndLoop(pThread);
}

/*----------------------------------------------------------------------------*/
/* pause till all general threads are idle (at system startup)                */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_PauseTillGeneralThreads(void)
{
  //
  // called from Engine::vStartSystem()
  //
  
  vThread_PauseGeneral();
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from dedicated thread vector                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveDedicatedThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_RemoveDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from general thread vector                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveGeneralThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_general()
  //
  
  vThread_RemoveGeneral(pThread);
}

/*----------------------------------------------------------------------------*/
/* general thread waits in idle state for task to be assigned,                */
/*   then executes task that was assigned                                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitForTaskAssignment(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  Queues* pQueues = AM::pQueues();
  
  vThread_WaitAssignment(pThread);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pTask = pThread->m_pTask;

  RASSERT(TS_DESISTED == pTask->eState());

  RASSERT(HS_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pTask == pThread->m_pTask);

  pTask->vThread(pThread);
  // ok to touch task
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pQueues->vAddExecutingSet(pThread->m_pTask);
}



////////////////////////////////////////////////////////////////////////////////
//                          implementation for threads                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add dedicated thread to dedicated thread vector                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddDedicated(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_ADD_DEDICATED,
              "Thread added to DedicatedThreadVec"_Stz); //!format-LogThd
  
  RASSERT(CN_NONE != pThread->m_eDedicated);
  
  Queues* pQueues = AM::pQueues();
  
  pThread->m_eThreadState = HS_IDLE;
  pQueues->vAddDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* add general thread to general thread vector                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneral(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_ADD_GENERAL,
              "Thread added to GeneralThreadVec"_Stz); //!format-LogThd
  
  RASSERT(CN_NONE == pThread->m_eDedicated);
  
  Queues* pQueues = AM::pQueues();
  
  pThread->m_eThreadState = HS_IDLE;
  pQueues->vAddGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* append unassigned general thread onto idle thread queue                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AppendIdle(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_APPEND_IDLE,
              "Thread moves onto IdleThreadQueue (no ready task available)"_Stz); //!format-LogThd
  
  RASSERT(! pThread->m_pTask);
  ASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(CN_NONE == pThread->m_eDedicated);
  RASSERT(pThread->m_pMutex->bIsLocked());

  Queues* pQueues = AM::pQueues();
  
  pThread->m_eThreadState = HS_IDLE;
  pQueues->vThreadAppendIdle(pThread);
}

/*----------------------------------------------------------------------------*/
/* attempt to pop an idle general thread and assign given task to it          */
/*----------------------------------------------------------------------------*/
bool Scheduler::bThread_AssignIdle(Task* pTask)
{
  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_ASSIGN_THREAD,
              "Task attempts to assign an idle thread to itself"_Stz); //!format-LogTsk
  
  RASSERT(TS_DESISTED == pTask->eState());
  
  Queues* pQueues = AM::pQueues();
  
  Thread* pThread = pQueues->pThreadPopIdleNL();
  
  if (! pThread)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(HS_IDLE == pThread->m_eThreadState);

  pThread->m_pTask = pTask;
  pThread->m_nAssignedTasks++;
  
  pThread->m_eThreadState = HS_ASSIGNED;
  
  AM::vLogTsk(SL_INF,pTask,TA_TSK_ASSIGNED_THREAD,
              "Task has popped and assigned an idle thread to itself"_Stz); //!format-LogTsk
  
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SIGNAL_THREAD,
              "Task signals its assigned thread"_Stz); //!format-LogTsk
  
  pThread->m_pMutex->vLock(pTask);
  pThread->m_pMutex->vSignal(pTask);
  pThread->m_pMutex->vUnlock(pTask);
  
  // mutex can now be acquired by the m_pPsxThd of pThread
  
  // DO NOT TOUCH TASK
  return true;
}

/*----------------------------------------------------------------------------*/
/* thread is at beginning of it's thread loop                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_BeginLoop(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_BEGIN_LOOP,
              "Thread is at beginning of it's thread loop"_Stz); //!format-LogThd
  
  pThread->vThisTLSThread();
  
  // thread has mutex while it is running
  pThread->m_eThreadState = HS_UNASSIGNED;
  pThread->m_pMutex->vLock(pThread);
  
  // increment m_ixThreads to indicate thread is running
  AM::vThreadBegins();
}

/*----------------------------------------------------------------------------*/
/* Thread continues executing same task (tried sleep or yield but continued)  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ContinueSame(Thread* pThread, Task* pTask)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_CONTINUE_TASK,
              "Thread continues executing same task"_Stz); //!format-LogThd
  
  RASSERT(TS_EXECUTING == pTask->eState());
  
  RASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(! pThread->m_pTask);
  
  pThread->m_pTask = pTask;
  pThread->m_eThreadState = HS_ASSIGNED;
}

/*----------------------------------------------------------------------------*/
/* dedicated thread ends it's thread loop                                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_EndLoop(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_END_LOOP,
              "Thread has ended it's thread loop"_Stz); //!format-LogThd
  
  pThread->m_pMutex->vUnlock(pThread);
}

/*----------------------------------------------------------------------------*/
/* dedicated thread executes its task                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ExecuteDedicated(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_EXECUTE_DEDICATED, //!format-LogThd
              "Thread executes dedicated task"_Stz);
  
  ASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  ASSERT(pThread->m_pMutex->bIsLocked());
  
  RASSERT(pThread->m_pTask);
  RASSERT(TS_DESISTED == pThread->m_pTask->eState());
  
  Queues* pQueues = AM::pQueues();
  
  pThread->m_pTask->vThread(pThread);
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pQueues->vAddExecutingSet(pThread->m_pTask);
}

/*----------------------------------------------------------------------------*/
/* pause till general threads are all idle                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_PauseGeneral(void)
{
  AM::vLogSys(SL_INF,nullptr,YA_SYS_PAUSE_GENERAL,
              "Pause till all general threads are all idle"_Stz); //!format-LogSys
  
  Queues* pQueues = AM::pQueues();
  
  // pause till all general threads have all begun
  while (AM::nThreadsBegun() != AM::nThreads())
    AM::vNapMsec(10,CN_NONE);
  
  RASSERT(pQueues->m_pGeneralThreadVec->nItems() == AM::nThreads());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // pause till all general threads are in idle state
  i8 nGeneralThreads = pQueues->m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nGeneralThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pGeneralThreadVec->pGet(i_Thread);
    
    while (HS_IDLE != pThread->m_eThreadState)
      AM::vNapMsec(10,CN_QUEUES);
  }
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from dedicated thread vector                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveDedicated(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_REMOVE_DEDICATED,
              "Thread removed from DedicatedThreadVec"_Stz); //!format-LogThd
  
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from general thread vector                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveGeneral(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_REMOVE_GENERAL,
              "Thread from from GeneralThreadVec"_Stz); //!format-LogThd
  
  //
  // called from Thread::aEntryPoint_general()
  //
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* spawn dedicated thread for given task                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_SpawnDedicated(Task* pTask)
{
  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SPAWN_DEDICATED,
              "Thread spawned for dedicated task"_Stz); //!format-LogTsk
  
  RASSERT(TS_DESISTED == pTask->eState());
  
  Thread* pThread = new Thread;
  
  pThread->m_pTask = pTask;
  pThread->m_eDedicated = pTask->pJob()->eClass();
  
  pThread->vSpawnPsxThd_dedicated();
}

/*----------------------------------------------------------------------------*/
/* unassign task from thread                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_UnassignTask(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_UNASSIGN_TASK,
              "Thread unassigns its task"_Stz); //!format-LogThd
  
  pThread->m_pTask->vThread(nullptr);
  
  pThread->m_pTask = nullptr;
  pThread->m_eThreadState = HS_UNASSIGNED;
}

/*----------------------------------------------------------------------------*/
/* append unassigned general thread onto idle queue, wait for task assignment */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_GET_TASK,
              "Thread attempts task assignment"_Stz); //!format-LogThd
  
  ASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pMutex->bIsLocked());
  
  Queues* pQueues = AM::pQueues();


  while (! pThread->m_pTask) {
        
    Task* pTask = pQueues->pPopReadyQueue();
    if (! pTask)
      pTask = pQueues->pPopReadyQueueLP();
    
    RASSERT(! pTask || TS_READY == pTask->eState());

    if (pTask) {
      pThread->m_pTask = pTask;
      pThread->m_nAssignedTasks++;
      
      pThread->m_eThreadState = HS_ASSIGNED;
      pTask->vState(TS_DESISTED);
    }
    else {
      vThread_AppendIdle(pThread);
      
      pThread->vIncStepNo();
      AM::vLogThd(SL_INF,pThread,HA_THD_WAIT_ASSIGN,
                  "Thread waits for task assignment"_Stz); //!format-LogThd
      pThread->m_pMutex->vWait(pThread);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // HS_ASSIGNED state will have been set by
  //
  //   ... -> vScheduleTask() -> bThread_AssignIdle()
  //
  
  RASSERT(pThread->m_pMutex->bIsLocked());
  RASSERT(HS_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pTask);
}

// /////////////////////////////////////////////////////////////////////////////
//                            core task scheduler                             //
// /////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* attempt to assign a thread to able-to-run task, else add to ready queue    */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleTask(Task* pTask)
{
  pTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SCHEDULE,
              "Task is being scheduled"_Stz); //!format-LogTsk
  
  eTS eTaskState = pTask->eState();
  eQK eQueue = pTask->eQueue();
  eSA eSchedulerAct = pTask->eSchedulerAct();
  
  if (TS_DESISTED != pTask->eState()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aSA[eSchedulerAct]);
  }
  
  if (QK_NONE != pTask->eQueue()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aSA[eSchedulerAct]);  }
  
  if (SA_NONE != pTask->eSchedulerAct()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aSA[eSchedulerAct]);
  }
  
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(SA_NONE == pTask->eSchedulerAct());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vGrabThreadIdleQueue();
  
  if (bThread_AssignIdle(pTask)) {
    pQueues->vDropThreadIdleQueue();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(SA_NONE == pTask->eSchedulerAct());
  
  pTask->vState(TS_READY);

  if (pTask->pJob()->aJobClass()->m_ePriority == PY_NORMAL)
    pQueues->vAppendReadyQueue(pTask);
  else
    pQueues->vAppendReadyQueueLP(pTask);
 
  pQueues->vDropThreadIdleQueue();
  
  // **do not touch task*
}




/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

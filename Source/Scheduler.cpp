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
void Scheduler::vTaskSelf_LoopTransition(Task* pSelfTask, Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_NONE != pSelfTask->eTranstionRequest());
  RASSERT(pSelfTask == pThread->m_pTask);
  RASSERT(pThread == pSelfTask->pvThread_DONT_DEREF());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) move task out of executing queue
  
  RASSERT(TS_EXECUTING == pSelfTask->eState());
  RASSERT(QK_EXECUTING_SET == pSelfTask->eQueue());

  // remove task from the executing set
  
  Queues* pQueues = AM::pQueues();
  pQueues->vRemoveExecutingSet(pSelfTask);
  RASSERT(QK_NONE == pSelfTask->eQueue());
  
  // set task to be desisted to indicate its under control of the scheduler
  
  pSelfTask->vState(TS_DESISTED);

  // unlink task from calling thread
  
  pSelfTask->vThread(nullptr);
  pThread->m_pTask = nullptr;
  
  // this thread no longer is assigned to a task
  
  pThread->m_eThreadState = HS_UNASSIGNED;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) transition task to different state+queue depending on transition request
  
  eTR eTranstionRequest = pSelfTask->eTranstionRequest();
  switch (eTranstionRequest) {
      
    // cases where task changes state -- -- -- -- -- -- -- -- -- -- -- -- -- --
      
    case TR_BIDE_FOR_OPERAND: // bide / wait for operand to arrive in pipein
      vTaskSelf_BideForOperand(pSelfTask,pThread);
      RASSERT(! pThread->pTask() ||
              (pSelfTask == pThread->pTask() &&
               pThread == pSelfTask->pvThread_DONT_DEREF()));
      break;
      
    case TR_SLEEP_SOME_TIME: // task sleeps till wakup time arrives
      vTaskSelf_SleepSomeTime(pSelfTask,pThread);
      RASSERT(! pThread->pTask() ||
              (pSelfTask == pThread->pTask() &&
               pThread == pSelfTask->pvThread_DONT_DEREF()));
      break;
      
    case TR_WAIT_FOR_RESUME: // wait till acked from an acking cross task
      vTaskSelf_WaitForResume(pSelfTask,pThread);
      RASSERT(! pThread->pTask() ||
              (pSelfTask == pThread->pTask() &&
               pThread == pSelfTask->pvThread_DONT_DEREF()));
      break;
      
    case TR_YIELD_FOR_OTHERS: // be nice and let thread get assigned to other
      vTaskSelf_YieldForOthers(pSelfTask,pThread);
      RASSERT(! pThread->pTask() ||
              (pSelfTask == pThread->pTask() &&
               pThread == pSelfTask->pvThread_DONT_DEREF()));
      break;
      
    // case where task is over and will be deleted -- -- -- -- -- -- -- -- -- --
      
    case TR_DELETE_THIS_TASK: // task is deleting itself
      vTaskSelf_DeleteThisTask(pSelfTask,pThread);
      RASSERT(!pThread->pTask());
      break;
            
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* operator task bides for operand                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_BideForOperand(Task* pSelfTask, Thread* pThread)
{
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_BIDE_FOR_OPERAND == pSelfTask->eTranstionRequest());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());

  pSelfTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pSelfTask,TA_TSK_BIDE,
              "Transition task to TS_BIDING; add to BidingSet"_Stz); //!format-LogTsk

  Queues* pQueues = AM::pQueues();
  
  pSelfTask->vTranstionRequestClear();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case: operator task bides for operand

  pSelfTask->vState(TS_BIDING);
  pQueues->vAddBidingSet(pSelfTask);
  
  pSelfTask->vDropTaskLock();
  
  /** do not touch task **/
}

/*----------------------------------------------------------------------------*/
/* task transitions to TS_COMPLETED state and is deleted                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_DeleteThisTask(Task* pSelfTask, Thread* pThread)
{
  //
  // example: op task has ack'd the pipeline job and now just self-delete
  //
  // who delete's the job in these cases?
  //
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_DELETE_THIS_TASK == pSelfTask->eTranstionRequest());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  
  // add assert on entry point

  pSelfTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pSelfTask,TA_TSK_EXIT,
              "Task to TS_COMPLETED state; and deleted"_Stz); //!format-LogTsk
    
  pSelfTask->vTranstionRequestClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete case: task is to be deleted

  pSelfTask->vState(TS_DELETING);
  
  pSelfTask->vDropTaskLock();
  
  delete pSelfTask; // note: this is the only point where tasks get deleted

  /** do not touch task **/
}

/*----------------------------------------------------------------------------*/
/* put task to sleep                                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_SleepSomeTime(Task* pSelfTask, Thread* pThread)
{
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_SLEEP_SOME_TIME == pSelfTask->eTranstionRequest());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());

  pSelfTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pSelfTask,TA_TSK_SLEEP,
              "Add desisted task to SleepingQueue"_Stz); //!format-LogTsk
  
  Queues* pQueues = AM::pQueues();

  pSelfTask->vTranstionRequestClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case: wakeup time has not arrived
  
  if (! pSelfTask->bWakeupNow()) {

    pSelfTask->vState(TS_SLEEPING);
    pQueues->vAddSleepingQueue(pSelfTask);
    
    pSelfTask->vDropTaskLock();
    /** do not touch task **/
    return;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // continue case: wakeup time has passed
    
  pSelfTask->vWakeupClear();
  
  pThread->m_pTask = pSelfTask;
  pThread->m_eThreadState = HS_ASSIGNED;

  pSelfTask->vState(TS_EXECUTING);
  pQueues->vAddExecutingSet(pSelfTask);

  pSelfTask->vDropTaskLock();
  
  // calling thread continues with self task
}

/*----------------------------------------------------------------------------*/
/* task waitS for a resume /ack from a cross acking task                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_WaitForResume(Task* pSelfTask, Thread* pThread)
{
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_WAIT_FOR_RESUME == pSelfTask->eTranstionRequest());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());

  pSelfTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pSelfTask,TA_TSK_WAIT,
              "Task moved to WaitingSet"_Stz); //!format-LogTsk
  
  Queues* pQueues = AM::pQueues();
  
  pSelfTask->vTranstionRequestClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case:
  
  pSelfTask->vState(TS_WAITING);
  pQueues->vAddWaitingSet(pSelfTask);
  
  pSelfTask->vDropTaskLock();
  /** do not touch task **/
}

/*----------------------------------------------------------------------------*/
/* desisted task has yielded and is scheduled                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_YieldForOthers(Task* pSelfTask, Thread* pThread)
{
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_YIELD_FOR_OTHERS == pSelfTask->eTranstionRequest());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());

  pSelfTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pSelfTask,TA_TSK_YIELD,
              "Yield desisted task; continue executing or schedule it"_Stz); //!format-LogTsk

  Queues* pQueues = AM::pQueues();

  pSelfTask->vTranstionRequestClear();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case: there are queued ready to run tasks
  
  i8 nReadyTasks = pQueues->nTasksReady() + pQueues->nTasksReadyLP();
  if (nReadyTasks) {

    vScheduleTask(pSelfTask);
    pSelfTask->vDropTaskLock();
    /** do not touch task **/
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // continue case: no other ready to run task

  pSelfTask->vState(TS_EXECUTING);
  pSelfTask->vThread(pThread);
  
  pThread->m_pTask = pSelfTask;
  pThread->m_eThreadState = HS_ASSIGNED;

  pQueues->vAddExecutingSet(pSelfTask);
  pSelfTask->vDropTaskLock();
  
  // calling thread continues with self task
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
  RASSERT(pTaskWaiting->eClass() == CN_TASK);
  
  if (pTaskWaiting->iAcksRmg_assert_only_() <= 0) {
    vTrace("[%s] Acks Remaining: %lld\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeNm(), pTaskWaiting->iAcksRmg_assert_only_());
    vTrace("[%s] Acks Original: %lld\n"_Stz,
           AM::pGrid()->pThisNode()->szNodeNm(), pTaskWaiting->iAcksOriginal());
  }
  
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
  RASSERT(TR_NONE == pTaskWaiting->eTranstionRequest());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveWaitingSet(pTaskWaiting);
    
  RASSERT(TS_DESISTED == pTaskWaiting->eState());
  RASSERT(QK_NONE == pTaskWaiting->eQueue());
  RASSERT(TR_NONE == pTaskWaiting->eTranstionRequest());
  
  vScheduleTask(pTaskWaiting);
  // **do not touch task**

  return true;
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_ScheduleNextPhase(Task* pTaskParticipant,
                                             Task* pActiviating2PcTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pTaskParticipant->eState());
  RASSERT(QK_NONE == pTaskParticipant->eQueue());
  RASSERT(TR_NONE == pTaskParticipant->eTranstionRequest());
  
  pTaskParticipant->vEntryPoint(EP_NEXTPHASE);
  pTaskParticipant->vState(TS_DESISTED);

  vScheduleTask(pTaskParticipant);
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
  RASSERT(TR_NONE == pTaskBiding->eTranstionRequest());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveBidingSet(pTaskBiding);
  
  pTaskBiding->vDropTaskLock();
  
  RASSERT(TS_DESISTED == pTaskBiding->eState());
  RASSERT(QK_NONE == pTaskBiding->eQueue());
  RASSERT(TR_NONE == pTaskBiding->eTranstionRequest());
  
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
  
  while (Task* pWakingTask = pQueues->pPopSleepingQueue()) {
        
    RASSERT(TS_DESISTED == pWakingTask->eState());
    RASSERT(QK_NONE == pWakingTask->eQueue());
    RASSERT(TR_NONE == pWakingTask->eTranstionRequest());
    
    vScheduleTask(pWakingTask);
    // **do not touch task**
  }
}

// /////////////////////////////////////////////////////////////////////////////
//                                task api                                    //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add new dedicated task into system (spawn a dedicated thread for it)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddDedicated(Task* pNewTask)
{
  //
  // called from Api::bSpawnJobDedicated()
  //
  
  pNewTask->vEntryPoint(EP_DEDICATED);
  pNewTask->vState(TS_DESISTED);
  
  pNewTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pNewTask,TA_TSK_SPAWN_DEDICATED,
              "Thread spawned for dedicated task"_Stz); //!format-LogTsk
  
  RASSERT(TS_DESISTED == pNewTask->eState());
  
  Thread* pThread = new Thread;
  
  pThread->m_pTask = pNewTask;
  pThread->m_eDedicated = pNewTask->pJob()->eClass();
  
  pThread->vSpawnPsxThd_dedicated();
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddConnector(Task* pNewTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTranstionRequest());
  
  pNewTask->vEntryPoint(EP_CONNECTOR);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddOp(Task* pNewTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTranstionRequest());
  
  pNewTask->vEntryPoint(EP_OP);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddPrompt(Task* pNewTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTranstionRequest());
  
  pNewTask->vEntryPoint(EP_PROMPT);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddReceived(Task* pNewTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTranstionRequest());
  
  pNewTask->vEntryPoint(EP_RECEIVED);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddService(Task* pNewTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTranstionRequest());
  
  pNewTask->vEntryPoint(EP_SERVICE);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddSystem(Task* pNewTask)
{
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTranstionRequest());
  
  pNewTask->vEntryPoint(EP_SYSTEM);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* set dedicated task to be executing by dedicated thread                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_BeginExecutingDedicated(Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
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



// /////////////////////////////////////////////////////////////////////////////
//                              job interface                                 //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add job to demon list                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_AddDemon(Job* pJob)
{
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pDemonJobs->vGrabLock();
  pQueues->m_pDemonJobs->vAddJob(pJob);
  pQueues->m_pDemonJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_DEMON,
              "Job added to DemonJobs"_Stz); //!format-LogJob
}

/*----------------------------------------------------------------------------*/
/* add job to local list                                                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_AddLocal(Job* pJob)
{
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pLocalJobs->vGrabLock();
  pQueues->m_pLocalJobs->vAddJob(pJob);
  pQueues->m_pLocalJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_LOCAL,
              "Job added to LocalJobs"_Stz); //!format-LogJob
}

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
/* dump demon job list                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_DumpDemon(void)
{
  Queues* pQueues = AM::pQueues();
  
  AM::vLogQue(SL_INF,pQueues,QA_QUE_ORIGINATORS,
              "DemonJobs list:"_Stz); //!format-LogQue
  
  pQueues->m_pDemonJobs->vGrabLock();
  pQueues->m_pDemonJobs->vContentDump();
  pQueues->m_pDemonJobs->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump local job list                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vJob_DumpLocal(void)
{
  Queues* pQueues = AM::pQueues();
  
  AM::vLogQue(SL_INF,pQueues,QA_QUE_ORIGINATORS,
              "LocalJobs list:"_Stz); //!format-LogQue
  
  pQueues->m_pLocalJobs->vGrabLock();
  pQueues->m_pLocalJobs->vContentDump();
  pQueues->m_pLocalJobs->vDropLock();
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
/* remove job from local list                                                 */
/*----------------------------------------------------------------------------*/
bool Scheduler::bJob_RemoveLocal(Job* pJob, bool bDump)
{
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pLocalJobs->vGrabLock();
  
  if (bDump)
    pQueues->m_pLocalJobs->vContentDump();
  
  bool bRemoved = pQueues->m_pLocalJobs->bRemoveJob(pJob);
  
  pQueues->m_pLocalJobs->vDropLock();
  
  pJob->vIncStepNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_LOCAL,
              "Job removed from LocalJobs"_Stz); //!format-LogJob
  
  return bRemoved;
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
  RASSERT(CN_NONE != pThread->m_eDedicated);
  
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_ADD_DEDICATED,
              "Thread added to DedicatedThreadVec"_Stz); //!format-LogThd
  
  Queues* pQueues = AM::pQueues();
  
  pThread->m_eThreadState = HS_IDLE;
  pQueues->vAddDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into general thread vector                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneralThread(Thread* pThread)
{
  RASSERT(CN_NONE == pThread->m_eDedicated);
  
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_ADD_GENERAL,
              "Thread added to GeneralThreadVec"_Stz); //!format-LogThd
  
  Queues* pQueues = AM::pQueues();
  
  pThread->m_eThreadState = HS_IDLE;
  pQueues->vAddGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* beginning of thread loop                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_BeginThreadLoop(Thread* pThread)
{
  RASSERT(HS_IDLE == pThread->m_eThreadState);
  
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
/* end of thread loop                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_EndThreadLoop(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_END_LOOP,
              "Thread has ended it's thread loop"_Stz); //!format-LogThd
  
  pThread->m_pMutex->vUnlock(pThread);
}

/*----------------------------------------------------------------------------*/
/* nap loop till all general threads are idle (main thread at system startup) */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_NapTillGeneralThreads(void)
{

  AM::vLogSys(SL_INF,nullptr,YA_SYS_PAUSE_GENERAL,
              "Pause till all general threads are all idle"_Stz); //!format-LogSys
  
  Queues* pQueues = AM::pQueues();
  
  // pause till all general threads have all begun
  while (AM::nThreadsBegun() != AM::nThreads())
    AM::vNapMsec(10,CN_NONE);
  
  RASSERT(pQueues->m_pGeneralThreadVec->nItems() == AM::nThreads());
  
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
void Scheduler::vThread_RemoveDedicatedThread(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_REMOVE_DEDICATED,
              "Thread removed from DedicatedThreadVec"_Stz); //!format-LogThd
    
  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from general thread vector                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveGeneralThread(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_REMOVE_GENERAL,
              "Thread from from GeneralThreadVec"_Stz); //!format-LogThd
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* general thread waits in idle state for task to be assigned,                */
/*   then executes task that was assigned                                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitForTaskAssignment(Thread* pThread)
{
  Queues* pQueues = AM::pQueues();
  
  vThread_WaitAssignment(pThread);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pKnownTask = pThread->m_pTask;
  RASSERT(TS_DESISTED == pKnownTask->eState());

  RASSERT(HS_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pKnownTask == pThread->m_pTask);

  pKnownTask->vThread(pThread);
  
  // ok to touch task
  
  pThread->m_pTask->vState(TS_EXECUTING);
  pQueues->vAddExecutingSet(pThread->m_pTask);
  
  RASSERT(pThread == pKnownTask->pvThread_DONT_DEREF());
}



////////////////////////////////////////////////////////////////////////////////
//                          implementation for threads                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* attempt to pop an idle general thread and assign given task to it          */
/*----------------------------------------------------------------------------*/
bool Scheduler::bThread_AssignIdle(Task* pKnownTask)
{
  pKnownTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pKnownTask,TA_TSK_ASSIGN_THREAD,
              "Task attempts to assign an idle thread to itself"_Stz); //!format-LogTsk
  
  RASSERT(TS_DESISTED == pKnownTask->eState());
  
  Queues* pQueues = AM::pQueues();
  
  Thread* pThread = pQueues->pThreadPopIdleNL();
  
  if (! pThread)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(HS_IDLE == pThread->m_eThreadState);

  pThread->m_pTask = pKnownTask;
  pThread->m_nAssignedTasks++;
  
  pThread->m_eThreadState = HS_ASSIGNED;
  
  AM::vLogTsk(SL_INF,pKnownTask,TA_TSK_ASSIGNED_THREAD,
              "Task has popped and assigned an idle thread to itself"_Stz); //!format-LogTsk
  
  AM::vLogTsk(SL_INF,pKnownTask,TA_TSK_SIGNAL_THREAD,
              "Task signals its assigned thread"_Stz); //!format-LogTsk
  
  pThread->m_pMutex->vLock(pKnownTask);
  pThread->m_pMutex->vSignal(pKnownTask);
  pThread->m_pMutex->vUnlock(pKnownTask);
  
  // mutex can now be acquired by the m_pPsxThd of pThread
  
  // DO NOT TOUCH TASK
  return true;
}

/*----------------------------------------------------------------------------*/
/* append unassigned general thread onto idle queue, wait for task assignment */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  pThread->vIncStepNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_GET_TASK,
              "Thread attempts task assignment"_Stz); //!format-LogThd
  
  RASSERT(pThread->m_pMutex->bIsLocked());
  RASSERT(pThread->m_pMutex->bIsLockedByCaller(pThread));
  RASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(! pThread->m_pTask);

  Queues* pQueues = AM::pQueues();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mutex-based loop waiting for
//for (;;) {
  while (! pThread->m_pTask) {
    
    // attempt to po a task from one of the ready queues
    Task* pPoppedTask = pQueues->pPopReadyQueue();
    if (! pPoppedTask)
      pPoppedTask = pQueues->pPopReadyQueueLP();
    
    // was a task popped?
    if (pPoppedTask) {
      RASSERT(TS_READY == pPoppedTask->eState());
      pPoppedTask->vState(TS_DESISTED);

      pThread->m_pTask = pPoppedTask;
      pThread->m_eThreadState = HS_ASSIGNED;
      pThread->m_nAssignedTasks++;
      break;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pThread->m_eThreadState = HS_IDLE;
    RASSERT(! pThread->m_pTask);
    pQueues->vThreadAppendIdle(pThread);

    ASSERT(pThread->m_pMutex->bIsLockedByCaller(pThread));
    pThread->m_pMutex->vWait(pThread);
    ASSERT(pThread->m_pMutex->bIsLockedByCaller(pThread));

    if (pThread->m_pTask)
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(pThread->m_pMutex->bIsLocked());
  RASSERT(HS_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pTask);
  RASSERT(TS_DESISTED == pThread->m_pTask->eState());
}

// /////////////////////////////////////////////////////////////////////////////
//                            core task scheduler                             //
// /////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* attempt to assign a thread to able-to-run task, else add to ready queue    */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleTask(Task* pKnownTask)
{
  pKnownTask->vIncStepNo();
  AM::vLogTsk(SL_INF,pKnownTask,TA_TSK_SCHEDULE,
              "Task is being scheduled"_Stz); //!format-LogTsk
  
  eTS eTaskState = pKnownTask->eState();
  eQK eQueue = pKnownTask->eQueue();
  eTR eTranstionRequest = pKnownTask->eTranstionRequest();
  
  if (TS_DESISTED != pKnownTask->eState()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aTR[eTranstionRequest]);
  }
  
  if (QK_NONE != pKnownTask->eQueue()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aTR[eTranstionRequest]);  }
  
  if (TR_NONE != pKnownTask->eTranstionRequest()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aTR[eTranstionRequest]);
  }
  
  RASSERT(TS_DESISTED == pKnownTask->eState());
  RASSERT(QK_NONE == pKnownTask->eQueue());
  RASSERT(TR_NONE == pKnownTask->eTranstionRequest());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vGrabThreadIdleQueue();
  
  if (bThread_AssignIdle(pKnownTask)) {
    pQueues->vDropThreadIdleQueue();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(TS_DESISTED == pKnownTask->eState());
  RASSERT(QK_NONE == pKnownTask->eQueue());
  RASSERT(TR_NONE == pKnownTask->eTranstionRequest());
  
  pKnownTask->vState(TS_READY);

  //! ALKA: while cleaning up JobClass all jobs had m_ePriority
  //! as PY_NORMAL hence Appending to readyQueue always
  
  //if (pKnownTask->pJob()->aJobClass()->m_ePriority == PY_NORMAL)
  //  pQueues->vAppendReadyQueue(pKnownTask);
  //else
  //  pQueues->vAppendReadyQueueLP(pKnownTask);
 
  pQueues->vAppendReadyQueue(pKnownTask);
  
  pQueues->vDropThreadIdleQueue();
  
  // **do not touch task*
}




/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

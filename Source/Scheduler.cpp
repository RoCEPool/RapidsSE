/*============================================================================*/
/* Scheduler.cpp -- Job/Task/Thread Scheduler Mark II                         */
/*============================================================================*/

#include "Scheduling.h"
#include "AM.hpp"


////////////////////////////////////////////////////////////////////////////////
//                              job interface                                 //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add job to originating list                                                */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddOriginatingJob(Job* pJob)
{
  pJob->vIncActNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_ORIGINATING,
              "Job added to OriginatingJobs"_Stz); //!format-LogJob
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pOriginatingJobs->vGrabLock();
  pQueues->m_pOriginatingJobs->vAddJob(pJob);
  pQueues->m_pOriginatingJobs->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add job to participating list                                              */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddParticipatingJob(Job* pJob)
{
  pJob->vIncActNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_PARTICIPATING,
              "Job added to ParticipatingJobs"_Stz); //!format-LogJob
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pParticipatingJobs->vGrabLock();
  pQueues->m_pParticipatingJobs->vAddJob(pJob);
  pQueues->m_pParticipatingJobs->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump originating job list                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vDumpOriginatingJobs(void)
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
void Scheduler::vDumpParticipatingJobs(void)
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
Job* Scheduler::pFindOriginatingJob(i8 iJobId)
{
  Queues* pQueues = AM::pQueues();
  
  AM::vLogQue(SL_INF,pQueues,QA_QUE_FIND_ORIGINATOR,
              "Find job %lld in OriginatingJobs"_Stz, //!format-LogQue
              iJobId);
  
  Job* pJob = pQueues->m_pOriginatingJobs->pFindJob(iJobId);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find job in participating list                                             */
/*----------------------------------------------------------------------------*/
Job* Scheduler::pFindParticipatingJob(i8 iJobId)
{
  Queues* pQueues = AM::pQueues();

  AM::vLogQue(SL_INF,pQueues,QA_QUE_FIND_PARTICPANT,
              "Find job %lld in ParticipatingJobs"_Stz, //!format-LogQue
              iJobId);
  
  Job* pJob = pQueues->m_pParticipatingJobs->pFindJob(iJobId);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* remove job from originating list                                           */
/*----------------------------------------------------------------------------*/
bool Scheduler::bRemoveOriginatingJob(Job* pJob, bool bDump)
{
  pJob->vIncActNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_ORIGINATING,
              "Job removed from OriginatingJobs"_Stz); //!format-LogJob
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pOriginatingJobs->vGrabLock();
  
  if (bDump)
    pQueues->m_pOriginatingJobs->vContentDump();
  
  bool bRemoved = pQueues->m_pOriginatingJobs->bRemoveJob(pJob);
  
  pQueues->m_pOriginatingJobs->vDropLock();
  
  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/* remove job from participating list                                         */
/*----------------------------------------------------------------------------*/
bool Scheduler::bRemoveParticipatingJob(Job* pJob, bool bDump)
{
  pJob->vIncActNo();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_PARTICIPATING,
              "Job fremoved from ParticipatingJobs"_Stz); //!format-LogJob
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pParticipatingJobs->vGrabLock();
  
  if (bDump)
    pQueues->m_pParticipatingJobs->vContentDump();

  bool bRemoved = pQueues->m_pParticipatingJobs->bRemoveJob(pJob);
  
  pQueues->m_pParticipatingJobs->vDropLock();
  
  return bRemoved;
}


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
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddTaskGeneral(Task* pTask)
{
  //
  // called from Api::bSpawnJob()
  //
  
  vTask_Schedule(pTask);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vDdlDropRead(Task* pTask)
{
  //
  // called from
  //
  
  vTask_DdlDropRead(pTask);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vDdlDropWrite(Task* pTask)
{
  //
  // called from
  //
  
  vTask_DdlDropWrite(pTask);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vDdlGrabRead(Task* pTask)
{
  //
  // called from
  //
  
  vTask_DdlGrabRead(pTask);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vDdlGrabWrite(Task* pTask)
{
  //
  // called from
  //
  
  vTask_DdlGrabWrite(pTask);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vDropTaskLock(Task* pTask)
{
  //
  // called from
  //
  
  vTask_Drop(pTask);
}

/*----------------------------------------------------------------------------*/
/* set dedicated task to be executed by dedicated thread                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vExecuteDedicatedTask(Task* pTask, Thread* pThread)
{
  //
  // called from Thread::vLoop_dedicated()
  //
  
  vThread_ExecuteDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vGrabTaskLock(Task* pTask)
{
  //
  // called from
  //
  
  vTask_Grab(pTask);
}

/*----------------------------------------------------------------------------*/
/* pulse a paused task                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vPulsePausedTask(Task* pTask)
{
  //
  // called from ???()
  //
  
  vTask_Pulse(pTask);
}

/*----------------------------------------------------------------------------*/
/* remove dedicated task from system                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemoveTaskDedicated(Task* pTask)
{
  //
  // called from ???
  //
  
}

/*----------------------------------------------------------------------------*/
/* remove general task from system                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vRemoveTaskGeneral(Task* pTask)
{
  //
  // called from ???
  //
  
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
  
  // transition task to different state+queue depending on transition request
  switch (pTask->eTransition()) {
      
    case TR_BIDE:   vTask_Bide(pTask);   break;
    case TR_EXIT:   vTask_Exit(pTask);   break;
    case TR_PAUSE:  vTask_Pause(pTask);   break;
    case TR_SLEEP:  vTask_Sleep(pTask);  break;
    case TR_WAIT:   vTask_Wait(pTask);   break;
    case TR_YIELD:  vTask_Yield(pTask);  break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* set number of acks required by (soon to be) waiting task                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskRequiresAcks(Task* pTask, i8 nAcksRequired)
{
  //
  // called from ???
  //
  
  vTask_Requires(pTask,nAcksRequired);
}

/*----------------------------------------------------------------------------*/
/* trigger op-task that operand has arrived while it is biding                */
/*----------------------------------------------------------------------------*/
void Scheduler::vTriggerBidingTask(Task* pTask)
{
  //
  // called from Op::vAppendOperand()
  //
  
  vTask_Trigger(pTask);
}

/*----------------------------------------------------------------------------*/
/* check to see if any tasks in the sleeping queue can be woken               */
/*----------------------------------------------------------------------------*/
void Scheduler::vWakeSleepingTasks(Task* pTask)
{
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  vTask_Wakeup(pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                             thread interface                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into dedicated thread vector                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vAddDedicatedThread(Thread* pThread)
{
  //
  // called from Thread::aEntryPoint_dedicated()
  //
  
  vThread_AddDedicated(pThread);
}

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into general thread vector                        */
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
/* pause till all general threads are idle (at system startup)                */
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
/* general thread waits in idle state for task to be assigned,                */
/*   then executes task that was assigned                                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vWaitForTaskAssignment(Thread* pThread)
{
  //
  // called from Thread::vLoop_general()
  //
  
  vThread_WaitAssignment(pThread);
  
  vTask_Thread(pThread->m_pTask,pThread);

  vTask_Execute(pThread->m_pTask);
}


////////////////////////////////////////////////////////////////////////////////
//                          implementation for tasks                          //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* change given task to TS_ABLE state (able to be executed)                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Able(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_ABLE,
              "Task transitions to TS_ABLE"_Stz); //!format-LogTsk
  
  // note: task can be in any current state
  
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  pTask->vState(TS_ABLE);
  
  JobLog::vNoteState(pTask->pJob(),"Able"_Stz);
}

/*----------------------------------------------------------------------------*/
/* ack a waiting job-task                                                     */
/*----------------------------------------------------------------------------*/
bool Scheduler::bTask_Ack(Task* pTaskWaiting, Task* pTaskAcking)
{
  pTaskWaiting->vIncActNo();
  AM::vLogTsk(SL_INF,pTaskWaiting,TA_TSK_ACK,
              "Task is ack'd"_Stz); //!format-LogTsk
  
  // note: the 'waiting' task may not yet be waiting.  there is an indefinite
  //       period between when the task grabs its lock + sets the number of
  //       acks and when it enters the waiting state and drops the lock
  
  RASSERT(pTaskWaiting->iAcksRmg_assert_only_() > 0);
  
  RASSERT(TS_EXECUTING == pTaskAcking->eState());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // decrement the acks remaining
  bool bAmDecider = pTaskWaiting->bAcksRmgDec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* op-task with empty operand queue transitions to TS_BIDING and              */
/*   is added to biding set                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Bide(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_BIDE,
              "Transition task to TS_BIDING; add to BidingSet"_Stz); //!format-LogTsk
  
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_BIDE == pTask->eTransition());
  
  Queues* pQueues = AM::pQueues();
  
  pTask->vTransitionClear();
  pTask->vState(TS_BIDING);
  
  JobLog::vNoteState(pTask->pJob(),"Bide"_Stz);
  
  pQueues->vAddBidingSet(pTask);
  // **do not touch task** (other than to drop lock)
  
  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_DdlDropRead(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_DDL_DROP_READ,
              "DdlLock drop read"_Stz); //!format-LogTsk
  
  AM::pDdlLock()->vDropRead();
  pTask->m_iDdlLocked = 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_DdlDropWrite(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_DDL_DROP_WRITE,
              "DdlLock drop write"_Stz); //!format-LogTsk
  
  AM::pDdlLock()->vDropWrite();
  pTask->m_iDdlLocked = 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_DdlGrabRead(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_DDL_GRAB_READ,
              "DdlLock grab read"_Stz); //!format-LogTsk
  
  AM::pDdlLock()->vGrabRead();
  pTask->m_iDdlLocked = C_HAVE_READ_LOCK;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_DdlGrabWrite(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_DDL_GRAB_WRITE,
              "DdlLock grab write"_Stz); //!format-LogTsk
  
  AM::pDdlLock()->vGrabWrite();
  pTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
}

/*----------------------------------------------------------------------------*/
/* remove executing task from executing set and change state to TS_DESISTED   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Desist(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_DESIST,
              "Task removed from ExecutingSet; transitions to TS_DESISTED"_Stz); //!format-LogTsk
  
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(QK_EXECUTING_SET == pTask->eQueue());
  RASSERT(TR_NONE != pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveExecutingSet(pTask);
  
  pTask->vState(TS_DESISTED);
  
  JobLog::vNoteState(pTask->pJob(),"Desist"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Drop(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_DROP,
              "Task drop task lock"_Stz); //!format-LogTsk
  
  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* change able-to-run task to TS_EXECUTING state and add to executing set     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Execute(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_EXECUTE,
              "Task to TS_EXECUTING state; added to ExecutingSet"_Stz); //!format-LogTsk
  
  RASSERT(TS_ABLE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pTask->vState(TS_EXECUTING);

  JobLog::vNoteState(pTask->pJob(),"Execute"_Stz);
  
  pQueues->vAddExecutingSet(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* desisted task transitions to TS_EXITED state and is deleted                */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Exit(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_EXIT,
              "Task to TS_EXITED state; and deleted"_Stz); //!format-LogTsk
  
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Grab(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_GRAB,
              "Task grab lock"_Stz); //!format-LogTsk
  
  pTask->vGrabTaskLock();
}

/*----------------------------------------------------------------------------*/
/* op-task which has perform its work quanta transitions to TS_PAUSED and     */
/*   is added to paused set                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Pause(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_PAUSE,
              "Task to TS_PAUSED state"_Stz); //!format-LogTsk
  
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_PAUSE == pTask->eTransition());
  
  Queues* pQueues = AM::pQueues();
  
  pTask->vTransitionClear();
  pTask->vState(TS_PAUSED);
  
  JobLog::vNoteState(pTask->pJob(),"Pause"_Stz);
  
  pQueues->vAddPausedSet(pTask);
  // **do not touch task** (other than to drop lock)
  
  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* pop a task from the ready/readyLP queue                                    */
/*----------------------------------------------------------------------------*/
Task* Scheduler::pTask_Pop(Thread* pThread)
{
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_POPREADY,
              "Thread attempts pop of task from ReadyQueue/ReadyQueueLP"_Stz); //!format-LogThd
  
  Queues* pQueues = AM::pQueues();

  Task* pTask = pQueues->pPopReadyQueue();
  if ( ! pTask )
    pTask = pQueues->pPopReadyQueueLP();
  
  RASSERT(! pTask || TS_READY == pTask->eState());
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* locked paused op-task is removed from paused set,                          */
/*   transitions to TS_ABLE state, unlocked and scheduled                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Pulse(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_PULSE,
              "Transition a paused op-task to TS_ABLE and schedule"_Stz); //!format-LogTsk
  
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_PAUSED == pTask->eState());
  RASSERT(QK_PAUSED_SET == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemovePausedSet(pTask);
  
  JobLog::vNoteState(pTask->pJob(),"Pulse"_Stz);
  
  pTask->vDropTaskLock();
  
  vTask_Able(pTask);
  vTask_Schedule(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* change able-to-run task to TS_READY state and append to a ready queue      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Ready(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_READY,
              "Task to TS_READY state; appended to ReadyQueue[LP]"_Stz); //!format-LogTsk
  
  RASSERT(TS_ABLE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pTask->vState(TS_READY);

  JobLog::vNoteState(pTask->pJob(),"Ready"_Stz);
  
  if (TK_JOB == pTask->eKind()) {
    if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
      pQueues->vAppendReadyQueue(pTask);
    else
      pQueues->vAppendReadyQueueLP(pTask);
  }
  else {
    pQueues->vAppendReadyQueue(pTask);
  }
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* resume a fully ack'd waiting task                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Resume(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_RESUME,
              "Task removed from WaitingSet; and scheduled"_Stz); //!format-LogTsk
  
  // note: the 'waiting' task may not yet be waiting.  there is an indefinite
  //       period between when the task grabs its lock + sets the number of
  //       acks and when it enters the waiting state and drops the lock
  
  RASSERT(! pTask->iAcksRmg_assert_only_());
  
  JobLog::vNoteState(pTask->pJob(),"Resume"_Stz);
  
  // cycle the task lock to ensure task has fully entered wait state
  pTask->vGrabTaskLock();
  pTask->vDropTaskLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_WAITING_SET == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveWaitingSet(pTask);
  
  vTask_Able(pTask);
  vTask_Schedule(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* set number of acks required by (soon to be) waiting task                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Requires(Task* pTask, i8 nAcksRequired)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_REQUIRES,
              "Tasks sets number of acks required (nAcks=%lld)"_Stz, //!format-LogTsk
              nAcksRequired);

  pTask->m_oAcksRmg.vSetRemaining(nAcksRequired);
}

/*----------------------------------------------------------------------------*/
/* attempt to assign a thread to able-to-run task, else add to ready queue    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Schedule(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SCHEDULE,
              "Task is being scheduled"_Stz); //!format-LogTsk
  
  RASSERT(TS_ABLE == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  JobLog::vNoteState(pTask->pJob(),"Schedule"_Stz);
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->vGrabThreadIdleQueue();
  
  if (bThread_AssignIdle(pTask)) {
    pQueues->vDropThreadIdleQueue();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTask_Ready(pTask);
  pQueues->vDropThreadIdleQueue();
}

/*----------------------------------------------------------------------------*/
/* transition desisted task to TS_SLEEPING state and add to sleeping queue    */
/*   (if wakeup time has already arrived, calling thread re-executes task)    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Sleep(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SLEEP,
              "Add desisted task to SleepingQueue"_Stz); //!format-LogTsk
  
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_SLEEP == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pTask->vTransitionClear();
  
  JobLog::vNoteState(pTask->pJob(),"Sleep"_Stz);

  // is the task to actually sleep?
  if (! pTask->bWakeupNow()) {
    pTask->vState(TS_SLEEPING);
    pQueues->vAddSleepingQueue(pTask);
    // **do not touch task**
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupClear();
  
  Thread* pThread = Thread::pThisTLSThread();
  vTask_Able(pTask);
  vTask_Execute(pTask);
  vThread_ContinueSame(pThread,pTask);

  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* able-to-run task has been assigned the given thread                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Thread(Task* pTask, Thread* pThread)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_THREAD,
              "Task assigned to given thread"_Stz); //!format-LogTsk
  
  RASSERT(TS_ABLE == pTask->eState());
  
  RASSERT(HS_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pTask == pThread->m_pTask);
  
  JobLog::vNoteState(pTask->pJob(),"Thread"_Stz);
  
  pTask->vThread(pThread);
  // ok to touch task
}

/*----------------------------------------------------------------------------*/
/* trigger biding task                                                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Trigger(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_TRIGGER,
              "Remove biding task from BidingSet and schedule it"_Stz); //!format-LogTsk
  
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_BIDING == pTask->eState());
  RASSERT(QK_BIDING_SET == pTask->eQueue());
  RASSERT(TR_NONE == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pQueues->vRemoveBidingSet(pTask);
  
  JobLog::vNoteState(pTask->pJob(),"Trigger"_Stz);
  
  vTask_Able(pTask);
  pTask->vDropTaskLock();
  
  vTask_Schedule(pTask);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to waiting set                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wait(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_WAIT,
              "Task added to WaitingSet"_Stz); //!format-LogTsk
  
  RASSERT(pTask->bIsLocked());
  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_WAIT == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pTask->vTransitionClear();
  pTask->vState(TS_WAITING);
  
  JobLog::vNoteState(pTask->pJob(),"Wait"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pQueues->vAddWaitingSet(pTask);
  // **do not touch task** (other than to drop lock)

  pTask->vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* wakeup any sleeping tasks whose wakeup time has arrived                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vTask_Wakeup(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_WAKEUP,
              "Wakeup any sleeping tasks whose wakeup time has arrived"_Stz); //!format-LogTsk
  
  Queues* pQueues = AM::pQueues();
  
  while (Task* pTask = pQueues->pPopSleepingQueue()) {
    JobLog::vNoteState(pTask->pJob(),"Wakeup"_Stz);
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
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_YIELD,
              "Yield desisted task; continue executing or schedule it"_Stz); //!format-LogTsk

  RASSERT(TS_DESISTED == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  RASSERT(TR_YIELD == pTask->eTransition());

  Queues* pQueues = AM::pQueues();
  
  pTask->vTransitionClear();

  Thread* pThread = Thread::pThisTLSThread();
  
  JobLog::vNoteState(pTask->pJob(),"Yield"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no other ready to run tasks, so thread continues executing the task
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL) {
    i8 iLengthReadyQueue = pQueues->nTasksReady();
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
/* add dedicated thread to dedicated thread vector                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddDedicated(Thread* pThread)
{
  pThread->vIncActNo();
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
  pThread->vIncActNo();
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
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_APPEND_IDLE,
              "Thread moves onto IdleThreadQueue (no ready task available)"_Stz); //!format-LogThd
  
  RASSERT(! pThread->m_pTask);
  ASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(CN_NONE == pThread->m_eDedicated);
  RASSERT(pThread->m_pMutexThread->bMutexIsLocked());

  Queues* pQueues = AM::pQueues();
  
  pThread->m_eThreadState = HS_IDLE;
  pQueues->vThreadAppendIdle(pThread);
}

/*----------------------------------------------------------------------------*/
/* attempt to pop an idle general thread and assign given task to it          */
/*----------------------------------------------------------------------------*/
bool Scheduler::bThread_AssignIdle(Task* pTask)
{
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_ASSIGN_IDLE,
              "Task attempts to assign an idle thread to itself"_Stz); //!format-LogTsk
  
  RASSERT(TS_ABLE == pTask->eState());
  
  Queues* pQueues = AM::pQueues();
  
  Thread* pThread = pQueues->pThreadPopIdleNL();
  
  if (! pThread)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  JobLog::vNoteState(pTask->pJob(),"AssignIdle"_Stz);
  
  RASSERT(HS_IDLE == pThread->m_eThreadState);

  AM::vLogTsk(SL_INF,pTask,TA_TSK_ASSIGNED_IDLE,
              "Task attempts to assign an idle thread to itself"_Stz); //!format-LogTsk

  pThread->m_pTask = pTask;
  pThread->m_nAssignedTasks++;
  
  pThread->m_eThreadState = HS_ASSIGNED;

  pThread->m_pMutexThread->vMutexLock(pTask);
  pThread->m_pMutexThread->vCondSignal();
  pThread->m_pMutexThread->vMutexUnlock(pTask);
  
  // mutex can now be acquired by the m_pPthread of pThread
  
  // DO NOT TOUCH TASK
  return true;
}

/*----------------------------------------------------------------------------*/
/* thread is at beginning of it's thread loop                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_BeginLoop(Thread* pThread)
{
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_BEGIN_LOOP,
              "Thread is at beginning of it's thread loop"_Stz); //!format-LogThd
  
  pThread->vThisTLSThread();
  
  // thread has mutex while it is running
  pThread->m_eThreadState = HS_UNASSIGNED;
  pThread->m_pMutexThread->vMutexLock(pThread);
  
  // increment m_ixThreads to indicate thread is running
  AM::vThreadBegins();
}

/*----------------------------------------------------------------------------*/
/* Thread continues executing same task (tried sleep or yield but continued)  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ContinueSame(Thread* pThread, Task* pTask)
{
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_CONTINUE_SAME,
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
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_END_LOOP,
              "Thread has ended it's thread loop"_Stz); //!format-LogThd
  
  pThread->m_pMutexThread->vMutexUnlock(pThread);
}

/*----------------------------------------------------------------------------*/
/* dedicated thread executes its task                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_ExecuteDedicated(Thread* pThread)
{
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_EXECUTE_DEDICATED, //!format-LogThd
              "Thread executes dedicated task"_Stz);
  
  ASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  ASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  RASSERT(pThread->m_pTask);
  RASSERT(TS_ABLE == pThread->m_pTask->eState());
  
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
    AM::vNap(10,CN_NONE);
  
  RASSERT(pQueues->m_pGeneralThreadVec->nItems() == AM::nThreads());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // pause till all general threads are in idle state
  i8 nGeneralThreads = pQueues->m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nGeneralThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pGeneralThreadVec->pGet(i_Thread);
    
    while (HS_IDLE != pThread->m_eThreadState)
      AM::vNap(10,CN_QUEUES);
  }
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from dedicated thread vector                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveDedicated(Thread* pThread)
{
  pThread->vIncActNo();
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
  pThread->vIncActNo();
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
  pTask->vIncActNo();
  AM::vLogTsk(SL_INF,pTask,TA_TSK_SPAWN_DEDICATED,
              "Thread spawned for dedicated task"_Stz); //!format-LogTsk
  
  RASSERT(TS_ABLE == pTask->eState());
  
  Thread* pThread = new Thread;
  
  pThread->m_pTask = pTask;
  pThread->m_eDedicated = pTask->pJob()->eClass();
  
  pThread->vSpawnPthread_dedicated();
}

/*----------------------------------------------------------------------------*/
/* unassign task from thread                                                  */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_UnassignTask(Thread* pThread)
{
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_UNASSIGN_TASK,
              "Thread unassigns its task"_Stz); //!format-LogThd
  
  pThread->m_pTask = nullptr;
  pThread->m_eThreadState = HS_UNASSIGNED;
}

/*----------------------------------------------------------------------------*/
/* append unassigned general thread onto idle queue, wait for task assignment */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  pThread->vIncActNo();
  AM::vLogThd(SL_INF,pThread,HA_THD_GET_ASSIGN,
              "Thread attempts task assignment"_Stz); //!format-LogThd
  
  ASSERT(HS_UNASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  
  while (! pThread->m_pTask) {
    
    // pop a ready task
    // RAY TO LOOK AT ????????????
    Task* pTask = pTask_Pop(pThread);
    if (pTask) {
      pThread->m_pTask = pTask;
      pThread->m_nAssignedTasks++;
      
      pThread->m_eThreadState = HS_ASSIGNED;
      pTask->vState(TS_ABLE);
    }
    else {
      vThread_AppendIdle(pThread);
      pThread->m_pMutexThread->vCondWaitIndefinite();
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // HS_ASSIGNED state will have been set by
  //
  //   ... -> vTask_Schedule() -> bThread_AssignIdle()
  //
  
  RASSERT(pThread->m_pMutexThread->bMutexIsLocked());
  RASSERT(HS_ASSIGNED == pThread->m_eThreadState);
  RASSERT(pThread->m_pTask);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

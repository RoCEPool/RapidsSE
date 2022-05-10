/*============================================================================*/
/* Scheduler.cpp -- Job/Task/Thread Scheduler Mark II                         */
/*============================================================================*/

#include "BASE.h"

#include "MY.hpp"

#include "Node.hpp"
#include "Grid.hpp"
#include "CmdLineArgs.hpp"

extern CmdLineArgs gxCmdLineArgs;

// /////////////////////////////////////////////////////////////////////////////
//                             scheduler boot                                 //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* scheduler boot sequence                                                    */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduler_Boot(void)
{
  _static_
  
  // spawn general threads
  for (i8 iThreads = 0; iThreads < gxCmdLineArgs.m_xSysArgs.m_nThreads; iThreads++) {
    Thread* pThread = AM::THREAD::pNewThread();
    pThread->vSpawnPsxThd_general();
  }
  
  // spawn dummy threads
  for (i8 iThreads = 0; iThreads < gxCmdLineArgs.m_xSysArgs.m_nDummyThreads; iThreads++) {
    Thread* pThread = AM::THREAD::pNewThread();
    pThread->vSpawnPsxThd_dummy();
  }
  
  // pause till all general threads are operational
  Scheduler::vThread_NapTillGeneralThreads();
}


// /////////////////////////////////////////////////////////////////////////////
//                             task transition                                //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* task has returned from task loop, action depends on transition request     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_LoopTransition(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  //
  // called from Thread::vLoop_general()
  //
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_NONE != pSelfTask->eTransition());
  RASSERT(pSelfTask == pThread->pTask());
  RASSERT(pThread == pSelfTask->pvThread_DONT_DEREF());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) move task out of executing queue
  
  RASSERT(TS_EXECUTING == pSelfTask->eState());
  RASSERT(QK_EXECUTING_SET == pSelfTask->eQueue());

  // remove task from the executing set
  
  Schedule* pSchedule = AM::pSchedule();
  pSchedule->vRemoveExecutingSet(pSelfTask);
  RASSERT(QK_NONE == pSelfTask->eQueue());
  
  // set task to be desisted to indicate its under control of the scheduler
  
  pSelfTask->vState(TS_DESISTED);

  // unlink task from calling thread
  
  pSelfTask->vThread(nullptr);
  pThread->vTask(nullptr);
  
  // this thread no longer is assigned to a task
  
  pThread->vThreadState(HS_UNASSIGNED);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) transition task to different state+queue depending on transition request
  
  eTR eTransition = pSelfTask->eTransition();
  switch (eTransition) {
      
    // cases where task changes state -- -- -- -- -- -- -- -- -- -- -- -- -- --
      
    case TR_BIDE_FOR_OPERAND: // bide / wait for operand to arrive in pipein
      vTaskSelf_BideForOperand(pSelfTask,pThread);
      RASSERT(! pThread->pTask() ||
              (pSelfTask == pThread->pTask() &&
               pThread == pSelfTask->pvThread_DONT_DEREF()));
      break;
      
    case TR_HIBERNATE_THIS_TASK: // task is hibernating itself
      vTaskSelf_HibernateThisTask(pSelfTask,pThread);
      RASSERT(!pThread->pTask());
      break;
            
    case TR_RETIRE_THIS_TASK: // task is retiring
      vTaskSelf_RetireThisTask(pSelfTask,pThread);
      RASSERT(!pThread->pTask());
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
            
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* operator task bides for operand                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_BideForOperand(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_BIDE_FOR_OPERAND == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  UNUSED(pThread);
  
  Schedule* pSchedule = AM::pSchedule();
  
  pSelfTask->vTranstionClear();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case: operator task bides for operand

  pSelfTask->vState(TS_BIDING);
  pSchedule->vAddBidingSet(pSelfTask);
  
  pSelfTask->vDropTaskLock((Stz*)__func__);
  
  /** do not touch task **/
}

/*----------------------------------------------------------------------------*/
/* task transitions to TS_COMPLETED state and is deleted                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_DeleteThisTask(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  //
  // example: op task has ack'd the pipeline job and now just self-delete
  //
  // who delete's the job in these cases?
  //
  
  if (!pSelfTask->bIsLocked())
    AM::SE::vWhereAmI();
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_HIBERNATE_THIS_TASK == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  UNUSED(pThread);

  // add assert on entry point

  pSelfTask->vTranstionClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete case: task is to be deleted

  pSelfTask->vState(TS_DELETING);
  
  pSelfTask->vDropTaskLock((Stz*)__func__);
    
  delete pSelfTask;

  /** do not touch task **/
}

/*----------------------------------------------------------------------------*/
/* task transitions to TS_HIBERNATING state (is in plist with job)            */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_HibernateThisTask(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  //
  // example: op task has ack'd the pipeline job and now just self-delete
  //
  // who delete's the job in these cases?
  //
  
  if (!pSelfTask->bIsLocked())
    AM::SE::vWhereAmI();
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_HIBERNATE_THIS_TASK == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  UNUSED(pThread);

  // add assert on entry point

  pSelfTask->vTranstionClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete case: task is to be deleted

  pSelfTask->vState(TS_HIBERNATING);
  
  pSelfTask->vDropTaskLock((Stz*)__func__);
  
  //delete pSelfTask;
  
//  pSelfTask->vGuardedDelete(CN_TASK,&pSelfTask,
//                            CN_SCHEDULER,nullptr,(Stz*)__func__);

  /** do not touch task **/
  
}

/*----------------------------------------------------------------------------*/
/* task transitions to TS_RETIRED state                                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_RetireThisTask(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  //
  // example: op task has ack'd the pipeline job and now just self-delete
  //
  // who delete's the job in these cases?
  //
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_RETIRE_THIS_TASK == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  UNUSED(pThread);

  pSelfTask->vTranstionClear();
  
  pSelfTask->vRetire();
  
  RASSERT(TS_RETIRED == pSelfTask->eState());

  pSelfTask->vDropTaskLock((Stz*)__func__);
  /* do not touch task */
}

/*----------------------------------------------------------------------------*/
/* put task to sleep                                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_SleepSomeTime(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_SLEEP_SOME_TIME == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  
  Schedule* pSchedule = AM::pSchedule();

  pSelfTask->vTranstionClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case: wakeup time has not arrived
  
  if (! pSelfTask->bWakeupNow()) {

    pSelfTask->vState(TS_SLEEPING);
    pSchedule->vAddSleepingQueue(pSelfTask);

    pSelfTask->vDropTaskLock((Stz*)__func__);
    /** do not touch task **/
    return;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // continue case: wakeup time has passed
    
  pSelfTask->vWakeupClear();
  
  pThread->vTask(pSelfTask);

  pSelfTask->vState(TS_EXECUTING);
  pSelfTask->vThread(pThread);

  pSchedule->vAddExecutingSet(pSelfTask);

  pSelfTask->vDropTaskLock((Stz*)__func__);
  
  // calling thread continues with self task
}

/*----------------------------------------------------------------------------*/
/* task waitS for a resume /ack from a cross acking task                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_WaitForResume(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_WAIT_FOR_RESUME == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());
  UNUSED(pThread);
  
  Schedule* pSchedule = AM::pSchedule();
  
  pSelfTask->vTranstionClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case:
  
  pSelfTask->vState(TS_WAITING);
  pSchedule->vAddWaitingSet(pSelfTask);
  
  pSelfTask->vDropTaskLock((Stz*)__func__);
  /** do not touch task **/
}

/*----------------------------------------------------------------------------*/
/* desisted task has yielded and is scheduled                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskSelf_YieldForOthers(Task* pSelfTask, Thread* pThread)
{
  _static_
  
  RASSERT(pSelfTask->bIsLocked());
  RASSERT(TR_YIELD_FOR_OTHERS == pSelfTask->eTransition());
  RASSERT(TS_DESISTED == pSelfTask->eState());
  RASSERT(QK_NONE == pSelfTask->eQueue());

  Schedule* pSchedule = AM::pSchedule();

  pSelfTask->vTranstionClear();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queued case: there are queued ready to run tasks
  
  i8 nReadyTasks = pSchedule->nTasksReady() + pSchedule->nTasksReadyLP();
  if (nReadyTasks) {

    vScheduleTask(pSelfTask);
    pSelfTask->vDropTaskLock((Stz*)__func__);
    /** do not touch task **/
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // continue case: no other ready to run task

  pSelfTask->vState(TS_EXECUTING);
  pSelfTask->vThread(pThread);
  
  pThread->vTask(pSelfTask);
 
  pSchedule->vAddExecutingSet(pSelfTask);
  pSelfTask->vDropTaskLock((Stz*)__func__);
  
  // calling thread continues with self task
}


// /////////////////////////////////////////////////////////////////////////////
//                               cross task                                   //
// /////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* ack a waiting task (returns true if task was resumed)                      */
/*----------------------------------------------------------------------------*/
bool Scheduler::bTaskCross_AckWaiting(Task* pTaskWaiting,
                                      Task* pTaskAcking,
                                      i8& nAcksWereRemaining)
{
  _static_
  
  //
  // called from Step::vTaskAck()
  //
  RASSERT(pTaskWaiting->eGetClassNo() == CN_TASK);
  
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
  bool bAmDecider = pTaskWaiting->bAcksRmgDec(nAcksWereRemaining);
    
  if (! bAmDecider)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  RASSERT(! pTaskWaiting->iAcksRmg_assert_only_());
//
//  // cycle the task lock to ensure task has fully entered wait state
//  pTaskWaiting->vGrabTaskLock((Stz*)__func__);
//  pTaskWaiting->vDropTaskLock((Stz*)__func__);
//
//  // having got the lock, the task should be TS_WAITING and QK_WAITING_SET
//  RASSERT(TS_WAITING == pTaskWaiting->eState());
//  RASSERT(QK_WAITING_SET == pTaskWaiting->eQueue());
//  RASSERT(TR_NONE == pTaskWaiting->eTransition());
//
//  Schedule* pSchedule = AM::pSchedule();
//
//  pSchedule->vRemoveWaitingSet(pTaskWaiting);
//
//  RASSERT(TS_DESISTED == pTaskWaiting->eState());
//  RASSERT(QK_NONE == pTaskWaiting->eQueue());
//  RASSERT(TR_NONE == pTaskWaiting->eTransition());
//
//  vScheduleTask(pTaskWaiting);
  
  vTaskCross_ResumeWaiting(pTaskWaiting,pTaskAcking);
  // **do not touch task**

  return true;
}

/*----------------------------------------------------------------------------*/
/* resume a waiting task (must have no acks remaining)                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_ResumeWaiting(Task* pTaskWaiting,
                                         Task* pTaskResuming)
{
  _static_
    
  RASSERT(pTaskWaiting->eGetClassNo() == CN_TASK);
  RASSERT(TS_EXECUTING == pTaskResuming->eState());
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // cycle the task lock to ensure task waiting has fully entered wait state
  pTaskWaiting->vGrabTaskLock((Stz*)__func__);
  pTaskWaiting->vDropTaskLock((Stz*)__func__);
    
  // having cycled the lock, the task should be TS_WAITING and QK_WAITING_SET
  RASSERT(TS_WAITING == pTaskWaiting->eState());
  RASSERT(QK_WAITING_SET == pTaskWaiting->eQueue());
  RASSERT(TR_NONE == pTaskWaiting->eTransition());

  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->vRemoveWaitingSet(pTaskWaiting);
    
  RASSERT(TS_DESISTED == pTaskWaiting->eState());
  RASSERT(QK_NONE == pTaskWaiting->eQueue());
  RASSERT(TR_NONE == pTaskWaiting->eTransition());
  
  vScheduleTask(pTaskWaiting);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_ScheduleNextPhase(Task* pTaskParticipant,
                                             Task* pActiviating2PcTask)
{
  _static_
  
  UNUSED(pActiviating2PcTask);

  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pTaskParticipant->eState() ||
          TS_HIBERNATING == pTaskParticipant->eState());
  RASSERT(QK_NONE == pTaskParticipant->eQueue());
  RASSERT(TR_NONE == pTaskParticipant->eTransition());
  
  pTaskParticipant->vEntryPoint(EP_NEXTPHASE);
  pTaskParticipant->vState(TS_DESISTED);

  vScheduleTask(pTaskParticipant);
}

/*----------------------------------------------------------------------------*/
/* trigger op-task that operand has arrived while it is biding                */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_TriggerBiding(Task* pTaskBiding, Task* pTriggeringTask)
{
  _static_
  
  UNUSED(pTriggeringTask);
  
  //
  // called from Op::vAppendOperand()
  //
  
  RASSERT(pTaskBiding->bIsLocked());
  RASSERT(TS_BIDING == pTaskBiding->eState());
  RASSERT(QK_BIDING_SET == pTaskBiding->eQueue());
  RASSERT(TR_NONE == pTaskBiding->eTransition());

  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->vRemoveBidingSet(pTaskBiding);
  
  pTaskBiding->vDropTaskLock((Stz*)__func__);
  
  RASSERT(TS_DESISTED == pTaskBiding->eState());
  RASSERT(QK_NONE == pTaskBiding->eQueue());
  RASSERT(TR_NONE == pTaskBiding->eTransition());
  
  vScheduleTask(pTaskBiding);
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* check to see if any tasks in the sleeping queue can be woken               */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskCross_WakeSleepers(Task* pWakerTask)
{
  _static_
  
  UNUSED(pWakerTask);
  
  //
  // called from Step::vQueuesCheckSleeping()
  //
  
  Schedule* pSchedule = AM::pSchedule();
  
  while (Task* pWakingTask = pSchedule->pPopSleepingQueue()) {
        
    RASSERT(TS_DESISTED == pWakingTask->eState());
    RASSERT(QK_NONE == pWakingTask->eQueue());
    RASSERT(TR_NONE == pWakingTask->eTransition());
    
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
  _static_
  
  //
  // called from Api::bSpawnJobDedicated()
  //
  
  pNewTask->vEntryPoint(EP_DEDICATED);
  pNewTask->vState(TS_DESISTED);
    
  RASSERT(TS_DESISTED == pNewTask->eState());
  
  Thread* pThread = AM::THREAD::pNewThread();
  
  pThread->vTask(pNewTask);
  pThread->vDedicated(CN_JOB);
  
  pThread->vSpawnPsxThd_dedicated();
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddConnector(Task* pNewTask)
{
  _static_
  
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTransition());
  
  pNewTask->vEntryPoint(EP_CONNECTOR);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddOp(Task* pNewTask)
{
  _static_
  
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTransition());
  
  pNewTask->vEntryPoint(EP_OP);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddPrompt(Task* pNewTask)
{
  _static_
  
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTransition());
  
  pNewTask->vEntryPoint(EP_PROMPT);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddReceived(Task* pNewTask)
{
  _static_
  
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTransition());
  
  pNewTask->vEntryPoint(EP_RECEIVED);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddService(Task* pNewTask)
{
  _static_
  
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTransition());
  
  pNewTask->vEntryPoint(EP_SERVICE);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* add new general task into system (start scheduling it as per normal)       */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_AddSystem(Task* pNewTask)
{
  _static_
  
  //
  // called from Api::bSpawnJob()
  //
  RASSERT(TS_NONE == pNewTask->eState());
  RASSERT(QK_NONE == pNewTask->eQueue());
  RASSERT(TR_NONE == pNewTask->eTransition());
  
  pNewTask->vEntryPoint(EP_SYSTEM);
  pNewTask->vState(TS_DESISTED);

  vScheduleTask(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* set dedicated task to be executing by dedicated thread                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vTaskApi_BeginExecutingDedicated(Thread* pThread)
{
  _static_
  
  //
  // called from Thread::vLoop_dedicated()
  //
  
  ASSERT(HS_UNASSIGNED == pThread->eThreadState());
  ASSERT(pThread->pMutex()->bIsLocked());
  
  RASSERT(pThread->pTask());
  RASSERT(TS_DESISTED == pThread->pTask()->eState());
  
  Schedule* pSchedule = AM::pSchedule();
  
  pThread->pTask()->vThread(pThread);
  
  pThread->pTask()->vState(TS_EXECUTING);
  pSchedule->vAddExecutingSet(pThread->pTask());
}



// /////////////////////////////////////////////////////////////////////////////
//                              job interface                                 //
// /////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                             thread interface                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into dedicated thread vector                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddDedicatedThread(Thread* pThread)
{
  _static_
  
  RASSERT(CN_NONE != pThread->eDedicated());
    
  Schedule* pSchedule = AM::pSchedule();
  
  pThread->vSetIdle();
  pSchedule->vAddDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* add new dedicated thread into general thread vector                        */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_AddGeneralThread(Thread* pThread)
{
  _static_
  
  RASSERT(CN_NONE == pThread->eDedicated());
    
  Schedule* pSchedule = AM::pSchedule();
  
  pThread->vSetIdle();
  pSchedule->vAddGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* beginning of thread loop                                                   */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_BeginThreadLoop(Thread* pThread)
{
  _static_
  
  RASSERT(HS_IDLE == pThread->eThreadState());
    
  pThread->vThisTLSThread();
  
  // thread has mutex while it is running
  pThread->vThreadState(HS_UNASSIGNED);
  pThread->pMutex()->vLock(pThread);
  
  // increment m_ixThreads to indicate thread is running
  AM::vThreadBegins();
}

/*----------------------------------------------------------------------------*/
/* end of thread loop                                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_EndThreadLoop(Thread* pThread)
{
  _static_
    
  pThread->pMutex()->vUnlock(pThread);
}

/*----------------------------------------------------------------------------*/
/* nap loop till all general threads are idle (main thread at system startup) */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_NapTillGeneralThreads(void)
{
  _static_
    
  Schedule* pSchedule = AM::pSchedule();
  
  // pause till all general threads have all begun
  while (AM::nThreadsBegun() != AM::nThreads())
    AM::vNapMsec(10,CN_NONE);
  
  RASSERT(pSchedule->m_pGeneralThreadVec->nItems() == AM::nThreads());
  
  // pause till all general threads are in idle state
  i8 nGeneralThreads = pSchedule->m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nGeneralThreads; i_Thread++) {
    
    Thread* pThread = pSchedule->m_pGeneralThreadVec->pGet(i_Thread);
    
    while (HS_IDLE != pThread->eThreadState())
      AM::vNapMsec(10,CN_SCHEDULE);
  }
}

/*----------------------------------------------------------------------------*/
/* remove dedicated thread from dedicated thread vector                       */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveDedicatedThread(Thread* pThread)
{
  _static_
      
  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->vRemoveDedicatedThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* remove general thread from general thread vector                           */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_RemoveGeneralThread(Thread* pThread)
{
  _static_
  
  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->vRemoveGeneralThread(pThread);
}

/*----------------------------------------------------------------------------*/
/* general thread waits in idle state for task to be assigned,                */
/*   then executes task that was assigned                                     */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitForTaskAssignment(Thread* pThread)
{
  _static_
  
  Schedule* pSchedule = AM::pSchedule();
  
  vThread_WaitAssignment(pThread);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pKnownTask = pThread->pTask();
  RASSERT(TS_DESISTED == pKnownTask->eState());

  RASSERT(HS_ASSIGNED == pThread->eThreadState());
  RASSERT(pKnownTask == pThread->pTask());

  pKnownTask->vThread(pThread);
  
  // ok to touch task
  
  pThread->pTask()->vState(TS_EXECUTING);
  pSchedule->vAddExecutingSet(pThread->pTask());
  
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
  _static_
    
  RASSERT(TS_DESISTED == pKnownTask->eState());
  
  Schedule* pSchedule = AM::pSchedule();
  
  Thread* pThread = pSchedule->pThreadPopIdleNL();
  
  if (! pThread)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(HS_IDLE == pThread->eThreadState());

  pThread->pMutex()->vLock(pKnownTask);
  
  pThread->vTask(pKnownTask);
    
  pThread->pMutex()->vSignal(pKnownTask);
  pThread->pMutex()->vUnlock(pKnownTask);
  
  // mutex can now be acquired by the m_pPsxThd of pThread
  
  // DO NOT TOUCH TASK
  return true;
}

/*----------------------------------------------------------------------------*/
/* append unassigned general thread onto idle queue, wait for task assignment */
/*----------------------------------------------------------------------------*/
void Scheduler::vThread_WaitAssignment(Thread* pThread)
{
  _static_
    
  RASSERT(pThread->pMutex()->bIsLocked());
  RASSERT(pThread->pMutex()->bIsLockedByCaller(pThread));
  RASSERT(HS_UNASSIGNED == pThread->eThreadState());
  RASSERT(! pThread->pTask());

  Schedule* pSchedule = AM::pSchedule();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mutex-based loop waiting for
//for (;;) {
  while (! pThread->pTask()) {
    
    // attempt to po a task from one of the ready queues
    
    pSchedule->vGrabThreadIdleQueue();
 
    Task* pPoppedTask = pSchedule->pPopReadyQueue();
    if (! pPoppedTask)
      pPoppedTask = pSchedule->pPopReadyQueueLP();
    
    // was a task popped?
    if (pPoppedTask) {
      RASSERT(TS_READY == pPoppedTask->eState());
      pPoppedTask->vState(TS_DESISTED);

      pThread->vTask(pPoppedTask);
      pSchedule->vDropThreadIdleQueue();
      break;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    RASSERT(! pThread->pTask());
    pThread->vSetIdle();
    
    pSchedule->vThreadAppendIdle(pThread);
    
    pSchedule->vDropThreadIdleQueue();

    ASSERT(pThread->pMutex()->bIsLockedByCaller(pThread));
    pThread->pMutex()->vWait(pThread);
    ASSERT(pThread->pMutex()->bIsLockedByCaller(pThread));

    if (pThread->pTask())
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(pThread->pMutex()->bIsLocked());
  RASSERT(HS_ASSIGNED == pThread->eThreadState());
  RASSERT(pThread->pTask());
  RASSERT(TS_DESISTED == pThread->pTask()->eState());
}

// /////////////////////////////////////////////////////////////////////////////
//                            core task scheduler                             //
// /////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* attempt to assign a thread to able-to-run task, else add to ready queue    */
/*----------------------------------------------------------------------------*/
void Scheduler::vScheduleTask(Task* pKnownTask)
{
  _static_
  
  eTS eTaskState = pKnownTask->eState();
  eQK eQueue = pKnownTask->eQueue();
  eTR eTransition = pKnownTask->eTransition();
  
  if (TS_DESISTED != pKnownTask->eState()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aTR[eTransition]);
  }
  
  if (QK_NONE != pKnownTask->eQueue()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aTR[eTransition]);  }
  
  if (TR_NONE != pKnownTask->eTransition()) {
    fprintf(stderr, "State: %s; Queue: %s; SchedulerAct: %s",
            (char*)aTS[eTaskState],
            (char*)aQK[eQueue],
            (char*)aTR[eTransition]);
  }
  
  RASSERT(TS_DESISTED == pKnownTask->eState());
  RASSERT(QK_NONE == pKnownTask->eQueue());
  RASSERT(TR_NONE == pKnownTask->eTransition());

  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->vGrabThreadIdleQueue();
  
  if (bThread_AssignIdle(pKnownTask)) {
    pSchedule->vDropThreadIdleQueue();
    return;
  }
  
//  Thread* pMyThread = MY::pThread();
//  if (pMyThread &&
//      (! pMyThread->eDedicated()) &&
//      1 == gxCmdLineArgs.m_xSysArgs.m_nThreads) {
//    vTrace("@UNI-THREAD @UNI-THREAD @UNI-THREAD @UNI-THREAD @UNI-THREAD "_Stz);
//    vDebugPoint();
//  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(TS_DESISTED == pKnownTask->eState());
  RASSERT(QK_NONE == pKnownTask->eQueue());
  RASSERT(TR_NONE == pKnownTask->eTransition());
  
  pKnownTask->vState(TS_READY);

  //! ALKA: while cleaning up JobClass all jobs had m_ePriority
  //! as PY_NORMAL hence Appending to readyQueue always
  
  //if (pKnownTask->pJob()->aJobClass()->m_ePriority == PY_NORMAL)
  //  pSchedule->vAppendReadyQueue(pKnownTask);
  //else
  //  pSchedule->vAppendReadyQueueLP(pKnownTask);
 
  pSchedule->vAppendReadyQueue(pKnownTask);
  
  pSchedule->vDropThreadIdleQueue();
  
  // **do not touch task*
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

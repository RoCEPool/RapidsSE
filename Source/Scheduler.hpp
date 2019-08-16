/*============================================================================*/
/* Scheduler.hpp -- Job/Task/Thread Scheduler Mark II                         */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Scheduler
  {
  public:

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // job interface
  
    static void vAddOriginatingJob(Job* pJob);
    static void vAddParticipatingJob(Job* pJob);
    static void vDumpOriginatingJobs(void);
    static void vDumpParticipatingJobs(void);
    static Job* pFindOriginatingJob(i8 iJobId);
    static Job* pFindParticipatingJob(i8 iJobId);
    static bool bRemoveOriginatingJob(Job* pJob, bool bDump=false);
    static bool bRemoveParticipatingJob(Job* pJob, bool bDump=false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // operator interface
  
    static void vAddOp(Op* pOp);
    static void vRemoveOp(Op* pOp);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task interface
  
    static bool bAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking);
    static void vAddTaskDedicated(Task* pTask);
    static void vAddTaskGeneral(Task* pTask);
    static void vDdlDropRead(Task* pTask);
    static void vDdlDropWrite(Task* pTask);
    static void vDdlGrabRead(Task* pTask);
    static void vDdlGrabWrite(Task* pTask);
    static void vDropTaskLock(Task* pTask);
    static void vExecuteDedicatedTask(Task* pTask, Thread* pThread);
    static void vGrabTaskLock(Task* pTask);
    static void vPulsePausedTask(Task* pTask);
    static void vRemoveTaskDedicated(Task* pTask);
    static void vRemoveTaskGeneral(Task* pTask);
    static void vTaskLoopTransition(Thread* pThread);
    static void vTaskRequiresAcks(Task* pTask, i8 nAcksRequired);
    static void vTriggerBidingTask(Task* pTask);
    static void vWakeSleepingTasks(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread interface
  
    static void vAddDedicatedThread(Thread* pThread);
    static void vAddGeneralThread(Thread* pThread);
    static void vBeginThreadLoop(Thread* pThread);
    static void vEndThreadLoop(Thread* pThread);
    static void vPauseTillGeneralThreads(void);
    static void vRemoveDedicatedThread(Thread* pThread);
    static void vRemoveGeneralThread(Thread* pThread);
    static void vWaitForTaskAssignment(Thread* pThread);

  private:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for tasks
  
    static void vTask_Able(Task* pTask);
    static bool bTask_Ack(Task* pTaskWaiting, Task* pTaskAcking);
    static void vTask_Add(Task* pTask);
    static void vTask_Bide(Task* pTask);
    static void vTask_Desist(Task* pTask);
    static void vTask_DdlDropRead(Task* pTask);
    static void vTask_DdlDropWrite(Task* pTask);
    static void vTask_DdlGrabRead(Task* pTask);
    static void vTask_DdlGrabWrite(Task* pTask);
    static void vTask_Drop(Task* pTask);
    static void vTask_Execute(Task* pTask);
    static void vTask_Exit(Task* pTask);
    static void vTask_Grab(Task* pTask);
    static void vTask_Pause(Task* pTask);
    static Task* pTask_Pop(void);
    static void vTask_Pulse(Task* pTask);
    static void vTask_Ready(Task* pTask);
    static void vTask_Remove(Task* pTask);
    static void vTask_Requires(Task* pTask, i8 nAcksRequired);
    static void vTask_Resume(Task* pTask);
    static void vTask_Schedule(Task* pTask);
    static void vTask_Sleep(Task* pTask);
    static void vTask_Thread(Task* pTask, Thread* pThread);
    static void vTask_Trigger(Task* pTask);
    static void vTask_Wait(Task* pTask);
    static void vTask_Wakeup(void);
    static void vTask_Yield(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for threads
  
    static void vThread_AddDedicated(Thread* pThread);
    static void vThread_AddGeneral(Thread* pThread);
    static void vThread_AppendIdle(Thread* pThread);
    static bool bThread_AssignIdle(Task* pTask);
    static void vThread_BeginLoop(Thread* pThread);
    static void vThread_ContinueSame(Thread* pThread, Task* pTask);
    static void vThread_EndLoop(Thread* pThread);
    static void vThread_ExecuteDedicated(Thread* pThread);
    static void vThread_PauseGeneral(void);
    static void vThread_RemoveDedicated(Thread* pThread);
    static void vThread_RemoveGeneral(Thread* pThread);
    static void vThread_SpawnDedicated(Task* pTask);
    static void vThread_UnassignTask(Thread* pThread);
    static void vThread_WaitAssignment(Thread* pThread);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

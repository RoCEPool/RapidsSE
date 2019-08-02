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
    static Job* pFindOriginatingJob(i8 iJobId);
    static Job* pFindParticipatingJob(i8 iJobId);
    static bool bRemoveOriginatingJob(Job* pJob, bool bDump=false);
    static bool bRemoveParticipatingJob(Job* pJob, bool bDump=false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task interface
  
    static bool bAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking);
    static void vContinuePausedTask(Task* pTask);
    static void vExecuteDedicatedTask(Task* pTask, Thread* pThread);
    static void vNewTaskDedicated(Task* pTask);
    static void vNewTaskGeneral(Task* pTask);
    static void vTaskLoopTransition(Thread* pThread);
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
    static void vTask_Bide(Task* pTask);
    static void vTask_Continue(Task* pTask);
    static void vTask_Desist(Task* pTask);
    static void vTask_Execute(Task* pTask);
    static void vTask_Exit(Task* pTask);
    static void vTask_Pause(Task* pTask);
    static void vTask_Ready(Task* pTask);
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

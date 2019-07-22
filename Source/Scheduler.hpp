/*============================================================================*/
/* Scheduler.hpp -- Thread/Task Event Scheduler                               */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Scheduler
  {
  public:

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task interface
  
    static bool bAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking);
    static void vAddTaskDedicated(Task* pTask);
    static void vAddTaskGeneral(Task* pTask);
    static void vReturnTaskLoop(Thread* pThread);
    static void vTriggerBidingTask(Task* pTask);
    static void vWakeSleepingTasks(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread interface
  
    static void vAddDedicatedThread(Thread* pThread);
    static void vAddGeneralThread(Thread* pThread);
    static void vAssignDedicatedThread(Thread* pThread);
    static void vBeginThread(Thread* pThread);
    static void vEndThread(Thread* pThread);
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
    static void vTask_Execute(Task* pTask);
    static void vTask_Exit(Task* pTask);
    static void vTask_Ready(Task* pTask);
    static void vTask_Resume(Task* pTask);
    static void vTask_Schedule(Task* pTask);
    static void vTask_Sleep(Task* pTask);
    static void vTask_Thread(Task* pTask, Thread* pThread);
    static void vTask_Trigger(Task* pTask);
    static void vTask_Wait(Task* pTask);
    static void vTask_Wake(Task* pTask);
    static void vTask_Wakeup(void);
    static void vTask_Yield(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for threads
  
    static void vThread_AddDedicated(Thread* pThread);
    static void vThread_AddGeneral(Thread* pThread);
    static void vThread_AppendIdle(Thread* pThread);
    static void vThread_AssignDedicated(Thread* pThread);
    static bool bThread_AssignIdle(Task* pTask);
    static void vThread_Beginning(Thread* pThread);
    static void vThread_ContinueSame(Thread* pThread, Task* pTask);
    static void vThread_Deassign(Thread* pThread);
    static void vThread_DedicatedTask(Task* pTask);
    static void vThread_Ending(Thread* pThread);
    static void vThread_PauseGeneral(void);
    static void vThread_RemoveDedicated(Thread* pThread);
    static void vThread_RemoveGeneral(Thread* pThread);
    static void vThread_WaitAssignment(Thread* pThread);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

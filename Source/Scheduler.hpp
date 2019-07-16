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
  
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vAddTaskDedicated(Task* pTask);
    static void vAddTaskGeneral(Task* pTask);
    static void vReturnTaskLoop(Thread* pThread);
    static void vScheduleReadyTask(Task* pTask);
    static void vWakeSleepingTasks(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread interface
  
    static void vAddThreadDedicated(Thread* pThread);
    static void vAddThreadGeneral(Thread* pThread);
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
  
    static bool bTask_AckWaiting(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vTask_Exit(Task* pTask);
    static void vTask_ResumeAck(Task* pTask);
    static void vTask_SpawnNew(Task* pTask);
    static void vTask_Sleep(Task* pTask);
    static void vTask_Wait(Task* pTask);
    static void vTask_WakeSleeping(void);
    static void vTask_Yield(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for threads
  
    static void vThread_AddDedicated(Thread* pThread);
    static void vThread_AddGeneral(Thread* pThread);
    static void vThread_AppendIdle(Thread* pThread);
    static bool bThread_AssignIdle(Task* pTask);
    static void vThread_Beginning(Thread* pThread);
    static void vThread_DeassignRunning(Thread* pThread);
    static void vThread_DedicatedTask(Thread* pThread, Task* pTask);
    static void vThread_Ending(Thread* pThread);
    static void vThread_Running(Thread* pThread);
    static void vThread_WaitAssignment(Thread* pThread);
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // support for queues
    //! MOVE TO QUEUES

    static void vQueue_AddExecuting(Task* pTask);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

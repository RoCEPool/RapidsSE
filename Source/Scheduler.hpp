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
    // task
  
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vAddTaskDedicated(Task* pTask);
    static void vAddTaskGeneral(Task* pTask);
    static void vCheckSleepingTasks(void);
    static void vReturnTaskLoop(Task** ppTask);
    static void vScheduleReadyTask(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread
  
    static void vAddThreadDedicated(Thread* pThread);
    static void vAddThreadGeneral(Thread* pThread);
    static void vAssignDedicatedThread(Thread* pThread);
    static void vBeginThread(Thread* pThread);
    static void vEndThread(Thread* pThread);
    static void vPauseTillGeneralThreads(void);
    static void vRemoveDedicatedThread(Thread* pThread);
    static void vRemoveGeneralThread(Thread* pThread);
    static void vWaitForTaskAssignment(Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // support
  
  private:
    static bool bAssign_IdleThread(Task* pTask);
  
  public:
    static void vQueue_ExecutingTask(Task* pTask);
  private:
    static void vQueue_IdleThread(Thread* pThread);
    static void vQueue_SleepingTask(Task* pTask);
    static void vQueue_YieldedTask(Task* pTask);
  
    static void vRemove_ExecutingSet(Task* pTask);
    static void vRemove_WaitingSet(Task* pTask);

    static void vTask_Exited(Task* pTask);
    static void vTask_Executing(Task* pTask);
    static void vTask_Ready(Task* pTask);
    static void vTask_Return(Task* pTask);
    static void vTask_Sleeping(Task* pTask);
    static void vTask_Waiting(Task* pTask);
    static void vTask_Yielded(Task* pTask);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

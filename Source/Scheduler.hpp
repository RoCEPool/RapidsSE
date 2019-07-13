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
    static void vTaskReturn(Task** ppTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread
  
    static void vAddDedicatedThread(Thread* pThread);
    static void vAddGeneralThread(Thread* pThread);
    static void vAssignDedicated(Thread* pThread);
    static void vBeginThread(Thread* pThread);
    static void vCheckSleeping(void);
    static void vEndThread(Thread* pThread);
    static void vPauseForGeneralThreads(void);
    static void vRemoveDedicatedThread(Thread* pThread);
    static void vRemoveGeneralThread(Thread* pThread);
    static void vWaitForTaskAssignment(Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // support
  
  private:
    static Thread* pPop_IdleThread(Task* pTask);
  
    static void vQueue_AckedTask(Task* pTask);
  public:
    static void vQueue_ExecutingTask(Task* pTask);
  private:
    static void vQueue_ExitedTask(Task* pTask);
    static void vQueue_IdleThread(Thread* pThread);
    static void vQueue_NewTask(Task* pTask);
    static void vQueue_ReadyTask(Task* pTask);
    static void vQueue_SleepingTask(Task* pTask);
    static void vQueue_WaitingTask(Task* pTask);
    static void vQueue_YieldedTask(Task* pTask);
  
    static void vRemove_WaitingTask(Task* pTask);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

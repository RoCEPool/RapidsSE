/*============================================================================*/
/* Scheduler.hpp -- Thread/Task Event Scheduler                               */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Scheduler
  {
  public:
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vAddDedicatedThread(Thread* pThread);
    static void vAddGeneralThread(Thread* pThread);
    static void vCheckSleeping(void);
    static void vIdleThread(Thread* pThread);
    static void vPauseForGeneralThreads(void);
    static void vSpawnedTask(Task* pTask);
    static void vSpawnedTaskDedicated(Task* pTask);
    static void vTaskReturn(Task** ppTask);
  
  public:
    static void vScheduleAckedTask(Task* pTask);
    static void vScheduleExitedTask(Task* pTask);
    static void vScheduleReadyTask(Task* pTask);
    static void vScheduleSleepingTask(Task* pTask);
    static void vScheduleSpawnedTask(Task* pTask);
    static void vScheduleWaitingTask(Task* pTask);
    static void vScheduleWokenTask(Task* pTask);
    static void vScheduleYieldedTask(Task* pTask);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

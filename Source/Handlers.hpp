/*============================================================================*/
/* Handlers.hpp -- Thread/Task Event Handlers                                 */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Handlers
  {
  public:
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
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

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

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

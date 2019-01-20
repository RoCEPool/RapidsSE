/*============================================================================*/
/* Handlers.hpp -- Thread/Task Event Handlers                                 */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers
  {
  public:
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vSpawnedTask(Task* pTask);
    static void vThreadLoop(Task** ppTask);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

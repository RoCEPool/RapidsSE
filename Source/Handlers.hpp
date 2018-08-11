/*============================================================================*/
/* Handlers.hpp -- Thread/Task Event Handlers                                 */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers
  {
  public:
    static void vAckTaskHandler(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vSpawnedTaskHandler(Task* pTask);
    static void vThreadLoopHandler(NP_Task* pNpTask);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

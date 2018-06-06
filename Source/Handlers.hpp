/*============================================================================*/
/* Handlers.hpp -- Queue/System/Thread Event Handlers                         */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers
  {
  public:
    static bool bQueueLevelHandler(Task* pTask);
    static bool bSpawnedTaskHandler(Task* pTask);
    static bool bThreadLoopHandler(NP_Task* pNpTask);
  
    static bool bAckTaskHandler(Task* pTask);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

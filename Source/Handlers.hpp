/*============================================================================*/
/* Handlers.hpp -- Thread/Task Event Handlers                                 */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers
  {
  public:
    static bool bAckTaskHandler(Task* pTask);
    static bool bSpawnedTaskHandler(Task* pTask);
    static bool bThreadLoopHandler(NP_Task* pNpTask); 
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

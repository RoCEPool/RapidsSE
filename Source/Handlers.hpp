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
    static bool bSystemLevelHandler(Task* pTask);
    static bool bThreadLevelHandler(NP_Task* pNpTask);
  
    static bool bAckTaskHandler(Task* pTask);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

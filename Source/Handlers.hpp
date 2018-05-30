/*============================================================================*/
/* Handlers.hpp -- Queue/System/Thread Event Handlers                         */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers
  {
  public:
    static bool bQueueHandler(Task* pTask);
    static bool bSystemHandler(Task* pTask);
    static bool bThreadHandler(Task* pTask);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

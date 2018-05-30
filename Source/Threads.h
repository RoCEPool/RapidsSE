/*============================================================================*/
/* Threads.h -- Threads namespace                                             */
/*============================================================================*/
#pragma once

#include "Tasks.h"

//-----------------------------------------------------------------------------
namespace Threads
{
  void vCheckAssumptions(void);
}

using namespace Threads;

//-----------------------------------------------------------------------------

#include "Queues.hpp"

#include "TlsKeys.hpp"
#include "Thread.hpp"
#include "ThreadList.hpp"
#include "ThreadQueue.hpp"
#include "ThreadCursor.hpp"

#include "Handlers.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

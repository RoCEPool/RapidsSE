/*============================================================================*/
/* Threads.h -- Threads namespace                                             */
/*============================================================================*/
#pragma once

#include "Tasks.h"

enum eQR // queue round robin
{
  /* -2 */ QR_SLEEPING_QUEUE=-2,
  /* -1 */ QR_GENERAL_QUEUE,
};

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

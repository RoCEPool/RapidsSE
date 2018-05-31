/*============================================================================*/
/* Threads.h -- Threads namespace                                             */
/*============================================================================*/
#pragma once

#include "Tasks.h"

enum eQR // queue round robin
{
  /* -3 */ QR_NONE=-3,
  /* -2 */ QR_SLEEPING_QUEUE,
  /* -1 */ QR_GENERAL_QUEUE,
};

static const i8 C_THREADS = 256;

//-----------------------------------------------------------------------------
namespace Threads
{
  void vCheckAssumptions(void);
}

using namespace Threads;

//-----------------------------------------------------------------------------

#include "TlsKeys.hpp"
#include "Thread.hpp"
#include "ThreadList.hpp"
#include "ThreadQueue.hpp"
#include "ThreadCursor.hpp"
#include "ThreadVector.hpp"

#include "Queues.hpp"

#include "Handlers.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

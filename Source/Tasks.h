/*============================================================================*/
/* Tasks.h -- Tasks namespace                                                 */
/*============================================================================*/
#pragma once

#include "Utilities.h"

enum eQR // queue round robin
{
  /* -5 */ QR_NONE=-5,
  /* -4 */ QR_EXECUTING_QUEUE,
  /* -3 */ QR_WAITING_QUEUE,
  /* -2 */ QR_SLEEPING_QUEUE,
  /* -1 */ QR_READY_QUEUE,
};

//-----------------------------------------------------------------------------

namespace Tasks
{
  void vCheckAssumptions(void);
}

using namespace Tasks;

//-----------------------------------------------------------------------------

#include "Compiler.hpp"

#include "Socket.hpp"

#include "Step.hpp"
#include "Task.hpp"
#include "TaskMacros.hpp"
#include "TaskList.hpp"
#include "TaskQueue.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

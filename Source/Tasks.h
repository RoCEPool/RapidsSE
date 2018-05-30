/*============================================================================*/
/* Tasks.h -- Tasks namespace                                                 */
/*============================================================================*/
#pragma once

#include "Utilities.h"

//-----------------------------------------------------------------------------

namespace Tasks
{
  void vCheckAssumptions(void);
}

using namespace Tasks;

//-----------------------------------------------------------------------------

#include "StepAccum.hpp"
#include "StepArray.hpp"

#include "Socket.hpp"

#include "Task.hpp"
#include "TaskList.hpp"
#include "TaskFifo.hpp"
#include "TaskQueue.hpp"
#include "TaskSet.hpp"

#include "TaskCursor.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

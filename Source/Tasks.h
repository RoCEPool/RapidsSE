/*============================================================================*/
/* Tasks.h -- Tasks namespace                                                 */
/*============================================================================*/
#pragma once

#include "MockEE.h"

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
#include "TaskSet.hpp"

#include "TaskCursor.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

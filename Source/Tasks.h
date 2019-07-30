/*============================================================================*/
/* Tasks.h -- Tasks namespace                                                 */
/*============================================================================*/
#pragma once

#include "Jobs.h"
#include "eQK.h"

//------------------------------------------------------------------------------

namespace Tasks
{
  void vCheckAssumptions(void);
}

using namespace Tasks;

//------------------------------------------------------------------------------

#include "Error.hpp"
#include "eTR.h"
#include "eTS.h"

#include "Task.hpp"
#include "TaskMacros.hpp"
#include "TaskList.hpp"
#include "TaskQueue.hpp"

#include "EnterExit.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

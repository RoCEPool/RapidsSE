/*============================================================================*/
/* Logging.h -- Logging Namespace                                             */
/*============================================================================*/
#pragma once

#include "Measurement.h"

namespace Logging
{
  void vCheckAssumptions(void);
}

using namespace Logging;

//------------------------------------------------------------------------------

#include "ShmLog.hpp"
#include "LogEntry.hpp"

#include "Action.hpp"
#include "ActionLog.hpp"

#include "ActivityLog.hpp"

#include "Log.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

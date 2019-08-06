/*============================================================================*/
/* Logging.h -- Logging Namespace                                             */
/*============================================================================*/
#pragma once

#include "Measurement.h"

//------------------------------------------------------------------------------

namespace Logging
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ShmLog.hpp"

#include "LogEntryJob.hpp"
#include "LogEntryOp.hpp"
#include "LogEntryRes.hpp"
#include "LogEntryThd.hpp"
#include "LogEntryTsk.hpp"
#include "LogEntryTxt.hpp"
#include "LogEntry.hpp"
#include "Log.hpp"
#include "ActLog.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

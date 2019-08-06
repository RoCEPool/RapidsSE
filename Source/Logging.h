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

#include "LogEntryInf.hpp"
#include "LogEntryJob.hpp"
#include "LogEntryOp.hpp"
#include "LogEntryRes.hpp"
#include "LogEntrySvc.hpp"
#include "LogEntryThd.hpp"
#include "LogEntryTsk.hpp"
#include "LogEntryTxn.hpp"
#include "LogEntry.hpp"
#include "Log.hpp"
#include "ActLog.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

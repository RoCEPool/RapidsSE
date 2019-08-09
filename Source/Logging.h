/*============================================================================*/
/* Logging.h -- Logging Namespace                                             */
/*============================================================================*/
#pragma once

#include "Chunks.h"

//------------------------------------------------------------------------------

namespace Logging
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ShmLog.hpp"

#include "LogEntryInf.hpp"
#include "LogEntryItf.hpp"
#include "LogEntryJob.hpp"
#include "LogEntryOp.hpp"
#include "LogEntryPrm.hpp"
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

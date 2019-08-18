/*============================================================================*/
/* Logging.h -- Logging Namespace                                             */
/*============================================================================*/
#pragma once

#include "Resources.h"

//------------------------------------------------------------------------------

namespace Logging
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ShmLog.hpp"

#include "LogEntryApi.hpp"
#include "LogEntryFyi.hpp"
#include "LogEntryJob.hpp"
#include "LogEntryLck.hpp"
#include "LogEntryLog.hpp"
#include "LogEntryObj.hpp"
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

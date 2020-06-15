/*============================================================================*/
/* Jobs.h -- Jobs namespace                                                   */
/*============================================================================*/
#pragma once

#include "Pipelines.h"

//------------------------------------------------------------------------------

namespace Jobs
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "FetchBuffer.hpp"

#include "Response.hpp"
#include "Result.hpp"

#include "JobFilePath.hpp"

#include "Job.hpp"

#include "JobLog.hpp"

#include "RowOnce.hpp"

#include "CmdHelper.hpp"

#include "Job__Find.hpp"

#include "Job_Distributed.hpp"

#include "Job_Local.hpp"

#include "XmitOperand.hpp"

#include "JobList.hpp"

#include "JobDesc.hpp"
#include "JobClass.hpp"
#include "JobCatalog.hpp"

#include "SP_Job.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

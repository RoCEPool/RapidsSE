/*============================================================================*/
/* Jobs.h -- Jobs namespace                                                   */
/*============================================================================*/
#pragma once

#include "Plans.h"

//------------------------------------------------------------------------------

namespace Jobs
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Response.hpp"
typedef Vector_t<CN_RESPONSEVEC,Response,C_MAX_NODES> ResponseVec;

#include "Result.hpp"

#include "JobFilePath.hpp"

#include "Job.hpp"

#include "JobLog.hpp"

#include "CmdHelper.hpp"

#include "Job__Find.hpp"

#include "XmitOperand.hpp"

#include "JobList.hpp"

#include "SP_Job.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

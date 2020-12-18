/*============================================================================*/
/* Plans.h -- includes for Plans namespace                                    */
/*============================================================================*/
#pragma once

#include "Sessions.h"

//------------------------------------------------------------------------------

namespace Plans
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"


#include "AccessPath.hpp"
typedef VectorNL_t<CN_PATHVEC,AccessPath,128> PathVec;

#include "Plan.hpp"
#include "Planner.hpp"


#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"



#include "Xmits.hpp"
#include "Op.hpp"
#include "Strand.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"



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

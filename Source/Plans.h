/*============================================================================*/
/* Plans.h -- includes for Plans namespace                                    */
/*============================================================================*/
#pragma once

#include "DBobjects.h"

//------------------------------------------------------------------------------

namespace Plans
{
  void vCheckAssumptions(void);
}

using namespace Plans;

//------------------------------------------------------------------------------

#include "Arg.hpp"
#include "Op.hpp"
#include "Predicate.hpp"

typedef Vector<CN_CANDIDATEPATH,Column,CN_COLUMN,C_INDEXES_PER_TABLE,u8> CandidatePath;
typedef Vector<CN_CANDIDATEPATHS,CandidatePath,CN_CANDIDATEPATH,C_INDEXES_PER_TABLE,u8> CandidatePaths;

#include "AccessPath.hpp"
//#include "CandidatePath.hpp"
//#include "CandidatePaths.hpp"
#include "Plan.hpp"
#include "Planner.hpp"
#include "PlanList.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

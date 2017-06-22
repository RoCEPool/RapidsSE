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

typedef Vector<CN_PREDICATEVEC,Predicate,CN_PREDICATE,128,u8> PredicateVec;


#include "AccessPath.hpp"
typedef Vector<CN_PATHVEC,AccessPath,CN_ACCESSPATH,128,u8> PathVec;

#include "Plan.hpp"
#include "Planner.hpp"
#include "PlanList.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

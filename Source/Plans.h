/*============================================================================*/
/* Plans.h -- includes for Plans namespace                                    */
/*============================================================================*/
#pragma once

#include "PushDown.h"

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
#include "PredicateVec.hpp"


#include "AccessPath.hpp"
typedef VectorNL_t<CN_PATHVEC,AccessPath,CN_ACCESSPATH,128,u8> PathVec;

#include "Plan.hpp"
#include "Planner.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

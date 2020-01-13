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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* PushDown.h -- PushDown namespace                                           */
/*============================================================================*/
#pragma once

#include "Partitioning.h"

namespace PushDown
{
  void vCheckAssumptions(void);
}

using namespace PushDown;

//------------------------------------------------------------------------------

#include "AggOp.hpp"
#include "FilterOp.hpp"
#include "QueryOp.hpp"
#include "RefSet.hpp"
#include "RefBuffer.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

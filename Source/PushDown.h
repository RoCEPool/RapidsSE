/*============================================================================*/
/* PushDown.h -- PushDown namespace                                           */
/*============================================================================*/
#pragma once

#include "Plans.h"

namespace PushDown
{
  void vCheckAssumptions(void);
}

using namespace PushDown;

//------------------------------------------------------------------------------

#include "CancelMarker.hpp"
#include "EndMarker.hpp"
#include "OperandQueue.hpp"

#include "Op.hpp"

#include "AggOp.hpp"
#include "BothOp.hpp"
#include "CombineOp.hpp"
#include "DistinctOp.hpp"
#include "FetchOp.hpp"
#include "HashOp.hpp"
#include "IntersectOp.hpp"
#include "JoinOp.hpp"
#include "LimitOp.hpp"
#include "MergeOp.hpp"
#include "OutputOp.hpp"
#include "ProjectOp.hpp"
#include "ReduceOp.hpp"
#include "SortOp.hpp"
#include "TakeawayOp.hpp"
#include "WhereOp.hpp"
#include "XmitOp.hpp"
#include "ZedOp.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

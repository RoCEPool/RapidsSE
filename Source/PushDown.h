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

#include "Op.hpp"
#include "AggOp.hpp"
#include "FetchOp.hpp"
#include "HashOp.hpp"
#include "JoinOp.hpp"
#include "LimitOp.hpp"
#include "MergeOp.hpp"
#include "OutputOp.hpp"
#include "ProjectOp.hpp"
#include "ReduceOp.hpp"
#include "SortOp.hpp"
#include "WhereOp.hpp"
#include "XmitOp.hpp"
#include "ZedOp.hpp"

#include "RefSet.hpp"
#include "RefBuffer.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Operators.h -- Operators namespace                                           */
/*============================================================================*/
#pragma once

#include "Plans.h"

//------------------------------------------------------------------------------

namespace Operators
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "OpDefn.hpp"
#include "OpDesc.hpp"

#include "Op.hpp"
#include "OpVec.hpp"
#include "OpCatalog.hpp"

#include "Digest.hpp"
#include "OpHelper.hpp"

#include "AggOp.hpp"
#include "BothOp.hpp"
#include "CombineOp.hpp"
#include "DistinctOp.hpp"
#include "FetchOp.hpp"
#include "GenerateOp.hpp"
#include "HashOp.hpp"
#include "IntersectOp.hpp"
#include "JoinOp.hpp"
#include "LimitOp.hpp"
#include "MergeOp.hpp"
#include "OutputOp.hpp"
#include "ProjectOp.hpp"
#include "ReduceOp.hpp"
#include "ScanOp.hpp"
#include "SortOp.hpp"
#include "TakeawayOp.hpp"
#include "WhereOp.hpp"
#include "XmitOp.hpp"
#include "ZedOp.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

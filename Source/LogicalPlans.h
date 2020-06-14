/*============================================================================*/
/* LogicalPlans.h -- Logical Plans                                            */
/*============================================================================*/
#pragma once

#include "Manifests.h"

//------------------------------------------------------------------------------

namespace LogicalPlans
{
  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Filter.hpp"
#include "Filter_Fetch.hpp"
#include "Filter_From.hpp"
#include "Filter_GroupBy.hpp"
#include "Filter_Inflate.hpp"
#include "Filter_Join.hpp"
#include "Filter_Limit.hpp"
#include "Filter_OrderBy.hpp"
#include "Filter_Output.hpp"
#include "Filter_Project.hpp"
#include "Filter_Scan.hpp"
#include "Filter_Select.hpp"
#include "Filter_SubQry.hpp"
#include "Filter_Where.hpp"
#include "Filter_Zed.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

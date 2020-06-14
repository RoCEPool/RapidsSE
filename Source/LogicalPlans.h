/*============================================================================*/
/* LogicalPlans.h -- Logical Plans                                            */
/*============================================================================*/
#pragma once

#include "MiddleEnd.h"

//------------------------------------------------------------------------------

namespace LogicalPlans
{
  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "LogicalOp.hpp"
#include "Lgl_Fetch.hpp"
#include "Lgl_From.hpp"
#include "Lgl_GroupBy.hpp"
#include "Lgl_Inflate.hpp"
#include "Lgl_Join.hpp"
#include "Lgl_Limit.hpp"
#include "Lgl_OrderBy.hpp"
#include "Lgl_Output.hpp"
#include "Lgl_Project.hpp"
#include "Lgl_Scan.hpp"
#include "Lgl_Select.hpp"
#include "Lgl_SubQry.hpp"
#include "Lgl_Where.hpp"
#include "Lgl_Zed.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

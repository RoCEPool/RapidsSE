/*============================================================================*/
/* CompilerStream.h -- CompilerStream namespace                               */
/*============================================================================*/
#pragma once

#include "ControlData.h"

//------------------------------------------------------------------------------

namespace CompilerStream
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Stmt.hpp"

#include "Cmd.hpp"

#include "Clause.hpp"
#include "Clause_SubQry.hpp"

#include "Descend.hpp"
#include "KeyWords.hpp"

#include "Clause_From.hpp"
#include "Clause_GroupBy.hpp"
#include "Clause_Join.hpp"
#include "Clause_Limit.hpp"
#include "Clause_OrderBy.hpp"
#include "Clause_Select.hpp"
#include "Clause_Where.hpp"


#include "Cmd_Select.hpp"

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

#include "Stmt_Select.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

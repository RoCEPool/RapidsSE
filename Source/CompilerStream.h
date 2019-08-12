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
#include "Stmt_Select.hpp"

#include "Clause.hpp"

#include "Clause_From.hpp"
#include "Clause_GroupBy.hpp"
#include "Clause_Join.hpp"
#include "Clause_Limit.hpp"
#include "Clause_OrderBy.hpp"
#include "Clause_Select.hpp"
#include "Clause_SubQry.hpp"
#include "Clause_Where.hpp"

#include "Filter.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

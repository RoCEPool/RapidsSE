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

#include "Clause.hpp"
#include "Stmt_SubQry.hpp"

#include "Descend.hpp"
#include "KeyWords.hpp"

#include "Clause_From.hpp"
#include "Clause_GroupBy.hpp"
#include "Clause_Join.hpp"
#include "Clause_Limit.hpp"
#include "Clause_OrderBy.hpp"
#include "Clause_Project.hpp"
#include "Clause_Where.hpp"

#include "Stmt_Select.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

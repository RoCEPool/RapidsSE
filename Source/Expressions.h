/*============================================================================*/
/* Expressions.h -- Expressions namespace                                     */
/*============================================================================*/
#pragma once

#include "DynamoCompiler.h"

//------------------------------------------------------------------------------

namespace Expressions
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Calculator.hpp"

#include "DynFun.hpp"
#include "ExprCompiler.hpp"
#include "Expr.hpp"
#include "ExprVec.hpp"

#include "AggregateRsb.hpp"
#include "FilterRsb.hpp"
#include "ProjectRsb.hpp"
#include "JoinRsb.hpp"

#include "RegSet.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

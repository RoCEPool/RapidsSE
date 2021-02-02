/*============================================================================*/
/* OperatorExpr.hpp - an expression operator                                  */
/*============================================================================*/

#include "BASE.h"
#include "OperatorExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "mathematical/MathematicalExpr.hpp"
#include "comparison/ComparisonExpr.hpp"
#include "logic/LogicExpr.hpp"
#include "aggregates/AggregateExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_OPERATOR_NAME_PREFIX = "OperatorExpr"_Stz;
static Stz* C_OPERATOR_DEFN = "/__root__/definitions/object/expr/operator"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void OperatorExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (MathematicalExpr::bIsOfType(pNode))  { MathematicalExpr::vDescribe(pNode, sBuf); }
  else if (ComparisonExpr::bIsOfType(pNode))    { ComparisonExpr::vDescribe(pNode, sBuf);   }
  else if (LogicExpr::bIsOfType(pNode))         { LogicExpr::vDescribe(pNode, sBuf);        }
  else if (AggregateExpr::bIsOfType(pNode))     { AggregateExpr::vDescribe(pNode, sBuf);    }
//  else if (FunctionExpr::bIsOfType(pNode))      { FunctionExpr::vDescribe(pNode, sBuf);   }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 OperatorExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_OPERATOR_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

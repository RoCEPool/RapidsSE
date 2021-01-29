/*============================================================================*/
/* OperatorExpr.hpp - an expression operator                                  */
/*============================================================================*/

#include "BASE.h"
#include "OperatorExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "ArithmeticExpr.hpp"

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

  if      (ArithmeticExpr::bIsOfType(pNode))    { ArithmeticExpr::vDescribe(pNode, sBuf); }
//  else if (RelationalExpr::bIsOfType(pNode))    { RelationalExpr::vDescribe(pNode, sBuf); }
//  else if (LogicalExpr::bIsOfType(pNode))       { LogicalExpr::vDescribe(pNode, sBuf);    }
//  else if (AggregateExpr::bIsOfType(pNode))     { AggregateExpr::vDescribe(pNode, sBuf);  }
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

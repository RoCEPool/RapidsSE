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
void OperatorExpr::vDescribe(PlanNode *pNode, s2K &sBuf, PlanNode *pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (MathematicalExpr::bIsOfType(pNode))  { MathematicalExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (  ComparisonExpr::bIsOfType(pNode))  {   ComparisonExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (       LogicExpr::bIsOfType(pNode))  {        LogicExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (   AggregateExpr::bIsOfType(pNode))  {    AggregateExpr::vDescribe(pNode, sBuf, pRootNode); }
//  else if (  FunctionExpr::bIsOfType(pNode))  {     FunctionExpr::vDescribe(pNode, sBuf, pRootNode);     }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 OperatorExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_OPERATOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OperatorExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_OPERATOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT OperatorExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (MathematicalExpr::bIsOfType(pExprNode))  { return MathematicalExpr::eType(pExprNode, pRootNode); }
  else if (  ComparisonExpr::bIsOfType(pExprNode))  { return   ComparisonExpr::eType(pExprNode, pRootNode); }
  else if (       LogicExpr::bIsOfType(pExprNode))  { return        LogicExpr::eType(pExprNode, pRootNode); }
  else if (   AggregateExpr::bIsOfType(pExprNode))  { return    AggregateExpr::eType(pExprNode, pRootNode); }
//  else if (  FunctionExpr::bIsOfType(pExprNode))  { return     FunctionExpr::eType(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

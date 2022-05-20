/*============================================================================*/
/* OperatorExpr.hpp - an expression operator                                  */
/*============================================================================*/

#include "BASE.h"
#include "OperatorExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/expressions/operators/mathematical/MathematicalExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/ComparisonExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/logic/LogicExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/AggregateExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/stringFunction/StringFunctionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/conditional/ConditionalExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/misc/MiscExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "OperatorExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator"_Stz;

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

  if      (  MathematicalExpr::bIsOfType(pNode))  {  MathematicalExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (    ComparisonExpr::bIsOfType(pNode))  {    ComparisonExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (         LogicExpr::bIsOfType(pNode))  {         LogicExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     AggregateExpr::bIsOfType(pNode))  {     AggregateExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (StringFunctionExpr::bIsOfType(pNode))  {StringFunctionExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (   ConditionalExpr::bIsOfType(pNode))  {   ConditionalExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (          MiscExpr::bIsOfType(pNode))  {          MiscExpr::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 OperatorExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OperatorExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId OperatorExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (  MathematicalExpr::bIsOfType(pExprNode))  { return   MathematicalExpr::xTypeId(pExprNode, pRootNode); }
  else if (    ComparisonExpr::bIsOfType(pExprNode))  { return     ComparisonExpr::xTypeId(pExprNode, pRootNode); }
  else if (         LogicExpr::bIsOfType(pExprNode))  { return          LogicExpr::xTypeId(pExprNode, pRootNode); }
  else if (     AggregateExpr::bIsOfType(pExprNode))  { return      AggregateExpr::xTypeId(pExprNode, pRootNode); }
  else if (StringFunctionExpr::bIsOfType(pExprNode))  { return StringFunctionExpr::xTypeId(pExprNode, pRootNode); }
  else if (   ConditionalExpr::bIsOfType(pExprNode))  { return    ConditionalExpr::xTypeId(pExprNode, pRootNode); }
  else if (          MiscExpr::bIsOfType(pExprNode))  { return           MiscExpr::xTypeId(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

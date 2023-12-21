/*============================================================================*/
/* AggregateExpr.cpp - a aggregate expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "AggregateExpr.hpp"
#include "CountExpr.hpp"
#include "SumExpr.hpp"
#include "MinExpr.hpp"
#include "MaxExpr.hpp"
#include "AvgExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_AGGREGATE_NAME_PREFIX = "ExprAggregate"_Stz;
static Stz* C_AGGREGATE_DEFN = "/__root__/definitions/object/expr/operator/aggregate"_Stz;
const Stz* AggregateExpr::C_DISTINCT_PROP_NAME = "isDistinct"_Stz;
const Stz* AggregateExpr::C_DISTINCT_PROP_TYPE = "/__root__/definitions/object/valueObject/booleanObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AggregateExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (CountExpr::bIsOfType(pNode))  { CountExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (  SumExpr::bIsOfType(pNode))  {   SumExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (  MinExpr::bIsOfType(pNode))  {   MinExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (  MaxExpr::bIsOfType(pNode))  {   MaxExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (  AvgExpr::bIsOfType(pNode))  {   AvgExpr::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void AggregateExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (CountExpr::bIsOfType(pNode))  { CountExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (  SumExpr::bIsOfType(pNode))  {   SumExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (  MinExpr::bIsOfType(pNode))  {   MinExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (  MaxExpr::bIsOfType(pNode))  {   MaxExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (  AvgExpr::bIsOfType(pNode))  {   AvgExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if the aggregate expression is a distinct expression          */
/*----------------------------------------------------------------------------*/
b1 AggregateExpr::bIsDistinct(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  PlanNode* pDistinctNode = GraphNav::pFindProperty(const_cast<Stz*>(C_DISTINCT_PROP_NAME), pNode);
  if (!pDistinctNode) {
    return false;
  }

  ASSERT(pDistinctNode->szType()->bEqual(const_cast<Stz*>(C_DISTINCT_PROP_TYPE)));
  return pDistinctNode->bValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 AggregateExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_AGGREGATE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 AggregateExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_AGGREGATE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId AggregateExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Manual expression polymorphism here

  if      (CountExpr::bIsOfType(pExprNode))  { return CountExpr::xTypeId(pExprNode, pRootNode); }
  else if (  SumExpr::bIsOfType(pExprNode))  {   return SumExpr::xTypeId(pExprNode, pRootNode); }
  else if (  MinExpr::bIsOfType(pExprNode))  {   return MinExpr::xTypeId(pExprNode, pRootNode); }
  else if (  MaxExpr::bIsOfType(pExprNode))  {   return MaxExpr::xTypeId(pExprNode, pRootNode); }
  else if (  AvgExpr::bIsOfType(pExprNode))  {   return AvgExpr::xTypeId(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 AggregateExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Manual expression polymorphism here

  if      (CountExpr::bIsOfType(pExprNode))  { return CountExpr::iPrecision(pExprNode, pRootNode); }
  else if (  SumExpr::bIsOfType(pExprNode))  {   return SumExpr::iPrecision(pExprNode, pRootNode); }
  else if (  MinExpr::bIsOfType(pExprNode))  {   return MinExpr::iPrecision(pExprNode, pRootNode); }
  else if (  MaxExpr::bIsOfType(pExprNode))  {   return MaxExpr::iPrecision(pExprNode, pRootNode); }
  else if (  AvgExpr::bIsOfType(pExprNode))  {   return AvgExpr::iPrecision(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 AggregateExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Manual expression polymorphism here

  if      (CountExpr::bIsOfType(pExprNode))  { return CountExpr::iScale(pExprNode, pRootNode); }
  else if (  SumExpr::bIsOfType(pExprNode))  {   return SumExpr::iScale(pExprNode, pRootNode); }
  else if (  MinExpr::bIsOfType(pExprNode))  {   return MinExpr::iScale(pExprNode, pRootNode); }
  else if (  MaxExpr::bIsOfType(pExprNode))  {   return MaxExpr::iScale(pExprNode, pRootNode); }
  else if (  AvgExpr::bIsOfType(pExprNode))  {   return AvgExpr::iScale(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

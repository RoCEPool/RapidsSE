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
i8 AggregateExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (CountExpr::bIsOfType(pExprNode))  { return CountExpr::iType(pExprNode, pRootNode); }
  else if (  SumExpr::bIsOfType(pExprNode))  { return SumExpr::iType(pExprNode, pRootNode); }
  else if (  MinExpr::bIsOfType(pExprNode))  { return MinExpr::iType(pExprNode, pRootNode); }
  else if (  MaxExpr::bIsOfType(pExprNode))  { return MaxExpr::iType(pExprNode, pRootNode); }
  else if (  AvgExpr::bIsOfType(pExprNode))  { return AvgExpr::iType(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

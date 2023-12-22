/*============================================================================*/
/* ComparisonExpr.hpp - an abstract expression operator for comparing exprs   */
/*============================================================================*/

#include "BASE.h"
#include "ComparisonExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "EqExpr.hpp"
#include "NeExpr.hpp"
#include "GtExpr.hpp"
#include "GeExpr.hpp"
#include "LtExpr.hpp"
#include "LeExpr.hpp"
#include "InExpr.hpp"
#include "ExistsExpr.hpp"
#include "LikeExpr.hpp"
#include "BetweenExpr.hpp"
//#include "AnyExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COMPARISON_NAME_PREFIX = "ComparisonExpr"_Stz;
static Stz* C_COMPARISON_DEFN = "/__root__/definitions/object/expr/operator/comparison"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ComparisonExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (     EqExpr::bIsOfType(pNode))   {      EqExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     NeExpr::bIsOfType(pNode))   {      NeExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     GtExpr::bIsOfType(pNode))   {      GtExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     GeExpr::bIsOfType(pNode))   {      GeExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     LtExpr::bIsOfType(pNode))   {      LtExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     LeExpr::bIsOfType(pNode))   {      LeExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (     InExpr::bIsOfType(pNode))   {      InExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if ( ExistsExpr::bIsOfType(pNode))   {  ExistsExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (   LikeExpr::bIsOfType(pNode))   {    LikeExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (BetweenExpr::bIsOfType(pNode))   { BetweenExpr::vDescribe(pNode, sBuf, pRootNode); }
//else if (    AnyExpr::bIsOfType(pNode))   {     AnyExpr::vDescribe(pNode, sBuf, pRootNode);    }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void ComparisonExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (     EqExpr::bIsOfType(pNode))   {      EqExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (     NeExpr::bIsOfType(pNode))   {      NeExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (     GtExpr::bIsOfType(pNode))   {      GtExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (     GeExpr::bIsOfType(pNode))   {      GeExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (     LtExpr::bIsOfType(pNode))   {      LtExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (     LeExpr::bIsOfType(pNode))   {      LeExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (     InExpr::bIsOfType(pNode))   {      InExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if ( ExistsExpr::bIsOfType(pNode))   {  ExistsExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (   LikeExpr::bIsOfType(pNode))   {    LikeExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
  else if (BetweenExpr::bIsOfType(pNode))   { BetweenExpr::vDescribeAsTypes(pNode, sBuf, pRootNode); }
//else if (    AnyExpr::bIsOfType(pNode))   {     AnyExpr::vDescribeAsTypes(pNode, sBuf, pRootNode);    }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ComparisonExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COMPARISON_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ComparisonExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COMPARISON_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId ComparisonExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Manual expression polymorphism here

  if      (       EqExpr::bIsOfType(pExprNode))   { return      EqExpr::xTypeId(pExprNode, pRootNode); }
  else if (       NeExpr::bIsOfType(pExprNode))   { return      NeExpr::xTypeId(pExprNode, pRootNode); }
  else if (       GtExpr::bIsOfType(pExprNode))   { return      GtExpr::xTypeId(pExprNode, pRootNode); }
  else if (       GeExpr::bIsOfType(pExprNode))   { return      GeExpr::xTypeId(pExprNode, pRootNode); }
  else if (       LtExpr::bIsOfType(pExprNode))   { return      LtExpr::xTypeId(pExprNode, pRootNode); }
  else if (       LeExpr::bIsOfType(pExprNode))   { return      LeExpr::xTypeId(pExprNode, pRootNode); }
  else if (       InExpr::bIsOfType(pExprNode))   { return      InExpr::xTypeId(pExprNode, pRootNode); }
  else if (   ExistsExpr::bIsOfType(pExprNode))   { return  ExistsExpr::xTypeId(pExprNode, pRootNode); }
  else if (     LikeExpr::bIsOfType(pExprNode))   { return    LikeExpr::xTypeId(pExprNode, pRootNode); }
  else if (  BetweenExpr::bIsOfType(pExprNode))   { return BetweenExpr::xTypeId(pExprNode, pRootNode); }
//else if (      AnyExpr::bIsOfType(pExprNode))          { return     AnyExpr::xTypeId(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 ComparisonExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Manual expression polymorphism here

  if      (       EqExpr::bIsOfType(pExprNode))   { return      EqExpr::iPrecision(pExprNode, pRootNode); }
  else if (       NeExpr::bIsOfType(pExprNode))   { return      NeExpr::iPrecision(pExprNode, pRootNode); }
  else if (       GtExpr::bIsOfType(pExprNode))   { return      GtExpr::iPrecision(pExprNode, pRootNode); }
  else if (       GeExpr::bIsOfType(pExprNode))   { return      GeExpr::iPrecision(pExprNode, pRootNode); }
  else if (       LtExpr::bIsOfType(pExprNode))   { return      LtExpr::iPrecision(pExprNode, pRootNode); }
  else if (       LeExpr::bIsOfType(pExprNode))   { return      LeExpr::iPrecision(pExprNode, pRootNode); }
  else if (       InExpr::bIsOfType(pExprNode))   { return      InExpr::iPrecision(pExprNode, pRootNode); }
  else if (   ExistsExpr::bIsOfType(pExprNode))   { return  ExistsExpr::iPrecision(pExprNode, pRootNode); }
  else if (     LikeExpr::bIsOfType(pExprNode))   { return    LikeExpr::iPrecision(pExprNode, pRootNode); }
  else if (  BetweenExpr::bIsOfType(pExprNode))   { return BetweenExpr::iPrecision(pExprNode, pRootNode); }
//else if (      AnyExpr::bIsOfType(pExprNode))          { return     AnyExpr::iPrecision(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 ComparisonExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Manual expression polymorphism here

  if      (       EqExpr::bIsOfType(pExprNode))   { return      EqExpr::iScale(pExprNode, pRootNode); }
  else if (       NeExpr::bIsOfType(pExprNode))   { return      NeExpr::iScale(pExprNode, pRootNode); }
  else if (       GtExpr::bIsOfType(pExprNode))   { return      GtExpr::iScale(pExprNode, pRootNode); }
  else if (       GeExpr::bIsOfType(pExprNode))   { return      GeExpr::iScale(pExprNode, pRootNode); }
  else if (       LtExpr::bIsOfType(pExprNode))   { return      LtExpr::iScale(pExprNode, pRootNode); }
  else if (       LeExpr::bIsOfType(pExprNode))   { return      LeExpr::iScale(pExprNode, pRootNode); }
  else if (       InExpr::bIsOfType(pExprNode))   { return      InExpr::iScale(pExprNode, pRootNode); }
  else if (   ExistsExpr::bIsOfType(pExprNode))   { return  ExistsExpr::iScale(pExprNode, pRootNode); }
  else if (     LikeExpr::bIsOfType(pExprNode))   { return    LikeExpr::iScale(pExprNode, pRootNode); }
  else if (  BetweenExpr::bIsOfType(pExprNode))   { return BetweenExpr::iScale(pExprNode, pRootNode); }
//else if (      AnyExpr::bIsOfType(pExprNode))          { return     AnyExpr::iScale(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

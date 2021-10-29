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
//#include "BetweenExpr.hpp"
//#include "AnyExpr.hpp"
//#include "LikeExpr.hpp"

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
else if (   ExistsExpr::bIsOfType(pNode))   {  ExistsExpr::vDescribe(pNode, sBuf, pRootNode); }
//else if (BetweenExpr::bIsOfType(pNode))   { BetweenExpr::vDescribe(pNode, sBuf, pRootNode);    }
//else if (    AnyExpr::bIsOfType(pNode))   {     AnyExpr::vDescribe(pNode, sBuf, pRootNode);    }
//else if (   LikeExpr::bIsOfType(pNode))   {    LikeExpr::vDescribe(pNode, sBuf, pRootNode);    }
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
eCT ComparisonExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (       EqExpr::bIsOfType(pExprNode))   { return      EqExpr::eType(pExprNode, pRootNode); }
  else if (       NeExpr::bIsOfType(pExprNode))   { return      NeExpr::eType(pExprNode, pRootNode); }
  else if (       GtExpr::bIsOfType(pExprNode))   { return      GtExpr::eType(pExprNode, pRootNode); }
  else if (       GeExpr::bIsOfType(pExprNode))   { return      GeExpr::eType(pExprNode, pRootNode); }
  else if (       LtExpr::bIsOfType(pExprNode))   { return      LtExpr::eType(pExprNode, pRootNode); }
  else if (       LeExpr::bIsOfType(pExprNode))   { return      LeExpr::eType(pExprNode, pRootNode); }
  else if (       InExpr::bIsOfType(pExprNode))   { return      InExpr::eType(pExprNode, pRootNode); }
  else if (   ExistsExpr::bIsOfType(pExprNode))   { return  ExistsExpr::eType(pExprNode, pRootNode); }
//  else if (BetweenExpr::bIsOfType(pExprNode))   { return BetweenExpr::eType(pExprNode, pRootNode); }
//  else if (    AnyExpr::bIsOfType(pExprNode))   { return     AnyExpr::eType(pExprNode, pRootNode); }
//  else if (   LikeExpr::bIsOfType(pExprNode))   { return    LikeExpr::eType(pExprNode, pRootNode); }
  else NEVER_REACH 
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

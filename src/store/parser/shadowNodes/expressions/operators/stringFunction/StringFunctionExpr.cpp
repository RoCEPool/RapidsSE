/*============================================================================*/
/* StringFunctionExpr.hpp - an operator for string processing functions       */
/*============================================================================*/

#include "BASE.h"
#include "StringFunctionExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "SubstringExpr.hpp"
//#include "TrimExpr.hpp"
//#include "ConcatExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_MATHEMATICAL_NAME_PREFIX = "StringFunctionExpr"_Stz;
static Stz* C_MATHEMATICAL_DEFN = "/__root__/definitions/object/expr/operator/stringFunction"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void StringFunctionExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (SubstringExpr::bIsOfType(pNode))  {SubstringExpr::vDescribe(pNode, sBuf, pRootNode); }
//  else if (     TrimExpr::bIsOfType(pNode))  {     TrimExpr::vDescribe(pNode, sBuf, pRootNode); }
//  else if (   ConcatExpr::bIsOfType(pNode))  {   ConcatExpr::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 StringFunctionExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MATHEMATICAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StringFunctionExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MATHEMATICAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT StringFunctionExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // manual expression polymorphism here

  if      (SubstringExpr::bIsOfType(pExprNode))  { return SubstringExpr::eType(pExprNode, pRootNode); }
//  else if (     TrimExpr::bIsOfType(pExprNode))  { return      TrimExpr::eType(pExprNode, pRootNode); }
//  else if (   ConcatExpr::bIsOfType(pExprNode))  { return    ConcatExpr::eType(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* MathematicalExpr.hpp - an mathematical expression operator                 */
/*============================================================================*/

#include "BASE.h"
#include "MathematicalExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "AdditionExpr.hpp"
#include "SubtractionExpr.hpp"
#include "MultiplicationExpr.hpp"
#include "DivisionExpr.hpp"
#include "ModulusExpr.hpp"
#include "NegationExpr.hpp"
#include "ExtractExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_MATHEMATICAL_NAME_PREFIX = "MathematicalExpr"_Stz;
static Stz* C_MATHEMATICAL_DEFN = "/__root__/definitions/object/expr/operator/mathematical"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MathematicalExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (      AdditionExpr::bIsOfType(pNode))  {       AdditionExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (   SubtractionExpr::bIsOfType(pNode))  {    SubtractionExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (MultiplicationExpr::bIsOfType(pNode))  { MultiplicationExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (      DivisionExpr::bIsOfType(pNode))  {       DivisionExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (       ModulusExpr::bIsOfType(pNode))  {        ModulusExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (      NegationExpr::bIsOfType(pNode))  {       NegationExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (       ExtractExpr::bIsOfType(pNode))  {          ExtractExpr::vDescribe(pNode, sBuf, pRootNode);    }
//  else if (       PowerExpr::bIsOfType(pNode))  {          PowerExpr::vDescribe(pNode, sBuf, pRootNode);    }
//  else if (         LogExpr::bIsOfType(pNode))  {            LogExpr::vDescribe(pNode, sBuf, pRootNode);    }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 MathematicalExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MATHEMATICAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MathematicalExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MATHEMATICAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
i8 MathematicalExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (      AdditionExpr::bIsOfType(pExprNode))  { return AdditionExpr::iType(pExprNode, pRootNode); }
  else if (   SubtractionExpr::bIsOfType(pExprNode))  { return SubtractionExpr::iType(pExprNode, pRootNode); }
  else if (MultiplicationExpr::bIsOfType(pExprNode))  { return MultiplicationExpr::iType(pExprNode, pRootNode); }
  else if (      DivisionExpr::bIsOfType(pExprNode))  { return DivisionExpr::iType(pExprNode, pRootNode); }
  else if (       ModulusExpr::bIsOfType(pExprNode))  { return ModulusExpr::iType(pExprNode, pRootNode); }
  else if (      NegationExpr::bIsOfType(pExprNode))  { return NegationExpr::iType(pExprNode, pRootNode); }
  else if (       ExtractExpr::bIsOfType(pExprNode))  { return ExtractExpr::iType(pExprNode, pRootNode); }
//  else if (       PowerExpr::bIsOfType(pExprNode))  { return          PowerExpr::eType(pExprNode, pRootNode); }
//  else if (         LogExpr::bIsOfType(pExprNode))  { return            LogExpr::eType(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

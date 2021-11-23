/*============================================================================*/
/* MiscExpr.hpp - an operator for expressions that don't fit anywhere else    */
/*============================================================================*/

#include "BASE.h"
#include "MiscExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "CastExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "MiscExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/misc"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MiscExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (CastExpr::bIsOfType(pNode))  {CastExpr::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 MiscExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MiscExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT MiscExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // manual expression polymorphism here

  if      (CastExpr::bIsOfType(pExprNode))  { return CastExpr::eType(pExprNode, pRootNode); }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

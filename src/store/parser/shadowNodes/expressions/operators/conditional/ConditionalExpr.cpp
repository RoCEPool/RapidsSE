/*============================================================================*/
/* ConditionalExpr.hpp - an abstract operator for conditional logic exprs     */
/*============================================================================*/

#include "BASE.h"
#include "ConditionalExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "CaseExpr.hpp"
#include "WhenExpr.hpp"
#include "ElseExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "ConditionalExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/conditional"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ConditionalExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (CaseExpr::bIsOfType(pNode))    { CaseExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (WhenExpr::bIsOfType(pNode))    { WhenExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (ElseExpr::bIsOfType(pNode))    { ElseExpr::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ConditionalExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ConditionalExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
i8 ConditionalExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
      ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (CaseExpr::bIsOfType(pExprNode))    { return CaseExpr::iType(pExprNode, pRootNode);  }
  else if (WhenExpr::bIsOfType(pExprNode))    { return WhenExpr::iType(pExprNode, pRootNode);  }
  else if (ElseExpr::bIsOfType(pExprNode))    { return ElseExpr::iType(pExprNode, pRootNode);  }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

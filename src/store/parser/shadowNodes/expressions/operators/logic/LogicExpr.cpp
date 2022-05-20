/*============================================================================*/
/* LogicExpr.hpp - an abstract expression operator for boolean logic exprs    */
/*============================================================================*/

#include "BASE.h"
#include "LogicExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "AndExpr.hpp"
#include "OrExpr.hpp"
#include "XorExpr.hpp"
#include "NotExpr.hpp"
#include "IsNullExpr.hpp"
//#include "LikeExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LOGIC_NAME_PREFIX = "LogicExpr"_Stz;
static Stz* C_LOGIC_DEFN = "/__root__/definitions/object/expr/operator/logic"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LogicExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (   AndExpr::bIsOfType(pNode))    {    AndExpr::vDescribe(pNode, sBuf, pRootNode);  }
  else if (    OrExpr::bIsOfType(pNode))    {     OrExpr::vDescribe(pNode, sBuf, pRootNode);  }
  else if (   XorExpr::bIsOfType(pNode))    {    XorExpr::vDescribe(pNode, sBuf, pRootNode);  }
  else if (   NotExpr::bIsOfType(pNode))    {    NotExpr::vDescribe(pNode, sBuf, pRootNode);  }
  else if (IsNullExpr::bIsOfType(pNode))    { IsNullExpr::vDescribe(pNode, sBuf, pRootNode);  }
//  else if (LikeExpr::bIsOfType(pNode))    {   LikeExpr::vDescribe(pNode, sBuf, pRootNode);     }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LogicExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LOGIC_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LogicExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LOGIC_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId LogicExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
      ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (   AndExpr::bIsOfType(pExprNode))    { return    AndExpr::xTypeId(pExprNode, pRootNode);  }
  else if (    OrExpr::bIsOfType(pExprNode))    { return     OrExpr::xTypeId(pExprNode, pRootNode);  }
  else if (   XorExpr::bIsOfType(pExprNode))    { return    XorExpr::xTypeId(pExprNode, pRootNode);  }
  else if (   NotExpr::bIsOfType(pExprNode))    { return    NotExpr::xTypeId(pExprNode, pRootNode);  }
  else if (IsNullExpr::bIsOfType(pExprNode))    { return IsNullExpr::xTypeId(pExprNode, pRootNode);  }
//else if (LikeExpr::bIsOfType(pExprNode))             { return   LikeExpr::eType(pExprNode, pRootNode);           }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

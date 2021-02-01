/*============================================================================*/
/* IsNullExpr.hpp - a logical IsNull expression operator                      */
/*============================================================================*/

#include "BASE.h"
#include "IsNullExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ISNULL_NAME_PREFIX = "IsNullExpr"_Stz;
static Stz* C_ISNULL_DEFN = "/__root__/definitions/object/expr/operator/logic/isnull"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* IsNullExpr::pCreate(QueryNode* pExpr1)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_ISNULL_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, C_ISNULL_DEFN);
  pExpr->vAppendChild(pExpr1);

  return pExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void IsNullExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = IsNullExpr::pChild1(static_cast<QueryNode*>(pNode));

  if (pChild1->nChildren() == 0) {
    Expr::vDescribe(pChild1, sBuf);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" IS NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* IsNullExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return static_cast<QueryNode*>(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 IsNullExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ISNULL_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

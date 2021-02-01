/*============================================================================*/
/* NotExpr.hpp - a logical NOT expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "NotExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NOT_NAME_PREFIX = "NotExpr"_Stz;
static Stz* C_NOT_DEFN = "/__root__/definitions/object/expr/operator/logic/not"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* NotExpr::pCreate(QueryNode* pExpr1)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_NOT_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, C_NOT_DEFN);
  pExpr->vAppendChild(pExpr1);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void NotExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = NotExpr::pChild1(static_cast<QueryNode*>(pNode));

  sBuf.vConcat("NOT "_Stz);

  if (pChild1->nChildren() == 0) {
    Expr::vDescribe(pChild1, sBuf);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* NotExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return static_cast<QueryNode*>(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 NotExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NOT_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* XorExpr.hpp - a logical XOR expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "XorExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_XOR_NAME_PREFIX = "XorExpr"_Stz;
static Stz* C_XOR_DEFN = "/__root__/definitions/object/expr/operator/logic/xor"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* XorExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_XOR_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, C_XOR_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void XorExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = XorExpr::pChild1(static_cast<QueryNode*>(pNode));
  QueryNode* pChild2 = XorExpr::pChild2(static_cast<QueryNode*>(pNode));

  if (pChild1->nChildren() == 0) {
    Expr::vDescribe(pChild1, sBuf);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" XOR "_Stz);

  if (pChild2->nChildren() == 0) {
    Expr::vDescribe(pChild2, sBuf);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild2, sBuf);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* XorExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return static_cast<QueryNode*>(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* XorExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return static_cast<QueryNode*>(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 XorExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_XOR_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

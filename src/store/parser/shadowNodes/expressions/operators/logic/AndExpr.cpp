/*============================================================================*/
/* AndExpr.hpp - a logical AND expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "AndExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_AND_NAME_PREFIX = "AndExpr"_Stz;
static Stz* C_AND_DEFN = "/__root__/definitions/object/expr/operator/logic/and"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* AndExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_AND_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, C_AND_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AndExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = AndExpr::pChild1(static_cast<QueryNode*>(pNode));
  QueryNode* pChild2 = AndExpr::pChild2(static_cast<QueryNode*>(pNode));

  if (pChild1->nChildren() == 0) {
    Expr::vDescribe(pChild1, sBuf);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" AND "_Stz);

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
QueryNode* AndExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return static_cast<QueryNode*>(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* AndExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return static_cast<QueryNode*>(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 AndExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_AND_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

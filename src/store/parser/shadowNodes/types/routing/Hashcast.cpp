/*============================================================================*/
/* Hashcast.hpp - Send data to a single endpoint based on some hashable expr  */
/*============================================================================*/

#include "BASE.h"
#include "Hashcast.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "GraphNav.hpp"

#include "ExprList.hpp"
#include "Expr.hpp"
#include "PlanObject.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_HASHCAST_DEFN = "/__root__/definitions/types/routing/hashcast"_Stz;

static Stz* C_HASHKEYS_PROP = "hashKeys"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Hashcast::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_HASHCAST_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) RT_HASHCAST);

  QueryNode* pNode = new QueryNode(sName, C_HASHCAST_DEFN, static_cast<Value&&>(xValue));

  // Add the property which is a list of expressions to hash to determine
  // the destination partition.
  QueryNode* pHashKeys = ExprList::pCreate(C_HASHKEYS_PROP);
  pNode->vAppendProperty(pHashKeys);

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Hashcast::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  sBuf.vConcat(Routing::szRoutingType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Hashcast::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_HASHCAST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Hashcast::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_HASHCAST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Adds an expression to the list of expressions to hash for destination ptn  */
/*----------------------------------------------------------------------------*/
void Parser::Hashcast::vAddHashExpr(QueryNode* pNode, QueryNode* pExpr)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));

  QueryNode* pHashKeys = _pQueryNode(GraphNav::pFindProperty(C_HASHKEYS_PROP, pNode));
  ASSERT(pHashKeys);
  ASSERT(ExprList::bIsOfType(pHashKeys));

  pHashKeys->vAppendChild(pExpr);
}

/*----------------------------------------------------------------------------*/
/* Returns the internal node that represents a list of hashable expressions   */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Hashcast::pHashExprList(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode *pHashExprs =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_HASHKEYS_PROP), pNode));

  return pHashExprs;
}

/*----------------------------------------------------------------------------*/
/* Returns a copy of the node that represents a list of hashable expressions  */
/* where each expression is in ordinal form. node must be freed after use.    */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Hashcast::pOrdinalHashExprList(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  // Get the symbolic list of hashable expressions
  QueryNode *pSymbolicExprs = pHashExprList(pNode);

  // Find the parent PlanObject that contains this Routing object.
  // First get the path of pNode in the plan.
  s2K sPathBuf;
  i8 iDepth = 0;
  b1 bFound = GraphNav::bGetPathAndDepth(pNode, pRootNode, sPathBuf, iDepth, true, true);

  // Now search up this path for the first PlanObject instance
  QueryNode* pPlanObj = nullptr;
  while (GraphNav::bParentPath(sPathBuf)) {
    QueryNode* pParent = _pQueryNode(GraphNav::pFind(sPathBuf, pRootNode));

    if (PlanObject::bIsOfType(pParent)) {
      pPlanObj = pParent;
      break;
    }
  }

  ASSERT(pPlanObj);

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pPlanObj->nChildren() == 1);
  QueryNodeList* pColExprList = PlanObject::pColumnExprs(_pQueryNode(pPlanObj->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pPlanObj, pRootNode);

  // Create a container node to hold the ordinal expressions, but without any children
  QueryNode* pOrdinalExprs = pSymbolicExprs->pDeepCopy(true, new PlanNodeList());

  // Iterate over each of the expression children of pSymbolicExprs
  PlanNodeIter* pIter = pSymbolicExprs->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pSymExpr = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pSymExpr));

    // Convert pSymExpr from a symbolic representation to an ordinal representation
    QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pSymExpr, pColExprList, pCorrelatedExprs, pRootNode);
    pOrdinalExprs->vAppendChild(pOrdExpr);
  }

  delete pIter;

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pOrdinalExprs;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

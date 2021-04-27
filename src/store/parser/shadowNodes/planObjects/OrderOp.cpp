/*============================================================================*/
/* OrderOp.hpp - operator that reorders rows based on a list of expressions   */
/*============================================================================*/

#include "BASE.h"
#include "OrderOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "store/parser/shadowNodes/expressions/Expr.hpp"
#include "OrderExprList.hpp"
#include "OrderExpr.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ORDEROP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/orderOp"_Stz;

static Stz* C_ORDEREXPRLIST_PROP = "orderExprList"_Stz;
static Stz* C_ORDEREXPRLIST_DEFN = "/__root__/definitions/object/list/orderExprList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create OrderOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pCreate(QueryNode* pOrderExprList)
{
  ASSERT(pOrderExprList);
  ASSERT(OrderExprList::bIsOfType(pOrderExprList));

  // Create a unique name for this OrderOp object
  s256 sName;
  NodeNamer::vGenerateName(C_ORDEROP_DEFN, sName);

  // Create the object and add its orderExprList property
  QueryNode* pOrderOp = new QueryNode(sName, C_ORDEROP_DEFN);

  pOrderExprList->vSetName(C_ORDEREXPRLIST_PROP);
  pOrderOp->vAppendProperty(pOrderExprList);

  return pOrderOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void OrderOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pOrderOp = _pQueryNode(pNode);

  sBuf.vConcat("OrderOp :\n  "_Stz);

  QueryNode* pOrderExprList = _pQueryNode(GraphNav::pFindProperty(C_ORDEREXPRLIST_PROP, pOrderOp));
  ASSERT(pOrderExprList);

  Expr::vDescribe(pOrderExprList, sBuf);
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void OrderOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the list of ordering expressions expression
  PlanNode* pOrderExprList = GraphNav::pFindProperty(const_cast<Stz*>(C_ORDEREXPRLIST_PROP), pNode);
  ASSERT(pOrderExprList);

  s2K sExprBuf;
  OrderExprList::vDescribe(pOrderExprList, sExprBuf);

  sBuf.vConcat("\n"_Stz);
  vAppendIndent(sBuf, iIndent, IF_PLAIN);
  sBuf.vCatSprint("order expr: (%s)"_Stz, sExprBuf.szStz());

  QueryNode* pOrdOrderExprList = pOrdinalOrderExprList(_pQueryNode(pNode), pRootNode);
  sExprBuf.vReset();
  OrderExprList::vDescribe(pOrdOrderExprList, sExprBuf);
  sBuf.vConcat("\n"_Stz);
  vAppendIndent(sBuf, iIndent, IF_PLAIN);
  sBuf.vCatSprint("ordinal order expr: (%s)"_Stz, sExprBuf.szStz());
  delete pOrdOrderExprList;

  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, sBuf, iIndent + 1);
    }
    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 OrderOp::bIsOfType(PlanNode *pNode)
{
  _static_
  return ShadowNode::bIsOfType(pNode, C_ORDEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrderOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ORDEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the orderExprList node of the OrderOp                              */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pOrderExprList(QueryNode* pNode)
{
  _static_
  QueryNode *pOrderExprListNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ORDEREXPRLIST_PROP), pNode));

  ASSERT(pOrderExprListNode);
  return pOrderExprListNode;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of the order expr list relative to column ordinals.     */
/* Must be freed after use.                                                   */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pOrdinalOrderExprList(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the symbolic order expression list
  QueryNode *pSymbolicList = pOrderExprList(pNode);
  ASSERT(pSymbolicList);

  // Create an empty list of ordinal orderExprs
  QueryNode* pOrdinalList = OrderExprList::pCreate();

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pNode->nChildren() == 1);
  QueryNodeList* pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Iterate each ordering expression and extract out the expression component
  PlanNodeIter* pIter = pSymbolicList->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pSymOrderExpr = _pQueryNode(pIter->pNext());
    ASSERT(OrderExpr::bIsOfType(pSymOrderExpr));

    // pSymOrderExpr is a compound object. Extract out the expr out of pSymOrderExpr
    QueryNode* pSymExpr = OrderExpr::pExprNode(pSymOrderExpr);
    ASSERT(Expr::bIsOfType(pSymExpr));

    b1 bIsAscending = OrderExpr::bIsAscending(pSymOrderExpr);

    // Use the column expression list and the symbolic expression to
    // generate an ordinal  expression (one that is relative to column
    // ordinals only).
    QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pSymExpr, pColExprList, pRootNode);
    ASSERT(pOrdinalExpr);

    // Now we need to create an ordinal version of the OrderExpr object and add
    // it to the pOrdinalList.
    QueryNode* pOrdinalOrderExpr = OrderExpr::pCreate(pOrdinalExpr, bIsAscending);
    pOrdinalList->vAppendChild(pOrdinalOrderExpr);
  }

  pColExprList->vDeleteAll();
  delete pColExprList;
  delete pIter;

  return pOrdinalList;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* OrderOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // The OrderOp doesn't change the projected expressions, so ask its child.
  QueryNode* pChild = _pQueryNode(pNode->pFirstChild());
  ASSERT(pChild);

  return PlanObject::pColumnExprs(pChild, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

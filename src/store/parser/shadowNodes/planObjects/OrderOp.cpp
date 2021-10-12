/*============================================================================*/
/* OrderOp.hpp - operator that reorders rows based on a list of expressions   */
/*============================================================================*/

#include "BASE.h"
#include "OrderOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "store/parser/shadowNodes/expressions/Expr.hpp"
#include "OrderItemList.hpp"
#include "OrderItem.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "MetadataNode.hpp"
#include "ShadowRoot.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ORDEROP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/orderOp"_Stz;

static Stz* C_ORDERITEMLIST_PROP = "orderItemList"_Stz;
static Stz* C_ORDERITEMLIST_DEFN = "/__root__/definitions/object/list/orderItemList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create OrderOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pCreate(QueryNode* pOrderItemList)
{
  ASSERT(pOrderItemList);
  ASSERT(OrderItemList::bIsOfType(pOrderItemList));

  // Create a unique name for this OrderOp object
  s256 sName;
  NodeNamer::vGenerateName(C_ORDEROP_DEFN, sName);

  // Create the object and add its orderItemList property
  QueryNode* pOrderOp = new QueryNode(sName, C_ORDEROP_DEFN);

  pOrderItemList->vSetName(C_ORDERITEMLIST_PROP);
  pOrderOp->vAppendProperty(pOrderItemList);

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

  QueryNode* pOrderItemList = _pQueryNode(GraphNav::pFindProperty(C_ORDERITEMLIST_PROP, pOrderOp));
  ASSERT(pOrderItemList);

  Expr::vDescribe(pOrderItemList, sBuf);
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void OrderOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }
    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void OrderOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    // Get the list of ordering expressions expression
    PlanNode* pOrderItemList = GraphNav::pFindProperty(const_cast<Stz*>(C_ORDERITEMLIST_PROP), pNode);
    ASSERT(pOrderItemList);

    s2K sExprBuf;
    OrderItemList::vDescribe(pOrderItemList, sExprBuf);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("order expr: (%s)"_Stz, sExprBuf.szStz());

    // Explaining very early plans are difficult because we cannot resolve the column expressions.
    // We don't want to retrieve any ordinal expressions below before we have been able
    // to resolve all table/column names in the system.
    // When we resolve names we also add metadata into plan. So if the metadata node has
    // no children then we probably haven't resolved any names yet (except for the case of
    // the plan only containing literals).

    if (MetadataNode::bHasMetadata(ShadowRoot::pMetadataNode(pRootNode))) {
      QueryNode* pOrdOrderItemList = pOrdinalOrderItemList(_pQueryNode(pNode), pRootNode);
      sExprBuf.vReset();
      OrderItemList::vDescribe(pOrdOrderItemList, sExprBuf);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("ordinal order expr: (%s)"_Stz, sExprBuf.szStz());
      delete pOrdOrderItemList;
    }

    return;
  }

  // Manual polymorphism here
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
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 OrderOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_ORDEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrderOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ORDEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrderOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_ORDEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the orderItemList node of the OrderOp                              */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pOrderItemList(QueryNode* pNode)
{
  _static_
  QueryNode *pOrderItemListNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ORDERITEMLIST_PROP), pNode));

  ASSERT(pOrderItemListNode);
  return pOrderItemListNode;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of the order expr list relative to column ordinals.     */
/* Must be freed after use.                                                   */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pOrdinalOrderItemList(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the symbolic order expression list
  QueryNode *pSymbolicList = pOrderItemList(pNode);
  ASSERT(pSymbolicList);

  // Create an empty list of ordinal orderItems
  QueryNode* pOrdinalList = OrderItemList::pCreate();

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pNode->nChildren() == 1);
  QueryNodeList* pColExprList = pColumnExprs(_pQueryNode(pNode), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pRootNode);

  // Iterate each ordering expression and extract out the expression component
  PlanNodeIter* pIter = pSymbolicList->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pSymOrderItem = _pQueryNode(pIter->pNext());
    ASSERT(OrderItem::bIsOfType(pSymOrderItem));

    // pSymOrderItem is a compound object. Extract out the expr out of pSymOrderItem
    QueryNode* pSymExpr = OrderItem::pExprNode(pSymOrderItem);
    ASSERT(Expr::bIsOfType(pSymExpr));

    b1 bIsAscending = OrderItem::bIsAscending(pSymOrderItem);

    // Use the column expression list and the symbolic expression to
    // generate an ordinal  expression (one that is relative to column
    // ordinals only).
    QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pSymExpr, pColExprList, pCorrelatedExprs, pRootNode);
    ASSERT(pOrdinalExpr);

    // Now we need to create an ordinal version of the OrderItem object and add
    // it to the pOrdinalList.
    QueryNode* pOrdinalOrderItem = OrderItem::pCreate(pOrdinalExpr, bIsAscending);
    pOrdinalList->vAppendChild(pOrdinalOrderItem);
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

/*----------------------------------------------------------------------------*/
/* Returns a new OrderOp node with ordering terms super-aggregating           */
/* aggregated expressions from each partition or node                         */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pCreateSuperAggregate(QueryNode* pSinglePartitionNode)
{
  ASSERT(OrderOp::bIsOfType(pSinglePartitionNode));
  ASSERT(!PlanObject::bNodeHasLocation(pSinglePartitionNode));

  QueryNode* pNewOp = pSinglePartitionNode->pDeepCopy(true, nullptr);

  QueryNode* pOrderItems =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ORDERITEMLIST_PROP), pNewOp));
  ASSERT(pOrderItems);

  // Iterate all of the order items
  PlanNodeIter* pIter = pOrderItems->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pOrderItem = _pQueryNode(pIter->pNext());

    ASSERT(OrderItem::bIsOfType(pOrderItem));

    // Get the ordering expression and super-aggregate any aggregates in it
    QueryNode* pOrderExpr = OrderItem::pExprNode(pOrderItem);
    Expr::vSuperAggregate(pOrderExpr);
  }

  delete pIter;
  return pNewOp;
}

/*----------------------------------------------------------------------------*/
/* Returns a new OrderOp node with ordering terms super-aggregating           */
/* aggregated expressions from each partition or node. This version used to   */
/* factor in node-level aggregation.                                          */
/*----------------------------------------------------------------------------*/
QueryNode* OrderOp::pCreateDoubleSuperAggregate(QueryNode* pSinglePartitionNode)
{
  ASSERT(OrderOp::bIsOfType(pSinglePartitionNode));
  ASSERT(!PlanObject::bNodeHasLocation(pSinglePartitionNode));

  QueryNode* pNewOp = pSinglePartitionNode->pDeepCopy(true, nullptr);

  QueryNode* pOrderItems =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ORDERITEMLIST_PROP), pNewOp));
  ASSERT(pOrderItems);

  // Iterate all of the order items
  PlanNodeIter* pIter = pOrderItems->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pOrderItem = _pQueryNode(pIter->pNext());

    ASSERT(OrderItem::bIsOfType(pOrderItem));

    // Get the ordering expression and super-aggregate any aggregates in it
    QueryNode* pOrderExpr = OrderItem::pExprNode(pOrderItem);
    Expr::vSuperAggregate(pOrderExpr);
    Expr::vSuperAggregate(pOrderExpr);
  }

  delete pIter;
  return pNewOp;
}

/*----------------------------------------------------------------------------*/
/* Returns a new OrderOp node with ordering terms super-aggregating           */
/* aggregated expressions from each partition or node                         */
/*----------------------------------------------------------------------------*/
b1 OrderOp::bContainsAnAggregate(QueryNode* pNode)
{
  ASSERT(OrderOp::bIsOfType(pNode));

  b1 bResult = false;

  QueryNode* pOrderItems =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ORDERITEMLIST_PROP), pNode));
  ASSERT(pOrderItems);

  // Iterate all of the order items
  PlanNodeIter* pIter = pOrderItems->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pOrderItem = _pQueryNode(pIter->pNext());

    ASSERT(OrderItem::bIsOfType(pOrderItem));

    // Get the ordering expression and super-aggregate any aggregates in it
    QueryNode* pOrderExpr = OrderItem::pExprNode(pOrderItem);

    if (Expr::bContainsAnAggregate(pOrderExpr)) {
      bResult = true;
      break;
    }
  }

  delete pIter;
  return bResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

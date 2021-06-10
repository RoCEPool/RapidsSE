/*============================================================================*/
/* ProjValidation.cpp - Validates that all expressions only use the projected */
/* columns from SQL operators below it.                                       */
/*============================================================================*/

#include "BASE.h"
#include "ProjValidation.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectOp.hpp"
#include "FilterOp.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "OrderItemList.hpp"
#include "Expr.hpp"
#include "ShadowPlan.hpp"
#include "ShadowRoot.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private classes/structs -------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* Validates that the right projected columns will be delivered to each       */
/* SQL operator that has expressions that use or build on those projected     */
/* columns. i.e. each expression in a SQL operator can be rewritten in        */
/* an ordinal form relative to the input projected columns.                   */
/*----------------------------------------------------------------------------*/
void ProjValidation::vValidateExprVisibility(PlanNode* pRootNode)
{
  // Walk every SQL operator in the graph.
  // If the SQL operator has expressions then validate that each expression
  // can be converted to an ordinal format and that it is fully ordinal.
  // If not, throw and error about an expression not being visible.


  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);

  // The plan node should have one child which is a StatementMarker node
  ASSERT(pPlanNode->nChildren() == 1);
  ASSERT(StatementMarker::bIsOfType(pPlanNode->pFirstChild()));

  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    // We are only interested in Sql operators that have expressions.
    // So no need to look at any of the following:
    //   ExplainOp, LimitOp, SetOp, UnionOp, etc

    if (FilterOp::bIsOfType(pNode)) {
      vValidateFilterOp(_pQueryNode(pNode), pRootNode);

    } else if (OrderOp::bIsOfType(pNode)) {
      vValidateOrderOp(_pQueryNode(pNode), pRootNode);

    } else if (ProjectOp::bIsOfType(pNode)) {
      // Covers Projects and AggregateOps
      vValidateProjectOp(_pQueryNode(pNode), pRootNode);
    }

    return GraphNav::WALK_CONTINUE;
  };

  // We do a bottom up walk because we want to catch errors at their incarnation,
  // not are a higher level where it has potentially compounded a few times.
  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validates that the expression in a FilterOp is fully ordinal               */
/* (can be converted to an ordinal representation with no ColumnRefs)         */
/*----------------------------------------------------------------------------*/
void ProjValidation::vValidateFilterOp(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(FilterOp::bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Extract out the expression in ordinal form.
  QueryNode* pOrdinalExpr = FilterOp::pOrdinalFilterExpr(pNode, pRootNode);

  if (!Expr::bIsFullyOrdinal(pOrdinalExpr)) {
    // This expression contains ColumnRefs.
    // Find the part of this that is non-ordinal
    QueryNode* pNonOrdinalExpr = Expr::pLargestNonOrdinalSubExpr(pOrdinalExpr);
    ASSERT(pNonOrdinalExpr);

    s2K sBuf;
    Expr::vDescribe(pNonOrdinalExpr, sBuf);
    delete pOrdinalExpr;

    SemanticException::vThrow(EC_EXPR_NOT_PROJECTED, ": %s ; WHERE/HAVING clause"_Stz, sBuf.szStz());
  }

  // All good
  delete pOrdinalExpr;
}

/*----------------------------------------------------------------------------*/
/* Validates that the expression in a OrderOp is fully ordinal                */
/* (can be converted to an ordinal representation with no ColumnRefs)         */
/*----------------------------------------------------------------------------*/
void ProjValidation::vValidateOrderOp(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(OrderOp::bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Extract out the expression in ordinal form.
  QueryNode* pOrdinalOrderList = OrderOp::pOrdinalOrderItemList(pNode, pRootNode);

  PlanNodeIter* pIter = pOrdinalOrderList->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pOrdinalOrderItem = _pQueryNode(pIter->pNext());
    ASSERT(OrderItem::bIsOfType(pOrdinalOrderItem));

    QueryNode* pOrdinalExpr = OrderItem::pExprNode(pOrdinalOrderItem);
    ASSERT(Expr::bIsOfType(pOrdinalExpr));

    if (!Expr::bIsFullyOrdinal(pOrdinalExpr)) {
      // This expression contains ColumnRefs.
      // Find the part of this that is non-ordinal
      QueryNode* pNonOrdinalExpr = Expr::pLargestNonOrdinalSubExpr(pOrdinalExpr);
      ASSERT(pNonOrdinalExpr);

      s2K sBuf;
      Expr::vDescribe(pNonOrdinalExpr, sBuf);
      delete pIter;
      delete pOrdinalOrderList;

      SemanticException::vThrow(EC_EXPR_NOT_PROJECTED, ": %s ; ORDER BY clause"_Stz, sBuf.szStz());
    }
  }

  // All good
  delete pIter;
  delete pOrdinalOrderList;
}

/*----------------------------------------------------------------------------*/
/* Validates that the expression in a ProjectOp is fully ordinal              */
/* (can be converted to an ordinal representation with no ColumnRefs)         */
/*----------------------------------------------------------------------------*/
void ProjValidation::vValidateProjectOp(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(ProjectOp::bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Extract out the expression in ordinal form.
  QueryNode* pOrdinalProjectedColumns = ProjectOp::pOrdinalProjectedColumns(pNode, pRootNode);

  PlanNodeIter* pIter = pOrdinalProjectedColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pOrdinalProjectedColumn = _pQueryNode(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pOrdinalProjectedColumn));

    QueryNode* pOrdinalExpr = ProjectedColumn::pExprNode(pOrdinalProjectedColumn);
    ASSERT(Expr::bIsOfType(pOrdinalExpr));

    if (!Expr::bIsFullyOrdinal(pOrdinalExpr)) {
      // This expression contains ColumnRefs.
      // Find the part of this that is non-ordinal
      QueryNode* pNonOrdinalExpr = Expr::pLargestNonOrdinalSubExpr(pOrdinalExpr);
      ASSERT(pNonOrdinalExpr);

      s2K sBuf;
      Expr::vDescribe(pNonOrdinalExpr, sBuf);
      delete pIter;
      delete pOrdinalProjectedColumns;

      SemanticException::vThrow(EC_EXPR_NOT_PROJECTED, ": %s"_Stz, sBuf.szStz());
    }
  }

  // All good
  delete pIter;
  delete pOrdinalProjectedColumns;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* ExprPushdown.hpp - Pushes down expressions into lower ProjectOps as needed */
/*============================================================================*/

#include "BASE.h"
#include "ExprPushdown.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectOp.hpp"
#include "AggregateOp.hpp"
#include "FilterOp.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "OrderItemList.hpp"
#include "Expr.hpp"
#include "ProjectedColumns.hpp"
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
/* Pushes down any intermediate/working expressions that are not being        */
/* output into a lower ProjectOp/AggregateOp.                                 */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vPushdownExprs(PlanNode* pRootNode)
{
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  PlanNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsOfType(pPlanRoot));

  // Visit every statement section
  auto fStmtMarkerVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    vStatementSectionPushdownExprs(pStatementMarker, pRootNode);

    return GraphNav::WALK_CONTINUE;
  };


  // Walk each node and look for the StatementMarkers. We will start from the
  // bottom up so that we fix lower level dependencies before we deal with
  // nodes higher up the graph.
  GraphNav::bGlobalWalkBottomUp(pPlanRoot,
                                false,
                                true,
                                fStmtMarkerVisitor,
                                pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Pushes down any intermediate/working expressions that are not being        */
/* output into a lower ProjectOp/AggregateOp, for the current statement       */
/* section.                                                                   */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vStatementSectionPushdownExprs(QueryNode* pStatementMarker, PlanNode* pRootNode)
{
  ASSERT(pStatementMarker);
  ASSERT(StatementMarker::bIsOfType(pStatementMarker));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // For each StatementMarker section
  //   Walk each node.
  //   If it is not a FilterOp, OrderOp or AggregateOp then keep walking.
  //   Find the ProjectOp/AggregateOp below it in this StatementMarker section, if any.
  //   If none, then keep walking.
  //   Otherwise extract the expression from the FilterOp/OrderOp or the AggregteOp's
  //   group by expr.
  //   Convert the expression to ordinal form. Determine if it is fully ordinal
  //   and has no aggregates. If not fully ordinal, or if it has aggregates then
  //   determine the largest non-ordinal/non-aggregate subexpression and add this
  //   to the projected columns list of the ProjectOp identified above.

  // Notes:
  //   We could either push down the whole expression or the largest non-ordinal
  //   / non-aggregate subexpression. In this case we choose the latter. The rationale
  //   being that there may be a base part of the expression that is already being
  //   computed and may be expensive to recalculate it.
  //   e.g. assume that we have the expression (AVG(col2) / col1 > 0). Lets say
  //   that AVG(col2) is already being projected. We can either:
  //     1. Project col1 and let (AVG(col2) / col1 > 0) be calculated in the
  //        SQL operator upstream as ($1 / $2 > 0); or
  //     2. Project the entire expression (AVG(col2) / col1 > 0) so that the
  //        upstream SQL operator just has to evaluate the boolean result.
  //   In this example the AVG(col2) is probably an expensive operation so we
  //   don't want to calculate and project this twice. So let's only pushdown
  //   the largest non-ordinal subset that is required.

  // Walk each node in this statement marker section
  auto fStmtSectionVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    // TODO: Add any other SQL operators here that contain expressions
    if (FilterOp::bIsOfType(pNode)) {

      // Get the filter expression in ordinal representation
      QueryNode* pOrdExpr = FilterOp::pOrdinalFilterExpr(_pQueryNode(pNode), pRootNode);

      b1 bFullyOrdinal = Expr::bIsFullyOrdinal(pOrdExpr);
      b1 bHasAggregate = Expr::bContainsAnAggregate(pOrdExpr);

      if (bFullyOrdinal && !bHasAggregate) {
        // All good, nothing to do here
        delete pOrdExpr;
        return GraphNav::WALK_CONTINUE;
      }

      vPushdownExpr(pOrdExpr, _pQueryNode(pNode), pRootNode);
      delete pOrdExpr;
      return GraphNav::WALK_CONTINUE;


    } else if (OrderOp::bIsOfType(pNode)) {

      // Get the list of order items in ordinal representation
      QueryNode* pOrderItemList = OrderOp::pOrdinalOrderItemList(_pQueryNode(pNode), pRootNode);
      ASSERT(OrderItemList::bIsOfType(pOrderItemList));

      // Iterate each order item in the list
      PlanNodeIter* pIter = pOrderItemList->pNewChildrenIter();
      while (pIter->bHasNext()) {
        QueryNode* pOrderItem = _pQueryNode(pIter->pNext());
        ASSERT(OrderItem::bIsOfType(pOrderItem));

        // The expression is already in an ordinal format, so just extract it
        QueryNode* pOrdExpr = OrderItem::pExprNode(pOrderItem);
        ASSERT(Expr::bIsOfType(pOrdExpr));

        b1 bFullyOrdinal = Expr::bIsFullyOrdinal(pOrdExpr);
        b1 bHasAggregate = Expr::bContainsAnAggregate(pOrdExpr);

        if (bFullyOrdinal && !bHasAggregate) {
          // All good, nothing to do here. Try the next order item
          continue;
        }

        // We have a non-ordinal component that needs to be pushed down
        vPushdownExpr(pOrdExpr, _pQueryNode(pNode), pRootNode);
      }

      delete pIter;
      delete pOrderItemList;
      return GraphNav::WALK_CONTINUE;


    } else if (AggregateOp::bIsOfType(pNode)) {
      // Check if the group by expressions are equal to or a superset of
      // one of the exprs in the select list.
      QueryNode* pGroupByList = AggregateOp::pGroupByList(_pQueryNode(pNode));
      PlanNodeIter* pGByIter = pGroupByList->pNewChildrenIter();

      while (pGByIter->bHasNext()) {
        QueryNode* pGroupByExpr = _pQueryNode(pGByIter->pNext());
        ASSERT(Expr::bIsOfType(pGroupByExpr));

        QueryNode* pProjColList = AggregateOp::pProjectedColumns(_pQueryNode(pNode));
        ASSERT(ProjectedColumns::bIsOfType(pProjColList));

        // If the group by expression is not equal to or a superset of a projected column
        // then it must be pushed down and a post-projection added. We will take care of
        // the post-project here. An example here is:
        //   SELECT col1 + 1, SUM(col2) FROM t GROUP BY col1;
        if (!ProjectedColumns::bHasExprEqualOrSuperset(pProjColList, pGroupByExpr, false)) {
          // We can add it straight to this AggregateOp.
          ProjectOp::vPushdownExpr(_pQueryNode(pNode), pGroupByExpr->pDeepCopy(true), nullptr);
        }
      }

      delete pGByIter;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk this statement marker section
  GraphNav::bBoundedWalk(pStatementMarker,
                         false,
                         true,
                         fStmtSectionVisitor,
                         pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Pushes down a given expression (or part thereof) from a given SqlOp to     */
/* the nearest ProjectOp.                                                     */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vPushdownExpr(QueryNode* pOrdExpr, QueryNode* pSqlOp, PlanNode* pRootNode)
{
  ASSERT(pOrdExpr);
  ASSERT(Expr::bIsOfType(pOrdExpr));
  ASSERT(!Expr::bIsFullyOrdinal(pOrdExpr) || Expr::bContainsAnAggregate(pOrdExpr));
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Find the nearest ProjectOp/AggregateOp below pSqlOp in the plan.
  // If we cannot find one in this statement marker section then we
  // won't push this expr down and we will catch it in the validation
  // check that follows (vValidateExprVisibility).

  auto fCondition = [](PlanNode* pNode, void* pvArg) {
    UNUSED(pvArg);
    return ProjectOp::bIsOfType(pNode);
  };

  QueryNode* pProjectOp = _pQueryNode(GraphNav::pBoundedSearch(pSqlOp, false, true, fCondition, nullptr));

  if (!pProjectOp) {
    return;
  }

  // Firstly, push down all aggregate terms in pOrdExpr.

  // Get a list of all aggregate terms. The expressions in the list are
  // shallow copied from pOrdExpr so don't delete them, only the list itself.
  QueryNodeList* pAggExprs = Expr::pAggregateExprs(pOrdExpr, true);

  if (pAggExprs->pHead()) {
    // list is not empty

    QueryNodeList* pColumnExprList = PlanObject::pColumnExprs(pSqlOp, pRootNode);

    QueryNodeIter* pIter = pAggExprs->pNewIter();
    while (pIter->bHasNext()) {
      QueryNode* pAggExpr = pIter->pNext();

      // Convert these ordinal expressions back to symbolic.
      // Returns a completely new copy of the expression.
      QueryNode* pSymExpr = Expr::pOrdinalToSymbolic(pAggExpr, pColumnExprList, pRootNode);
      ASSERT(Expr::bIsNotOrdinal(pSymExpr));

      // Add it to the ProjectOp
      ProjectOp::vPushdownExpr(pProjectOp, pSymExpr, nullptr);
    }

    pColumnExprList->vDeleteAll();
    delete pColumnExprList;
    delete pIter;
  }

  pAggExprs->vRemoveAll();
  delete pAggExprs;


  // Now push down any non-aggregate, columnRefs in pOrdExpr
  QueryNode* pSubExpr = Expr::pLargestNonOrdinalSubExpr(pOrdExpr, false);

  if (pSubExpr) {
    ProjectOp::vPushdownExpr(pProjectOp, pSubExpr->pDeepCopy(true), nullptr);
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

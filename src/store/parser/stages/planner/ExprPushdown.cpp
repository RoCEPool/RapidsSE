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
/* Pushes down any intermediate/working expressions that are not being        */
/* output into a lower ProjectOp/AggregateOp.                                 */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vPushdownExprs(PlanNode* pRootNode)
{
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  PlanNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // Visit every statement section
  auto fStmtMarkerVisitor = [pRootNode]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

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
  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(),
                                false,
                                true,
                                fStmtMarkerVisitor,
                                pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validates that the right projected columns will be delivered to each       */
/* SQL operator that has expressions that use or build on those projected     */
/* columns. i.e. each expression in a SQL operator can be rewritten in        */
/* an ordinal form relative to the input projected columns.                   */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vValidateExprVisibility(PlanNode* pRootNode)
{
  // Walk every SQL operator in the graph.
  // If the SQL operator has expressions then validate that each expression
  // can be converted to an ordinal format and that it is fully ordinal.
  // If not, throw and error about an expression not being visible.

  // Copy ExprValidator::vValidateOrderExprAboveSetOp() and generalise it.
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
  //   If it is not a FilterOp, OrderOp then keep walking.
  //   Find the ProjectOp/AggregateOp below it in this StatementMarker section, if any.
  //   If none, then keep walking.
  //   Otherwise extract the expression from the FilterOp/OrderOp.
  //   Convert the expression to ordinal form. Determine if it is fully ordinal.
  //   If not fully ordinal then determine the largest non-ordinal subexpression
  //   and add this to the projected columns list of the ProjectOp identified above.

  // Notes:
  //   We could either push down the whole expression or the largest non-ordinal
  //   subexpression. In this case we choose the latter. The rationale being that
  //   there may be a base part of the expression that is already being computed
  //   and may be expensive to recalculate it.
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
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    // TODO: Add any other SQL operators here that contain expressions
    if (FilterOp::bIsOfType(pNode)) {

      // Get the filter expression in ordinal representation
      QueryNode* pOrdExpr = FilterOp::pOrdinalFilterExpr(_pQueryNode(pNode), pRootNode);

      if (Expr::bIsFullyOrdinal(pOrdExpr)) {
        // All good, nothing to do here
        delete pOrdExpr;
        return GraphNav::WALK_CONTINUE;
      }

      // We have a non-ordinal component that needs to be pushed down
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

        if (Expr::bIsFullyOrdinal(pOrdExpr)) {
          // All good, nothing to do here. Try the next order item
          continue;
        }

        // We have a non-ordinal component that needs to be pushed down
        vPushdownExpr(pOrdExpr, _pQueryNode(pNode), pRootNode);
      }

      delete pIter;
      delete pOrderItemList;
      return GraphNav::WALK_CONTINUE;
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
void ExprPushdown::vPushdownExpr(QueryNode* pExpr, QueryNode* pSqlOp, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(!Expr::bIsFullyOrdinal(pExpr));
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Find the nearest ProjectOp/AggregateOp below pSqlOp in the plan.
  // If we cannot find one in this statement marker section then we
  // won't push this expr down and we will catch it in the validation
  // check that follows (vValidateExprVisibility).

  auto fCondition = [](PlanNode* pNode, void* pvArg) {
    return ProjectOp::bIsOfType(pNode);
  };

  QueryNode* pProjectOp = _pQueryNode(GraphNav::pBoundedSearch(pSqlOp, false, true, fCondition, nullptr));

  if (!pProjectOp) {
    return;
  }

  // Determine the largest non-ordinal subexpression of pExpr
  QueryNode* pSubExpr = Expr::pLargestNonOrdinalSubExpr(pExpr);

  // Add it to the ProjectOp
  ProjectOp::vPushdownExpr(pProjectOp, pSubExpr->pDeepCopy(true), nullptr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AggregateSplit.hpp - Splits multiple aggregations into separate proj cols  */
/*============================================================================*/

#include "BASE.h"
#include "AggregateSplit.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowPlan.hpp"
#include "ShadowRoot.hpp"
#include "ProjectOp.hpp"
#include "AggregateOp.hpp"
#include "ProjectedColumn.hpp"
#include "Expr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Adds in StageMarker nodes to the plan where required.                      */
/* This tells the transliterator where to create a new stage in the pipeline. */
/* We explicitly place these rather than replace StatementMarkers because we  */
/* don't want to mix the semantics of a StatementMarker and StageMarker.      */
/*----------------------------------------------------------------------------*/
void AggregateSplit::vSplitAggregates(PlanNode *pRootNode)
{
  QueryNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));


  // Lambda called as we walk the graph
  auto fVisitor = [pRootNode](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pSqlOp = _pQueryNode(pNode);

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNodeList* pCommonAggList = nullptr;

    // We have a ProjectOp (or AggregateOp)
    // Check each ordinal projected column to see if it has multiple aggregation terms
    QueryNode* pOrdProjCols = ProjectOp::pOrdinalProjectedColumns(pSqlOp, pRootNode);
    PlanNodeIter* pIter = pOrdProjCols->pNewChildrenIter();

    // Check the expression of each ordinal projected column
    while (pIter->bHasNext()) {
      QueryNode* pOrdProjCol = _pQueryNode(pIter->pNext());
      ASSERT(ProjectedColumn::bIsOfType(pOrdProjCol));

      QueryNode* pOrdExpr = ProjectedColumn::pExprNode(pOrdProjCol);
      ASSERT(Expr::bIsOfType(pOrdExpr));
      ASSERT(Expr::bIsFullyOrdinal(pOrdExpr));

      if (!Expr::bContainsAnAggregate(pOrdExpr)) {
        // Nothing to worry about here
        continue;
      }

      // The ordinal expression contains aggregations. But does it contain
      // more than one expression?
      QueryNodeList* pAggList = Expr::pAggregateExprs(pOrdExpr, false);

      if (pAggList->nItems() > 1) {
        // Make a list of aggregates to be split. After we have looked at all projected
        // columns then we can insert a ProjectOp underneath.

        if (pCommonAggList) {
          // move the aggregation terms to the common list
          while (pAggList->nItems() > 0){
            pCommonAggList->vAppend(pAggList->pPop());
          }

        } else {
          pCommonAggList = pAggList;
          pAggList = nullptr;
        }
      }

      if (pAggList) {
        pAggList->vRemoveAll();
        delete pAggList;
      }
    }

    // Insert the Project/AggregateOp with these aggregates
    if (pCommonAggList) {
      vInsertProjection(pCommonAggList, pSqlOp, pRootNode);
      ASSERT(pCommonAggList->nItems() > 0);
      delete pCommonAggList;
    }

    delete pIter;
    delete pOrdProjCols;

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node from the bottom up and inspect each ProjectOp/AggregateOp.
  // We use a bottom-up walk because we may be modifying the children of
  // the current node. Can't do that with a top-down walk.
  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(), false, true, fVisitor, nullptr);
}


/*----------------------------------------------------------------------------*/
/* Inserts a ProjectOp or AggregateOp below pParentNode with the aggregate    */
/* expressions from pAggList. pAggList items are borrowed and must be copied  */
/*----------------------------------------------------------------------------*/
void AggregateSplit::vInsertProjection(QueryNodeList* pAggList, QueryNode* pParentNode, PlanNode* pRootNode)
{
  ASSERT(pAggList);
  ASSERT(pAggList->nItems() > 0);
  ASSERT(pParentNode);
  ASSERT(ProjectOp::bIsOfType(pParentNode));

  QueryNodeList* pColumnExprList = ProjectOp::pColumnExprs(_pQueryNode(pParentNode->pFirstChild()), pRootNode);

  // Duplicate pParentNode as pChild
  QueryNode* pChild = pParentNode->pDeepCopy(true, nullptr);

  QueryNodeIter* pIter = pAggList->pNewIter();
  while (pIter->bHasNext()) {

    QueryNode* pOrdExpr = pIter->pNext();

    // Convert pAggList children from ordinal back to symbolic.
    QueryNode* pSymExpr = Expr::pOrdinalToSymbolic(pOrdExpr, pColumnExprList, pRootNode);

    // Push down each pAggList child into pChild
    // TODO: ensure that the pushdown checks for duplicates and ignores them.
    ProjectOp::vPushdownExpr(pChild, pSymExpr, nullptr);
  }

  delete pIter;

  pColumnExprList->vDeleteAll();
  delete pColumnExprList;

  // Insert pChild as child of pParent
  pParentNode->vInsertAsParentOfChildren(pChild);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

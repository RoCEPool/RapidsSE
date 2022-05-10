/*============================================================================*/
/* AggregateSplit.hpp - Splits multiple aggregations into separate proj cols  */
/*============================================================================*/

#include "BASE.h"
#include "AggregateSplit.hpp"
#include "StageMarkers.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowPlan.hpp"
#include "ShadowRoot.hpp"
#include "ProjectOp.hpp"
#include "AggregateOp.hpp"
#include "ProjectedColumn.hpp"
#include "ProjectedColumns.hpp"
#include "Expr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Multiple operations regarding expressions with multiple aggregates         */
/*----------------------------------------------------------------------------*/
void AggregateSplit::vAggregateSplit(PlanNode *pRootNode)
{
  QueryNode* pPlanNode = _pQueryNode(ShadowRoot::pPlanRoot(pRootNode));
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  vSplitAggregates(pPlanNode, pRootNode);

  // After splitting aggregates we also need to separate two or more projections
  // (or aggregations) under the same StageMarker.
  StageMarkers::vSeparateProjectionsWithStageMarkers(pPlanNode, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Splits multiple aggregations into separate proj columns so that we don't   */
/* have more than one aggregation in a single expression.                     */
/*----------------------------------------------------------------------------*/
void AggregateSplit::vSplitAggregates(QueryNode *pPlanNode, PlanNode *pRootNode)
{
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));


  // Lambda called as we walk the graph
  auto fVisitor = [pRootNode](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pSqlOp = _pQueryNode(pNode);

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNodeList* pCommonAggList = nullptr;



    i8 nCols = ProjectOp::nColumns(pSqlOp);
    b1 bRemoveCol[nCols]; // remove associated column if true

    QueryNodeList* pColumnExprList = PlanObject::pColumnExprs(_pQueryNode(pSqlOp->pFirstChild()), pRootNode);

    // Get the list of correlated expressions from the enclosing subquery.
    // We don't need to free this object.
    QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pSqlOp, pRootNode);

    // We have a ProjectOp (or AggregateOp)
    // Check each ordinal projected column to see if it has multiple aggregation terms
    QueryNode* pSymProjCols = ProjectOp::pProjectedColumns(pSqlOp);
    PlanNodeIter* pIter = pSymProjCols->pNewChildrenIter();

    // Iterate each symbolic projected column
    i8 i = -1;
    while (pIter->bHasNext()) {
      QueryNode* pSymProjCol = _pQueryNode(pIter->pNext());
      ASSERT(ProjectedColumn::bIsOfType(pSymProjCol));

      ++i;
      bRemoveCol[i] = false;

      QueryNode* pSymExpr = ProjectedColumn::pExprNode(pSymProjCol);
      ASSERT(Expr::bIsOfType(pSymExpr));

      QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pSymExpr, pColumnExprList, pCorrelatedExprs, pRootNode);
      ASSERT(Expr::bIsOfType(pOrdExpr));
      ASSERT(Expr::bIsFullyOrdinal(pOrdExpr));

      if (!Expr::bContainsAnAggregate(pOrdExpr)) {
        // Nothing to worry about here
        delete pOrdExpr;
        continue;
      }

      // The ordinal expression contains aggregations. But does it contain
      // more than one aggregate? Get a list of aggregation expressions that exist
      // in pOrdExpr. Note that these are pointers to exprs in pOrdExpr, not copies.
      QueryNodeList* pAggList = Expr::pAggregateExprs(pOrdExpr, false);

      if (pAggList->nItems() > 1) {
        bRemoveCol[i] = true;

        // Make a list of aggregates to be split. After we have looked at all projected
        // columns then we can insert a ProjectOp underneath.

        if (!pCommonAggList) {
          pCommonAggList = new QueryNodeList();
        }

        // Copy the aggregation terms to the common list
        while (pAggList->nItems() > 0){
          pCommonAggList->vAppend(pAggList->pPop()->pDeepCopy(true));
        }
      }

      if (pAggList) {
        pAggList->vRemoveAll();
        delete pAggList;
      }

      delete pOrdExpr;
    }

    // Insert the Project/AggregateOp with these aggregates
    if (pCommonAggList) {
      vInsertProjection(pCommonAggList, bRemoveCol, nCols, pSqlOp, pColumnExprList, pRootNode);
      ASSERT(pCommonAggList->nItems() > 0);
      pCommonAggList->vDeleteAll();
      delete pCommonAggList;
    }

    pColumnExprList->vDeleteAll();
    delete pColumnExprList;

    delete pIter;

    return GraphNav::WALK_CONTINUE;
  };


  // Walk each node from the bottom up and inspect each ProjectOp/AggregateOp.
  // We use a bottom-up walk because we may be modifying the children of
  // the current node. Can't do that with a top-down walk.
  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fVisitor, nullptr);
}


/*----------------------------------------------------------------------------*/
/* Inserts a ProjectOp or AggregateOp below pParentNode with the aggregate    */
/* expressions from pAggList. pAggList items are borrowed and must be copied  */
/*----------------------------------------------------------------------------*/
void AggregateSplit::vInsertProjection(QueryNodeList* pAggList,
                                       b1* bRemoveCols,
                                       i8 nColumns,
                                       QueryNode* pParentNode,
                                       QueryNodeList* pColumnExprList,
                                       PlanNode* pRootNode)
{
  UNUSED(nColumns);

  ASSERT(pAggList);
  ASSERT(pAggList->nItems() > 0);
  ASSERT(pParentNode);
  ASSERT(ProjectOp::bIsOfType(pParentNode));

  // Duplicate pParentNode as pChild, but without any children.
  QueryNode* pChild = pParentNode->pDeepCopy(true, nullptr);

  // Remove projected columns from pChild that contain multiple aggregations
  // in the one expression. These expressions exist in the parent but are not
  // required in this child ProjectOp.
  PlanNodeIter* pColIter = ProjectOp::pProjectedColumns(pChild)->pNewChildrenIter();
  i8 i = -1;

  while (pColIter->bHasNext()) {
    PlanNode* pProjCol = pColIter->pNext();
    ++i;

    if (bRemoveCols[i]) {
      pColIter->vRemove();
      delete pProjCol;
    }
  }
  delete pColIter;


  // Now we can go through the list of expressions that need to be projected and add them
  QueryNodeIter* pIter = pAggList->pNewIter();
  while (pIter->bHasNext()) {

    QueryNode* pOrdExpr = pIter->pNext();

    // Convert pAggList children from ordinal back to symbolic.
    QueryNode* pSymExpr = Expr::pOrdinalToSymbolic(pOrdExpr, pColumnExprList, pRootNode);

    // Push down each pAggList child into pChild
    ProjectOp::vPushdownExpr(pChild, pSymExpr, nullptr);
  }

  delete pIter;

  // Insert pChild as child of pParent
  pParentNode->vInsertAsParentOfChildren(pChild);
  PlanObject::vClearLocation(pParentNode);

  // Do we need to change pParent from an AggregateOp to a ProjectOp?
  // The expressions in pParent that combine multiple aggregate terms probably
  // don't involve any grouping.
  if (AggregateOp::bIsOfType(pParentNode)) {

    QueryNode* pOrdProjCols = AggregateOp::pOrdinalProjectedColumns(pParentNode, pRootNode);
    ASSERT(ProjectedColumns::bIsOfType(pOrdProjCols));

    b1 bHasAggs = ProjectedColumns::bContainsAnAggregate(pOrdProjCols);
    delete pOrdProjCols;

    if (!bHasAggs) {
      // We need to modify pParent in place because we are walking a graph.
      // First make a template.

      // Make a list of projected columns, because that's what we need to construct a ProjectOp.
      QueryNodeList* pProjCols = new QueryNodeList();

      // Deep copy the columns as they will be taken in the ProjectOp
      PlanNodeIter* pSymbolicColIter = ProjectOp::pProjectedColumns(pParentNode)->pNewChildrenIter();
      while (pSymbolicColIter->bHasNext()) {
        QueryNode* pProjectedColumn = _pQueryNode(pSymbolicColIter->pNext());
        ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

        pProjCols->vAppend(pProjectedColumn->pDeepCopy(true));
      }
      delete pSymbolicColIter;

      QueryNode* pProjectOpTemplate = ProjectOp::pCreate(pProjCols);

      // Snip pChild from pParentNode. We will add it back in later.
      ASSERT(pParentNode->iNumChildren() == 1);
      ASSERT(pParentNode->pFirstChild() == pChild);
      pParentNode->pPopChild();


      ASSERT(pProjCols->nItems() == 0);
      delete pProjCols;

      pParentNode->vTransmogrifyTo(pProjectOpTemplate);
      delete pProjectOpTemplate;

      // Now put pChild back as a child of pParentNode
      pParentNode->vAppendChild(pChild);
    }
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

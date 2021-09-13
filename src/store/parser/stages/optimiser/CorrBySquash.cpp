/*============================================================================*/
/* CorrBySquash.hpp - Push down ProjectOps that implement the CORRESPONDINB BY*/
/* clause and squash any redundant ProjectOps below it                        */
/*============================================================================*/

#include "BASE.h"
#include "CorrBySquash.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectedColumn.hpp"
#include "StringObject.hpp"
#include "ProjectOp.hpp"
#include "Expr.hpp"
#include "ColumnRef.hpp"
#include "ColumnOrdinal.hpp"
#include "ProjectOp.hpp"
#include "AggregateOp.hpp"

#include "SetOp.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Searches for any CORRESPONDING BY ProjectOps and squashes them if possible */
/*----------------------------------------------------------------------------*/
void CorrBySquash::vSearchAndSquash(PlanNode *pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // Iterate each node, looking for a StatementMarker
  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    // Search the statement marker section for union ops with CORRESPONDING BY clauses
    vSearchStatementSection(pStatementMarker, pRootNode);

    // We keep walking down each section because we will only push a ProjectOp
    // down one level at a time.
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Searches for any CORRESPONDING BY ProjectOps in a statement marker section */
/*----------------------------------------------------------------------------*/
void CorrBySquash::vSearchStatementSection(QueryNode* pStatementMarker, PlanNode* pRootNode)
{
  ASSERT(pStatementMarker);
  ASSERT(StatementMarker::bIsOfType(pStatementMarker));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  auto fCondition = [](PlanNode* pStatementMarker, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return (SetOp::bIsOfType(pStatementMarker));
  };

  QueryNode* pSetOp = _pQueryNode(GraphNav::pBoundedSearch(pStatementMarker, false, true, fCondition, nullptr));

  if (!pSetOp) {
    // No set operators, so nothing to do.
    return;
  }

  // We do have a set operator. Does this operator have ProjectOps as children?
  ASSERT(pSetOp->nChildren() == 2);
  QueryNode* pChild1 = _pQueryNode(pSetOp->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pSetOp->pSecondChild());

  ASSERT(pChild1);
  ASSERT(pChild2);

  // Normally if there is a CORRESPONDING BY clause we would expect both children of
  // a SetOp to be ProjectOps. However the algorithm used here will only push a
  // ProjectOp down in the plan graph one statement section (to allow this recursive
  // walk/squash to be simplified). As a result, we may encounter a SetOp where only
  // one of the children is a ProjectOp that needs to be squashed.

  if (!ProjectOp::bIsExactType(pChild1) && !ProjectOp::bIsExactType(pChild2)) {
    return;
  }

  // One or both children are ProjectOps for a CORRESPONDING BY clause
  ASSERT(ProjectOp::bIsExactType(pChild1) || ProjectOp::bIsExactType(pChild2));

  // Snip each child and push it down the hierarchy
  if (ProjectOp::bIsExactType(pChild1)) {
    GraphEditor::pSnipChild(pSetOp, pChild1, true);
    ASSERT(StatementMarker::bIsOfType(pSetOp->pFirstChild()));
    vPushDownProjectOp(pChild1, _pQueryNode(pSetOp->pFirstChild()), pRootNode);
    delete pChild1;
  }

  if (ProjectOp::bIsExactType(pChild2)) {
    GraphEditor::pSnipChild(pSetOp, pChild2, true);
    ASSERT(StatementMarker::bIsOfType(pSetOp->pSecondChild()));
    vPushDownProjectOp(pChild2, _pQueryNode(pSetOp->pSecondChild()), pRootNode);
    delete pChild2;
  }
}

/*----------------------------------------------------------------------------*/
/* Pushes down and squashes a ProjectOp down a given subtree                  */
/*----------------------------------------------------------------------------*/
void CorrBySquash::vPushDownProjectOp(QueryNode* pProjectOp, QueryNode* pStartingNode, PlanNode* pRootNode)
{
  ASSERT(pProjectOp);
  ASSERT(ProjectOp::bIsExactType(pProjectOp));
  ASSERT(pProjectOp->nChildren() == 0);
  ASSERT(pStartingNode);
  ASSERT(StatementMarker::bIsOfType(pStartingNode));

  // The ProjectOp has already been removed from the plan graph. pStartingNode is
  // the original child of pProjectOp. pProjectOp needs to be pushed down some
  // amount under pStartingNode.

  b1 bSquashed = false;

  // Descend the tree and look for particular conditions. Specifically:
  //  - If we encounter a StatementMarker, ignore it and keep descending
  //  - If we encounter a ProjectOp or AggregateOp then squash it.
  //  - If we encounter a SetOp (union/intersect/except) and that SetOp has ProjectOps
  //    as children then squash pProjectOp into this child.
  //  - If we encounter a SetOp (union/intersect/except) and that SetOp does NOT have
  //    ProjectOps as children then insert pProjectOp as a child of the SetOp. The recursive
  //    walk down of this algorithm will keep squashing this down.
  //  - Else insert pProjectOp as a parent of current node.

  // Because we wish to skip over StatementMarkers we need to do a global walk
  auto fVisitor = [&bSquashed, pProjectOp, pStartingNode, pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);
    UNUSED(pvArg);

    //  - If we encounter a StatementMarker, ignore it and keep descending
    if (StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    //  - If we encounter a ProjectOp or AggregateOp then squash it.
    // We separate out the handling of Project vs Aggregate because the
    // AggregateOp also has to consider aliases in the group by list.
    if (ProjectOp::bIsExactType(pNode)) {
      ProjectOp::vSquash(_pQueryNode(pNode), pProjectOp, pRootNode);
      bSquashed = true;
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (AggregateOp::bIsExactType(pNode)) {
      AggregateOp::vSquash(_pQueryNode(pNode), pProjectOp, pRootNode);
      bSquashed = true;
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    //  - If we encounter a SetOp (union/intersect/except) and ...
    if (SetOp::bIsOfType(pNode)) {

      QueryNode* pSetOp = _pQueryNode(pNode);

      // We need to peek ahead here to see what lies beneath...
      ASSERT(pSetOp->nChildren() == 2);
      QueryNode* pChild1 = _pQueryNode(pSetOp->pFirstChild());
      QueryNode* pChild2 = _pQueryNode(pSetOp->pSecondChild());

      // If this SetOp had a CORRESPONDING BY clause then we would expect
      // both pChild1 and pChild2 to be ProjectOps.
      // If this is a regular SetOp then we expect pChild1 and pChild2
      // to both be StatementMarkers.

      if (ProjectOp::bIsOfType(pChild1)) {
        RASSERT(ProjectOp::bIsOfType(pChild2));
        // Pushing the ProjectOp down into a SetOp with a CORRESPONDING BY clause

        //  - ...and that SetOp has ProjectOps as children then squash
        //  pProjectOp into this child.
        ProjectOp::vSquash(pChild1, pProjectOp, pRootNode);

        // Before we squash pProjectOp into child2 we need to handle the situation where the
        // ProjectOp we are pushing down has valid aliases but the expressions match against
        // child1 and not child2.
        // This is because the ProjectOp would have been created based on the projected
        // columns of the SetOp, which itself uses the projected columns of its first child.
        // Therefore pushing a ProjectOp below the SetOp into the second child branch we
        // may need to update these expressions first and then push down a copy of the
        // updated ProjectOp.
        QueryNode* pProjectOp2a = pUpdateExprsFor(pProjectOp, pChild2, pRootNode);
        ProjectOp::vSquash(pChild2, pProjectOp2a, pRootNode);
        delete pProjectOp2a;


      } else if (StatementMarker::bIsOfType(pChild1)) {
        RASSERT(StatementMarker::bIsOfType(pChild2));
        // Pushing the ProjectOp down into a SetOp WITHOUT a CORRESPONDING BY clause

        //  - ...and that SetOp does NOT have ProjectOps as children then
        //    insert pProjectOp as a child of the SetOp. The recursive
        //    walk down of this algorithm will keep squashing this down.

        // For child1 we can just insert pProjectOp as its parent because the column
        // names/symbols are based on this first child so it should all just work.
        pSetOp->vInsertAsParentOfChild1(pProjectOp->pDeepCopy(true));

        // For child2 we need to determine which column ordinals are being projected in
        // the ProjectOp we just inserted, then create a new ProjectOp for child2 that
        // only projects the same column ordinals.

        ASSERT(ProjectOp::bIsExactType(pSetOp->pFirstChild()));
        QueryNode* pProjectOp2b = pCreateProjectUsingOrdinals(_pQueryNode(pSetOp->pFirstChild()), pChild2, pRootNode);
        pSetOp->vInsertAsParentOfChild2(pProjectOp2b);

      } else {
        NEVER_REACH
      }

      bSquashed = true;
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    //  - Else insert pProjectOp as a parent of current node.
    QueryNode* pParent = _pQueryNode(GraphNav::pGetParentOfPath(sPath, pStartingNode));
    ASSERT(pParent);

    pParent->vInsertAsParentOfChild(_pQueryNode(pNode), pProjectOp->pDeepCopy(true));
    bSquashed = true;
    return GraphNav::WALK_SKIP_SUBTREE;
  };

  GraphNav::bGlobalWalk(pStartingNode, false, true, fVisitor, nullptr);
  ASSERT(bSquashed);
}


/*----------------------------------------------------------------------------*/
/* Creates a copy of pProjectOp, updated for the columns returned by pChild   */
/*----------------------------------------------------------------------------*/
QueryNode* CorrBySquash::pUpdateExprsFor(QueryNode* pProjectOp, QueryNode* pChild, PlanNode* pRootNode)
{
  ASSERT(pProjectOp);
  ASSERT(ProjectOp::bIsExactType(pProjectOp));
  ASSERT(pChild);
  ASSERT(PlanObject::bIsOfType(pChild));
  ASSERT(pRootNode);

  // The new ProjectOp to return
  QueryNode* pNewProjectOp = nullptr;

  // Create an empty list of projected columns for the new ProjectOp.
  QueryNodeList* pNewColumnList = new QueryNodeList();

  // Get the list of columns being projected by pChild
  QueryNodeList* pChildColumnList = PlanObject::pColumnExprs(pChild, pRootNode);

  // Iterate the columns being projected by pProjectOp
  PlanNodeIter* pProjectOpIter = ProjectOp::pProjectedColumns(pProjectOp)->pNewChildrenIter();
  while (pProjectOpIter->bHasNext()) {

    QueryNode* pProjectOpCol = _pQueryNode(pProjectOpIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjectOpCol));

    Stz* szProjectOpColAlias = ProjectedColumn::szAliasName(pProjectOpCol);

    if (!szProjectOpColAlias) {
      // We can just duplicate the projected column
      QueryNode* pDupCol = pProjectOpCol->pDeepCopy(true);
      pNewColumnList->vAppend(pDupCol);
      continue;
    }

    // There is an alias. Match it to an alias in pChild's column list
    // and use the corresponding expr from pChild's column list.

    b1 bColumnAdded = false;
    QueryNodeIter* pChildColIter = pChildColumnList->pNewIter();
    while (pChildColIter->bHasNext()) {
      QueryNode* pChildCol = pChildColIter->pNext();
      ASSERT(pChildCol);
      ASSERT(ProjectedColumn::bIsOfType(pChildCol));

      Stz* szChildColAlias = ProjectedColumn::szAliasName(pChildCol);
      RASSERT(szChildColAlias);

      if (szChildColAlias->bSame(szProjectOpColAlias)) {
        // Create a new projected column using the projected column of pChild
        pNewColumnList->vAppend(pChildCol->pDeepCopy(true));
        bColumnAdded = true;
        break;
      }
    }

    RASSERT(bColumnAdded);
    delete pChildColIter;
  }

  // Create a new ProjectOp
  pNewProjectOp = ProjectOp::pCreate(pNewColumnList);

  // Cleanup
  delete pProjectOpIter;

  ASSERT(pNewColumnList->nItems() == 0);
  delete pNewColumnList;

  pChildColumnList->vDeleteAll();
  delete pChildColumnList;

  return pNewProjectOp;
}

/*----------------------------------------------------------------------------*/
/* Creates and returns a ProjectOp for pushing down as the second child of a  */
/* SetOp where the SetOp does not have a CORRESPONDING BY clause              */
/*----------------------------------------------------------------------------*/
QueryNode* CorrBySquash::pCreateProjectUsingOrdinals(QueryNode* pChild1, QueryNode* pChild2, PlanNode* pRootNode)
{
  ASSERT(pChild1);
  ASSERT(ProjectOp::bIsExactType(pChild1));
  ASSERT(pChild2);
  ASSERT(pRootNode);

  QueryNode* pNewProject = nullptr;
  QueryNodeList* pNewProjectList = new QueryNodeList();

  // Get the list of columns being projected from pChild1, in ordinal form
  QueryNode* pChild1OrdinalCols = ProjectOp::pOrdinalProjectedColumns(pChild1, pRootNode);
  PlanNodeIter* pChild1OrdinalColIter = pChild1OrdinalCols->pNewChildrenIter();

  // Get the list of columns currently being projected from pChild2. Keep in symbolic form.
  QueryNodeList* pChild2Columns = PlanObject::pColumnExprs(pChild2, pRootNode);

  while (pChild1OrdinalColIter->bHasNext()) {
    QueryNode* pOrdinalCol = _pQueryNode(pChild1OrdinalColIter->pNext());
    ASSERT(pOrdinalCol);
    ASSERT(ProjectedColumn::bIsOfType(pOrdinalCol));

    QueryNode* pOrdinalExpr = ProjectedColumn::pExprNode(pOrdinalCol);
    RASSERT(ColumnOrdinal::bIsOfType(pOrdinalExpr));

    i8 iOrdinal = ColumnOrdinal::iOrdinal(pOrdinalExpr);
    ASSERT(iOrdinal >= 1);

    // Remember, ordinal is 1-based
    RASSERT(iOrdinal <= pChild2Columns->nItems());

    QueryNode* pChild2Col = pChild2Columns->pPeek(iOrdinal - 1);
    ASSERT(pChild2Col);
    ASSERT(ProjectedColumn::bIsOfType(pChild2Col));

    // Add this projected column to the list
    pNewProjectList->vAppend(pChild2Col->pDeepCopy(true));
  }

  // We can now create the new ProjectOp for pChild2
  pNewProject = ProjectOp::pCreate(pNewProjectList);

  delete pChild1OrdinalColIter;
  delete pChild1OrdinalCols;

  pChild2Columns->vDeleteAll();
  delete pChild2Columns;

  ASSERT(pNewProjectList->nItems() == 0);
  delete pNewProjectList;

  return pNewProject;
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  PlanNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // Iterate each node, looking for a StatementMarker
  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

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

  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(), false, true, fVisitor, pRootNode);
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
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    //  - If we encounter a StatementMarker, ignore it and keep descending
    if (StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    //  - If we encounter a ProjectOp or AggregateOp then squash it.
    if (ProjectOp::bIsOfType(pNode)) {
      ProjectOp::vSquash(_pQueryNode(pNode), pProjectOp, pRootNode);
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

      // Deal with child1
      if (ProjectOp::bIsOfType(pChild1)) {
        //  - ...and that SetOp has ProjectOps as children then squash
        //  pProjectOp into this child.
        ProjectOp::vSquash(pChild1, pProjectOp, pRootNode);

      } else {
        //  - ...and that SetOp does NOT have ProjectOps as children then
        //    insert pProjectOp as a child of the SetOp. The recursive
        //    walk down of this algorithm will keep squashing this down.
        pSetOp->vInsertAsParentOfChild1(pProjectOp->pDeepCopy(true));
      }

      // Deal with child2
      if (ProjectOp::bIsOfType(pChild2)) {
        //  - ...and that SetOp has ProjectOps
        //    as children then squash pProjectOp into this child.
        ProjectOp::vSquash(pChild2, pProjectOp, pRootNode);

      } else {
        //  - ...and that SetOp does NOT have ProjectOps as children then
        //    insert pProjectOp as a child of the SetOp. The recursive
        //    walk down of this algorithm will keep squashing this down.
        pSetOp->vInsertAsParentOfChild2(pProjectOp->pDeepCopy(true));
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

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

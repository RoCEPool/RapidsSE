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

  if (!ProjectOp::bIsExactType(pChild1)) {
    ASSERT(!ProjectOp::bIsExactType(pChild2));
    return;
  }

  // Both children are ProjectOps for a CORRESPONDING BY clause
  ASSERT(ProjectOp::bIsExactType(pChild1));
  ASSERT(ProjectOp::bIsExactType(pChild2));
  ASSERT(StatementMarker::bIsOfType(pChild1->pFirstChild()));
  ASSERT(StatementMarker::bIsOfType(pChild2->pFirstChild()));

  // Snip each child and push it down the hierarchy
  GraphEditor::pSnipChild(pSetOp, pChild1, true);
  ASSERT(StatementMarker::bIsOfType(pSetOp->pFirstChild()));
  vSquashProjectOp(pChild1, _pQueryNode(pSetOp->pFirstChild()));

  GraphEditor::pSnipChild(pSetOp, pChild2, true);
  ASSERT(StatementMarker::bIsOfType(pSetOp->pSecondChild()));
  vSquashProjectOp(pChild2, _pQueryNode(pSetOp->pSecondChild()));
}

/*----------------------------------------------------------------------------*/
/* Pushes down and squashes a ProjectOp down a given subtree                  */
/*----------------------------------------------------------------------------*/
void CorrBySquash::vSquashProjectOp(QueryNode* pProjectOp, QueryNode* pStartingNode)
{
  ASSERT(pProjectOp);
  ASSERT(ProjectOp::bIsExactType(pProjectOp));
  ASSERT(pStartingNode);

  // TODO: handle the pushing down of the ProjectOp into:
  //   1. a select query
  //   2. a unionOp with corresponding by clauses
  //   3. a unionOp without corresponding by clauses
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

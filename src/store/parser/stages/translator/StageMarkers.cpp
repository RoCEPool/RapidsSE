/*============================================================================*/
/* StageMarkers.hpp - Replaces StatementMarkers in the plan with StageMarkers */
/*============================================================================*/

#include "BASE.h"
#include "StageMarkers.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "StageMarker.hpp"
#include "SetOp.hpp"
#include "ExplainOp.hpp"
#include "ExplainMarker.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Removes all StatementMarker nodes from the plan.                           */
/* StatementMarkers are a concept for the planner. They don't mean the same   */
/* thing as a StageMarker, which is used by the transliteration.              */
/*----------------------------------------------------------------------------*/
void StageMarkers::vRemoveStatementMarkers(PlanNode *pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // A lambda that checks if a node is a StatementMarker to be removed. Used later.
  // We don't remove ExplainMarkers.
  auto fCondition = [](QueryNode* pChildNode) -> b1 {
    return StatementMarker::bIsOfType(pChildNode) && !ExplainMarker::bIsOfType(pChildNode);
  };

  // Lambda called as we walk the graph
  auto fVisitor = [fCondition](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(!StatementMarker::bIsOfType(pNode) || ExplainMarker::bIsOfType(pNode));

    QueryNodeList* pSnippedNodes = GraphEditor::pSnipChild(_pQueryNode(pNode), fCondition, true);

    if (pSnippedNodes) {
      pSnippedNodes->vDeleteAll();
      delete pSnippedNodes;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node and look for the StatementMarkers
  GraphNav::bGlobalWalk(pPlanNode, false, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Adds in StageMarker nodes to the plan where required.                      */
/* This tells the transliterator where to create a new stage in the pipeline. */
/* We explicitly place these rather than replace StatementMarkers because we  */
/* don't want to mix the semantics of a StatementMarker and StageMarker.      */
/*----------------------------------------------------------------------------*/
void StageMarkers::vAddStageMarkers(PlanNode *pRootNode)
{
  QueryNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));


  // Lambda called as we walk the graph
  auto fVisitor = [](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(!StatementMarker::bIsOfType(pNode) || ExplainMarker::bIsOfType(pNode));
    QueryNode* pQueryNode = _pQueryNode(pNode);

    // Rule: The children of a SetOp are always StageMarkers
    if (SetOp::bIsOfType(pQueryNode)) {
      ASSERT(pNode->nChildren() == 2);
      pQueryNode->vInsertAsParentOfChild1(StageMarker::pCreate());
      pQueryNode->vInsertAsParentOfChild2(StageMarker::pCreate());
    }

    // Rule: Add a StageMarker after an ExplainOp
    if (ExplainOp::bIsOfType(pQueryNode)) {
      pQueryNode->vInsertAsParentOfChildren(StageMarker::pCreate());
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node and add in StageMarkers as appropriate
  GraphNav::bGlobalWalk(pPlanNode, false, true, fVisitor, pRootNode);

  // Rule: The node under the plan node is always a StageMarker, unless it already
  //       has an ExplainMarker, in which case we will leave it.
  ASSERT(pPlanNode->nChildren() == 1);
  if (!ExplainMarker::bIsOfType(pPlanNode->pFirstChild())) {
    pPlanNode->vInsertAsParentOfChildren(StageMarker::pCreate());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* StageMarkers.hpp - Replaces StatementMarkers in the plan with StageMarkers */
/*============================================================================*/

#include "BASE.h"
#include "StageMarkers.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowPlan.hpp"
#include "ShadowRoot.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/StatementMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/dml/ExplainMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/StageMarker.hpp"
#include "SetOp.hpp"
#include "ExplainOp.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Run all changes for this transformation stage                              */
/*----------------------------------------------------------------------------*/
void StageMarkers::vStageMarkers(PlanNode *pRootNode)
{
  QueryNode* pPlanNode = _pQueryNode(ShadowRoot::pPlanRoot(pRootNode));
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  vRemoveStatementMarkers(pPlanNode, pRootNode);
  vAddStageMarkers(pPlanNode, pRootNode);
  vStandardiseSetOpChildrenLocation(pPlanNode, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Removes all StatementMarker nodes from the plan.                           */
/* StatementMarkers are a concept for the planner. They don't mean the same   */
/* thing as a StageMarker, which is used by the transliteration.              */
/*----------------------------------------------------------------------------*/
void StageMarkers::vRemoveStatementMarkers(QueryNode *pPlanNode, PlanNode* pRootNode)
{
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
void StageMarkers::vAddStageMarkers(QueryNode *pPlanNode, PlanNode* pRootNode)
{
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

      QueryNode* pChild1 = _pQueryNode(pQueryNode->pFirstChild());
      QueryNode* pChild2 = _pQueryNode(pQueryNode->pSecondChild());

      Location::eLO eLocation1 = PlanObject::eLocation(pChild1, true);
      Location::eLO eLocation2 = PlanObject::eLocation(pChild2, true);

      pQueryNode->vInsertAsParentOfChild1(StageMarker::pCreate(eLocation1, Routing::RT_UNICAST));
      pQueryNode->vInsertAsParentOfChild2(StageMarker::pCreate(eLocation2, Routing::RT_UNICAST));

    // Rule: Add a StageMarker after an ExplainOp
    } else if (ExplainOp::bIsOfType(pQueryNode)) {
      QueryNode *pChild1 = _pQueryNode(pQueryNode->pFirstChild());
      Location::eLO eLocation1 = PlanObject::eLocation(pChild1, true);

      pQueryNode->vInsertAsParentOfChildren(StageMarker::pCreate(eLocation1, Routing::RT_UNICAST));

//    } else if (JoinOp::bIsOfType(pQueryNode)) {
//      // TODO: Set location and routing as appropriate
//    }

    } else if (pQueryNode->nChildren() == 1 && PlanObject::bNodeHasLocation(pQueryNode)) {
      Location::eLO eCurrentLoc = PlanObject::eLocation(pQueryNode, false);
      Location::eLO eChildLoc   = PlanObject::eLocation(_pQueryNode(pQueryNode->pFirstChild()), true);
      ASSERT(eChildLoc > Location::LO_NONE);
      ASSERT(eChildLoc < Location::LO_NENUMS);

      if (eCurrentLoc != eChildLoc) {
        Routing::eRT eRouting = Routing::eDefaultRouting(eChildLoc, eCurrentLoc);
        pQueryNode->vInsertAsParentOfChildren(StageMarker::pCreate(eChildLoc, eRouting));
      }
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node and add in StageMarkers as appropriate
  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(), false, true, fVisitor, pRootNode);

  // Rule: The node under the plan node is always a StageMarker, unless it already
  //       has an ExplainMarker, in which case we will leave it.
  ASSERT(pPlanNode->nChildren() == 1);
  if (!ExplainMarker::bIsOfType(pPlanNode->pFirstChild())) {
    Location::eLO eLocation1 = PlanObject::eLocation(_pQueryNode(pPlanNode->pFirstChild()), true);

    pPlanNode->vInsertAsParentOfChildren(StageMarker::pCreate(eLocation1, Routing::RT_UNICAST));
  }
}

/*----------------------------------------------------------------------------*/
/* Ensure all children of set operators are StageMarkers with a location set  */
/* to "origin"                                                                */
/*----------------------------------------------------------------------------*/
void StageMarkers::vStandardiseSetOpChildrenLocation(QueryNode* pPlanNode, PlanNode* pRootNode)
{
  // All Set operators run on origin only (TODO: need to check about distinct
  // set operators). To avoid problems with the pipeline transliteration, all
  // children of a SetOp (which are StageMarkers) must also have a location of
  // "origin". If they don't then we will add in a StageMarker with that location.

  // Lambda called as we walk the graph
  auto fVisitor = [](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);


    if (!SetOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode *pSetOp = _pQueryNode(pNode);


    // Check the children
    ASSERT(pSetOp->nChildren() == 2);

    QueryNode* apChildren[2] = { _pQueryNode(pSetOp->pFirstChild()), _pQueryNode(pSetOp->pSecondChild()) };

    for (QueryNode* pChild : apChildren) {
      ASSERT(StageMarker::bIsOfType(pChild));

      if (StageMarker::eLocation(pChild) != Location::LO_ORIGIN) {
        QueryNode* pNewStageMarker = StageMarker::pCreate(Location::LO_ORIGIN, Routing::RT_UNICAST);
        pSetOp->vInsertAsParentOfChild(pChild, pNewStageMarker);
      }
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node and add in StageMarkers as appropriate
  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

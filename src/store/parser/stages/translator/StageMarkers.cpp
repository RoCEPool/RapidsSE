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
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"
#include "SetOp.hpp"
#include "ExplainOp.hpp"
#include "JoinOp.hpp"
#include "HintConstants.hpp"
#include "Hint.hpp"
#include "SqlException.hpp"
#include "Hashcast.hpp"
#include "Expr.hpp"
#include "ProjectOp.hpp"

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
  vSeparateProjectionsWithStageMarkers(pPlanNode, pRootNode);
  vStandardiseSetOpChildrenLocation(pPlanNode, pRootNode);
  vProcessJoinRoutingHint(pPlanNode, pRootNode);
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
  auto fVisitor = [fCondition](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

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
  auto fVisitor = [](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(!StatementMarker::bIsOfType(pNode) || ExplainMarker::bIsOfType(pNode));
    QueryNode *pQueryNode = _pQueryNode(pNode);

    if (ShadowPlan::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;

    // Rule: The children of a SetOp are always StageMarkers
    } if (SetOp::bIsOfType(pQueryNode)) {
      ASSERT(pNode->nChildren() == 2);

      QueryNode* pChild1 = _pQueryNode(pQueryNode->pFirstChild());
      QueryNode* pChild2 = _pQueryNode(pQueryNode->pSecondChild());

      Location::eLO eLocation1 = PlanObject::eLocation(pChild1, true);
      Location::eLO eLocation2 = PlanObject::eLocation(pChild2, true);

      pQueryNode->vInsertAsParentOfChild1(StageMarker::pCreate(eLocation1, Routing::RT_UNICAST));
      pQueryNode->vInsertAsParentOfChild2(StageMarker::pCreate(eLocation2, Routing::RT_UNICAST));


//    // Rule: Add a StageMarker after an ExplainOp
//    } else if (ExplainOp::bIsOfType(pQueryNode)) {
//      QueryNode *pChild1 = _pQueryNode(pQueryNode->pFirstChild());
//      Location::eLO eLocation1 = PlanObject::eLocation(pChild1, true);
//
//      pQueryNode->vInsertAsParentOfChildren(StageMarker::pCreate(eLocation1, Routing::RT_UNICAST));


    // Rule: We always move the left-side rows of a join to the location of the right-side rows
    } else if (JoinOp::bIsOfType(pQueryNode)) {
      // Add a stage marker after the left (first) child of the join
      ASSERT(pQueryNode->nChildren() == 2);
      QueryNode *pChild1 = _pQueryNode(pQueryNode->pFirstChild());
      Location::eLO eLocation1 = PlanObject::eLocation(pChild1, true);
      ASSERT(eLocation1 == Location::LO_ALL_PARTITIONS);

      pQueryNode->vInsertAsParentOfChild1(StageMarker::pCreate(eLocation1, Routing::RT_BROADCAST));


    // Rule: Add a StageMarker above a JoinOp, if one doesn't already exist
    } else if (pQueryNode->nChildren() >= 1 && JoinOp::bIsOfType(pQueryNode->pFirstChild())) {
      Location::eLO eCurrentLoc = PlanObject::eLocation(pQueryNode, true);
      Location::eLO eJoinLoc   = PlanObject::eLocation(_pQueryNode(pQueryNode->pFirstChild()), true);
      ASSERT(eJoinLoc > Location::LO_NONE);
      ASSERT(eJoinLoc < Location::LO_NENUMS);

      Routing::eRT eRouting = Routing::eDefaultRouting(eJoinLoc, eCurrentLoc);
      pQueryNode->vInsertAsParentOfChild1(StageMarker::pCreate(eJoinLoc, eRouting));

    // Rule: The node under a SubqueryMarker node is always a StageMarker, unless it already
    //       has an ExplainMarker, in which case we will leave it.
    } else if (SubqueryMarker::bIsOfType(pQueryNode)) {
      ASSERT(pQueryNode->nChildren() >= 1);

      if (!ExplainMarker::bIsOfType(pQueryNode->pFirstChild())) {
        Location::eLO eLocation1 = PlanObject::eLocation(_pQueryNode(pQueryNode->pFirstChild()), true);

        pQueryNode->vInsertAsParentOfChildren(StageMarker::pCreate(eLocation1, Routing::RT_UNICAST));
      }

    // Otherwise: If location of this node and its child don't match then add
    // a StageMarker with appropriate routing
    } else if (pQueryNode->nChildren() == 1 &&
               PlanObject::bNodeHasLocation(pQueryNode) &&
               !ExplainOp::bIsOfType(pQueryNode)) {
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
  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* We want to avoid having 2 or more projections being transliterated to the  */
/* same pipeline stage, as this causes complexities dealing with the row      */
/* changing multiple times. So we will separate them by adding StageMarkers.  */
/* This will end up putting the second projection into a separate pipeline    */
/* stage.                                                                     */
/*----------------------------------------------------------------------------*/
void StageMarkers::vSeparateProjectionsWithStageMarkers(QueryNode *pPlanNode, PlanNode* pRootNode)
{
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // We want to first find each StageMarker so we can then do a bounded walk of that
  // StageMarker section to count how many ProjectOps we encounter, and add a StageMarker
  // below the second and subsequent ones.

  i8 iProjectOpCount;

  // Define the second lambda that looks for more than one ProjectOp in a StageMarker section.
  auto fProjectOpSeparator = [&iProjectOpCount](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {
    UNUSED(sPath);
    UNUSED(iDepth);
    ASSERT(pvArg);
    ASSERT(StageMarker::bIsOfType(_pQueryNode(pvArg)));

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pParentStageMarker = _pQueryNode(pvArg);
    QueryNode* pProjectOp = _pQueryNode(pNode);
    ++iProjectOpCount;

    if (iProjectOpCount == 1) {
      return GraphNav::WALK_CONTINUE;
    }

    // We've got more than one ProjectOp in this StageMarker section. Add another StageMarker
    // below this ProjectOp.

    // We are currently only expecting this to occur on the origin as a result of a
    // super projection being required to eliminate "working" columns.
    // e.g. SELECT 1 FROM nation HAVING SUM(n_nationkey) > 1;
    // In this query we end up with two projections occurring in the same pipeline
    // stage on the origin - one that aggregates the sum from each node and one
    // that then projects the literal 1.

    ASSERT(StageMarker::eLocation(pParentStageMarker) == Location::LO_ORIGIN);
    ASSERT(StageMarker::eRouting(pParentStageMarker) == Routing::RT_UNICAST);
    QueryNode* pNewStageMarker = StageMarker::pCreate(StageMarker::eLocation(pParentStageMarker),
                                                      StageMarker::eRouting(pParentStageMarker));
    pProjectOp->vInsertAsParentOfChildren(pNewStageMarker);

    return GraphNav::WALK_CONTINUE;
  };

    // Define the first lambda that simply looks for StageMarker sections
  auto fStageMarkerFinder = [fProjectOpSeparator, &iProjectOpCount](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StageMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    iProjectOpCount = 0;

    // Walk this section, looking for multiple ProjectOps.
    // Pass the current StageMarker as pvArg.
    GraphNav::bBoundedWalkBottomUp(pNode, false, true, fProjectOpSeparator, pNode);
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fStageMarkerFinder, pRootNode);
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
  auto fVisitor = [](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

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
  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Where a join routing hint has been specified, update the routing on the    */
/* left child of the join                                                     */
/*----------------------------------------------------------------------------*/
void StageMarkers::vProcessJoinRoutingHint(QueryNode* pPlanNode, PlanNode* pRootNode)
{
  auto fVisitor = [](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);


    if (!JoinOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pJoinRoutingHint = PlanObject::pGetHint(_pQueryNode(pNode), C_HINT_KEY_JOIN_ROUTING);

    if (!pJoinRoutingHint) {
      return GraphNav::WALK_CONTINUE;
    }

    // Sanity checking on the hint value provided
    if (pJoinRoutingHint->nChildren() == 0) {
      SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s (no value provided)"_Stz, C_HINT_KEY_JOIN_ALGO);

    } else if (pJoinRoutingHint->nChildren() > 1) {
      SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s (more than one value provided)"_Stz, C_HINT_KEY_JOIN_ALGO);

    } else if (Hint::pFirstValue(pJoinRoutingHint)->eValueType() != Value::VT_STRING) {
      SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s (expecting string value)"_Stz, C_HINT_KEY_JOIN_ALGO);
    }

    Routing::eRT eRouteType = Routing::eRoutingType(Hint::pFirstValue(pJoinRoutingHint)->szValue());

    if (eRouteType <= Routing::RT_NONE || eRouteType >= Routing::RT_NENUMS) {
      SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s (invalid routing value)"_Stz, C_HINT_KEY_JOIN_ALGO);
    }

    QueryNode* pHashKeyHint = nullptr;
    if (eRouteType == Routing::RT_HASHCAST) {
      // Check that a hashkey hint was also given
      pHashKeyHint = PlanObject::pGetHint(_pQueryNode(pNode), C_HINT_KEY_HASH_KEY);

      if (!pHashKeyHint || pHashKeyHint->nChildren() == 0) {
        SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED,
                                ": Hint %s requires hint %s to also be set"_Stz,
                                C_HINT_KEY_JOIN_ALGO,
                                C_HINT_KEY_HASH_KEY);
      }
    }

    // Apply this routing to the first child only. This is because we move rows from the left
    // subtree to the location of the right tree to complete the join.
    ASSERT(pNode->nChildren() == 2);
    QueryNode* pStageMarker = _pQueryNode(pNode->pFirstChild());
    ASSERT(StageMarker::bIsOfType(pStageMarker));

    QueryNode* pRoutingProp = StageMarker::pRouting(pStageMarker);
    ASSERT(pRoutingProp);
    ASSERT(Routing::bIsOfType(pRoutingProp));

    // Change the type of routing of this StageMarker.
    QueryNode* pRemovedProp = pStageMarker->pRemoveProperty(pRoutingProp->szName());
    ASSERT(pRemovedProp);
    ASSERT(pRemovedProp == pRoutingProp);

    QueryNode* pNewRouting = Routing::pCreate(eRouteType);
    pNewRouting->vSetName(pRoutingProp->szName());
    pStageMarker->vAppendProperty(pNewRouting);

    if (eRouteType == Routing::RT_HASHCAST) {
      ASSERT(Hashcast::bIsOfType(pNewRouting));
      ASSERT(pHashKeyHint);
      ASSERT(pHashKeyHint->nChildren() > 0);

      QueryNode* pExpr = pHashKeyHint->pPopChild();
      ASSERT(Expr::bIsOfType(pExpr));

      Hashcast::vAddHashExpr(pNewRouting, pExpr);
    }

    delete pRoutingProp;
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fVisitor, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

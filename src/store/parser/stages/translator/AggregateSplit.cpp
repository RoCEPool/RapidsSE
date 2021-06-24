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
#include "store/parser/shadowNodes/planObjects/planMarkers/StageMarker.hpp"

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
  auto fVisitor = [](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pSqlOp = _pQueryNode(pNode);

    if (ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node and inspect each ProjectOp/AggregateOp
  GraphNav::bGlobalWalkBottomUp(pPlanNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

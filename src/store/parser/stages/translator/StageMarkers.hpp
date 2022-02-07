/*============================================================================*/
/* StageMarkers.hpp - Replaces StatementMarkers in the plan with StageMarkers */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"
#include "GraphNav.hpp"

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class QueryNode;

  class StageMarkers
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    StageMarkers();

  public:
    static void vStageMarkers(PlanNode* pRootNode);

    // We want to avoid having 2 or more projections being transliterated to the same
    // pipeline stage, so separate them with StageMarkers.
    static void vSeparateProjectionsWithStageMarkers(QueryNode* pPlanNode, PlanNode* pRootNode);

  protected:

    // Removes all StatementMarker nodes from the plan
    static void vRemoveStatementMarkers(QueryNode* pPlanNode, PlanNode* pRootNode);

    // Adds in StageMarker nodes to the plan where required
    static void vAddStageMarkers(QueryNode* pPlanNode, PlanNode* pRootNode);

    // Ensure all children of set operators are StageMarkers with a location of "origin"
    static void vStandardiseSetOpChildrenLocation(QueryNode* pPlanNode, PlanNode* pRootNode);

    // Where a join routing hint has been specified, update the routing on the
    // left child of the join
    static void vProcessJoinRoutingHint(QueryNode* pPlanNode, PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    // Removes all StatementMarker nodes from the plan
    static void vRemoveStatementMarkers(PlanNode* pChildNode);

    // Adds in StageMarker nodes to the plan where required
    static void vAddStageMarkers(PlanNode* pRootNode);


  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

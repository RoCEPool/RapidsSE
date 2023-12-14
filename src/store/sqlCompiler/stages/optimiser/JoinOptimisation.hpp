/*============================================================================*/
/* JoinOptimisation.hpp - forcing join algorithms from query hints            */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class QueryNode;

  class JoinOptimisation
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    JoinOptimisation();

  public:
    static void vOptimiseJoins(PlanNode* pRootNode);

  private:
    static void vApplyJoinHint(PlanNode* pRootNode);

    // Converts a given join to an index join based on a join hint directive
    static void vConvertJoinToIndexJoin(QueryNode* pJoinOp, PlanNode* pRootNode, s2K& sPath);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

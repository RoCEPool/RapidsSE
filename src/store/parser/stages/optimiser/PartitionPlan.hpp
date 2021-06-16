/*============================================================================*/
/* PartitionPlan.hpp - Parallelise the plan by partitions                     */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeStack.hpp"

namespace Parser
{
  class QueryNode;

  class PartitionPlan
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    PartitionPlan();

  public:
    // Parallelise the plan into partitions
    static void vPartitionPlan(PlanNode* pRootNode);

  private:
    static QueryNode* pPartitionStatementSection(QueryNode* pStatementMarker,
                                                 PlanNode* pRootNode,
                                                 QueryNode* pPlanRoot,
                                                 QueryNodeStack* pStmtSectionStack);

    // Consolidates separate plain chains from the partition, node & origin level
    static QueryNode* pConsolidateChains(QueryNode* pOriginChain,
                                         QueryNode* pNodeChain,
                                         QueryNode* pPartitionChain);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

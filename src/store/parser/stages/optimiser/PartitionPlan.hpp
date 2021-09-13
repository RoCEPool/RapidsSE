/*============================================================================*/
/* PartitionPlan.hpp - Parallelise the plan by partitions                     */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeStack.hpp"

namespace Parser
{
  class QueryNode;
  class GraphChains;

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
    static void vPartitionSubquerySection(QueryNode* pSubqueryMarker,
                                                PlanNode* pRootNode);

    static QueryNode* pPartitionStatementSection(QueryNode* pStatementMarker,
                                                 PlanNode* pRootNode,
                                                 QueryNodeStack* pStmtSectionStack);

    // Consolidates separate plain chains from the partition, node & origin level
    static QueryNode* pConsolidateChains(QueryNode* pOriginChain,
                                         QueryNode* pNodeChain,
                                         QueryNode* pPartitionChain);

    static void vPartitionStatementMarker(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionTableScanOp(    QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
//  static void vPartitionIndexScanOp(    QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionAggregateOp(    QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionExplainOp(      QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionFilterOp(       QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionLimitOp(        QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionOrderOp(        QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionProjectOp(      QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionUnionOp(        QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);
    static void vPartitionJoinOp(         QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack);

    // Returns true if a given ProjectOp is a "post-project" that simply
    // trims the select list
    static b1   bIsPostProject(QueryNode* pProjectOp);

    static b1   bCanPushDownOrderOp(QueryNode* pOrderOp);
    static b1   bCanPushDownLimitOp(QueryNode* pLimitOp);

    static void vPushOnPartitionChain(QueryNode* pNode, GraphChains* pChains);
    static void vPushOnNodeChain(QueryNode* pNode, GraphChains* pChains);
    static void vPushOnOriginChain(QueryNode* pNode, GraphChains* pChains);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AggregateSplit.hpp - Splits multiple aggregations into separate proj cols  */
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

  class AggregateSplit
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    AggregateSplit();

  public:

    //
    static void vAggregateSplit(PlanNode* pRootNode);

  private:
    // Splits multiple aggregations into separate proj columns so that we don't
    // have more than one aggregation in a single expression.
    static void vSplitMultipleAggregates(QueryNode *pPlanNode, PlanNode* pRootNode);

    // Inserts a ProjectOp or AggregateOp below pParentNode with the aggregate
    // expressions from pAggList. pAggList items are borrowed and must be copied
    static void vInsertProjection(QueryNodeList* pAggList,
                                  b1* bRemoveCols,
                                  i8 nColumns,
                                  QueryNode* pParentNode,
                                  QueryNodeList* pColumnExprList,
                                  PlanNode* pRootNode);

    // Splits a cast of an aggregation into separate projections so the aggregation
    // and casting are done in separate pipeline stages.
    static void vSplitCastOfAggregates(QueryNode *pPlanNode, PlanNode* pRootNode);

    // Given an expression, return a list of aggregate expression terms found in
    // that expression who have a CAST expression as a parent. Returned list is
    // to be deleted but NOT the list items.
    static QueryNodeList* pCastedAggregateExprs(QueryNode* pExpr);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

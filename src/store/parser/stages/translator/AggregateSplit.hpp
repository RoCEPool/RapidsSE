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
    static void vSplitAggregates(PlanNode* pRootNode);

  private:
    // Inserts a ProjectOp or AggregateOp below pParentNode with the aggregate
    // expressions from pAggList. pAggList items are borrowed and must be copied
    static void vInsertProjection(QueryNodeList* pAggList,
                                  b1* bRemoveCols,
                                  i8 nColumns,
                                  QueryNode* pParentNode,
                                  QueryNodeList* pColumnExprList,
                                  PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AggregateSplit.hpp - Splits multiple aggregations into separate proj cols  */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
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

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

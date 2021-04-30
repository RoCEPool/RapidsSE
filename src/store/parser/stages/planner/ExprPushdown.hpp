/*============================================================================*/
/* ExprPushdown.hpp - Pushes down expressions into lower ProjectOps as needed */
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

  class ExprPushdown
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    ExprPushdown();

  public:
    // Pushes down any intermediate/working expressions that are not being
    // output into a lower ProjectOp/AggregateOp.
    static void vPushdownExprs(PlanNode* pRootNode);

    // Validates that the right projected columns will be delivered to each
    // SQL operator that has expressions that use or build on those projected
    // columns. i.e. each expression in a SQL operator can be rewritten in
    // an ordinal form relative to the input projected columns.
    static void vValidateExprVisibility(PlanNode* pRootNode);


  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

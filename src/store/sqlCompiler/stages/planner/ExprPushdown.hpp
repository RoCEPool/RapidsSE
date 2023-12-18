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


  private:
    // Pushes down any intermediate/working expressions that are not being
    // output into a lower ProjectOp/AggregateOp, for the current statement section
    static void vStatementSectionPushdownExprs(QueryNode* pStatementMarker, PlanNode* pRootNode);

    // Pushes down a given expression (or part thereof) from a given SqlOp to
    // the nearest ProjectOp.
    static void vPushdownExpr(QueryNode* pOrdExpr, QueryNode* pSqlOp, PlanNode* pRootNode);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

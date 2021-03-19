/*============================================================================*/
/* ExprValidator.hpp - Validates that expressions are being specified and     */
/* used correctly                                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ExprValidator
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    ExprValidator();

  public:
    // Walk the graph and check the semantics of the expressions.
    // e.g. Check that the operation supports the input data types.
    // A SqlException will be thrown if an invalid semantic is found.
    static void vValidateExprs(PlanNode* pRootNode);

  private:
    // Validate that all expressions do not have an invalid result type
    static void vValidateExprResultType(PlanNode* pRootNode, QueryNode* pPlanNode);

    // Validate that all predicate expressions give a boolean result type
    static void vValidatePredicatesAreBoolean(PlanNode* pRootNode, QueryNode* pPlanNode);

    // Validate that the aggregate and group-by expressions are correctly specified
    static void vValidateGroupByExprs(PlanNode* pRootNode, QueryNode* pPlanNode);

    // Checks that all non-aggregate exprs being projected are in the group by list
    static void vCheckNonAggExprsAreInGroupBy(QueryNode* pProjectExprs, QueryNode *pGroupByList);

    // Check that the group by expressions cannot be aggregate expressions
    static void vCheckGroupByExprsNotAggregates(QueryNode *pGroupByList);

    // Check that the group by expressions are not literals.
    static void vCheckGroupByExprsNotLiterals(QueryNode *pGroupByList);

    // Validate that if there is no group by list then projections must either be
    // all aggregates or all non-aggregates
    static void vValidateAggsWithNoGroupBy(PlanNode* pRootNode, QueryNode* pPlanNode);

    // Validate that any HAVING exprs are correctly specified
    static void vValidateHavingExprs(PlanNode* pRootNode, QueryNode* pPlanNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

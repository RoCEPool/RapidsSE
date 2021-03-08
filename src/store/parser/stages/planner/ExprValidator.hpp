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

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

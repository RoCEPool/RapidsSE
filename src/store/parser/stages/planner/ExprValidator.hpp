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
    static void vValidateExprResultType(PlanNode* pRootNode, QueryNode* pStartNode);

    // Validate that all case expression predicates have a boolean result type
    static void vValidateCasePredicatesAreBoolean(PlanNode* pRootNode, QueryNode* pStartNode);

    // Validate that all predicate expressions give a boolean result type
    static void vValidateOperatorPredicatesAreBoolean(PlanNode* pRootNode, QueryNode* pStartNode);

    // Validate that all join predicate expressions give a boolean result type
    static void vValidateJoinPredicatesAreBoolean(PlanNode* pRootNode, QueryNode* pStartNode);

    // Validate that all join predicates are equijoins
    static void vValidateJoinPredicatesAreEquiJoins(PlanNode* pRootNode, QueryNode* pStartNode);

    // Validate that the aggregate and group-by expressions are correctly specified
    static void vValidateGroupByExprs(PlanNode* pRootNode, QueryNode* pStartNode);

      // Check that the group by expressions cannot be aggregate expressions
      static void vCheckGroupByExprsNotAggregates(QueryNode *pGroupByList, PlanNode* pRootNode);

      // Check that the group by expressions are not literals.
      static void vCheckGroupByExprsNotLiterals(QueryNode *pGroupByList, PlanNode* pRootNode);

    // Validate that if there is no group by list then projections must either be
    // all aggregates or all non-aggregates
    static void vValidateProjectExprs(PlanNode* pRootNode, QueryNode* pStartNode);

    // If there is a GROUP BY then validate that all sql operators that sit above
    // an AggregateOp and have expressions, those expressions must only refer to
    // aggregated columns or columns that appear in the group by list
    static void vValidateSqlOpExprsAgainstGroupBy(PlanNode* pRootNode, QueryNode* pStartNode);

      // Checks that all non-aggregate exprs being projected are in the group by list
      static void vCheckNonAggExprIsInGroupBy(QueryNode* pExpr, QueryNode *pGroupByList, PlanNode* pRootNode);

    // If there is no GROUP BY then validate the expressions of all sql operators
    // that sit above the ProjectOp. Aggregate exprs may or may not be allowed
    // depending on the expressions in the ProjectOp.
    static void vValidateSqlOpExprsAgainstProjectOp(PlanNode* pRootNode, QueryNode* pStartNode);

    // Validate that any expression subqueries are projecting the right number of columns
    // for the operator they are used in.
    static void vValidateExprSubqueryProjections(PlanNode* pRootNode, QueryNode* pStartNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

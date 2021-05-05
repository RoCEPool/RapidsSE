/*============================================================================*/
/* ProjValidation.hpp - Validates that all expressions only use the projected */
/* columns from SQL operators below it.                                       */
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

  class ProjValidation
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    ProjValidation();

  public:
    // Validates that the right projected columns will be delivered to each
    // SQL operator that has expressions that use or build on those projected
    // columns. i.e. each expression in a SQL operator can be rewritten in
    // an ordinal form relative to the input projected columns.
    static void vValidateExprVisibility(PlanNode* pRootNode);


  private:
    static void vValidateFilterOp(QueryNode* pNode, PlanNode* pRootNode);
    static void vValidateOrderOp(QueryNode* pNode, PlanNode* pRootNode);
    static void vValidateProjectOp(QueryNode* pNode, PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

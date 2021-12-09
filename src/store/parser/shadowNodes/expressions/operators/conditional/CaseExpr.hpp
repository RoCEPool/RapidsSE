/*============================================================================*/
/* CaseExpr.hpp - a CASE expression is a container of WHEN and ELSE exprs     */
/*============================================================================*/
#pragma once

#include "ConditionalExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class CaseExpr : public ConditionalExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    CaseExpr();

  public:
    static QueryNode* pCreate(QueryNode* pElseExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Add a WHEN expression to the start of this CASE expression
    static void vPushWhenExpr(QueryNode* pCaseExpr, QueryNode* pWhenExpr);

    // Add a WHEN expression to the end of this CASE expression
    static void vAppendWhenExpr(QueryNode* pCaseExpr, QueryNode* pWhenExpr);

    // An iterator of WHEN expressions. Must be deleted after use.
    static ConditionalQueryNodeIter* pWhenExprIter(QueryNode* pNode);

    // Retrieve the result expression to be used if the test expression evaluates to true
    static QueryNode* pElseExpr(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static eCT  eType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

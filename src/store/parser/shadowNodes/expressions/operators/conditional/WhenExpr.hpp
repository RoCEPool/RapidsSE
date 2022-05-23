/*============================================================================*/
/* WhenExpr.hpp - an expr test and result value within a CASE expression      */
/*============================================================================*/
#pragma once

#include "ConditionalExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class WhenExpr : public ConditionalExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    WhenExpr();

  public:
    static QueryNode* pCreate(QueryNode* pTestExpr, QueryNode* pResultExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Retrieve the expression to be tested
    static QueryNode* pPredicateExpr(QueryNode* pNode);

    // Retrieve the result expression to be used if the test expression evaluates to true
    static QueryNode* pResultExpr(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    // Modifies subexpressions by promoting/casting data types so that they
    // are type compatible with this expression. Modifies the expression
    // tree in place.
    static void vCoerceSubExprs(QueryNode* pExpr, PlanNode* pRootNode);

    // Tries to coerce the result expression of this WHEN statement to a given type
    static void vCoerceResultExprTo(TypeId xCoercibleTargetId, QueryNode* pWhenExpr, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

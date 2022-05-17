/*============================================================================*/
/* ElseExpr.hpp - final expr value of a CASE expression                       */
/*============================================================================*/
#pragma once

#include "ConditionalExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ElseExpr : public ConditionalExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ElseExpr();

  public:
    static QueryNode* pCreate(QueryNode* pElseExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    static QueryNode* pElseExpr(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);

    // Tries to coerce the result expression of this ELSE statement to a given type
    static void vCoerceResultExprTo(eCT eCoercibleTarget, QueryNode* pElseExpr, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

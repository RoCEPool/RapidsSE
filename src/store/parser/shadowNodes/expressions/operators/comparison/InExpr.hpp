/*============================================================================*/
/* InExpr.hpp - an IN expression operator                                     */
/*============================================================================*/
#pragma once

#include "ComparisonExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class InExpr : public ComparisonExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    InExpr();

  public:
    static QueryNode* pCreate(QueryNode* pLeftExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the expression left of the IN clause that is being compared
    static QueryNode* pLeftExpr(PlanNode* pNode);

    // Returns an iterator starting at the first expression to the right of
    // the IN clause. e.g. leftExpr IN (rightExpr1, rightExpr2, ...).
    // Iterator must be freed after use.
    static PlanNodeIter* pRightExprIter(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

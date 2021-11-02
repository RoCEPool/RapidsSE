/*============================================================================*/
/* BetweenExpr.hpp - a BETWEEN expression operator                            */
/*============================================================================*/
#pragma once

#include "ComparisonExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class BetweenExpr : public ComparisonExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    BetweenExpr();

  public:
    static QueryNode* pCreate(QueryNode* pPredicand, QueryNode* pPattern, QueryNode* pEscape);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // The expression node being compared
    static QueryNode* pPredicand(QueryNode* pNode);

    // The lower bound being compared to
    static QueryNode* pLowerBound(QueryNode* pNode);

    // The upper bound being compared to
    static QueryNode* pUpperBound(QueryNode* pNode);

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

/*============================================================================*/
/* ComparisonExpr.hpp - an abstract expression operator for comparing exprs   */
/*============================================================================*/
#pragma once

#include "OperatorExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ComparisonExpr : public OperatorExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ComparisonExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static eCT  eType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

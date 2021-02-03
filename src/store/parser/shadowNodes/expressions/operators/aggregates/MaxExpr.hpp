/*============================================================================*/
/* MaxExpr.hpp - an MAX() aggregate expression                                */
/*============================================================================*/
#pragma once

#include "AggregateExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class MaxExpr : public AggregateExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    MaxExpr();

  public:
    static QueryNode* pCreate(b1 bIsDistinct, QueryNode* pChildExpr);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AndExpr.hpp - a logical AND expression operator                            */
/*============================================================================*/
#pragma once

#include "LogicExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class AndExpr : public LogicExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    AndExpr();

  public:
    static QueryNode* pCreate(QueryNode* pExpr1, QueryNode* pExpr2);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    static QueryNode* pChild1(QueryNode* pNode);
    static QueryNode* pChild2(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

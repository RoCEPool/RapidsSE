/*============================================================================*/
/* NegationExpr.hpp - an negation (-) expression operator                     */
/*============================================================================*/
#pragma once

#include "ArithmeticExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class NegationExpr : public ArithmeticExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    NegationExpr();

  public:
    static QueryNode* pCreate(QueryNode* pExpr1);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    static QueryNode* pChild1(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

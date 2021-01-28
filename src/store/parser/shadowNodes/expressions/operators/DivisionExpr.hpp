/*============================================================================*/
/* DivisionExpr.hpp - an division (/) expression operator                     */
/*============================================================================*/
#pragma once

#include "ArithmeticExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class DivisionExpr : public ArithmeticExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    DivisionExpr();

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

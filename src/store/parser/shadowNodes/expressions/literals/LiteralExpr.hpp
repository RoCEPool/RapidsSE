/*============================================================================*/
/* LiteralExpr.hpp - a literal expression                                     */
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class LiteralExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    LiteralExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static b1   bIsNull(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* LiteralExpr.hpp - a literal expression                                     */
/*============================================================================*/
#pragma once

#include "Expr.hpp"
#include "PlanNode.hpp"

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
    static QueryNode* pCreate(eET eType, Value xVal);

    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

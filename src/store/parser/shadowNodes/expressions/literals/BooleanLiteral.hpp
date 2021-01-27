/*============================================================================*/
/* BooleanLiteral.hpp - an boolean literal expression                         */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class BooleanLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    BooleanLiteral();

  public:
    static QueryNode* pCreate(b1 bVal);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static b1         bValue(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

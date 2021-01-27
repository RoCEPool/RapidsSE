/*============================================================================*/
/* TimeLiteral.hpp - a time literal expression                                */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TimeLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TimeLiteral();

  public:
    static QueryNode* pCreate(Tme xTme);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static Tme        xValue(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AggregateExpr.hpp - a aggregate expression                                     */
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class AggregateExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    AggregateExpr();

  protected:
    static const Stz* C_DISTINCT_PROP_NAME;
    static const Stz* C_DISTINCT_PROP_TYPE;

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static b1   bIsDistinct(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

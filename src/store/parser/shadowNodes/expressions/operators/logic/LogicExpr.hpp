/*============================================================================*/
/* LogicExpr.hpp - an abstract expression operator for boolean logic exprs    */
/*============================================================================*/
#pragma once

#include "OperatorExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class LogicExpr : public OperatorExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    LogicExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

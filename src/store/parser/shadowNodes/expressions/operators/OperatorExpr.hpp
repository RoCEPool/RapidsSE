/*============================================================================*/
/* OperatorExpr.hpp - an expression operator                                  */
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class OperatorExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    OperatorExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

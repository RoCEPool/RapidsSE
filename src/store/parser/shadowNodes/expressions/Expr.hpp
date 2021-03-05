/*============================================================================*/
/* Expr.hpp - a generic expression                                            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class Expr : public ShadowNode
  {
  private:
    // Shalt never be instantiated. Static functions only.
    Expr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT eType(QueryNode* pExprNode, PlanNode* pRootNode);

    // Returns true if this expression (and therefore any subexpression elements
    // it contains) is an aggregate expression
    static b1 bIsAnAggregate(PlanNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

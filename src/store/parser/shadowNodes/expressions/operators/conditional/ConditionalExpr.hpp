/*============================================================================*/
/* ConditionalExpr.hpp - an abstract operator for conditional logic exprs     */
/*============================================================================*/
#pragma once

#include "OperatorExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ConditionalExpr : public OperatorExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ConditionalExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

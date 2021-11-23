/*============================================================================*/
/* MiscExpr.hpp - an operator for expressions that don't fit anywhere else    */
/*============================================================================*/
#pragma once

#include "OperatorExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class MiscExpr : public OperatorExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    MiscExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static eCT  eType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

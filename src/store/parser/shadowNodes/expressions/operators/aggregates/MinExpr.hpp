/*============================================================================*/
/* MinExpr.hpp - an MIN() aggregate expression                                */
/*============================================================================*/
#pragma once

#include "AggregateExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class MinExpr : public AggregateExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    MinExpr();

  public:
    static QueryNode* pCreate(b1 bIsDistinct, QueryNode* pChildExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static QueryNode* pChild1(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* OrderExprList.hpp - a list of OrderExpr objects                            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class OrderExprList : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    OrderExprList();

  public:
    static QueryNode* pCreate();
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static i8 nExprs(QueryNode* pNode);

    // An iterator of OrderExpr objects
    static PlanNodeIter* pOrderExprIter(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

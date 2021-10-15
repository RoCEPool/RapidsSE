/*============================================================================*/
/* OrderItemList.hpp - a list of OrderItem objects                            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class OrderItemList : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    OrderItemList();

  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static i8 nExprs(QueryNode* pOrderItemList);

    // An iterator of OrderItem objects
    static PlanNodeIter* pOrderItemIter(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

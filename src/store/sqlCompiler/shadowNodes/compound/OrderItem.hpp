/*============================================================================*/
/* OrderItem.hpp - a expression and direction for ordering rows               */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class OrderItem : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    OrderItem();

  public:
    static QueryNode* pCreate(QueryNode* pExpr, b1 bAscendingDirection);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the node that is an expression to order by
    static QueryNode* pExprNode(QueryNode* pNode);

    // Returns true if the ordering should be in ascending order or false for descending order
    static b1 bIsAscending(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

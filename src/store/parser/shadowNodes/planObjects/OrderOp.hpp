/*============================================================================*/
/* OrderOp.hpp - operator that reorders rows based on a list of expressions   */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class OrderOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    OrderOp();

  public:
    static QueryNode* pCreate(QueryNode* pOrderItemList);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Returns the internal node that represents the list of symbolic OrderItem nodes
    // (an OrderItemList object)
    static QueryNode* pOrderItemList(QueryNode* pNode);

    // Returns a new copy of the list of OrderItems (OrderItemList) rewritten relative
    // to column ordinals. Must be freed after use.
    static QueryNode* pOrdinalOrderItemList(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns a new OrderOp node with the ordering terms super-aggregating aggregated
    // expressions from each partition or node
    static QueryNode* pCreateSuperAggregate(QueryNode* pSinglePartitionNode, PlanNode* pRootNode);

    // Returns a new OrderOp node with the ordering terms super-aggregating aggregated
    // expressions from each partition or node. This version used to factor in node-level aggregation.
    static QueryNode* pCreateDoubleSuperAggregate(QueryNode* pSinglePartitionNode, PlanNode* pRootNode);

    // Returns true if any of the ordering terms contain an aggregate expression
    static b1 bContainsAnAggregate(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

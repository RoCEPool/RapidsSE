/*============================================================================*/
/* ProjectedColumn.hpp - a projected expression that may have an alias        */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class ProjectedColumn : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ProjectedColumn();

  public:
    static Stz*       szAliasName(QueryNode* pQueryNode);
    static QueryNode* pCreate(QueryNode* pExprNode, Text* pAlias);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf, QueryNodeList* pColumnExprList,
                                QueryNode* pCorrelatedExprs, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if the expression in this node is an aggregate expression.
    static b1 bContainsAnAggregate(QueryNode* pNode);

    // Returns true if the expression in this node is literal.
    static b1 bIsFullyLiteral(QueryNode* pNode);

    // Returns the node that represents the expression
    static QueryNode* pExprNode(QueryNode* pQueryNode);

    // Returns a new copy of this projected expression rewritten relative to column ordinals.
    // Must be freed after use.
    static QueryNode* pSymbolicToOrdinal(QueryNode*     pQueryNode,
                                         QueryNodeList* pColExprList,
                                         QueryNode*     pCorrelatedExprs,
                                         PlanNode*      pRootNode);

    // Changes the alias for this projected column
    static void vSetAlias(QueryNode* pNode, Text* pNewAlias);

    // If the given projectedColumn requires partitioning then a non-null
    // QueryNodeList is returned which contains those partitioned
    // projected columns.
    static QueryNodeList* pPartitionAggregates(QueryNode* pNode);

    // Changes the aggregate expression in pNode to super-aggregate results from each partition/node
    static void vSuperAggregate(QueryNode* pNode, PlanNode* pRootNode);

    // Changes the aggregate expression in pNode to super-aggregate results from each partition/node.
    // This version super-aggregates twice to factor in node-level aggregation.
    static void vDoubleSuperAggregate(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

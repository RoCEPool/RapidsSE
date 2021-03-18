/*============================================================================*/
/* ProjectedExpr.hpp - a projected expression that may have an alias          */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class ProjectedExpr : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ProjectedExpr();

  public:
    static Stz*       szAliasName(QueryNode* pQueryNode);
    static QueryNode* pCreate(QueryNode* pExprNode, Text* pAlias);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf, PlanNodeList* pColumnExprList, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if the expression in this node is an aggregate expression.
    static b1 bIsAnAggregate(QueryNode* pQueryNode);

    // Returns the node that represents the expression
    static QueryNode* pExprNode(QueryNode* pQueryNode);

    // Returns a new copy of this projected expression rewritten relative to column ordinals.
    // Must be freed after use.
    static QueryNode* pSymbolicToOrdinal(QueryNode* pQueryNode,
                                         PlanNodeList* pColExprList,
                                         PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

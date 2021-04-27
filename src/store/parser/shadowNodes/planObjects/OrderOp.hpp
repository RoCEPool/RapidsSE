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
    static QueryNode* pCreate(QueryNode* pOrderExprList);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the internal node that represents the list of symbolic OrderExpr nodes
    // (an OrderExprList object)
    static QueryNode* pOrderExprList(QueryNode* pNode);

    // Returns a new copy of the list of OrderExprs (OrderExprList) rewritten relative
    // to column ordinals. Must be freed after use.
    static QueryNode* pOrdinalOrderExprList(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

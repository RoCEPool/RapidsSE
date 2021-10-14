/*============================================================================*/
/* FilterOp.hpp - operator that removes rows that don't match the condition   */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class FilterOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    FilterOp();

  public:
    static QueryNode* pCreate(QueryNode* pFilterExpr);
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

    // Returns the internal symbolic filterExpr node of the filterOp
    static QueryNode* pFilterExpr(QueryNode* pNode);

    // Returns a new copy of the filter expression rewritten relative to column ordinals.
    // Must be freed after use.
    static QueryNode* pOrdinalFilterExpr(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

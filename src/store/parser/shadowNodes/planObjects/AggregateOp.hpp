/*============================================================================*/
/* AggregateOp.hpp - operator that extends ProjectOp to add a groupBy list    */
/*============================================================================*/
#pragma once

#include "ProjectOp.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class AggregateOp : public ProjectOp
  {
  private:
    // Shalt never be instantiated. Static functions only.
    AggregateOp();

  public:
    static QueryNode* pCreate(QueryNodeList* pSelectExprs, QueryNodeList* pGroupByExprs);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this given type only
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Returns the number of groupby expressions
    static i8 nGroups(QueryNode* pNode);

    // Returns the internal node that represents a list of groupby expressions. Each groupby
    // expression can be iterated by calling pNewChildrenIter() on the returned node
    static QueryNode* pGroupByList(QueryNode* pNode);

    // Returns a copy of the node that represents a list of groupby expressions. Each groupby
    // expression can be iterated by calling pNewChildrenIter() on the returned node.
    // Must be freed after use.
    static QueryNode* pOrdinalGroupByList(QueryNode* pNode, PlanNode* pRootNode);

    // Returns a new copy of the projected expressions rewritten in ordinal
    // orientation (where possible) where the ordinals are relative to group by
    // list. Not all columns will be expressed relative to the group by list and
    // so they will retain use of ColumnRefs.
    // Must be freed after use.
    static QueryNode* pOrdinalProjectedColumnsRelativeToGroupBy(QueryNode* pNode, PlanNode* pRootNode);

    // Merges a ProjectOp, pOtherOp, into this AggregateOp
    static void vSquash(QueryNode* pNode, QueryNode* pOtherOp, PlanNode* pRootNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this given type only
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the number of groupby expressions
    static i8 nGroups(QueryNode* pNode);

    // Returns the internal node that represents a list of groupby expressions. Each groupby
    // expression can be iterated by calling pNewChildrenIter() on the returned node
    static QueryNode* pGroupByList(QueryNode* pNode);

    // Returns a copy of the node that represents a list of groupby expressions. Each groupby
    // expression can be iterated by calling pNewChildrenIter() on the returned node.
    // Must be freed after use.
    static QueryNode* pOrdinalGroupByList(QueryNode* pNode, PlanNode* pRootNode);



  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

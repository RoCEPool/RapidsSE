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

    // Returns the number of groupby expressions
    static i8 nGroups(QueryNode* pNode);

    // Return an iterator to the groupby expression list. Must be freed afterwards.
    static PlanNodeIter* pGroupByIter(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

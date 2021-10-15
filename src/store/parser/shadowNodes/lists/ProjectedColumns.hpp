/*============================================================================*/
/* ProjectedColumns.hpp - a list of ProjectedColumn instances                 */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ProjectedColumns : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ProjectedColumns();

  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if the given expression is equal to or a superset of
    // one of the projected columns
    static b1 bHasExprEqualOrSuperset(QueryNode* pNode, QueryNode* pExpr, b1 bAllowAggregateSupersets);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

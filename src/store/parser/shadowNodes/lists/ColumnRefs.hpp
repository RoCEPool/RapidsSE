/*============================================================================*/
/* ColumnRefs.hpp - a list of references to columns                           */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnRefs : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ColumnRefs();

  public:
    static QueryNode* pCreate();
    static void       vAddColumnRef(QueryNode* pNode, QueryNode* pColumnRef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static i8 nColumns(QueryNode* pColumnRefs);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* ProjectedExpr.hpp - a projected expression that may have an alias          */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ProjectedExpr : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ProjectedExpr();

  public:
    static QueryNode* pCreate(QueryNode* pExprNode, Text* pAlias);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

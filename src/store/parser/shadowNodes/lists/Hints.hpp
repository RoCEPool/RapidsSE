/*============================================================================*/
/* Hints.hpp - a list of query hints                                          */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class Hints : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    Hints();

  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static void vAddHint(QueryNode* pNode, QueryNode* pHint);
    static i8 nHints(QueryNode* pHints);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* ShadowRoot.hpp - a shadow node of the root of the plan graph               */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ShadowRoot : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ShadowRoot();

  public:
    static void vDescribe(PlanNode *pNode, s2K &sBuf, PlanNode *pRootNode);
    static void vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static QueryNode* pMetadataNode(PlanNode* pNode);
    static QueryNode* pPlanRoot(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

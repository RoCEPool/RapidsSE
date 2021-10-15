/*============================================================================*/
/* LocalNode.hpp - Send data to a common stage on the same node               */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/types/routing/Routing.hpp"


namespace Parser
{
  class PlanNode;
  class QueryNode;

  class LocalNode : public Routing
  {
  private:
    // Shalt never be instantiated. Static functions only.
    LocalNode();


  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Types.hpp - an abstract base class of custom types used in the plan        */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class Types : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    Types();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

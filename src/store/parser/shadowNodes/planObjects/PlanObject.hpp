/*============================================================================*/
/* PlanObject.hpp - an abstract base class of all object that appear in the   */
/* main data path of the plan graph.                                          */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class PlanNode;

  class PlanObject : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    PlanObject();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

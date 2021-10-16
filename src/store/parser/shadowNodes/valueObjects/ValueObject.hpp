/*============================================================================*/
/* ValueObject.hpp - a base class simple object that contains a primitive value*/
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class ValueObject : public ShadowNode
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ValueObject();

  public:
    // There is no pCreate() function as this is a base class.

    // Describe the object and write it into the buffer
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

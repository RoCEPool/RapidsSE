/*============================================================================*/
/* StringObject.hpp - a simple object that contains a string value            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class StringObject : public ShadowNode
  {
  private:
    // Shalt never be instantiated. Static functions only.
    StringObject();

  public:
    static QueryNode* pCreate(Text* pText);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

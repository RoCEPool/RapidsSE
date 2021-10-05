/*============================================================================*/
/* StringObject.hpp - a simple object that contains a string value            */
/*============================================================================*/
#pragma once

#include "ValueObject.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class StringObject : public ValueObject
  {
  private:
    // Shalt never be instantiated. Static functions only.
    StringObject();

  public:
    static QueryNode* pCreate(Text* pText);
    static QueryNode* pCreate(Stz*  szText);

    // Describe the object and write it into the buffer
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

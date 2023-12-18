/*============================================================================*/
/* DecimalObject.hpp - a simple object that contains a decimal value          */
/*============================================================================*/
#pragma once

#include "ValueObject.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class DecimalObject : public ValueObject
  {
  private:
    // Shalt never be instantiated. Static functions only.
    DecimalObject();

  public:
    static QueryNode* pCreate(Bcd xVal);

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

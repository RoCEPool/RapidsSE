/*============================================================================*/
/* StageRange.hpp - an object that identifies a range of SQL compiler stages  */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;

  class StageRange : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    StageRange();

  public:
    static QueryNode* pCreate(QueryNode* pBegin, QueryNode* pEnd);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);



  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

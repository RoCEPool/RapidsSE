/*============================================================================*/
/* StringList.hpp - a list of string value nodes                              */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class StringList : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    StringList();

  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if this string list contains a given string value
    static b1 bContains(QueryNode* pNode, b1 bCaseSensitive, Stz* szVal);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

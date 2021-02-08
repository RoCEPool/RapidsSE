/*============================================================================*/
/* IndexDefs.hpp - a list of index definitions                                */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class IndexDefs : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    IndexDefs();

  public:
    static QueryNode* pCreate();
    static void       vAddIndexDef(QueryNode* pNode, QueryNode* pIndexDef);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static i8 nIndexes(QueryNode* pIndexDefs);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

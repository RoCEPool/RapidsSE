/*============================================================================*/
/* TableRef.hpp - a reference to a named table                                */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TableRef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableRef();

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static b1         bIsResolved(QueryNode* pTableRef);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

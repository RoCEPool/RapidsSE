/*============================================================================*/
/* TableDefs.hpp - a list of tables definitions                               */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TableDefs : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableDefs();

  public:
    static QueryNode* pCreate();
    static void       vAddTableDef(QueryNode* pNode, QueryNode* pTableDef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static i8 nTables(QueryNode* pTableDefs);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

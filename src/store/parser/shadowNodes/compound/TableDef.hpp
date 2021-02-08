/*============================================================================*/
/* TableDef.hpp - a definition of a table used in the plan                    */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TableDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableDef();

  public:
    static QueryNode* pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                              QueryNode* pColumnDefs, QueryNode* pIndexDefs);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static Stz* szTableName(QueryNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

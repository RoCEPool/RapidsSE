/*============================================================================*/
/* TableDef.hpp - a definition of a table used in the plan                    */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class TableDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableDef();

  public:
    static QueryNode* pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                              QueryNode* pColumnDefs, QueryNode* pIndexDefs, QueryNode* pPartitionDef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static Stz* szTableName(QueryNode* pNode);

    static QueryNode* pColumnDef(QueryNode* pNode, Stz* szColumnName);
    static PlanNodeIter* pColumnDefIter(QueryNode* pNode);

    static QueryNode* pIndexDef(QueryNode* pNode, Stz* szIndexName);
    static PlanNodeIter* pIndexDefIter(QueryNode* pNode);

    static QueryNode* pPartitionDef(QueryNode* pNode);
    static PlanNodeIter* pPartitionDefIter(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

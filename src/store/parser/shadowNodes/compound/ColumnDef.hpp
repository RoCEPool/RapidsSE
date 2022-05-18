/*============================================================================*/
/* ColumnDef.hpp - a definition of a column used in the plan                  */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "SqlType.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ColumnDef();

  public:
    // Helper function to create columnDef nodes
    // Ordinal is 0 based. Use -1 for precision and scale if not set or n/a
    static QueryNode* pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                              Stz* szColumnName, QueryNode* pTypeNode, b1 bNotNull,
                              i8 iOrdinal, i8 iPrecision, i8 iScale);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static Stz* szTableName(QueryNode* pNode);
    static Stz* szColumnName(QueryNode* pNode);
    static i8   iColumnType(QueryNode* pNode);
    static b1   bNotNull(QueryNode* pNode);
    static i8   iOrdinal(QueryNode* pNode);   // 0 based column number
    static i8   iPrecision(QueryNode* pNode); // returns -1 if not set or n/a
    static i8   iScale(QueryNode* pNode);     // returns -1 if not set or n/a

    static void vSetColumnName(QueryNode* pNode, Stz* szNewName);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

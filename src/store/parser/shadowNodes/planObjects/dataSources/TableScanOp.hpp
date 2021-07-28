/*============================================================================*/
/* TableScanOp.hpp - an operator that linearly reads all rows in a table      */
/*============================================================================*/
#pragma once

#include "DataSource.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class TableScanOp : public DataSource
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TableScanOp();

  public:
    static QueryNode* pCreate(QueryNode* pTableRef);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    static QueryNode* pTableRef(QueryNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static Stz* szTableName(QueryNode* pNode);

    // Get the list of column definitions for this data source
    // Returned list must be freed, but not its contents.
    static QueryNodeList* pColumnDefs(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if this data source can resolve the given column reference
    static b1 bResolvesColumnRef(QueryNode* pNode, QueryNode* pMetadataNode,
                                 Stz* szColRefTableAliasName,
                                 Stz* szCatalogName, Stz* szSchemaName,
                                 Stz* szTableName, Stz* szColumnName);

    // Prints into the buffer the identifier for the data source.
    // If an alias is set then that is used. Otherwise it could be a table name.
    static void vPrintName(QueryNode* pNode, s2K& sBuf);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

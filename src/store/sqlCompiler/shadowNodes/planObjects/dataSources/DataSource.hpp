/*============================================================================*/
/* DataSource.hpp - a source of row data into the query                       */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class DataSource : public SqlOperator
  {

  private:
    // Shalt never be instantiated. Static functions only.
    DataSource();

  public:
    static void vDescribe(PlanNode *pNode, s2K &sBuf, PlanNode *pRootNode);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Alias operations, if applicable. Has/set/get
    static b1   bHasAlias(QueryNode* pNode);
    static void vSetAlias(QueryNode* pNode, Text* pAliasText);
    static Stz* szAliasName(QueryNode* pNode);

    // Get the catalog/schema/table name of the data source, if applicable
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
                                 PlanNode* pRootNode, Stz* szColRefTableAliasName,
                                 Stz* szCatalogName,  Stz* szSchemaName,
                                 Stz* szTableName,    Stz* szColumnName);

    // Prints into the buffer the identifier for the data source.
    // If an alias is set then that is used. Otherwise it could be a table name.
    static void vPrintName(QueryNode* pNode, s2K& sBuf);

    // Returns true if this operator (and any expressions it may have) should be
    // partitioned when the plan is partitioned. Most operators will simply ask
    // this question of their child until we get to a data source. Most tables
    // would answer true. A replicated table or data source would answer false.
    static b1 bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

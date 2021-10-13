/*============================================================================*/
/* SubqueryRefOp.hpp - references a table subquery elsewhere in the plan      */
/*============================================================================*/
#pragma once

#include "DataSource.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class SubqueryRefOp : public DataSource
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubqueryRefOp();

  public:
    static QueryNode* pCreate(Stz* szSubqueryAlias);
    static QueryNode* pCreate(Text* pSubqueryAlias);
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Return the node that this SubqueryRefOp is referencing
    static QueryNode* pSubquery(QueryNode* pNode, PlanNode* pRootNode);

    // Get the list of column definitions for this data source
    // Returned list must be freed, but not its contents.
    static QueryNodeList* pColumnDefs(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if this data source can resolve the given column reference
    static b1 bResolvesColumnRef(QueryNode* pNode,    QueryNode* pMetadataNode,
                                 PlanNode* pRootNode, Stz* szColRefTableAliasName,
                                 Stz* szCatalogName,  Stz* szSchemaName,
                                 Stz* szTableName,    Stz* szColumnName);

    // Returns true if this operator (and any expressions it may have) should be
    // partitioned when the plan is partitioned. Most operators will simply ask
    // this question of their child until we get to a data source. Most tables
    // would answer true. A replicated table or data source would answer false.
    static b1 bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode);

  protected:

    // Return the CteDef node that represents this subquery's definition.
    // If psPathBuf is not null then the path of the CteDef is written into it.
    static QueryNode* pDefnNode(QueryNode* pNode, PlanNode* pRootNode, s2K* psPathBuf = nullptr);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

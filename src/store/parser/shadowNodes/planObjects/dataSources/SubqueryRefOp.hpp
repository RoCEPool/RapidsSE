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
    static QueryNode* pCreate(Stz* pSubqueryAlias);
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

    // Returns the name of the virtual table that the table subquery is known as
    static Stz* szAliasName(PlanNode* pNode);

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


    // TODO: add a method to look up and return the QueryNode* of the referenced subquery
    //       static QueryNode* pSubqueryNode(QueryNode* pNode);

  protected:

    // Return the CteDef node that represents this subquery's definition.
    static QueryNode* pDefnNode(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

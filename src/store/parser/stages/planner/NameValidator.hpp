/*============================================================================*/
/* NameValidator.hpp - Validates the names of objects used in the plan.       */
/*                     e.g. tables, columns, indexes, etc                     */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class QueryNode;

  class NameValidator
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    NameValidator();

  public:
    // Walk the graph and look up all names encountered in the system metadata.
    // This function will fill in any unresolved name parts as well as populate
    // the plan with metadata.
    // A SqlException will be thrown if a name cannot be resolved.
    static void vValidateNames(PlanNode* pRootNode);

  private:
    // Walks the graph and validates table subqueries and main subqueries only.
    static void vValidateTableAndMainSubqueries(QueryNode* pStartingNode, PlanNode* pRootNode,
                                                QueryNode* pMetadataNode, STORE::SeApi* pSeApi);

    // Walk the graph and validate expression subqueries and any subqueries they contain.
    static void vValidateExprSubqueries(QueryNode* pStartingNode, PlanNode* pRootNode,
                                        QueryNode* pMetadataNode, STORE::SeApi* pSeApi);

    // Validates a table subquery only. We separate validation as we descend
    // the graph (top-down) and when we ascend back up (bottom-up).
    static void vValidateTableSubqueryEntry(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                            PlanNode* pRootNode, QueryNode* pMetadataNode,
                                            STORE::SeApi* pSeApi);

    // Completes the validation of the table subquery using main subquery col info
    static void vValidateTableSubqueryExit(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                           PlanNode* pRootNode, QueryNode* pMetadataNode,
                                           STORE::SeApi* pSeApi);

    // Validates a main subquery only
    static void vValidateMainSubquery(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                      PlanNode* pRootNode, QueryNode* pMetadataNode,
                                      STORE::SeApi* pSeApi);

    // Validates an expression subquery only
    static void vValidateExprSubquery(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                      PlanNode* pRootNode, QueryNode* pMetadataNode,
                                      STORE::SeApi* pSeApi);

    // Validate that all table names can be found in the system metadata.
    // Populate the plan metadata at the same time.
    static void vValidateTables(PlanNode* pRootNode, QueryNode* pStartNode,
                                QueryNode* pEnclosingSubqueryNode,
                                QueryNode* pMetadataNode, STORE::SeApi* pSeApi,
                                QueryNodeList* pResolvableSubqueries);

    // Tries to resolve a table name to a table subquery. If successful, returns true
    // and writes the changes to pOpReplacmentList.
    static b1 bResolveTableNameToSubquery(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                                          QueryNodeList* pResolvableSubqueries, s2K& sPath,
                                          QueryNode* pStartNode, QueryNodeList* pOpReplacementList);

    // Tries to resolve a table name to a base table. Returns true if successful
    // and qualifies the tableRef.
    // Will throw an exception if the name is ambiguous within base tables.
    static b1 bResolveTableNameToBaseTable(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                                           QueryNode* pTableRef, QueryNode* pMetadataNode, SeApi* pSeApi);

    // Convert any column wildcards into column references.
    static void vResolveColumnWildcards(PlanNode* pRootNode, QueryNode* pStartNode,
                                        QueryNode* pEnclosingSubqueryNode, QueryNode* pMetadataNode);

    // Look up and return a list of columns that match a column wildcard
    // for a given location in the plan hierarchy.
    static QueryNodeList* pLookupWildcardColumns(QueryNode* pStartNode, PlanNode* pRootNode,
                                                Stz* szCatalog, Stz* szSchema, Stz* szTable);

    // Validate that all column references can be found in the plan metadata.
    static void vValidateColumns(PlanNode* pRootNode, QueryNode* pStartNode,
                                 QueryNode* pEnclosingSubqueryNode, QueryNode* pMetadataNode);

    // Return a list of TableDef nodes that are reachable from a given sql operator
    // and that contain a column as referenced by pColumnRef.
    static QueryNodeList* pMatchingDataSourcesFor(QueryNode* pColumnRef,
                                                  QueryNode* pSqlOp,
                                                  PlanNode*  pRootNode,
                                                  QueryNode* pMetadataNode);

    // Return a list of ProjectedColumn nodes that are reachable from a given sql operator
    // and that contain a given alias name. Need to free the list and nodes returned.
    static QueryNodeList* pMatchingExprAliasesFor(Stz* szAlias,
                                                  QueryNode* pSqlOp,
                                                  PlanNode* pRootNode);


    // Assert that all tables and columns are fully resolved and that there are
    // no columnWildcards left in the plan
    static void vAssertNamesResolved(PlanNode* pRootNode, QueryNode* pPlanNode);

    // Validate the column names of any join USING clauses
    static void vValidateJoinUsingColumns(PlanNode* pRootNode, QueryNode* pStartNode);

    // Replace the USING list with a join predicate in a JoinOp
    static void vReplaceJoinUsingColumn(PlanNode* pRootNode, QueryNode* pJoinOp, QueryNode* pUsingList);

    // Tries to resolve a column name in a given projected column list.
    // Returns a ColumnRef/AliasRef that can be used to form a join predicate.
    static QueryNode* pResolveUsingColumn(Stz* szUsingCol, QueryNodeList* pProjColList);

    // Given a subquery node and its path, return a list of subqueries that are resolvable.
    // The returned list contains pointers to SubqueryMarker nodes.
    // The returned list must be freed, but not its contents. List may be null.
    static QueryNodeList* pResolvableSubqueries(QueryNode* pSubqueryNode, s2K& sSubqueryPath, PlanNode* pRootNode);

    // Given a root node and a path to a subquery marker, try to find the parent
    // subquery marker and return that node, if one exists.
    static QueryNode* pFindEnclosingSubqueryDefn(PlanNode* pPlanRoot, s2K& sSubqueryPath, PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

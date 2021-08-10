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
    // Validate that all table names can be found in the system metadata.
    // Populate the plan metadata at the same time.
    static void vValidateTables(PlanNode* pRootNode, QueryNode* pStartNode,
                                QueryNode* pMetadataNode, STORE::SeApi* pSeApi);

    // Convert any column wildcards into column references.
    static void vResolveColumnWildcards(PlanNode* pRootNode, QueryNode* pStartNode,
                                        QueryNode* pMetadataNode);

    // Look up and return a list of columns that match a column wildcard
    // for a given location in the plan hierarchy.
    static QueryNodeList* pLookupWildcardColumns(QueryNode* pStartNode, PlanNode* pRootNode,
                                                Stz* szCatalog, Stz* szSchema, Stz* szTable);

    // Validate that all column references can be found in the plan metadata.
    static void vValidateColumns(PlanNode* pRootNode, QueryNode* pStartNode,
                                QueryNode* pMetadataNode);

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
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

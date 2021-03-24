/*============================================================================*/
/* NameValidator.hpp - Validates the names of objects used in the plan.       */
/*                     e.g. tables, columns, indexes, etc                     */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"

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
    static PlanNodeList* pLookupWildcardColumns(QueryNode* pStartNode, PlanNode* pRootNode,
                                                Stz* szCatalog, Stz* szSchema, Stz* szTable);

    // Validate that all column references can be found in the plan metadata.
    static void vValidateColumns(PlanNode* pRootNode, QueryNode* pStartNode,
                                QueryNode* pMetadataNode);

    // Assert that all tables and columns are fully resolved and that there are
    // no columnWildcards left in the plan
    static void vAssertNamesResolved(PlanNode* pRootNode, QueryNode* pPlanNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

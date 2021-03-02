/*============================================================================*/
/* NameValidator.hpp - Validates the names of objects used in the plan.       */
/*                     e.g. tables, columns, indexes, etc                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class PlanNode;
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
    static void vValidateTables(PlanNode* pRootNode, QueryNode* pPlanNode,
                                QueryNode* pMetadataNode, STORE::SeApi* pSeApi);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

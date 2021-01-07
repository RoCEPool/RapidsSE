/*============================================================================*/
/* GraphEditor.hpp - a utility for editing plan graphs                        */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanNode;

  class GraphEditor
  {
  private:
    GraphEditor();

  public:

    // Create a new copy of the plan graph using the template
    static PlanNode* pNewFromTemplate(PlanNode* pRootNode);

    // Validate the entire plan, including definitions
    static b1 bValidateAll(PlanNode* pRootNode);

    // Validate only the /plan section of the plan (ignoring definitions & primitives)
    static b1 bValidatePlan(PlanNode* pPlanNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

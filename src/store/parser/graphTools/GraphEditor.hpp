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

    // TODO: Add extra methods here for manipulating plan graphs.
    //       e.g. snipping nodes, reordering, etc.

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

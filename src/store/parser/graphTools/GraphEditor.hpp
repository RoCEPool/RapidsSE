/*============================================================================*/
/* GraphEditor.hpp - a utility for editing plan graphs                        */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  // The purpose of this class is to be a utility add-on class for PlanNode.
  // It will contain more advanced operations, such as editing of graphs,
  // so that the PlanNode class is kept simpler and isn't filled up with all
  // of this lesser-used code.

  class GraphEditor
  {
  private:
    GraphEditor();

  public:

    // Create a new copy of the plan graph using the template
    static PlanNode* pNewFromTemplate(const PlanNode* pTemplateRoot);

    // Remove and replace pReplaceChild in parent node pParentNode with the
    // nodes in pReplacementList.
    static void vReplaceChild(QueryNode* pParentNode, QueryNode* pReplaceChild,
                              QueryNodeList* pReplacementList);

    // Remove a child from its parent and optionally transplant that child's
    // children in its place. Returns the snipped child.
    static QueryNode* pSnipChild(QueryNode* pParent, QueryNode* pChild, b1 bTransplant);

    // TODO: Add extra methods here for manipulating plan graphs.
    //       e.g. snipping nodes, reordering, etc.

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

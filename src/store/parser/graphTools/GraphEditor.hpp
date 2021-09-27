/*============================================================================*/
/* GraphEditor.hpp - a utility for editing plan graphs                        */
/*============================================================================*/
#pragma once

#include <functional> // for std::function
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

    // Remove and replace pExistingChild in parent node pParentNode with pReplacementNode.
    // Deletes pExistingChild.
    static void vReplaceChild(QueryNode* pParentNode, QueryNode* pExistingChild,
                              QueryNode* pReplacementNode);

    // Remove and replace pExistingChild in parent node pParentNode with the
    // nodes in pReplacementNode.
    static void vReplaceChild(QueryNode* pParentNode, QueryNode* pExistingChild,
                              QueryNodeList* pReplacementNode);

    // Remove a child from its parent and optionally transplant that child's
    // children in its place. Returns the snipped child.
    static QueryNode* pSnipChild(QueryNode* pParent, QueryNode* pChild, b1 bTransplant);

    // Remove child nodes from its parent if the child nodes meet some condition.
    // Optionally transplant the child's children in its place.
    // Returns nullptr if no children were replaced, otherwise a list of the
    // replaced children is returned. List and children need to be deleted.
    // The condition that is evaluated is passed the child node being tested
    // and must return true if the child is to be snipped, or false it it
    // is to be left alone.
    static QueryNodeList* pSnipChild(QueryNode* pParent, std::function<b1(QueryNode*)> fCondition, b1 bTransplant);


    // Connects two distinct chains of nodes (nodes with at most 1 child each)
    // by making pSecond a child of the leaf node of pFirst.
    // i.e. navigate down the child hierarchy of pFirst to find the leaf node
    // and make pSecond a child of it.
    // e.g. if pFirst was /a/b/c and pSecond was /d/e then pFirst would end up
    // as /a/b/c/d/e
    static void vConnectChains(QueryNode* pFirst, QueryNode* pSecond);

    // Search for pExistingNode in the graph, starting at pStartingNode, and replace
    // it with pNewNode. pExistingNode must exist in the graph.
    static void vReplaceNode(PlanNode* pStartingNode, QueryNode* pExistingNode, QueryNode* pNewNode);
    // TODO: Add extra methods here for manipulating plan graphs.
    //       e.g. snipping nodes, reordering, etc.

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* GraphNav.hpp - a utility for walking plan graphs                           */
/*============================================================================*/
#pragma once
#include <functional> // for std::function

namespace Parser
{
  class PlanNode;
  class DefinitionNode;
  class PrimitiveNode;
  class QueryNode;

class GraphNav
  {

  private:
    // Not to be instantiated
    GraphNav();

  public:

    /*
     * Notes:
     *
     * The plan graph of a query can be comprised of multiple distinct logical sub-graphs
     * that represent different parts of the query.
     * Some of these sub-graphs may be very different or they may be almost identical.
     * e.g. an INSERT..SELECT statement comprises a SELECT sub-graph and an
     * INSERT sub-graph. A UNIONed SELECT statement comprises of the UNION sub-graph
     * and two SELECT sub-graphs.
     *
     * These logical sub-graphs are separated by StatementMarker objects (please refer
     * to the PlannerDef.json5 definition file). These StatementMarker objects identify
     * what a logical sub-graph does (is it a SELECT statement, INSERT statement, etc).
     *
     * A logical sub-graph is bounded by a StatementMarker as its root. It's leaves are
     * bounded by the physical leaf nodes of the graph, or another StatementMarker node.
     *
     * When we navigate the graph we can either choose to navigate the entire graph
     * or we can navigate one of these logical sub-graphs only. We refer to navigating
     * the entire graph as a global operation whereas navigating a sub-graph is a
     * bounded operation. So a global walk of the graph will visit every node regardless
     * of its type. A bounded walk of the graph will stop when it hits a StatementMarker
     * node or the end of the graph (whichever comes first).
     *
     * This named distinction in operations is used in the API that follows.
     */


    // Returns the leaf element of a type definition path.
    // e.g. given a definition path of /a/b/c this will return 'c'.
    static Stz* szLeafType(Stz* szTypePath);

    // Returns a string that represents the separator used between the leaf node
    // and its parent. Returns either C_PROPERTY_SEPARATOR_STZ,
    // C_CHILD_SEPARATOR_STZ or nullptr in the case of an invalid path.
    static Stz* szLeafSeparator(Stz* szPath);

  // Returns the parent of the type definition path given.
    static void vParentType(Stz* szTypePath, s2K& buf);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // -----------------------------------
    // Methods for debugging a plan graph.
    // -----------------------------------

    // Print a graph structure to the console, starting from some node
    static void vPrintPlan(PlanNode* pRootNode);
    static void vPrintCteMetadata(PlanNode* pRootNode);

    static void vPrintNode(PlanNode* pNode);
    static void vPrintNode(PlanNode* pNode, b1 bPrintProperties);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // ---------------------------------
    // Methods for walking a plan graph.
    // ---------------------------------

    // An enum return value so that the visitor can control the traversal to some degree.
    enum eWalkCtrl {
      WALK_NONE = 0,
      WALK_CONTINUE,        // Keep walking.
      WALK_ABORT,           // Abort all further walking of nodes in the graph.
      WALK_SKIP_SUBTREE,    // Skip all properties and children. Not applicable on a bottom-up traversal.
      WALK_SKIP_PROPERTIES, // Skip properties only. Not applicable on a bottom-up traversal.
      WALK_SKIP_CHILDREN,   // Skip children only. Not applicable on a bottom-up traversal.
      WALK_NENUMS
    };

    // An enum passed to the callback for the event-based walk implementation
    enum eWalkEvent {
      EVENT_NONE = 0,
      EVENT_ENTER,    // Entering a node before processing its properties/children
      EVENT_EXIT,     // Leaving a node after processing its properties/children
      EVENT_NENUMS
    };

    // The following functions require a visitor function with the definition:
    //   eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
    // Params:
    //   pNode - the current node being walked
    //   sPath - the path of the current node being walked
    //   iDepth - the depth of the current node, where the first node has
    //            a depth of 0.
    //   pvArg - an optional argument passed to the walk function
    // The return value controls whether walking continues, skips a subtree
    // or aborts altogether.

    // Walk the current node and (potenitially) its properties and children,
    // calling the visitor function for each node encountered. This is a
    // top-down, depth-first traversal. Visitor implementation (fVisitor is
    // called only once for each node visited in the graph).
    // Returns false if the walk was aborted or true if it completed normally.
    // A global walk traverses all nodes in the graph.
    static b1 bGlobalWalk(PlanNode* pNode,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                          void* pvArg);
    static b1 bGlobalWalk(PlanNode* pNode,
                          b1 bWalkProperties,
                          b1 bWalkChildren,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                          void* pvArg);

    // Event-based implementation of bGlobalWalk. For each node in the graph
    // we call fEnterEvent before processing any properties/children and then
    // call fExitEvent after processing any properties/children.
    // Note that the callback definition is slightly different to the visitor-
    // implementation above.
    static b1 bGlobalWalk(PlanNode* pNode,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                          void* pvArg);
    static b1 bGlobalWalk(PlanNode* pNode,
                          b1 bWalkProperties,
                          b1 bWalkChildren,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                          void* pvArg);


    // As per bGlobalWalk() above except a bounded walk will stop when it hits a
    // PlanMarker node as this represents the start of another logical sub-graph.
    // Starting on a PlanMarker will walk until the subsequent PlanMarker
    // node is encountered.
    static b1 bBoundedWalk(PlanNode* pNode,
                           std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                           void* pvArg);
    static b1 bBoundedWalk(PlanNode* pNode,
                           b1 bWalkProperties,
                           b1 bWalkChildren,
                           std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                           void* pvArg);

    // Event-based implementation of bBoundedWalk. For each node in the graph
    // we call fEnterEvent before processing any properties/children and then
    // call fExitEvent after processing any properties/children.
    // Note that the callback definition is slightly different to the visitor-
    // implementation above.
    static b1 bBoundedWalk(PlanNode* pNode,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                          void* pvArg);
    static b1 bBoundedWalk(PlanNode* pNode,
                          b1 bWalkProperties,
                          b1 bWalkChildren,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                          void* pvArg);

    // Walk the current node and (potenitially) its properties and children,
    // calling the visitor function for each node encountered. This is a
    // bottom-up, depth-first traversal.
    // Returns false if the walk was aborted or true if it completed normally.
    // A global walk traverses all nodes in the graph.
    static b1 bGlobalWalkBottomUp(PlanNode* pNode, std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg);
    static b1 bGlobalWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                                  std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg);

    // As per bGlobalWalkBottomUp() above except a bounded walk will stop when it hits a
    // PlanMarker node as this represents the start of another logical sub-graph.
    // Starting on a PlanMarker will walk until the subsequent PlanMarker
    // node is encountered.
    static b1 bBoundedWalkBottomUp(PlanNode* pNode, std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg);
    static b1 bBoundedWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                                   std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg);

    // Starting at pNode, walk the graph upwards until the walk is either aborted
    // or we hit the root node. Call the callback function for each node visited.
    // This is different from a bottom-up walk because a bottom-up walk specifies
    // the top node to start from and it will likely visit other unrelated branches.
    // This walk starts at a specified bottom node and only walks up its direct ancestors.
    // The visitor function can only return WALK_CONTINUE or WALK_ABORT as the other
    // enum values do not make sense for an upward walk.
    static b1 bWalkUp(QueryNode* pNode, PlanNode* pRootNode,
                      std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fVisitor, void* pvArg);

    // From a given starting node (typically the root node), walk a path given by
    // a string and call callback functions for each node visited, both on descent
    // of the path and ascent.
    static b1 bWalkPath(PlanNode* pStartNode, s2K& sPathToWalk,
                        std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterVisitor,
                        std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitVisitor,
                        void* pvArg);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    // -------------------------------------------
    // Methods for finding/searching a plan graph.
    // -------------------------------------------

    // Returns a PlanNode* for a given path if one is found. Otherwise null.
    // The path may be absolute (starts with a '/') or relative.
    // If relative then the first name in the path must match the name of the
    // node being passed in. e.g. if the current node is named "foo"
    // then we can get specific children and properties by using the following
    // paths:  "foo/child3", "foo:property81"
    // Use '/' to separate children and ':' to denote a property.
    static PlanNode* pFind(Stz* szPath, PlanNode* pNode);

    // Looks for a property with a given name of the specified node and returns
    // it if found. Otherwise returns nullptr.
    static PlanNode* pFindProperty(Stz* szPropName, PlanNode* pNode);

    // Looks for a child with a given name of the specified node and returns
    // it if found. Otherwise returns nullptr.
    static PlanNode* pFindChild(Stz* szChildName, PlanNode* pNode);

    // Looks for a child which matches a given condition. Returns the first
    // matching child, if found, otherwise nullptr.
    static PlanNode* pFindChild(std::function<b1(PlanNode*)> fCondition, PlanNode* pNode);

    // Search for a node that matches a given condition. If found then the node*
    // is returned.
    // The condition function is a lambda or function with the following definition:
    //   b1 bCondition(PlanNode* pNode, void* pvArg);
    // Returning true out of this function will return pNode and abort the search.
    // Returning false out of this function will continue the search.
    // If no node matching the condition is found then a nullptr is returned.
    static PlanNode* pGlobalSearch(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                   std::function<b1(PlanNode*, void*)> fCondition, void* pvArg);
    static PlanNode* pGlobalSearchBottomUp(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                   std::function<b1(PlanNode*, void*)> fCondition, void* pvArg);

    static PlanNode* pBoundedSearch(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                    std::function<b1(PlanNode*, void*)> fCondition, void* pvArg);
    static PlanNode* pBoundedSearchBottomUp(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                    std::function<b1(PlanNode*, void*)> fCondition, void* pvArg);


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


    // Returns the parent node of a given definition node
    static PlanNode* pGetParent(DefinitionNode* pDefnNode, PlanNode* pRootNode);

    // Returns the parent node of a given primitive node
    static PlanNode* pGetParent(PrimitiveNode* pDefnNode, PlanNode* pRootNode);

    // Given a relative/absolute path and an initial starting node, return
    // the node that represents the parent of this path, or nullptr if not found.
    static PlanNode* pGetParentOfPath(Stz* szPath, PlanNode* pInitialNode);

    // Given a path, write the parent path to sParentBuf. Return false if there
    // isn't a parent.
    static b1 bParentPath(Stz* szPath, s2K& sParentBuf);

    // Given a buffer containing a path, modify it to give the parent's path
    static b1 bParentPath(s2K& sParentBuf);

    // Walks the plan graph to find the given query node.
    // Saves the path and depth. Returns true if found
    static b1 bGetPathAndDepth(QueryNode* pSearchNode, PlanNode* pRootNode, s2K& sPathBuf,
                               i8& iDepth, b1 bPlanSubtree, b1 bWalkProps = true);
    static b1 bGetPath(QueryNode* pSearchNode, PlanNode* pStartingNode, s2K& sPathBuf);


  private:
    // Visitor walk implementation
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                    void* pvArg, s2K& sPath, i8 iDepth, b1 bBoundedWalk);
    static b1 bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                            std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                            void* pvArg, s2K& sPath, i8 iDepth, b1 bBoundedWalk);

    // Event walk implementation
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                    std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                    void* pvArg, s2K& sPath, i8 iDepth, b1 bBoundedWalk);

    // Walking a specific path
    static b1 bWalkPath(PlanNode* pCurrentNode, s2K& sPathToBeWalked, s2K& sCurrentPath, i8 iDepth,
                        std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterVisitor,
                        std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitVisitor,
                        void* pvArg);

    static i8 iNextSeparator(Stz* szPath);


  };

  GraphNav* _pGraphNav(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

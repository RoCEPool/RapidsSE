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
    GraphNav();

  public:

    // Returns the leaf element of a type definition path.
    // e.g. given a definition path of /a/b/c this will return 'c'.
    static Stz* szLeafType(Stz* szTypePath);

    // Returns the parent of the type definition path given.
    static void vParentType(Stz* szTypePath, s256& buf);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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

    // The following functions require a visitor function with the definition:
    //   eWalkCtrl func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
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
    // top-down, depth-first traversal.
    // Returns false if the walk was aborted or true if it completed normally.
    static b1 bWalk(PlanNode* pNode, std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor, void* pvArg);
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor, void* pvArg);

    // Walk the current node and (potenitially) its properties and children,
    // calling the visitor function for each node encountered. This is a
    // bottom-up, depth-first traversal.
    // Returns false if the walk was aborted or true if it completed normally.
    static b1 bWalkBottomUp(PlanNode* pNode, std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor, void* pvArg);
    static b1 bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                            std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor, void* pvArg);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

    // Returns the parent node of a given definition node
    static PlanNode* pGetParent(DefinitionNode* pDefnNode, PlanNode* pRootNode);

    // Returns the parent node of a given primitive node
    static PlanNode* pGetParent(PrimitiveNode* pDefnNode, PlanNode* pRootNode);

    // Walks the plan graph to find the given query node. Saves the path and depth
    static void vGetPathAndDepth(QueryNode* pPlanNode, PlanNode* pRootNode, s256& sPathBuf, i8& iDepth);

  private:
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor,
                    void* pvArg, s256& sPath, i8 iDepth);
    static b1 bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                            std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor,
                            void* pvArg, s256& sPath, i8 iDepth);

    static i8 iNextSeparator(Stz* szPath);

    // Given a path, write the parent path to sParentBuf. Return false if there
    // isn't a parent.
    static b1 bParentPath(Stz* szPath, s2K& sParentBuf);
  };

  GraphNav* _pGraphNav(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

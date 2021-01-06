/*============================================================================*/
/* GraphNav.hpp - a utility for walking plan graphs                           */
/*============================================================================*/
#pragma once
#include <functional> // for std::function

namespace Parser
{
  class PlanNode;

  class GraphNav : public Object
  {
  private:
    Footer m_xFooter;

  public:

    void* operator new(size_t);
    void  operator delete(void*);

    GraphNav();
    ~GraphNav(void);

    Footer* aFooter(void);

    // Returns the leaf element of a type definition path.
    // e.g. given a definition path of /a/b/c this will return 'c'.
    static Stz* szLeafType(Stz* szTypePath);

    // Returns the parent of the type definition path given.
    static void vParentType(Stz* szTypePath, s256& buf);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // Methods for walking a plan graph.
    // ---------------------------------
    // The following functions require a visitor function with the definition:
    //   b1 func(PlanNode* pNode, s256& sPath)
    // Returning true in this visitor function indicates that the walk should
    // continue. Returning false indicates that the walk is to abort.

    // Walk the current node and (potenitially) its properties and children,
    // calling the visitor function for each node encountered. This is a
    // top-down, depth-first traversal.
    static b1 bWalk(PlanNode* pNode, std::function<b1(PlanNode*, s256&)> fVisitor);
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor);

    // Walk the current node and (potenitially) its properties and children,
    // calling the visitor function for each node encountered. This is a
    // bottom-up, depth-first traversal.
    static b1 bWalkBottomUp(PlanNode* pNode, std::function<b1(PlanNode*, s256&)> fVisitor);
    static b1 bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor);

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

  private:
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<b1(PlanNode*, s256&)> fVisitor, s256& sPath);
    static b1 bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<b1(PlanNode*, s256&)> fVisitor, s256& sPath);

    static i8 iNextSeparator(Stz* szPath);
  };

  GraphNav* _pGraphNav(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

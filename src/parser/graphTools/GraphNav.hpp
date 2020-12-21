/*============================================================================*/
/* GraphNav.hpp - a utility for walking plan graphs                           */
/*============================================================================*/
#pragma once

namespace Parser
{
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

    // Methods for walking a plan graph.

    // The following functions require a functor with the definition:
    //   b1 func(PlanNode* pNode, s256& sPath)
    static b1 bWalk(PlanNode* pNode, std::function<b1(PlanNode*, s256&)> fVisitor);
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor);

    // Returns a PlanNode* for a given path if one is found. Otherwise null.
    // The path may be absolute (starts with a '/') or relative.
    // If relative then the first name in the path must match the name of the
    // node being passed in.
    // Use '/' to separate children and ':' to denote a property.
    static PlanNode* pFind(Stz* szPath, PlanNode* pNode);

  private:
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<b1(PlanNode*, s256&)> fVisitor, s256& sPath);

    static i8 iNextSeparator(Stz* szPath);
  };

  GraphNav* _pGraphNav(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

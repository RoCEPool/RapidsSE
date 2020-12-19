/*============================================================================*/
/* PlanWalker.hpp - a utility for walking plan graphs                         */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanWalker : public Object
  {
  private:
    Footer m_xFooter;

  public:

    void* operator new(size_t);
    void  operator delete(void*);

    PlanWalker();
    ~PlanWalker(void);

    Footer* aFooter(void);

    // Methods for walking a plan graph.

    // The following functions require a functor with the definition:
    //   b1 func(PlanNode* pNode, s256& sPath)
    static b1 bWalk(PlanNode* pNode, std::function<b1(PlanNode*, s256&)> fVisitor);
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor);

  private:
    static b1 bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                    std::function<b1(PlanNode*, s256&)> fVisitor, s256& sPath);
  };

  PlanWalker* _pPlanWalker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* JoinOrdering.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class JoinOrdering : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    JoinOrdering(CardIter* pCI);
   ~JoinOrdering(void) noexcept(false);

    void vJoinOrdering(void);

    void vGroup_PredicatePullup(void);
    void vGroup_IndexBounds(void);
    void vGroup_JoinOrdering(void);

    void vTransform_MarkCmpPredWrtTable(void);
    void vTransform_CopyMarkedCmpPred(void);
    void vTransform_CullMarkedCmpPred(void);
    void vTransform_DeleteEmptyWhere(void);

    void vTransform_JoinOrdering_originalModel(void);
    void vTransform_PredicateReposition_originalModel(void);
    void vTransform_DeriveBoundsFromFilter_originalModel(void);

  protected:
    Footer* aFooter(void);
  };

  JoinOrdering* _pJoinOrdering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

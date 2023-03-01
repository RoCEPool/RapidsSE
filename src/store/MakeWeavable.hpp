/*============================================================================*/
/* MakeWeavable.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class MakeWeavable : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeWeavable(CardIter* pCI);
   ~MakeWeavable(void) noexcept(false);

    void vMakeWeavable(void);

  private:

    void vTransform_CullNonusedBounds(void);
    void vTransform_CullUsedBoundsFromFilter(void);
    void vTransform_IndividuateFilter(void);

    void vTransform_ReinstateStages(void);
    void vTransform_AddPartitioning(void);

    void vTransform_TblColToStmCol(void);
    void vTransform_StmColToColNmOfKeyExists(void);

  protected:
    Footer* aFooter(void);
  };

  MakeWeavable* _pMakeWeavable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

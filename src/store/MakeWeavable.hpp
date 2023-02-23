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

    void vSeries_MakeWeavable(void);

    void vGroup_CullBounds(void);
    void vGroup_Filters(void);
    void vGroup_Staging(void);
    void vGroup_Partitioning(void);
    void vGroup_FinalAdjustments(void);

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

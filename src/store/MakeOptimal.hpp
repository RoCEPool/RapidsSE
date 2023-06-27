/*============================================================================*/
/* MakeOptimal.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class MakeOptimal : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeOptimal(CardIter* pCI);
   ~MakeOptimal(void) noexcept(false);

    void vOptimise(void);

    void vOptimiseLoop(void);

    bool bOptimise_CutSort(void);
    bool bOptimise_DupFilter(void);
    bool bOptimise_ElyFilters(void);
    bool bOptimise_ElyProjections(void);
    bool bOptimise_MinGroupby(void);
    bool bOptimise_OtmCalculations(void);
    bool bOptimise_PpnProjections(void);
    bool bOptimise_RplAggregByOrdAgg(void);
    bool bOptimise_RplAggregBySortOrdAgg(void);
    bool bOptimise_RplMaxByTop(void);
    bool bOptimise_RplScanByIndexScan(void);
    bool bOptimise_RplSortDescLimitByTop(void);
    bool bOptimise_RplSortLimitOnOriginByAllLevels(void);

    void vCut_sort(void);
    void vPromote_gbyAgg_To_ordAgg(void);

    void vCullNonusedBounds(void);
    void vCullUsedBoundsFromFilter(void);

  protected:
    Footer* aFooter(void);
  };

  MakeOptimal* _pMakeOptimal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

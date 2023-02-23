/*============================================================================*/
/* Optimisations.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class Optimisations : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Optimisations(CardIter* pCI);
   ~Optimisations(void) noexcept(false);

    void vGroup_ChooseEEops(void);

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
    bool bOptimise_SeqJoinOrder(void);

    void vTransform_MorphTableToEEop(void);


  protected:
    Footer* aFooter(void);
  };

  Optimisations* _pOptimisations(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

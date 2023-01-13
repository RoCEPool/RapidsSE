/*============================================================================*/
/* Xtransforms.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"
#include "Bench.hpp"

#include "PermGen.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"
#include "eOT.h"

namespace STORE
{
  class Xtransforms : public Object
  {

  public:
    CardIter*    m_pCI;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xtransforms(Bench* pBench);
   ~Xtransforms(void) noexcept(false);

    void vInitialAdjustments(void);
    void vCondenseInfo(void);
    void vMetaAnalysis(void);
    void vPredicatePullup(void);
    void vJoinOrdering(void);
    void vIndexBounds(void);
    void vFlattenSubquery(void);
    void vChooseEEops(void);

    void vCullBounds(void);
    void vStaging(void);
    void vPartitioning(void);
    void vFinalAdjustments(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // optimization transformations
    void vOptimiseLoop(void);

    bool bOT_Cut_Sort(void);
    bool bOT_Dup_Filter(void);
    bool bOT_Ely_Filters(void);
    bool bOT_Ely_Projections(void);
    bool bOT_Min_Groupby(void);
    bool bOT_Otm_Calculations(void);
    bool bOT_Ppn_Projections(void);
    bool bOT_Rpl_Aggreg_By_OrdAgg(void);
    bool bOT_Rpl_Aggreg_By_Sort_OrdAgg(void);
    bool bOT_Rpl_Max_By_Top(void);
    bool bOT_Rpl_Scan_by_IndexScan(void);
    bool bOT_Rpl_SortDesc_Limit_By_Top(void);
    bool bOT_Rpl_Sort_Limit_OnOrigin_By_AllLevels(void);

    bool bOT_Seq_Join_Order(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    i8           m_iOdnLo;
    i8           m_iOdnHi;
    char         m_cTableSyn;

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

    // vSeries_10_Make_Runnable
    void vS10_T00_Begin(void);
    void vS05_T_Move_On(void);
    void vS10_T_Condense_Info(void);

    // vSeries_20_Table_Analysis
    void vS20_T_Table_Analysis(void);

    // vSeries_24_Index_Analysis
    void vS26_T_Index_Analysis(void);

    // vSeries_22_Stream_Analysis
    void vS22_T_Stream_Analysis(void);
    void vS24_T_Projection_Analysis(void);

    // vSeries_26_SubQuery_Analysis

    void vS26_T10_SubQuery_Analysis_Of_Main_Query(void);
    void vS26_T20_SubQuery_Analysis_Of_Sub_Query(void);

    // vSeries_32_To_BoolCalc
    void vS32_T_SubQuery_Analysis(void);

    // vSeries_33_To_TblColOdn
    void vS33_T10_Convert_stmColOdn_To_tblColOdn(void);

    // vSeries_34_Predicate_Pullup
    void vS34_T_Predicate_Pullup(void);
    void vS34_T_Predicate_Reposition(void);

    // vSeries_36_Bound_Analysis
    void vS36_T10_Derive_bounds_From_filter(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vStreamAnalysisReset(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

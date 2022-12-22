/*============================================================================*/
/* Xtransforms.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "Xreshape.hpp"
#include "eOT.h"

namespace STORE
{
  class Xtransforms : public Xreshape
  {
  private:

    eOT m_eOptTranforms[OT_ENUMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xtransforms(Bench* pBench);
   ~Xtransforms(void) noexcept(false);

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
    void vS10_T_Condense_Info(void);

    // vSeries_20_Table_Analysis
    void vS20_T_Table_Analysis(void);

    // vSeries_24_Index_Analysis
    void vS24_T_Index_Analysis(void);

    // vSeries_22_Stream_Analysis
    void vS22_T_Stream_Analysis(void);

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

    // vSeries_40_Flatten_Query
    void vS40_T05_Flatten_By_Decision_Tree(void);
    void vS40_T10_Fold_F(void);
    void vS40_T20_Fold_W_A_C_ro(void);
    void vS40_T30_Fold_W_A_NC_I(void);
    void vS40_T40_Fold_W_A_NC_ro(void);
    void vS40_T50_Fold_W_L_C_E(void);
    void vS40_T60_Fold_W_L_NC_NI(void);
    void vS40_T70_Fold_H_A_NC_ro(void);

    // vSeries_50_Join_Ordering
    void vS50_T00_Join_Ordering_v2(void);


    // vSeries_75_EE_operators
    void vS75_T10_Morph_table_To_EEop(void);
    void vS75_T20_Morph_keyExists_filter_To_constraint(void);
    void vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_80_Index_Scan
    bool bS80_T00_Test(bool bEnabled);
    void vS80_T00_Skip(void);
    void vS80_T00_Begin(void);
    void vS80_T30_Morph_scan_To_indexScan(void);
    void vS80_TZZ_End(void);

    // vSeries_90_Stream_Ordering
    bool bS90_T00_Test(bool bEnabled);
    void vS90_T00_Skip(void);
    void vS90_T00_Begin(void);
    void vS90_T30_Derive_order(void);
    void vS90_TZZ_End(void);

    // vSeries_92_Project_Pullup
    bool bS92_T00_Test(bool bEnabled);
    void vS92_T00_Skip(void);
    void vS92_T00_Begin(void);
    void vS92_T10_Project_Pullup(void);
    void vS92_TZZ_End(void);

    // vSeries_A0_Make_Weavable
    void vSA0_T00_Begin(void);
    void vSA0_T10_Eliminate_bounds_From_filter(void);
    void vSA0_T20_Individuate_filter(void);
    void vSA0_TZZ_End(void);

    // vSeries_E0_To_StmOdn
    void vSE0_T10_Convert_TblColOdn_To_StmOdn(void);
    void vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_Z0_Restate_Pipeline
    void vSZ0_T20_Reinstate_stagesV3(void);
    void vSZ0_T30_Add_Partioning(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  private:

    void vAssert_Board_any(void);
    void vAssert_Board_mqA(void);
    void vAssert_Board_sqB(void);

    void vST00_Begin(void);
    void vSTZZ_End(void);

    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

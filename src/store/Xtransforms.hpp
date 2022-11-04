/*============================================================================*/
/* Xtransforms.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "Xreshape.hpp"

namespace STORE
{
  class Xtransforms : public Xreshape
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xtransforms(Bench* pBench);
   ~Xtransforms(void) noexcept(false);

    //void vAdd_columnRef_Under_term(void);

    void vH10_T00_Begin(void);
    void vH10_T10_Delete_Redundant_stages(void);
    void vH10_T15_Hide_Ignorable_stages(void);
    void vH10_T20_Delete_All_absJoin(void);
    void vH10_T30_Delete_All_combined(void);
    void vH10_T40_Delete_All_send(void);
    void vH10_T50_Cull_scan_Leaving_table(void);
    void vH10_T99_End(void);

    void vH20_T00_Begin(void);
    void vH20_T10_Affix_tableSyn_ToAll_table(void);
    void vH20_T20_Add_tablePtn_UnderAll_table(void);
    void vH20_T30_Add_stream_UnderAll_table(void);
    void vH20_T99_End(void);

    void vH30_T00_Begin(void);
    void vH30_T20_Calc_ordinalRange_ForAll_stream(void);
    void vH30_T99_End(void);

    void vH40_T00_Begin(void);
    void vH40_T10_Convert_where_rpnCalc_To_boolCalc(void);
    void vH40_T15_Convert_stmColOdn_To_tblColOdn(void);
    void vH40_T20_Mark_where_pred_Wrt_table(void);
    void vH40_T30_Move_Marked_where_pred_To_table_filter(void);
    void vH40_T40_Delete_Empty_where(void);
    void vH40_T99_End(void);

    void vH50_T00_Begin(void);
    void vH50_T10_Extract_subSet_Of_call(void);
    void vH50_T20_Copy_Called_Subset(void);
    void vH50_T30(void);
    void vH50_T40(void);
    void vH50_T50(void);
    void vH50_T60(void);
    void vH50_T99_End(void);

    void vH55_T00_Begin(void);
    void vH55_T10_Rework_Exists_SubQueries(void);
    void vH55_T20_Copy_Up_SubQueries(void);
    void vH55_T24_Correct_Streams(void);
    void vH55_T26_Correct_Predicates(void);
    void vH55_T30_Resolve_Parameter_Linkage(void);
    void vH55_T40_Resolve_Parameter_Terms(void);
    void vH55_T50_Cull_Exists_Call(void);
    void vH55_T60_Delete_Empty_where(void);
    void vH55_T99_End(void);

    void vH60_T00_Begin(void);
    void vH60_T10_Add_Scratch_indexJoin_UnderAllFiltered_table(void);
    void vH60_T20_Affix_tblColOdn_ToAll_idxCol(void);
    void vH60_T30_Add_bounds_UnderAllFiltered_table(void);
    void vH60_T40_Cull_pred_MovedTo_table_bounds(void);
    void vH60_T50_Delete_Empty_table_filter(void);
    void vH60_T99_End(void);

    void vH70_T00_Begin(void);
    void vH70_T00_Extract_tableList_From_queryBoard(void);
    void vH70_T10_Extract_joinPredList(void);
    void vH70_T20_Compute_coPtnPredList(void);
    void vH70_T30_Compute_coPtnJoinList(void);
    void vH70_T40_Compute_crossPtnJoinList(void);
    void vH70_T50_Compute_ptndJoinSet(void);
    void vH70_T60_Copy_ptndJoinSet(void);
    void vH70_T70_Generate_ptndPermutations(void);
    void vH70_T80_Remove_invalidPermutations(void);
    void vH70_T90_Setup_costEst(void);
    void vH70_T92_Estimate_ptndJoins(void);
    void vH70_T94_Estimate_network(void);
    void vH70_T96_Estimate_joinOrder(void);
    void vH70_T98_Extract_joinOrderSeq(void);
    void vH70_T99_End(void);

    void vH75_T00_Begin(void);
    void vH75_T10_Convert_table_To_EEop(void);
    void vH75_T20_Convert_keyExists_filter_To_bounds(void);
    void vH75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vH75_T40_Convert_stmColOdn_Of_bounds_To_stmColNm(void);
    void vH75_T99_End(void);

    void vH80_T00_Begin(void);
    void vH80_T10_Add_Scratch_indexList_Under_scan(void);
    void vH80_T20_Affix_idxColOdn_ToAll_idxCol(void);
    void vH80_T30_Mark_ULRangeOf_scan_filter_cmpPred(void);
    void vH80_T40_Mark_IndexMatchOf_scan_filter_terms(void);
    void vH80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void);
    void vH80_T60_Cull_Marked_filter_cmpPred(void);
    void vH80_T70_Promote_scan_To_indexScan(void);
    void vH80_T99_End(void);

    void vH90_T00_Begin(void);
    void vH90_T10_Remove_sort_IfSameAs_gby(void);
    void vH90_T99_End(void);

    void vHA0_T00_Begin(void);
    void vHA0_T10_Individuate_filter(void);
    void vHA0_T99_End(void);

    void vHZ0_T00_Begin(void);
    void vHZ0_T10_Reinstate_stages(void);
    void vHZ0_T20_Reinstate_combined(void);
    void vHZ0_T99_End(void);

  private:

    void vHH_Begin(void);
    void vHH_End(void);

    void vREPLACE_WITH_RESHAPE(void);

    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

    //void vAdd_columnRef_Under_term(bool bPrint, bool bTrace);

    void vH10_T10_Delete_Redundant_stages(bool bPrint, bool bTrace);
    void vH10_T15_Hide_Ignorable_stages(bool bPrint, bool bTrace);
    void vH10_T20_Delete_All_absJoin(bool bPrint, bool bTrace);
    void vH10_T30_Delete_All_combined(bool bPrint, bool bTrace);
    void vH10_T40_Delete_All_send(bool bPrint, bool bTrace);
    void vH10_T50_Cull_scan_Leaving_table(bool bPrint, bool bTrace);

    void vH20_T10_Affix_tableSyn_ToAll_table(bool bPrint, bool bTrace);
    void vH20_T20_Add_tablePtn_UnderAll_table(bool bPrint, bool bTrace);
    void vH30_T10_Add_stream_UnderAll_table(bool bPrint, bool bTrace);

    void vH30_T20_Calc_ordinalRange_ForAll_stream(bool bPrint, bool bTrace);

    void vH40_T10_Convert_where_rpnCalc_To_boolCalc(bool bPrint, bool bTrace);
    void vH40_T20_Mark_where_pred_Wrt_table(bool bPrint, bool bTrace);
    void vH40_T30_Move_Marked_where_pred_To_table_filter(bool bPrint, bool bTrace);
    void vH40_T40_Delete_Empty_where(bool bPrint, bool bTrace);

    void vH50_T10_Extract_subSet_Of_call(bool bPrint, bool bTrace);
    void vH50_T20_Copy_Called_Subset(bool bPrint, bool bTrace);
    void vH50_T30(bool bPrint, bool bTrace);

    void vH55_T10_Rework_Exists_SubQueries(bool bPrint, bool bTrace);
    void vH55_T20_Copy_Up_SubQueries(bool bPrint, bool bTrace);
    void vH55_T24_Correct_Streams(bool bPrint, bool bTrace);
    void vH55_T26_Correct_Predicates(bool bPrint, bool bTrace);
    void vH55_T30_Resolve_Parameter_Linkage(bool bPrint, bool bTrace);
    void vH55_T40_Resolve_Parameter_Terms(bool bPrint, bool bTrace);
    void vH55_T50_Cull_Exists_Call(bool bPrint, bool bTrace);
    void vH55_T60_Delete_Empty_where(bool bPrint, bool bTrace);

    void vH60_T10_Add_Scratch_indexJoin_UnderAllFiltered_table(bool bPrint, bool bTrace);
    void vH60_T20_Affix_idxColOdn_ToAll_idxCol(bool bPrint, bool bTrace);
    void vH60_T30_Add_bounds_UnderAllFiltered_table(bool bPrint, bool bTrace);
    void vH60_T40_Cull_pred_MovedTo_table_bounds(bool bPrint, bool bTrace);
    void vH60_T50_Delete_Empty_table_filter(bool bPrint, bool bTrace);

    void vH70_T00_Extract_tableList_From_queryBoard(bool bPrint, bool bTrace);
    void vH70_T10_Extract_joinPredList(bool bPrint, bool bTrace);
    void vH70_T20_Compute_coPtnPredList(bool bPrint, bool bTrace);
    void vH70_T30_Compute_coPtnJoinList(bool bPrint, bool bTrace);
    void vH70_T40_Compute_crossPtnJoinList(bool bPrint, bool bTrace);
    void vH70_T50_Compute_ptndJoinSet(bool bPrint, bool bTrace);
    void vH70_T60_Copy_ptndJoinSet(bool bPrint, bool bTrace);
    void vH70_T70_Generate_ptndPermutations(bool bPrint, bool bTrace);
    void vH70_T80_Remove_invalidPermutations(bool bPrint, bool bTrace);
    void vH70_T90_Setup_costEst(bool bPrint, bool bTrace);
    void vH70_T92_Estimate_ptndJoins(bool bPrint, bool bTrace);
    void vH70_T94_Estimate_network(bool bPrint, bool bTrace);
    void vH70_T96_Estimate_joinOrder(bool bPrint, bool bTrace);
    void vH70_T98_Extract_joinOrderSeq(bool bPrint, bool bTrace);

    void vH75_T10_Convert_table_To_EEop(bool bPrint, bool bTrace);
    void vH75_T20_Convert_keyExists_filter_To_bounds(bool bPrint, bool bTrace);
    void vH75_T30_Correct_stmColOdn_Of_keyExists_stream(bool bPrint, bool bTrace);
    void vH75_T40_Convert_stmColOdn_Of_bounds_To_stmColNm(bool bPrint, bool bTrace);

    void vH80_T10_Add_Scratch_indexList_Under_scan(bool bPrint, bool bTrace);
    void vH80_T20_Affix_idxColOdn_ToAll_idxCol(bool bPrint, bool bTrace);
    void vH80_T30_Mark_ULRangeOf_scan_filter_cmpPred(bool bPrint, bool bTrace);
    void vH80_T40_Mark_IndexMatchOf_scan_filter_terms(bool bPrint, bool bTrace);
    void vH80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(bool bPrint, bool bTrace);
    void vH80_T60_Cull_Marked_filter_cmpPred(bool bPrint, bool bTrace);
    void vH80_T70_Promote_scan_To_indexScan(bool bPrint, bool bTrace);

    void vH90_T10_Remove_sort_IfSameAs_gby(bool bPrint, bool bTrace);

    void vHA0_T10_Individuate_filter(bool bPrint, bool bTrace);

    void vHZ0_T10_Reinstate_stages(bool bPrint, bool bTrace);
    void vHZ0_T20_Reinstate_combined(bool bPrint, bool bTrace);

  private:


    void vREPLACE_WITH_RESHAPE(void);

    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

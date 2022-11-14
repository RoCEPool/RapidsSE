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

    // vSeries_10_Condense_Info
    bool bS10_T00_Test(bool bEnabled);
    void vS10_T00_Skip(void);
    void vS10_T00_Begin(void);
    void vS10_T10_Delete_All_absJoin(void);
    void vS10_T20_Delete_All_combined(void);
    void vS10_T30_Delete_All_send(void);
    void vS10_T40_Delete_All_stage(void);
    void vS10_T50_Cull_scan_Leaving_table(void);
    void vS10_T60_Add_All_level(void);
    void vS10_T80_Delete_Redundant_stages(void);
    void vS10_T90_Hide_Ignorable_stages(void);
    void vS10_TZZ_End(void);

    // vSeries_20_Table_Analysis
    bool bS20_T00_Test(bool bEnabled);
    void vS20_T00_Skip(void);
    void vS20_T00_Begin(void);
    void vS20_T10_Affix_tableSyn_ToAll_table(void);
    void vS20_T20_Produce_tablePtn_UnderAll_table(void);
    void vS20_TZZ_End(void);

    // vSeries_25_Stream_Analysis
    bool bS25_T00_Test(bool bEnabled);
    void vS25_T00_Skip(void);
    void vS25_T00_Begin(void);
    void vS25_T10_Produce_stream_UnderAll_table(void);
    void vS25_T12_Produce_stream_UnderAll_project(void);
    void vS25_T14_Produce_stream_UnderAll_aggreg(void);
    void vS25_T20_Derive_ordinalRange_ForAll_table_stream(void);
    void vS25_T22_Derive_ordinalRange_ForAll_project_stream(void);
    void vS25_T24_Derive_ordinalRange_ForAll_aggreg_stream(void);
    void vS25_TZZ_End(void);

    // vSeries_27_Query_Analysis
    bool bS27_T00_Test(bool bEnabled);
    void vS27_T00_Skip(void);
    void vS27_T00_Begin(void);
    void vS27_T10_Main_Query_Analysis(void);
    void vS27_T20_Sub_Query_Analysis(void);
    void vS27_T30(void);
    void vS27_TZZ_End(void);

    // vSeries_29_To_RpnForm
    bool bS29_T00_Test(bool bEnabled);
    void vS29_T00_Skip(void);
    void vS29_T00_Begin(void);
    void vS29_T10_Convert_where_rpnCalc_To_boolCalc(void);
    void vS29_T20_Convert_stmColOdn_To_tblColOdn(void);
    void vS29_T30(void);
    void vS29_TZZ_End(void);

    // vSeries_35_Flatten_From
    bool bS35_T00_Test(bool bEnabled);
    void vS35_T00_Skip(void);
    void vS35_T00_Begin(void);
    void vS35_T10_Push_sqB_Into_mqA(void);
    void vS35_T20_Delete_call(void);
    void vS35_T30_Delete_twist(void);
    void vS35_TZZ_End(void);

    // vSeries_36_Flatten_Where
    bool bS36_T00_Test(bool bEnabled);
    void vS36_T00_Skip(void);
    void vS36_T00_Begin(void);
    void vS36_T10_Push_sqB_Into_mqA(void);
    void vS36_T20_Transfer_whereB_To_whereA(void);
    void vS36_T30_Replace_param_In_whereA(void);
    void vS36_T40_Delete_tstPred_In_whereA(void);
    void vS36_T50_Adjust_StmOdn(void);
    void vS36_T60_Adjust_StmColOdn(void);
    void vS36_T70_Replace_term_In_cmpPred(void);
    void vS36_T80_Delete_twist(void);

    void vS36_TZZ_End(void);

    // vSeries_37_Flatten_Having
    bool bS37_T00_Test(bool bEnabled);
    void vS37_T00_Skip(void);
    void vS37_T00_Begin(void);
    void vS37_T10_Append_sqB_After_mqA(void);
    void vS37_T20_Find_stmCol_Link(void);
    void vS37_T30_Link_sqCall_In_having(void);
    void vS37_T35_Bump_gbyElist_rpnCalc_In_sqB(void);
    void vS37_T40_Add_sqCol_To_aggGbyElist(void);
    void vS37_T45_Bump_aggElist_rpnCalc_In_sqB(void);
    void vS37_T50_Add_sqCol_To_aggAggElist(void);
    void vS37_T55_Bump_having_rpnCalc_In_sqB(void);
    void vS37_T60_Unproject_sqCol(void);
    void vS37_T70_Exhange_sqB_With_mqA(void);
    void vS37_T80_Adjust_StmOdn(void);
    void vS37_T90_Adjust_StmColOdn(void);
    void vS37_TA0_Delete_twist(void);
    void vS37_TB0(void);
    void vS37_TZZ_End(void);

    // vSeries_40_Predicate_Pullup
    bool bS40_T00_Test(bool bEnabled);
    void vS40_T00_Skip(void);
    void vS40_T00_Begin(void);
    void vS40_T10_AtMark_where_pred_Wrt_table(void);
    void vS40_T20_Move_AtMarked_where_pred_To_table_filter(void);
    void vS40_T30_Delete_Empty_where(void);
    void vS40_TZZ_End(void);

    // vSeries_50_Join_Ordering
    bool bS50_T00_Test(bool bEnabled);
    void vS50_T00_Skip(void);
    void vS50_T00_Begin(void);
    void vS50_T00_Extract_tableList_From_queryBoard(void);
    void vS50_T10_Extract_joinPredList(void);
    void vS50_T20_Compute_coPtnPredList(void);
    void vS50_T30_Compute_coPtnJoinList(void);
    void vS50_T40_Compute_crossPtnJoinList(void);
    void vS50_T50_Compute_ptndJoinSet(void);
    void vS50_T60_Copy_ptndJoinSet(void);
    void vS50_T70_Generate_ptndPermutations(void);
    void vS50_T80_Remove_invalidPermutations(void);
    void vS50_T90_Setup_costEst(void);
    void vS50_T92_Estimate_ptndJoins(void);
    void vS50_T94_Estimate_network(void);
    void vS50_T96_Estimate_joinOrder(void);
    void vS50_T98_Extract_joinOrderSeq(void);
    void vS50_TZZ_End(void);

    // vSeries_60_Index_Join
    bool bS60_T00_Test(bool bEnabled);
    void vS60_T00_Skip(void);
    void vS60_T00_Begin(void);
    void vS60_T05_Twist_in_Becomes_filter(void);
    void vS60_T10_Produce_maybeIndexJoin_Under_filter(void);
    void vS60_T20_Affix_tblColOdn_ToAll_idxCol(void);
    void vS60_T30_UpCopy_filter_pred_Into_bounds(void);
    void vS60_T40_Cull_UpCopied_filter_pred(void);
    void vS60_T50_Delete_Empty_filter(void);
    void vS60_T60_Delete_Twist(void);
    void vS60_TZZ_End(void);

    // vSeries_75_EE_operators
    bool bS75_T00_Test(bool bEnabled);
    void vS75_T00_Skip(void);
    void vS75_T00_Begin(void);
    void vS75_T10_Morph_table_To_EEop(void);
    void vS75_T20_Morph_keyExists_filter_To_constraint(void);
    void vS75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);
    void vS75_TZZ_End(void);

    // vSeries_80_Index_Scan
    bool bS80_T00_Test(bool bEnabled);
    void vS80_T00_Skip(void);
    void vS80_T00_Begin(void);
    void vS80_T10_Produce_Scratch_indexList_Under_scan(void);
    void vS80_T20_Affix_idxColOdn_ToAll_idxCol(void);
    void vS80_T30_Mark_ULRangeOf_scan_filter_cmpPred(void);
    void vS80_T40_Mark_IndexMatchOf_scan_filter_terms(void);
    void vS80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void);
    void vS80_T60_Cull_Marked_filter_cmpPred(void);
    void vS80_T70_Morph_scan_To_indexScan(void);
    void vS80_TZZ_End(void);

    // vSeries_85_Cross_Join
    bool bS85_T00_Test(bool bEnabled);
    void vS85_T00_Skip(void);
    void vS85_T00_Begin(void);
    void vS85_T10_Transmute_All_table(void);
    void vS85_T20_Add_index_ForAll_crossJoin(void);
    void vS85_T30_Add_bounds_ForAll_crossJoin(void);
    void vS85_TZZ_End(void);

    // vSeries_90_Stream_Ordering
    bool bS90_T00_Test(bool bEnabled);
    void vS90_T00_Skip(void);
    void vS90_T00_Begin(void);
    void vS90_T10_Remove_sort_IfSameAs_gby(void);
    void vS90_TZZ_End(void);

    // vSeries_92_Ordered_Aggreg
    bool bS92_T00_Test(bool bEnabled);
    void vS92_T00_Skip(void);
    void vS92_T00_Begin(void);
    void vS92_T10(void);
    void vS92_T20(void);
    void vS92_T30(void);
    void vS92_TZZ_End(void);

    // vSeries_A0_Filter_Refinement
    bool bSA0_T00_Test(bool bEnabled);
    void vSA0_T00_Skip(void);
    void vSA0_T00_Begin(void);
    void vSA0_T10_Individuate_filter(void);
    void vSA0_TZZ_End(void);

    // vSeries_B0_RestoreStmOdn
    bool bSB0_T00_Test(bool bEnabled);
    void vSB0_T00_Skip(void);
    void vSB0_T00_Begin(void);
    void vSB0_TZZ_End(void);

    // vSeries_Z0_Restate_Pipeline
    bool bSZ0_T00_Test(bool bEnabled);
    void vSZ0_T00_Skip(void);
    void vSZ0_T00_Begin(void);
    void vSZ0_T10_Restore_stmColOdn_from_tblColOdn(void);
    void vSZ0_T20_Reinstate_stages(void);
    void vSZ0_T20_Reinstate_stagesV2(void);
    void vSZ0_T20_Reinstate_stagesV3(void);
    void vSZ0_T30_Reinstate_combined(void);
    void vSZ0_T40_Reinstate_next(void);
    void vSZ0_TZZ_End(void);

  private:

    void vAssert_Board_any(void);
    void vAssert_Board_mqA(void);
    void vAssert_Board_sqB(void);


    void vST00_Begin(void);
    void vSTZZ_End(void);

    void vSH_DeleteMain(eCK eCard);

    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

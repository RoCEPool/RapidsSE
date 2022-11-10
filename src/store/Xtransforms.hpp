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

    // vHand_10_Condense_Info
    bool bH10_T00_Test(void);
    void vH10_T00_Skip(void);
    void vH10_T00_Begin(void);
    void vH10_T10_Delete_All_absJoin(void);
    void vH10_T20_Delete_All_combined(void);
    void vH10_T30_Delete_All_send(void);
    void vH10_T40_Delete_All_stage(void);
    void vH10_T50_Cull_scan_Leaving_table(void);
    void vH10_T60_Add_All_level(void);
    void vH10_T80_Delete_Redundant_stages(void);
    void vH10_T90_Hide_Ignorable_stages(void);
    void vH10_T99_End(void);

    // vHand_20_Table_Analysis
    bool bH20_T00_Test(void);
    void vH20_T00_Skip(void);
    void vH20_T00_Begin(void);
    void vH20_T10_Affix_tableSyn_ToAll_table(void);
    void vH20_T20_Produce_tablePtn_UnderAll_table(void);
    void vH20_T99_End(void);

    // vHand_30_Stream_Analysis
    bool bH30_T00_Test(void);
    void vH30_T00_Skip(void);
    void vH30_T00_Begin(void);
    void vH30_T10_Produce_stream_UnderAll_table(void);
    void vH30_T12_Produce_stream_UnderAll_project(void);
    void vH30_T14_Produce_stream_UnderAll_aggreg(void);
    void vH30_T20_Derive_ordinalRange_ForAll_table_stream(void);
    void vH30_T22_Derive_ordinalRange_ForAll_project_stream(void);
    void vH30_T24_Derive_ordinalRange_ForAll_aggreg_stream(void);
    void vH30_T30_Convert_where_rpnCalc_To_boolCalc(void);
    void vH30_T40_Convert_stmColOdn_To_tblColOdn(void);
    void vH30_T99_End(void);

    // vHand_35_Flatten_From
    bool bH35_T00_Test(void);
    void vH35_T00_Skip(void);
    void vH35_T00_Begin(void);
    void vH35_T10_Push_sqB_Into_mqA(void);
    void vH35_T20_Delete_call(void);
    void vH35_T30_Delete_twist(void);
    void vH35_T99_End(void);

    // vHand_36_Flatten_Where
    bool bH36_T00_Test(void);
    void vH36_T00_Skip(void);
    void vH36_T00_Begin(void);
    void vH36_T10_Push_sqB_Into_mqA(void);
    void vH36_T20_Transfer_whereB_To_whereA(void);
    void vH36_T30_Replace_param_In_whereA(void);
    void vH36_T40_Delete_tstPred_In_whereA(void);
    void vH36_T50_Adjust_StmOdn(void);
    void vH36_T60_Adjust_StmColOdn(void);
    void vH36_T70_Replace_term_In_cmpPred(void);
    void vH36_T80_Delete_twist(void);

    void vH36_T99_End(void);

    // vHand_37_Flatten_Having
    bool bH37_T00_Test(void);
    void vH37_T00_Skip(void);
    void vH37_T00_Begin(void);
    void vH37_T10_Append_sqB_After_mqA(void);
    void vH37_T20_Find_stmCol_Link(void);
    void vH37_T30_Link_sqCall_In_having(void);
    void vH37_T40_Add_sqCol_To_aggGbyElist(void);
    void vH37_T50_Add_sqlCol_To_aggAggElist(void);
    void vH37_T60(void);
    void vH37_T70(void);
    void vH37_T99_End(void);

    // vHand_40_Predicate_Pullup
    bool bH40_T00_Test(void);
    void vH40_T00_Skip(void);
    void vH40_T00_Begin(void);
    void vH40_T10_AtMark_where_pred_Wrt_table(void);
    void vH40_T20_Move_AtMarked_where_pred_To_table_filter(void);
    void vH40_T30_Delete_Empty_where(void);
    void vH40_T99_End(void);

    // vHand_50_FlattenCall
    bool bH50_T00_Test(void);
    void vH50_T00_Skip(void);
    void vH50_T00_Begin(void);
    void vH50_T10_Extract_subSet_Of_call(void);
    void vH50_T20_Copy_Called_Subset(void);
    void vH50_T30(void);
    void vH50_T40(void);
    void vH50_T50(void);
    void vH50_T60(void);
    void vH50_T99_End(void);

    // vHand_52_FlattenIN
    bool bH52_T00_Test(void);
    void vH52_T00_Skip(void);
    void vH52_T00_Begin(void);
    void vH52_T10_Collect_In_SubQuery_As_SubSet(void);
    void vH52_T20_CopyUp_In_Subset_nc(void);
    void vH52_T30_Cull_ExeMarked_exePred(void);
    void vH52_T40_Copy_TwistMarked_tstPred(void);
    void vH52_T50_Cull_TwistMarked_tstPred(void);
    void vH52_T60_Delete_Empty_where(void);
    void vH52_T70_Adjust_StmOdn_PostTwist(void);
    void vH52_T99_End(void);

    // vHand_55_FlattenExists
    bool bH55_T00_Test(void);
    void vH55_T00_Skip(void);
    void vH55_T00_Begin(void);
    void vH55_T10_Collapse_Exists_SubQueries(void);
    void vH55_T20_Copy_Up_ExistsSubQueries(void);
    void vH55_T30_Adjust_Stream_OrdinalRange(void);
    void vH55_T40_Adjust_BoolCalc_Terms(void);
    void vH55_T50_Add_tblColOdn_Under_param(void);
    void vH55_T60_Substitute_paramOdn_With_tblColOdn(void);
    void vH55_T70_Cull_CallMarked_CallSub(void);
    void vH55_T80_Delete_Empty_where(void);
    void vH55_T99_End(void);

    // vHand_60_Index_Join
    bool bH60_T00_Test(void);
    void vH60_T00_Skip(void);
    void vH60_T00_Begin(void);
    void vH60_T05_Twist_in_Becomes_filter(void);
    void vH60_T10_Produce_maybeIndexJoin_Under_filter(void);
    void vH60_T20_Affix_tblColOdn_ToAll_idxCol(void);
    void vH60_T30_UpCopy_filter_pred_Into_bounds(void);
    void vH60_T40_Cull_UpCopied_filter_pred(void);
    void vH60_T50_Delete_Empty_filter(void);
    void vH60_T60_Delete_Twist(void);
    void vH60_T99_End(void);

    // vHand_70_Join_Ordering
    bool bH70_T00_Test(void);
    void vH70_T00_Skip(void);
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

    // vHand_75_EE_operators
    bool bH75_T00_Test(void);
    void vH75_T00_Skip(void);
    void vH75_T00_Begin(void);
    void vH75_T10_Morph_table_To_EEop(void);
    void vH75_T20_Morph_keyExists_filter_To_constraint(void);
    void vH75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vH75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);
    void vH75_T99_End(void);

    // vHand_80_Index_Scan
    bool bH80_T00_Test(void);
    void vH80_T00_Skip(void);
    void vH80_T00_Begin(void);
    void vH80_T10_Produce_Scratch_indexList_Under_scan(void);
    void vH80_T20_Affix_idxColOdn_ToAll_idxCol(void);
    void vH80_T30_Mark_ULRangeOf_scan_filter_cmpPred(void);
    void vH80_T40_Mark_IndexMatchOf_scan_filter_terms(void);
    void vH80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void);
    void vH80_T60_Cull_Marked_filter_cmpPred(void);
    void vH80_T70_Morph_scan_To_indexScan(void);
    void vH80_T99_End(void);

    // vHand_85_Cross_Join
    bool bH85_T00_Test(void);
    void vH85_T00_Skip(void);
    void vH85_T00_Begin(void);
    void vH85_T10_Transmute_All_table(void);
    void vH85_T20_Add_index_ForAll_crossJoin(void);
    void vH85_T30_Add_bounds_ForAll_crossJoin(void);
    void vH85_T99_End(void);

    // vHand_90_Stream_Ordering
    bool bH90_T00_Test(void);
    void vH90_T00_Skip(void);
    void vH90_T00_Begin(void);
    void vH90_T10_Remove_sort_IfSameAs_gby(void);
    void vH90_T99_End(void);

    // vHand_A0_Filter_Refinement
    bool bHA0_T00_Test(void);
    void vHA0_T00_Skip(void);
    void vHA0_T00_Begin(void);
    void vHA0_T10_Individuate_filter(void);
    void vHA0_T99_End(void);

    // vHand_B0_RestoreStmOdn
    bool bHB0_T00_Test(void);
    void vHB0_T00_Skip(void);
    void vHB0_T00_Begin(void);
    void vHB0_T99_End(void);

    // vHand_Z0_Restate_Pipeline
    bool bHZ0_T00_Test(void);
    void vHZ0_T00_Skip(void);
    void vHZ0_T00_Begin(void);
    void vHZ0_T10_Restore_stmColOdn_from_tblColOdn(void);
    void vHZ0_T20_Reinstate_stages(void);
    void vHZ0_T20_Reinstate_stagesV2(void);
    void vHZ0_T20_Reinstate_stagesV3(void);
    void vHZ0_T30_Reinstate_combined(void);
    void vHZ0_T40_Reinstate_next(void);
    void vHZ0_T99_End(void);

  private:

    void vHH_Begin(void);
    void vHH_End(void);

    void vHH_DeleteMain(eCK eCard);

    void vREPLACE_WITH_RESHAPE(void);

    Footer* aFooter(void);
  };

  Xtransforms* _pXtransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

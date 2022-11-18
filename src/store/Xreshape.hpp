/*============================================================================*/
/* Xreshape.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xgearing.hpp"


namespace STORE
{
  class Table;

  class Xreshape : public Xgearing
  {
  public:

    // vSeries_10_Condense_Info
    void vR_S10_T80_Delete_Redundant_stage(void);
    void vR_S10_T10_Delete_absJoin(void);
    void vR_S10_T20_Delete_combined(void);
    void vR_S10_T30_Delete_send(void);
    void vR_S10_T40_Delete_stage(void);
    void vR_S10_T50_Cull_scan_Leaving_table(void);
    void vR_S10_T60_Add_level(void);
    void vR_S10_T90_Hide_Ignorable_stage(void);

    // vSeries_20_Table_Analysis
    void vR_S20_T10_AAffix_tableSyn_To_table(void);
    void vR_S20_T20_Produce_tablePtn_Under_table(void);

    // vSeries_22_Stream_Analysis
    void vR_S22_T10_Produce_stream_Under_table(void);
    void vR_S22_T12_Produce_stream_Under_project(void);
    void vR_S22_T14_Produce_stream_Under_aggreg(void);
    void vR_S22_T20_Derive_ordinalRange_For_table_stream(void);
    void vR_S22_T22_Derive_ordinalRange_For_project_stream(void);
    void vR_S22_T24_Derive_ordinalRange_For_aggreg_stream(void);


    // vSeries_24_Query_Analysis
    void vR_S24_T10a_Main_Query_Analysis_aggreg(void);
    void vR_S24_T10b_Main_Query_Analysis_gbyagg(void);
    void vR_S24_T10c_Main_Query_Analysis_Of_from(void);
    void vR_S24_T10d_Main_Query_Analysis_Of_where_rpnCalc(void);
    void vR_S24_T10e_Main_Query_Analysis_Of_having_rpnCalc(void);
    void vR_S24_T10f_Main_Query_Genesis(void);
    void vR_S24_T20a_Sub_Query_Analysis_Only_main(void);
    void vR_S24_T20b_Sub_Query_Analysis_paramList(void);
    void vR_S24_T20c_Sub_Query_Analysis_aggreg(void);
    void vR_S24_T20d_Sub_Query_Analysis_gbyagg(void);
    void vR_S24_T10e_Sub_Query_Genesis(void);
    void vR_S24_T30(void);

    // vSeries_29_To_BoolCalc
    void vR_S29_T10_Convert_where_rpnCalc_To_boolCalc(void);
    void vR_S29_T20_Convert_stmColOdn_To_tblColOdn(void);

    // vSeries_30_Flatten_Query
    void vR_S30_T10a_Fold_F_Before_Mq_As_opSeq(void);
    void vR_S30_T20a_Fold_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey(void);
    void vR_S30_T30a_Fold_W_A_NC_I_Before_Mq_As_aggFanAval(void);
    void vR_S30_T40a_Fold_W_A_NC_ro_Before_Mq_As_aggFanAval(void);
    void vR_S30_T50a_Fold_W_L_C_E_Before_Mq_As_keyExists(void);
    void vR_S30_T60a_Fold_W_L_NC_NI_Into_Mq_As_keyNotExists(void);
    void vR_S30_T70a_Fold_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq(void);

    void vR_S30_Ute_a(void);
    void vR_S30_Ute_b(void);
    void vR_S30_Ute_c(void);
    void vR_S30_Ute_d(void);
    void vR_S30_Ute_e(void);
    void vR_S30_Ute_f(void);

    // vSeries_35X_Flatten_From
    void vR_S35X_T10a_Copy_From_SubQuery(void);
    void vR_S35X_T10b_Push_sqB_Before_mqA(void);
    void vR_S35X_T30_Adjust_From_SubQuery(void);

    // vSeries_36X_Flatten_Where
    void vR_S36X_T104_Mark_pmzWhere_In_sqB(void);
    void vR_S36X_T105_Pullup_pmzWhere_Into_mqA(void);
    void vR_S36X_T10a_Copy_From_SubQuery(void);
    void vR_S36X_T10b_Push_sqB_Before_mqA(void);
    void vR_S36X_T20a_Get_cmpPred_Of_whereB(void);
    void vR_S36X_T20b_Push_cmpPred_Onto_whereA(void);
    void vR_S36X_T30a_Replace_param_In_whereA(void);
    void vR_S36X_T30b_Delete_param_In_whereB(void);
    void vR_S36X_T40_Delete_tstPred_In_whereA(void);
    void vR_S36X_T50_Adjust_StmOdn(void);
    void vR_S36X_T60_Adjust_StmColOdn(void);
    void vR_S36X_T70_Replace_term_In_cmpPred(void);

    // vSeries_37X_Flatten_Having
    void vR_S37X_T10a_Copy_sqB(void);
    void vR_S37X_T10a_Append_sqB_After_mqA(void);
    void vR_S37X_T20_Find_stmCol_Link(void);
    void vR_S37X_T30_Link_sqCall_In_having(void);
    void vR_S37X_T35_Bump_gbyElist_rpnCalc_In_sqB(void);
    void vR_S37X_T40Add_sqCol_To_AggGbyElist(void);
    void vR_S37X_T45_Bump_aggElist_rpnCalc_In_sqB(void);
    void vR_S37X_T50_Add_sqCol_To_aggAggElist(void);
    void vR_S37X_T55_Bump_having_rpnCalc_In_sqB(void);
    void vR_S37X_T60_Unproject_sqCol(void);
    void vR_S37X_T70_Exhange_sqB_With_mqA(void);
    void vR_S37X_T80_AdjustStmOdn(void);
    void vR_S37X_T90_Adjust_StmColOdn(void);
    void vR_S37X_TA0(void);
    void vR_S37X_TB0(void);

    // vSeries_34_Predicate_Pullup
    void vR_S34_T10_AtMark_where_pred_Wrt_table(void);
    void vR_S34_T20_Move_AtMarked_where_pred_To_table_filter(void);
    void vR_S34_T30_Delete_Empty_where(void);

    // vSeries_60_Index_Join
    void vR_S60_T05_Twist_in_Becomes_filter(void);
    void vR_S60_T10_Produce_maybeIndexJoin_Under_filter(void);
    void vR_S60_T20_Affix_tblColOdn_To_idxCol(void);
    void vR_S60_T30_UpCopy_filter_pred_Into_bounds(void);
    void vR_S60_T40_Cull_UpCopied_filter_pred(void);
    void vR_S60_T50_Delete_Empty_filter(void);
    void vR_S60_T60_Delete_Twist(void);

    // vSeries_50_Join_Ordering
    void vR_S50_T00_Extract_tableList_From_queryBoard(void);
    void vR_S50_T10_Extract_joinPredList(void);
    void vR_S50_T20_Compute_coPtnPredList(void);
    void vR_S50_T30_Compute_coPtnJoinList(void);
    void vR_S50_T40_Compute_crossPtnJoinList(void);
    void vR_S50_T50_Compute_ptndJoinSet(void);
    void vR_S50_T60_Copy_ptndJoinSet(void);
    void vR_S50_T70_Generate_ptndPermutations(void);
    void vR_S50_T80_Remove_invalidPermutations(void);
    void vR_S50_T90_Setup_costEst(void);
    void vR_S50_T92_Estimate_ptndJoins(void);
    void vR_S50_T94_Estimate_network(void);
    void vR_S50_T96_Estimate_joinOrder(void);
    void vR_S50_T98_Extract_joinOrderSeq(void);

    // vSeries_75_EE_operators
    void vR_S75_T10_Morph_table_To_EEop(void);
    void vR_S75_T10_Morph_table_To_EEop_b(void);
    void vR_S75_T10_Morph_table_To_EEop_c(void);
    void vR_S75_T20_Morph_keyExists_filter_To_constraint(void);
    void vR_S75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_80_Index_Scan
    void vR_S80_T10_Produce_indexList_Under_scan(void);
    void vR_S80_T20_Affix_idxCol_name_Of_scan_To_ordinal(void);
    void vR_S80_T30_Mark_ULBoundsOf_scan_filter_cmpPred(void);
    void vR_S80_T40_Mark_IndexMatchOf_scan_filter_terms(void);
    void vR_S80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void);
    void vR_S80_T60_Cull_Marked_filter_cmpPred(void);
    void vR_S80_T70_Morph_scan_To_indexScan(void);
    void vR_S80_T70_Morph_scan_To_indexScan_b(void);
    void vR_S80_T70_Morph_scan_To_indexScan_c(void);

    // vSeries_85_Cross_Join
    void vR_S85_T10a_Transmute_table_To_Scan(void);
    void vR_S85_T10b_Transmute_table_To_crossJoin(void);
    void vR_S85_T20_Add_index_For_crossJoin(void);
    void vR_S85_T30_Add_bounds_For_crossJoin(void);

    // vSeries_90_Stream_Ordering
    void vR_S90_T10_Remove_sort_IfSameAs_gby(void);

    // vSeries_A0_Filter_Refinement
    void vR_SA0_T10_Individuate_scan_filter(void);
    void vR_SA0_T10_Individuate_indexJoin_filter_b(void);

    // vSeries_Z0_Restate_Pipeline
    void vR_SZ0_T10_Restore_stmColOdn_from_tblColOdn(void);
    void vR_SZ0_T20_Reinstate_stages(void);
    void vR_SZ0_T20_Reinstate_stagesV2(void);
    void vR_SZ0_T20_Reinstate_stagesV3(void);
    void vR_SZ0_T30_Reinstate_combined(void);
    void vR_SZ0_T40_Reinstate_next(void);
    void vR_SZ0_T40_Reinstate_next_b(void);

    void vRR_Convert_All_TblColOrd_to_StmColOrd(void);
    void vRR_Convert_All_StmColOrd_to_TblColOrd(void);

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xreshape(eCN eClass, Bench* pBench);
   ~Xreshape(void) noexcept(false);

  private:


    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

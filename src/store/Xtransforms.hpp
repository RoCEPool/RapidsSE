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
    void vOT_Cut_Sort(void);

    bool bOT_Dup_Filter(void);
    void vOT_Dup_Filter(void);

    bool bOT_Ely_Filters(void);
    void vOT_Ely_Filters(void);
    bool bOT_Ely_Projections(void);
    void vOT_Ely_Projections(void);
    bool bOT_Ely_Sort_Limit(void);
    void vOT_Ely_Sort_Limit(void);

    bool bOT_Min_Groupby(void);
    void vOT_Min_Groupby(void);

    bool bOT_Otm_Calculations(void);
    void vOT_Otm_Calculations(void);

    bool bOT_Ppn_Projections(void);
    void vOT_Ppn_Projections(void);

    bool bOT_Rpl_Aggreg_By_OrdAgg(void);
    void vOT_Rpl_Aggreg_By_OrdAgg(void);
    bool bOT_Rpl_Aggreg_By_Sort_OrdAgg(void);
    void vOT_Rpl_Aggreg_By_Sort_OrdAgg(void);
    bool bOT_Rpl_Max_By_Top(void);
    void vOT_Rpl_Max_By_Top(void);
    bool bOT_Rpl_Scan_by_IndexScan(void);
    void vOT_Rpl_Scan_by_IndexScan(void);
    bool bOT_Rpl_SortDesc_Limit_By_Top(void);
    void vOT_Rpl_SortDesc_Limit_By_Top(void);
    bool bOT_Rpl_Sort_Limit_OnOrogin_By_AllLevels(void);
    void vOT_Rpl_Sort_Limit_OnOrogin_By_AllLevels(void);

    bool bOT_Seq_Join_Order(void);
    void vOT_Seq_Join_Order(void);

    //void vAdd_columnRef_Under_term(void);

    // vSeries_10_Condense_Info
    bool bS10_T00_Test(bool bEnabled);
    void vS10_T00_Skip(void);
    void vS10_T00_Begin(void);
    void vS10_T_Condense_Info(void);
    void vS10_T80_Delete_Redundant_stages(void);
    void vS10_T90_Hide_Ignorable_stages(void);
    void vS10_TZZ_End(void);

    // vSeries_20_Table_Analysis
    bool bS20_T00_Test(bool bEnabled);
    void vS20_T00_Skip(void);
    void vS20_T00_Begin(void);
    void vS20_T_Table_Analysis(void);
    void vS20_TZZ_End(void);

    // vSeries_24_Index_Analysis
    bool bS24_T00_Test(bool bEnabled);
    void vS24_T00_Skip(void);
    void vS24_T00_Begin(void);
    void vS24_T_Index_Analysis(void);
    void vS24_TZZ_End(void);

    // vSeries_22_Stream_Analysis
    bool bS22_T00_Test(bool bEnabled);
    void vS22_T00_Skip(void);
    void vS22_T00_Begin(void);
    void vS22_T_Stream_Analysis(void);
    void vS22_TZZ_End(void);

    // vSeries_26_SubQuery_Analysis
    bool bS26_T00_Test(bool bEnabled);
    void vS26_T00_Skip(void);
    void vS26_T00_Begin(void);
    void vS26_T10_SubQuery_Analysis_Of_Main_Query(void);
    void vS26_T20_SubQuery_Analysis_Of_Sub_Query(void);
    void vS26_T30(void);
    void vS26_TZZ_End(void);

    // vSeries_32_To_BoolCalc
    bool bS32_T00_Test(bool bEnabled);
    void vS32_T00_Skip(void);
    void vS32_T00_Begin(void);
    void vS32_T10_Convert_sqyOp_not_To_notSqyOp(void);
    void vS32_T20_Convert_rpnCalc_To_boolCalc(void);
    void vS32_TZZ_End(void);

    // vSeries_33_To_TblColOdn
    bool bS33_T00_Test(bool bEnabled);
    void vS33_T00_Skip(void);
    void vS33_T00_Begin(void);
    void vS33_T10_Convert_stmColOdn_To_tblColOdn(void);
    void vS33_TZZ_End(void);

    // vSeries_34_Predicate_Pullup
    bool bS34_T00_Test(bool bEnabled);
    void vS34_T00_Skip(void);
    void vS34_T00_Begin(void);
    void vS34_T10_AtMark_where_pred_Wrt_table(void);
    void vS34_T20_Move_AtMarked_where_pred_To_table_filter(void);
    void vS34_T30_Delete_Empty_where(void);
    void vS34_TZZ_End(void);

    // vSeries_36_Bound_Analysis
    bool bS36_T00_Test(bool bEnabled);
    void vS36_T00_Skip(void);
    void vS36_T00_Begin(void);
    void vS36_T10_UpCopy_filter_pred_Into_bounds(void);
    void vS36_T20_Cull_UpCopied_filter_pred(void);
    void vS36_T30_Delete_Empty_filter(void);
    void vS36_T40(void);
    void vS36_TZZ_End(void);

    // vSeries_40_Flatten_Query
    bool bS40_T00_Test(bool bEnabled);
    void vS40_T00_Skip(void);
    void vS40_T00_Begin(void);
    void vS40_T05_Flatten_By_Decision_Tree(void);
    void vS40_T10_Fold_F_Before_Mq_As_opSeq(void);
    void vS40_T20_Fold_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey(void);
    void vS40_T30_Fold_W_A_NC_I_Before_Mq_As_aggFanAval(void);
    void vS40_T40_Fold_W_A_NC_ro_Before_Mq_As_aggFanAval(void);
    void vS40_T50_Fold_W_L_C_E_Before_Mq_As_keyExists(void);
    void vS40_T60_Fold_W_L_NC_NI_Into_Mq_As_keyNotExists(void);
    void vS40_T70_Fold_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq(void);
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
    void vS60_T60_Delete_Twist(void);
    void vS60_TZZ_End(void);

    // vSeries_75_EE_operators
    bool bS75_T00_Test(bool bEnabled);
    void vS75_T00_Skip(void);
    void vS75_T00_Begin(void);
    void vS75_T05_ChooseIndexes(void);
    void vS75_T10_Morph_table_To_EEop(void);
    void vS75_T20_Morph_keyExists_filter_To_constraint(void);
    void vS75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);
    void vS75_TZZ_End(void);

    // vSeries_80_Index_Scan
    bool bS80_T00_Test(bool bEnabled);
    void vS80_T00_Skip(void);
    void vS80_T00_Begin(void);

    void vS80_T10_filter_For_scan(void);
    void vS80_T20_bounds_For_scan(void);
    void vS80_T20_bounds_For_scan_v2(void);
    void vS80_T30_Morph_scan_To_indexScan(void);

    void vS80_TF1_Produce_Scratch_indexList_Under_scan(void);
    void vS80_TF2_Affix_idxColOdn_ToAll_idxCol(void);
    void vS80_TF3_Mark_ULRangeOf_scan_filter_cmpPred(void);
    void vS80_TF4_Mark_IndexMatchOf_scan_filter_terms(void);
    void vS80_TF5_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void);
    void vS80_TF6_Cull_Marked_filter_cmpPred(void);
    void vS80_TF7_Morph_scan_To_indexScan(void);
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
    void vS90_T30_Add_stmOrdList(void);
    void vS90_TF0_Remove_sort_IfSameAs_gby(void);
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
    void vSA0_T10_Eliminate_bounds_From_filter(void);
    void vSA0_T20_Individuate_filter(void);
    void vSA0_TZZ_End(void);

    // vSeries_B0_RestoreStmOdn
    bool bSB0_T00_Test(bool bEnabled);
    void vSB0_T00_Skip(void);
    void vSB0_T00_Begin(void);
    void vSB0_TZZ_End(void);

    // vSeries_E0_To_StmOdn
    bool bSE0_T00_Test(bool bEnabled);
    void vSE0_T00_Skip(void);
    void vSE0_T00_Begin(void);
    void vSE0_T10_Convert_TblColOdn_To_StmOdn(void);
    void vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm(void);
    void vSE0_TZZ_End(void);

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


    // vSeries_35X_Flatten_From
    bool bS35X_T00_Test(bool bEnabled);
    void vS35X_T00_Skip(void);
    void vS35X_T00_Begin(void);
    void vS35X_T10_Push_sqB_Into_mqA(void);
    void vS35X_T20_Delete_call(void);
    void vS35X_T30_Delete_twist(void);
    void vS35X_TZZ_End(void);

    // vSeries_36X_Flatten_Where
    bool bS36X_T00_Test(bool bEnabled);
    void vS36X_T00_Skip(void);
    void vS36X_T00_Begin(void);
    void vS36X_T04_Mark_pmzWhere_In_sqB(void);
    void vS36X_T05_Pullup_pmzWhere_Into_mqA(void);
    void vS36X_T10_Push_sqB_Into_mqA(void);
    void vS36X_T20_Transfer_whereB_To_whereA(void);
    void vS36X_T30_Replace_param_In_whereA(void);
    void vS36X_T40_Delete_tstPred_In_whereA(void);
    void vS36X_T50_Adjust_StmOdn(void);
    void vS36X_T60_Adjust_StmColOdn(void);
    void vS36X_T70_Replace_term_In_cmpPred(void);
    void vS36X_T80_Delete_twist(void);

    void vS36X_TZZ_End(void);

    // vSeries_37X_Flatten_Having
    bool bS37X_T00_Test(bool bEnabled);
    void vS37X_T00_Skip(void);
    void vS37X_T00_Begin(void);
    void vS37X_T10_Append_sqB_After_mqA(void);
    void vS37X_T20_Find_stmCol_Link(void);
    void vS37X_T30_Link_sqCall_In_having(void);
    void vS37X_T35_Bump_gbyElist_rpnCalc_In_sqB(void);
    void vS37X_T40_Add_sqCol_To_aggGbyElist(void);
    void vS37X_T45_Bump_aggElist_rpnCalc_In_sqB(void);
    void vS37X_T50_Add_sqCol_To_aggAggElist(void);
    void vS37X_T55_Bump_having_rpnCalc_In_sqB(void);
    void vS37X_T60_Unproject_sqCol(void);
    void vS37X_T70_Exhange_sqB_With_mqA(void);
    void vS37X_T80_Adjust_StmOdn(void);
    void vS37X_T90_Adjust_StmColOdn(void);
    void vS37X_TA0_Delete_twist(void);
    void vS37X_TB0(void);
    void vS37X_TZZ_End(void);


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

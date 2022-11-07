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

    // vHand_10_CondenseInfo
    void vR_H10_T80_Delete_Redundant_stage(void);
    void vR_H10_T10_Delete_absJoin(void);
    void vR_H10_T20_Delete_combined(void);
    void vR_H10_T30_Delete_send(void);
    void vR_H10_T40_Delete_stage(void);
    void vR_H10_T50_Cull_scan_Leaving_table(void);
    void vR_H10_T60_Add_level(void);
    void vR_H10_T90_Hide_Ignorable_stage(void);

    // vHand_20_TableAnalysis
    void vR_H20_T10_AAffix_tableSyn_To_table(void);
    void vR_H20_T20_Produce_tablePtn_Under_table(void);

    // vHand_30_StreamAnalysis
    void vR_H30_T10_Produce_stream_Under_table(void);
    void vR_H30_T12_Produce_stream_Under_project(void);
    void vR_H30_T14_Produce_stream_Under_aggreg(void);
    void vR_H30_T20_Derive_ordinalRange_For_table_stream(void);
    void vR_H30_T22_Derive_ordinalRange_For_project_stream(void);
    void vR_H30_T24_Derive_ordinalRange_For_aggreg_stream(void);
    void vR_H30_T30_Convert_where_rpnCalc_To_boolCalc(void);
    void vR_H30_T40_Convert_stmColOdn_To_tblColOdn(void);

    // vHand_40_PredicatePullup
    void vR_H40_T10_AtMark_where_pred_Wrt_table(void);
    void vR_H40_T20_Move_AtMarked_where_pred_To_table_filter(void);
    void vR_H40_T30_Delete_Empty_where(void);

    // vHand_50_FlattenCall
    void vR_H50_T10_Extract_subSet_Of_call(void);
    void vR_H50_T20_Yank_SubSet(void);
    void vR_H50_T20_Put_SubSet(void);
    void vR_H50_T30_RemoveDelete_call(void);
    void vR_H50_T40(void);
    void vR_H50_T50(void);
    void vR_H50_T60(void);

    // vHand_52_FlattenIn
    void vR_H52_T10_Collect_In_SubQuery_Into_SubSet_Pile(void);
    void vR_H52_T20_Yank_SubSet(void);
    void vR_H52_T20_Put_SubSet_nc(void);
    void vR_H52_T30_Cull_ExeMarked_exePred(void);
    void vR_H52_T40_Copy_TwistMarked_tstPred(void);
    void vR_H52_T50_Cull_TwistMarked_tstPred(void);
    void vR_H52_T60_Delete_Empty_where(void);
    void vR_H52_T70_Adjust_StmOdn_PostTwist(void);

    // vHand_55_FlattenExists
    void vR_H55_T10_Collapse_Exists_SubQuery(void);
    void vR_H55_T20_Copy_Up_SubQuery(void);
    void vR_H55_T30_Adjust_Stream_OrdinalRange(void);
    void vR_H55_T40_Adjust_BoolCalc_Terms(void);
    void vR_H55_T50_Produce_Parameter_Linkage(void);
    void vR_H55_T60_Substitute_paramOdn_With_tblColOdn(void);
    void vR_H55_T70_Cull_CallMarked_CallSub(void);
    void vR_H55_T80_Delete_Empty_where(void);

    // vHand_60_IndexJoin
    void vR_H60_T05_Twist_in_Becomes_filter(void);
    void vR_H60_T10_Produce_maybeIndexJoin_Under_filter(void);
    void vR_H60_T20_Affix_tblColOdn_To_idxCol(void);
    void vR_H60_T30_UpCopy_filter_pred_Into_bounds(void);
    void vR_H60_T40_Cull_UpCopied_filter_pred(void);
    void vR_H60_T50_Delete_Empty_filter(void);
    void vR_H60_T60_Delete_Twist(void);

    // vHand_70_JoinOrdering
    void vR_H70_T00_Extract_tableList_From_queryBoard(void);
    void vR_H70_T10_Extract_joinPredList(void);
    void vR_H70_T20_Compute_coPtnPredList(void);
    void vR_H70_T30_Compute_coPtnJoinList(void);
    void vR_H70_T40_Compute_crossPtnJoinList(void);
    void vR_H70_T50_Compute_ptndJoinSet(void);
    void vR_H70_T60_Copy_ptndJoinSet(void);
    void vR_H70_T70_Generate_ptndPermutations(void);
    void vR_H70_T80_Remove_invalidPermutations(void);
    void vR_H70_T90_Setup_costEst(void);
    void vR_H70_T92_Estimate_ptndJoins(void);
    void vR_H70_T94_Estimate_network(void);
    void vR_H70_T96_Estimate_joinOrder(void);
    void vR_H70_T98_Extract_joinOrderSeq(void);

    // vHand_75_EE_operators
    void vR_H75_T10_Morph_table_To_EEop(void);
    void vR_H75_T10_Morph_table_To_EEop_b(void);
    void vR_H75_T10_Morph_table_To_EEop_c(void);
    void vR_H75_T20_Morph_keyExists_filter_To_constraint(void);
    void vR_H75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vR_H75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vHand_80_IndexScan
    void vR_H80_T10_Produce_indexList_Under_scan(void);
    void vR_H80_T20_Affix_idxCol_name_Of_scan_To_ordinal(void);
    void vR_H80_T30_Mark_ULBoundsOf_scan_filter_cmpPred(void);
    void vR_H80_T40_Mark_IndexMatchOf_scan_filter_terms(void);
    void vR_H80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void);
    void vR_H80_T60_Cull_Marked_filter_cmpPred(void);
    void vR_H80_T70_Morph_scan_To_indexScan(void);
    void vR_H80_T70_Morph_scan_To_indexScan_b(void);
    void vR_H80_T70_Morph_scan_To_indexScan_c(void);

    // vHand_90_StreamOrdering
    void vR_H90_T10_Remove_sort_IfSameAs_gby(void);

    // vHand_A0_FilterRefinement
    void vR_HA0_T10_Individuate_scan_filter(void);
    void vR_HA0_T10_Individuate_indexJoin_filter_b(void);

    // vHand_Z0_RestatePipeline
    void vR_HZ0_T10_Restore_stmColOdn_from_tblColOdn(void);
    void vR_HZ0_T20_Reinstate_stages(void);
    void vR_HZ0_T20_Reinstate_stagesV2(void);
    void vR_HZ0_T30_Reinstate_combined(void);
    void vR_HZ0_T40_Reinstate_next(void);
    void vR_HZ0_T40_Reinstate_next_b(void);

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

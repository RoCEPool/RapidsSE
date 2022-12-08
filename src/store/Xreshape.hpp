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

    // utilities
    void R_vUTE_Push_Correlated_cmpPred_Onto_mq_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED* p_PRED);
    void R_vUTE_AddPredToTableFilter(Card_Table* p_table, Card_PRED* p_PRED);
    void R_vUTE_AdjustStmLoHi(void);
    void R_vUTE_Append_unproject_Of_foldExpr_After_having(void);
    void R_vUTE_Bump_keyOdn_In_rpnCalc_Of_Elist(Card_ELIST* p_ELIST);
    void R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(Card_ELIST* p_ELIST);
    void R_vUTE_Bump_where_BaseOn_foldExpr(void);
    void R_vUTE_Derive_foldElist_From_project_stream(void);
    void R_vUTE_Derive_stmOrdList_For_gbyAgg(void);
    void R_vUTE_Derive_stmOrdList_For_project(void);
    void R_vUTE_Enrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(void);
    void R_vUTE_Enhance_sq_project_To_Include_Correlation_Column(void);
    void R_vUTE_Convert_tblColOdn_Of_constraint_To_stmColNm(void);
    void R_vUTE_Convert_tblColOdn_To_stmOdn(void);
    void R_vUTE_Correct_having_BaseOn_foldExpr(void);
    void R_vUTE_Derive_foldExpr(void);
    bool R_bUTE_FindAndMarkSubQryCall(void);
    void R_vUTE_FoldIn_foldExpr(void);
    void R_vUTE_Identify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(void);
    void R_vUTE_Individuate_filter(void);
    void R_vUTE_Find_stmCol_Definition_Of_Correlation_Column(void);
    void R_vUTE_Move_corrPred_To_mq_table_As_bounds(Card_BoolCalc* p_boolCalc);
    void R_vUTE_MoveWhereToTableAsConstraint(Card_Table* p_table, Card_Where* p_where);
    void R_vUTE_MoveWhereToTableAsFilter(Card_Table* p_table, Card_Where* p_where);
    void R_vUTE_Push_foldExpr_Into_aggElist(void);
    void R_vUTE_Push_foldExpr_Into_gbyElist(void);
    void R_vUTE_RelabelExpressions(void);
    void R_vUTE_Remove_filter_IfEmpty(Card* pParent, Card_Filter* p_filter);
    void R_vUTE_Remove_PRED_From_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);
    void R_vUTE_RenumberStmOdn(void);
    bool R_bUTE_ReplaceValueOfSub(Card* pCard, eCK eSub, Stz* szValue, Stz* szWith);
    void R_vUTE_Resolve_Param_Ordinal(Card_Term* p_term);
    Card_Table* R_pUTE_Resolve_table_By_tblSyn(char cTblSyn, i8 i_Before);
    Card_StmColOdn* R_pUTE_Resolve_tblColOdn_Into_stmColOdn(Card_Term* p_term_tblColOdn, i8 i_Before);
    void R_vUTE_Resolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(Card_BoolCalc* p_boolCalc);
    void R_vUTE_Unproject_foldExpr_After_having(void);
    void R_vUTE_Unproject_foldExpr_After_where(void);
    void R_vUTE_Yank_Correlated_cmpPred_From_sq_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED*& rp_PRED);

    // vSeries_10_Condense_Info
    void vR_S10_T80_Delete_Redundant_stage(void);
    void vR_S10_T90_Hide_Ignorable_stage(void);

    // vSeries_20_Table_Analysis

    // vSeries_22_Stream_Analysis

    // vSeries_24_Index_Analysis

    // vSeries_26_SubQuery_Analysis
    void vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void);
    void vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void);

    // vSeries_32_To_BoolCalc
    void vR_S32_T20_Convert_where_rpnCalc_To_boolCalc(void);

    // vSeries_33_To_TblColOdn
    void vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void);

    // vSeries_34_Predicate_Pullup

    // vSeries_36_Bound_Analysis
    void vR_S36_T30_Delete_Empty_filter(void);

    // vSeries_40_Flatten_Query

    // vSeries_50_Join_Ordering
    void vR_S50_T30_Compute_coPtnJoinList(void);
    void vR_S50_T40_Compute_crossPtnJoinList(void);
    void vR_S50_T50_Compute_ptndJoinSet(void);
    void vR_S50_T60_Copy_ptndJoinSet(void);
    void vR_S50_T70_Generate_ptndPermutations(void);
    void vR_S50_T80_Remove_invalidPermutations(void);
    void vR_S50_T92_Estimate_ptndJoins(void);
    void vR_S50_T94_Estimate_network(void);
    void vR_S50_T96_Estimate_joinOrder(void);
    void vR_S50_T98_Extract_joinOrderSeq(void);

    // vSeries_60_Index_Join
    void vR_S60_T05_Twist_in_Becomes_filter(void);

    // vSeries_75_EE_operators
    void vR_S75_T10_Morph_table_To_EEop(void);
    void vR_S75_T10_Morph_table_To_EEop_c(void);
    void vR_S75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_80_Index_Scan

    // vSeries_85_Cross_Join

    // vSeries_90_Stream_Ordering

    // vSeries_A0_Filter_Refinement
    void vR_SA0_T10_Individuate_indexJoin_filter_b(void);
    void vR_SA0_T10c_Individuate_crossJoin_filter(void);

    // vSeries_Z0_Restate_Pipeline
    void vR_SZ0_T20_Reinstate_stages(void);
    void vR_SZ0_T20_Reinstate_stagesV2(void);
    void vR_SZ0_T20_Reinstate_stagesV3(void);
    void vR_SZ0_T30_Reinstate_combined(void);
    void vR_SZ0_T40_Reinstate_next(void);
    void vR_SZ0_T40_Reinstate_next_b(void);

    void vRR_Convert_All_TblColOrd_to_StmColOrd(void);
    void vRR_Convert_All_StmColOrd_to_TblColOrd(void);



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

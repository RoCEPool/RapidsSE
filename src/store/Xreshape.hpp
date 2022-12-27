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
    void R_vUTE_Add_where(Card* p_after);
    void R_vUTE_Append_unproject_Of_foldExpr_After_having(void);
    void R_vUTE_Bump_keyOdn_In_rpnCalc_Of_Elist(Card_ELIST* p_ELIST);
    void R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(Card_ELIST* p_ELIST);
    void R_vUTE_Bump_where_BaseOn_foldExpr(void);
    void R_vUTE_Choose_index_For_crossJoin(void);
    void R_vUTE_Choose_index_For_indexJoin(void);
    void R_vUTE_Choose_index_For_indexScan(void);
    void R_vUTE_Choose_index_For_keyExists(void);
    void R_vUTE_Choose_index_For_keyNotExists(void);
    void R_vUTE_Derive_foldElist_From_project_stream(void);
    void R_vUTE_Derive_group_order_For_gbyAgg(void);
    void R_vUTE_Derive_stream_order_For_gbyAgg(void);
    void R_vUTE_Derive_stream_order_For_indexJoin(void);
    void R_vUTE_Derive_stream_order_For_indexScan(void);
    void R_vUTE_Derive_stream_order_For_project(void);
    void R_vUTE_Derive_stream_order_For_sort(void);
    void R_vUTE_Derive_order(void);
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
    void R_vUTE_Move_on_To_where(void);
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
    bool R_bUTE_Stage_Partitioning(Card* p_main_prior,
                                   Card_Combined* p_combined_prior,
                                   Card_Next* p_next_prior,
                                   Card* p_main_curr);
    void R_vUTE_Stream_AdjustLoHi(void);
    void R_vUTE_Unproject_foldExpr_After_having(void);
    void R_vUTE_Unproject_foldExpr_After_where(void);
    void R_vUTE_Yank_Correlated_cmpPred_From_sq_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED*& rp_PRED);

    // vSeries_26_SubQuery_Analysis
    void vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void);
    void vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void);

    // vSeries_33_To_TblColOdn
    void vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void);

    // vSeries_36_Bound_Analysis
    void vR_S36_T30_Delete_Empty_filter(void);

    // vSeries_75_EE_operators
    void vR_S75_T10_Morph_table_To_EEop_v2(void);
    void vR_S75_T30_Correct_stmColOdn_Of_keyExists_stream(void);
    void vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_A0_Make_Weavable
    void vR_SA0_T10_Individuate_indexJoin_filter_b(void);
    void vR_SA0_T10c_Individuate_crossJoin_filter(void);

    // vSeries_Z0_Restate_Pipeline
    void vR_SZ0_T20_Reinstate_stagesV3(void);

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

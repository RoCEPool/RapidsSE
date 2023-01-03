/*============================================================================*/
/* Xute.hpp --                                                                */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"
#include "Bench.hpp"

#include "PermGen.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"
#include "STORE.h"
#include "stddef.h"
#include "../base/eCK.h"
#include "../base/BASE.h"
#include "../primitives/BaseTypes_tcc.h"


namespace STORE
{
  class Table;

  class Xute
  {
  public:

    // utilities
    static void R_vUTE_AddPredToTableFilter(Card_Table* p_table, Card_PRED* p_PRED);
    static void R_vUTE_Add_where(Card* p_after);
    static void R_vUTE_Append_unproject_Of_foldExpr_After_having(void);

    static void vBump_keyOdn_In_rpnCalc_Of_Elist(CardIter* pCardIter, Card_ELIST* p_ELIST);
    static void vBump_stmOdn_In_rpnCalc_Of_Elist(CardIter* pCardIter, Card_ELIST* p_ELIST);
    static void R_vUTE_Bump_where_BaseOn_foldExpr(void);

    static void vChoose_index_For_crossJoin(CardIter* pCardIter);
    static void vChoose_index_For_indexJoin(CardIter* pCardIter);
    static void vChoose_index_For_indexScan(CardIter* pCardIter);
    static void vChoose_index_For_keyExists(CardIter* pCardIter);
    static void vChoose_index_For_keyNotExists(CardIter* pCardIter);
    static void R_vUTE_Convert_tblColOdn_Of_constraint_To_stmColNm(void);
    static void R_vUTE_Convert_tblColOdn_To_stmOdn(void);
    static void R_vUTE_Correct_having_BaseOn_foldExpr(void);

    static void R_vUTE_Derive_foldElist_From_project_stream(void);
    static void R_vUTE_Derive_group_order_For_gbyAgg(void);
    static void R_vUTE_Derive_stream_order_For_gbyAgg(void);
    static void vDerive_stream_order_For_indexJoin(CardIter* pCardIter);
    static void R_vUTE_Derive_stream_order_For_indexScan(void);
    static void R_vUTE_Derive_stream_order_For_project(void);
    static void vDerive_stream_order_For_sort(CardIter* pCardIter);
    static void R_vUTE_Derive_order(void);
    static void R_vUTE_Derive_foldExpr(void);

    static void R_vUTE_Enrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(void);
    static void R_vUTE_Enhance_sq_project_To_Include_Correlation_Column(void);

    static bool R_bUTE_FindAndMarkSubQryCall(void);
    static void R_vUTE_Find_stmCol_Definition_Of_Correlation_Column(void);
    static void R_vUTE_FoldIn_foldExpr(void);

    static void R_vUTE_Identify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(void);
    static void vIndividuate_filter(CardIter* pCardIter);

    static void R_vUTE_Move_corrPred_To_mq_table_As_bounds(Card_BoolCalc* p_boolCalc);
    static void vMove_on_To_where(CardIter* pCardIter);
    static void R_vUTE_MoveWhereToTableAsConstraint(Card_Table* p_table, Card_Where* p_where);
    static void R_vUTE_MoveWhereToTableAsFilter(Card_Table* p_table, Card_Where* p_where);

    static bool bOrder_Joins(CardIter* pCardIter);
    static void vPush_Correlated_cmpPred_Onto_mq_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED* p_PRED);
    static void R_vUTE_Push_foldExpr_Into_aggElist(void);
    static void R_vUTE_Push_foldExpr_Into_gbyElist(void);

    static void R_vUTE_RelabelExpressions(void);
    static void R_vUTE_RenumberStmOdn(void);
    static bool bReplaceValueOfSub(CardIter* pCardIter, Card* pCard, eCK eSub,
                                   Stz* szValue, Stz* szWith);
    static void R_vUTE_Resolve_Param_Ordinal(Card_Term* p_term);
    static Card_Table* R_pUTE_Resolve_table_By_tblSyn(char cTblSyn, i8 i_Before);
    static Card_StmColOdn* R_pUTE_Resolve_tblColOdn_Into_stmColOdn(Card_Term* p_term_tblColOdn, i8 i_Before);
    static void R_vUTE_Resolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(Card_BoolCalc* p_boolCalc);

    static bool R_bUTE_Stage_Partitioning(Card* p_main_prior,
                                   Card_Combined* p_combined_prior,
                                   Card_Next* p_next_prior,
                                   Card* p_main_curr);
    static void R_vUTE_Stream_AdjustLoHi(void);

    static void R_vUTE_Unproject_foldExpr_After_having(void);
    static void R_vUTE_Unproject_foldExpr_After_where(void);

    static void R_vUTE_Yank_Correlated_cmpPred_From_sq_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED*& rp_PRED);

    // vSeries_26_SubQuery_Analysis
    static void vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void);
    static void vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void);

    // vSeries_33_To_TblColOdn
    static void vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void);

    // vSeries_36_Bound_Analysis
    static void vS36_T30_Delete_Empty_filter(CardIter* pCardIter);

    // vSeries_75_EE_operators
    static void vR_S75_T10_Morph_table_To_EEop_v2(void);
    static void vS75_T30_Correct_stmColOdn_Of_keyExists_stream(CardIter* pCardIter);
    static void vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_A0_Make_Weavable
    static void vSA0_T10_Individuate_indexJoin_filter_b(CardIter* pCardIter);
    static void vSA0_T10c_Individuate_crossJoin_filter(CardIter* pCardIter);

    // vSeries_Z0_Restate_Pipeline
    static void vR_SZ0_T20_Reinstate_stagesV3(void);


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xute(void);
    ~Xute(void);

//  private:
//
//
//    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

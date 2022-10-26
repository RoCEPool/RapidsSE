/*============================================================================*/
/* Transforms.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "Xgearing.hpp"

namespace STORE
{
  class Transforms : public Xgearing
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Transforms(Bench* pBench);
   ~Transforms(void) noexcept(false);

    //void vAdd_columnRef_Under_term(bool bPrint, bool bTrace);

    void vP10_T10_Delete_Redundant_stage(bool bPrint, bool bTrace);
    void vP10_T20_Delete_absJoin(bool bPrint, bool bTrace);
    void vP10_T30_Delete_combined(bool bPrint, bool bTrace);
    void vP10_T40_Delete_send(bool bPrint, bool bTrace);
    void vP10_T50_Cull_scan_Leaving_table(bool bPrint, bool bTrace);

    void vP20_T10_Add_tableSyn_ForAll_table(bool bPrint, bool bTrace);
    void vP20_T20_Add_tablePtn_UnderAll_table(bool bPrint, bool bTrace);
    void vP20_T30_Add_stream_UnderAll_table(bool bPrint, bool bTrace);

    void vP30_T10_Calc_ordinalRange_ForAll_stream(bool bPrint, bool bTrace);

    void vP40_T10_Convert_where_calc_To_where_calcBool(bool bPrint, bool bTrace);
    void vP40_T20_Mark_where_pred_Wrt_table(bool bPrint, bool bTrace);
    void vP40_T30_Move_Marked_where_pred_To_table_filter(bool bPrint, bool bTrace);
    void vP40_T40_Delete_Empty_where(bool bPrint, bool bTrace);

    void vP50_T10_Rework_Exists_SubQueries(bool bPrint, bool bTrace);
    void vP50_T20_Copy_Up_SubQueries(bool bPrint, bool bTrace);
    void vP50_T30_Resolve_Parameter_Linkage(bool bPrint, bool bTrace);
    void vP50_T40_Resolve_Parameter_Terms(bool bPrint, bool bTrace);

    void vP60_T10_Add_indexJoin_UnderAllJoinable_table(bool bPrint, bool bTrace);
    void vP60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord(bool bPrint, bool bTrace);
    void vP60_T30_Add_joinPred_UnderAll_indexJoin(bool bPrint, bool bTrace);
    void vP60_T40_Cull_filter_pred_MovedUpTo_joinPred(bool bPrint, bool bTrace);
    void vP60_T50_Delete_Empty_filter(bool bPrint, bool bTrace);

    void vP70_T00_Extract_tableList_From_queryBoard(bool bPrint, bool bTrace);
    void vP70_T10_Extract_joinPredList(bool bPrint, bool bTrace);
    void vP70_T20_Compute_coPtnPredList(bool bPrint, bool bTrace);
    void vP70_T30_Compute_coPtnJoinList(bool bPrint, bool bTrace);
    void vP70_T40_Compute_crossPtnJoinList(bool bPrint, bool bTrace);
    void vP70_T50_Compute_ptndJoinSet(bool bPrint, bool bTrace);
    void vP70_T60_Copy_ptndJoinSet(bool bPrint, bool bTrace);
    void vP70_T70_Generate_ptndPermutations(bool bPrint, bool bTrace);
    void vP70_T80_Remove_invalidPermutations(bool bPrint, bool bTrace);
    void vP70_T90_Setup_costEst(bool bPrint, bool bTrace);
    void vP70_T92_Estimate_ptndJoins(bool bPrint, bool bTrace);
    void vP70_T94_Estimate_network(bool bPrint, bool bTrace);
    void vP70_T96_Estimate_joinOrder(bool bPrint, bool bTrace);
    void vP70_T98_Extract_joinOrderSeq(bool bPrint, bool bTrace);
    void vP70_T99_Convert_table_To_scan_Or_indexJoin(bool bPrint, bool bTrace);

    void vP80_T10_Insert_indexList_ToAll_scan(bool bPrint, bool bTrace);
    void vP80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(bool bPrint, bool bTrace);
    void vP80_T30_Mark_scan_pred_bounds(bool bPrint, bool bTrace);
    void vP80_T40_Mark_scan_filter_term_WhichMatchIndex(bool bPrint, bool bTrace);
    void vP80_T50_Add_scanPred(bool bPrint, bool bTrace);
    void vP80_T60_Cull_Marked_cmpPred(bool bPrint, bool bTrace);
    void vP80_T70_Promote_scan_To_indexScan(bool bPrint, bool bTrace);

    void vP90_T10_Remove_sort_IfSameAs_gby(bool bPrint, bool bTrace);

    void vPA0_T10_Individuate_filter(bool bPrint, bool bTrace);

    void vPZ0_T10_Reinstate_stages(bool bPrint, bool bTrace);
    void vPZ0_T20_Reinstate_combined(bool bPrint, bool bTrace);

  private:

    void vMarkPredLeftwards(Card_BoolCalc* p_bool);
    void vMarkPredDrillDown(Card_PRED* p_pred);

    char cMovePredLeftwards(Card_PRED* p_pred);
    char cMovePredToTable(Card_Table* p_table, Card_PRED* p_pred);
    bool bStreamContainsPred(Card_Stream* p_stream, Card_PRED* pCard_PRED);
    void vCopyPredLeftwards(Card_BoolCalc* p_calcBool);
    bool bCopyPredDrillDown(Card_PRED*, Card_BoolCalc* p_calcBool);
    void vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred);
    void vCopyPredToNewFilter(Card_Filter* p_filter_new, Card_PRED* p_pred);

    void vMarkPredAsMoved(Card* pPredCard);

    Footer* aFooter(void);
  };

  Transforms* _pTransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Transforms.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "Modifications.hpp"

namespace STORE
{
  class Transforms : public Modifications
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Transforms(Bench* pBench);
   ~Transforms(void) noexcept(false);

    void vAdd_columnRef_Under_term(void);

    void vReinstate_combined(eBK eBoard, bool bTrace);
    void vAdd_scanPred(eBK eBoard, bool bTrace);
    void vReinstate_stages(eBK eBoard, bool bTrace);
    void vAdd_tablePtn_UnderAll_table(eBK eBoard, bool bTrace);
    void vInsert_indexList_ToAll_scan(eBK eBoard, bool bTrace);
    void vAdd_stream_UnderAll_table(eBK eBoard, bool bTrace);
    void vMark_where_pred_Wrt_table(eBK eBoard, bool bTrace);
    void vCalc_ordinalRange_ForAll_stream(eBK eBoard, bool bTrace);
    void vAdd_tableSyn_ForAll_table(eBK eBoard, bool bTrace);
    void vCompute_coPtnJoinList(eBK eBoard, bool bTrace);
    void vCompute_coPtnPredList(eBK eBoard, bool bTrace);
    void vCompute_crossPtnJoinList(eBK eBoard, bool bTrace);
    void vCompute_ptndJoinSet(eBK eBoard, bool bTrace);
    void vConvert_idxCol_name_Of_scan_To_idxCol_ord(eBK eBoard, bool bTrace);
    void vConvert_idxCol_name_Of_table_To_idxCol_ord(eBK eBoard, bool bTrace);
    void vConvert_where_calc_To_where_calcBool(eBK eBoard, bool bTrace);
    void vCopy_ptndJoinSet(eBK eBoard, bool bTrace);
    void vCull_filter_pred_MovedUpTo_joinPred(eBK eBoard, bool bTrace);
    void vCull_scan_Leaving_table(eBK eBoard, bool bTrace);
    void vCull_markedPred(eBK eBoard, bool bTrace);
    void vRemove_sort_IfSameAs_gby(eBK eBoard, bool bTrace);
    void vDelete_absJoin(eBK eBoard, bool bTrace);
    void vDelete_combined(eBK eBoard, bool bTrace);
    void vDelete_send(eBK eBoard, bool bTrace);
    void vDelete_Empty_where(eBK eBoard, bool bTrace);
    void vDelete_Empty_filter(eBK eBoard, bool bTrace);
    void vEstimate_joinOrder(eBK eBoard, bool bTrace);
    void vEstimate_network(eBK eBoard, bool bTrace);
    void vEstimate_ptndJoins(eBK eBoard, bool bTrace);
    void vExpandedForm(eBK eBoard, bool bTrace);
    void vExtract_joinOrderSeq(eBK eBoard, bool bTrace);
    void vExtract_joinPredList(eBK eBoard, bool bTrace);
    void vExtract_tableList_From_queryBoard(eBK eBoard, bool bTrace);
    void vAdd_joinPred_UnderAll_indexJoin(eBK eBoard, bool bTrace);
    void vGenerate_ptndPermutations(eBK eBoard, bool bTrace);
    void vIndividuate_filter(eBK eBoard, bool bTrace);
    void vAdd_indexJoin_UnderAllJoinable_table(eBK eBoard, bool bTrace);
    void vDelete_Redundant_stage(eBK eBoard, bool bTrace);
    void vMark_scan_pred_bounds(eBK eBoard, bool bTrace);
    void vMark_scan_cmpPred_WhichMatchIndex(eBK eBoard, bool bTrace);
    void vMove_Marked_where_pred_To_table_filter(eBK eBoard, bool bTrace);
    void vPromote_scan_To_indexScan(eBK eBoard, bool bTrace);
    void vConvert_table_To_scan_Or_indexJoin(eBK eBoard, bool bTrace);
    void vRemove_invalidPermutations(eBK eBoard, bool bTrace);
    void vSetup_costEst(eBK eBoard, bool bTrace);

  private:

    void vMarkPredLeftwards(Card_BoolCalc* p_bool);
    void vMarkPredDrillDown(Card_PRED* p_pred);

    char cMovePredLeftwards(Card_PRED* p_pred);
    char cMovePredToTable(Card_Table* p_table, Card_PRED* p_pred);

    void vCopyPredLeftwards(Card_BoolCalc* p_calcBool);
    bool bCopyPredDrillDown(Card_PRED*, Card_BoolCalc* p_calcBool);
    void vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred);
    void vCopyPredToNewFilter(Card_Filter* p_filter_new, Card_PRED* p_pred);

    void vMarkPredAsMoved(Card* pPredCard);

    Card_IndexPred*  pTo_IndexPred (Card_AbsJoin* pCard_AbsJoin,
                                    Card_CmpPred* pCard_PredCmp);

    Footer* aFooter(void);
  };

  Transforms* _pTransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

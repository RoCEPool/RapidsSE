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

    void vReinstate_combined(void);
    void vAdd_scanPred(void);
    void vReinstate_stages(void);
    void vAdd_tablePtn_UnderAll_table(void);
    void vAppend_indexList_ToAll_scan(void);
    void vAdd_stream_UnderAll_table(void);
    void vMark_where_pred_Wrt_table(void);
    void vCalc_ordinalRange_ForAll_stream(void);
    void vAdd_tableSyn_ForAll_table(void);
    void vCompute_coPtnJoinList(void);
    void vCompute_coPtnPredList(void);
    void vCompute_crossPtnJoinList(void);
    void vCompute_ptndJoinSet(void);
    void vConvert_idxCol_name_Of_scan_To_idxCol_ord(void);
    void vConvert_idxCol_name_Of_table_To_idxCol_ord(void);
    void vConvert_where_calc_To_where_calcBool(void);
    void vCopy_ptndJoinSet(void);
    void vCull_filter_pred_MovedUpTo_joinPred(void);
    void vCull_scan_Leaving_table(void);
    void vCull_markedPred(void);
    void vRemove_sort_IfSameAs_gby(void);
    void vDelete_absJoin(void);
    void vDelete_combined(void);
    void vDelete_send(void);
    void vDelete_Empty_where(void);
    void vDelete_Empty_filter(void);
    void vEstimate_joinOrder(void);
    void vEstimate_network(void);
    void vEstimate_ptndJoins(void);
    void vExpandedForm(void);
    void vExtract_joinOrderSeq(void);
    void vExtract_joinPredList(void);
    void vExtract_tableList_From_queryBoard(void);
    void vAdd_joinPred_UnderAll_indexJoin(void);
    void vGenerate_ptndPermutations(void);
    void vIndividuate_filter(void);
    void vAdd_indexJoin_UnderAllJoinable_table(void);
    void vDelete_Redundant_stage(void);
    void vMark_scan_pred_bounds(void);
    void vMark_scan_pred_ordinals(void);
    void vMove_Marked_where_pred_To_table_filter(void);
    void vPromote_scan_To_indexScan(void);
    void vConvert_table_To_scan_Or_indexJoin(void);
    void vRemove_invalidPermutations(void);
    void vSetup_costEst(void);

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

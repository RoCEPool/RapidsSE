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
    void vConvert_streamColOrdinal_To_tableColOrdinal(void);

    void vAdd_combined(void);
    void vAdd_scanPred(void);
    void vAdd_stages(void);
    void vAppend_tablePtn_UnderAll_table(void);
    void vAppend_indexList_ToAll_table(void);
    void vAppend_stream_UnderAll_table(void);
    void vAssignAll_cmpPred_To_table(void);
    void vCalc_ordinalRanges_ForAll_stream(void);
    void vCalc_tableSyn_ForAll_table(void);
    void vCompute_coPtnJoinList(void);
    void vCompute_coPtnPredList(void);
    void vCompute_crossPtnJoinList(void);
    void vCompute_ptndJoinSet(void);
    void vConvertAll_idxColNm_To_streamColOrd(void);
    void vConvert_calc_OfAll_filter_To_calcBool(void);
    void vCopy_ptndJoinSet(void);
    void vCullAll_filterPred_Of_indexJoin(void);
    void vCullAll_scan(void);
    void vCull_markedPred(void);
    void vCull_sby_SameAs_gby(void);
    void vDeleteAll_absJoin(void);
    void vDeleteAll_combined(void);
    void vDeleteAll_send(void);
    void vDeleteAll_Empty_filter(void);
    void vDeleteAll_EmptySub_filter(void);
    void vEstimate_joinOrder(void);
    void vEstimate_network(void);
    void vEstimate_ptndJoins(void);
    void vExtract_joinOrderSeq(void);
    void vExtract_joinPredList(void);
    void vExtract_tableList_From_queryBoard(void);
    void vFormAll_joinPred_of_indexJoin(void);
    void vGenerate_ptndPermutations(void);
    void vIndividuate_filter(void);
    void vInsert_indexJoin_UnderAll_table(void);
    void vLinearizeAll_stage(void);
    void vMark_scan_pred_bounds(void);
    void vMark_scan_pred_ordinals(void);
    void vMoveAll_cmpPred_To_table(void);
    void vPromote_scan_To_indexScan(void);
    void vRefactorAll_table_Into_join(void);
    void vRemove_invalidPermutations(void);
    void vSetup_costEst(void);
    void vSubsume_scan_Under_absJoin(void);

  private:

    void vAssignFilterLeftwards(Card_Filter* pCard_filter);
    void vAssignPredDrillDown(Card_PRED*, Card_Filter* pCard_filter);

    char cAssignPredLeft(Card_PRED* pCard_PRED, Card_Filter* pCard_filter);
    char cAssignPredToTable(Card_Table* pCard_table, Card_PRED* pCard_PRED);

    void vCopyFilterLeftwards(Card_Filter* pCard_filter);
    bool bCopyPredDrillDown(Card_PRED*, Card_Filter* pCard_filter);
    void vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred);
    void vCopyPredToNewFilter(Card_Filter* pCard_filter_new, Card_PRED* pCard_pred);

    void vMarkPredCardsAsMoved(Card* pPredCard);

    Card_IndexPred*  pTo_IndexPred (Card_AbsJoin* pCard_AbsJoin,
                                    Card_CmpPred* pCard_PredCmp);

    Footer* aFooter(void);
  };

  Transforms* _pTransforms(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

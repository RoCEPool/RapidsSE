/*============================================================================*/
/* Transforms.hpp --                                                          */
/*============================================================================*/
#pragma once

#include "Xlator.hpp"

namespace STORE
{
  class Transforms : public Xlator
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

    void vReinstate_combined(bool bPrint, bool bTrace);
    void vAdd_scanPred(bool bPrint, bool bTrace);
    void vReinstate_stages(bool bPrint, bool bTrace);
    void vAdd_tablePtn_UnderAll_table(bool bPrint, bool bTrace);
    void vInsert_indexList_ToAll_scan(bool bPrint, bool bTrace);
    void vAdd_stream_UnderAll_table(bool bPrint, bool bTrace);
    void vMark_where_pred_Wrt_table(bool bPrint, bool bTrace);
    void vCalc_ordinalRange_ForAll_stream(bool bPrint, bool bTrace);
    void vAdd_tableSyn_ForAll_table(bool bPrint, bool bTrace);
    void vCompute_coPtnJoinList(bool bPrint, bool bTrace);
    void vCompute_coPtnPredList(bool bPrint, bool bTrace);
    void vCompute_crossPtnJoinList(bool bPrint, bool bTrace);
    void vCompute_ptndJoinSet(bool bPrint, bool bTrace);
    void vConvert_idxCol_name_Of_scan_To_idxCol_ord(bool bPrint, bool bTrace);
    void vConvert_idxCol_name_Of_table_To_idxCol_ord(bool bPrint, bool bTrace);
    void vConvert_where_calc_To_where_calcBool(bool bPrint, bool bTrace);
    void vCopy_ptndJoinSet(bool bPrint, bool bTrace);
    void vCull_filter_pred_MovedUpTo_joinPred(bool bPrint, bool bTrace);
    void vCull_scan_Leaving_table(bool bPrint, bool bTrace);
    void vCull_Marked_cmpPred(bool bPrint, bool bTrace);
    void vRemove_sort_IfSameAs_gby(bool bPrint, bool bTrace);
    void vDelete_absJoin(bool bPrint, bool bTrace);
    void vDelete_combined(bool bPrint, bool bTrace);
    void vDelete_send(bool bPrint, bool bTrace);
    void vDelete_Empty_where(bool bPrint, bool bTrace);
    void vDelete_Empty_filter(bool bPrint, bool bTrace);
    void vEstimate_joinOrder(bool bPrint, bool bTrace);
    void vEstimate_network(bool bPrint, bool bTrace);
    void vEstimate_ptndJoins(bool bPrint, bool bTrace);
    void vExpandedForm(bool bPrint, bool bTrace);
    void vExtract_joinOrderSeq(bool bPrint, bool bTrace);
    void vExtract_joinPredList(bool bPrint, bool bTrace);
    void vExtract_tableList_From_queryBoard(bool bPrint, bool bTrace);
    void vAdd_joinPred_UnderAll_indexJoin(bool bPrint, bool bTrace);
    void vGenerate_ptndPermutations(bool bPrint, bool bTrace);
    void vIndividuate_filter(bool bPrint, bool bTrace);
    void vAdd_indexJoin_UnderAllJoinable_table(bool bPrint, bool bTrace);
    void vDelete_Redundant_stage(bool bPrint, bool bTrace);
    void vMark_scan_pred_bounds(bool bPrint, bool bTrace);
    void vMark_scan_filter_term_WhichMatchIndex(bool bPrint, bool bTrace);
    void vMove_Marked_where_pred_To_table_filter(bool bPrint, bool bTrace);
    void vPromote_scan_To_indexScan(bool bPrint, bool bTrace);
    void vConvert_table_To_scan_Or_indexJoin(bool bPrint, bool bTrace);
    void vRemove_invalidPermutations(bool bPrint, bool bTrace);
    void vSetup_costEst(bool bPrint, bool bTrace);
    void vAccess_IndexMetadata(bool bPrint, bool bTrace);
    void vAccess_PartitionMetadata(bool bPrint, bool bTrace);
    void vAccess_StreamMetadata(bool bPrint, bool bTrace);

    void vRework_Exists_SubQueries(bool bPrint, bool bTrace);
    void vCopy_Up_SubQueries(bool bPrint, bool bTrace);
    void vResolve_Parameter_Linkage(bool bPrint, bool bTrace);
    void vResolve_Parameter_Terms(bool bPrint, bool bTrace);

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

 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "Board.hpp"

namespace STORE
{
  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    char m_cPrefix;

    bool m_bIsFlat;

    i8           m_iOdnLo;
    i8           m_iOdnHi;
    char         m_cTableSyn;

  public:
    CardIter*    m_pCI;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(Optimiser* pOptimiser);
   ~Bench(void) noexcept(false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // series
    bool bApplyTransformationMatrix(eEC* peErrorCode);
    void vApplyTransformationMatrix_originalModel(void);
    void vApplyTransformationMatrix_recursiveModel(void);

    void vSeries_PreparePipelines_originalModel(void);
    void vSeries_PreparePipelines_recursiveModel(void);
    void vSeries_MakeRunnable_originalModel(void);
    void vSeries_MakeRunnable_recursiveModel(void);
    void vSeries_MakeRunnable_recurse(void);
    void vSeries_Optimisations(void);
    void vSeries_MakeWeavable(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // group

    void vGroup_InitialAdjustments(void);
    void vGroup_CondenseInfo_originalModel(void);
    void vGroup_CondenseInfo_recursiveModel(void);
    void vGroup_MetaAnalysis(void);
    void vGroup_MainConversions(void);
    void vGroup_PreliminaryConversions_recursiveModel(void);
    void vGroup_FinalConversions_recursiveModel(void);

    void vGroup_PredicatePullup_originalModel(void);
    void vGroup_PredicatePullup_recursiveModel(void);
    void vGroup_IndexBounds_originalModel(void);
    void vGroup_IndexBounds_recursiveModel(void);
    void vGroup_JoinOrdering_originalModel(void);
    void vGroup_JoinOrdering_recursiveModel(void);
    void vGroup_FlattenSubquery_originalModel(void);
    void vGroup_FlattenSubquery_recursiveModel(void);

    void vGroup_ChooseEEops(void);

    void vGroup_CullBounds(void);
    void vGroup_Filters(void);
    void vGroup_Staging(void);
    void vGroup_Partitioning(void);
    void vGroup_FinalAdjustments(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // transform

    void vTransform_MoveOn(void);
    void vTransform_DeleteUnneeded(void);
    void vTransform_ScanBecomesTable(void);
    void vTransform_AddLevel(void);
    void vTransform_NotOps(void);

    void vTransform_TableAnalysis(void);
    void vTransform_StreamAnalysis(void);
    void vTransform_IndexAnalysis(void);
    void vTransform_ProjectionAnalysis(void);

    void vTransform_StmColToTblCol_originalModel(void);
    void vTransform_StmColToTblCol_recursiveModel(void);
    void vTransform_RpnCalcToBoolCalc(void);

    void vTransform_MarkCmpPredWrtTable(void);
    void vTransform_CopyMarkedCmpPred(void);
    void vTransform_CullMarkedCmpPred(void);
    void vTransform_DeleteEmptyWhere(void);

    void vTransform_JoinOrdering_originalModel(void);
    void vTransform_PredicateReposition_originalModel(void);
    void vTransform_DeriveBoundsFromFilter_originalModel(void);

    void vTransform_OuterQueryAnalysis(void);
    void vTransform_SubQueryAnalysisMainQuery(void);
    void vTransform_SubQueryAnalysisSubQuery(void);
    void vTransform_FlattenByDecisionTree_originalModel(void);
    void vTransform_FlattenByDecisionTree_recursiveModel(void);

    void vTransform_MorphTableToEEop(void);

    void vTransform_CullNonusedBounds(void);
    void vTransform_CullUsedBoundsFromFilter(void);
    void vTransform_IndividuateFilter(void);

    void vTransform_ReinstateStages(void);
    void vTransform_AddPartitioning(void);

    void vTransform_TblColToStmCol(void);
    void vTransform_StmColToColNmOfKeyExists(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // fold

    void vFlatten_10_F(void);
    void vFlatten_20_W_A_C_ro(void);
    void vFlatten_30_W_A_NC_I(void);
    void vFlatten_40_W_A_NC_ro(void);
    void vFlatten_50_W_L_C_E(void);
    void vFlatten_60_W_L_NC_NI(void);
    void vFlatten_70_H_A_NC_ro(void);
    void vFlatten_70_H_A_NC_ro_smoothing(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // optimisation
    void vOptimiseLoop(void);

    bool bOptimise_CutSort(void);
    bool bOptimise_DupFilter(void);
    bool bOptimise_ElyFilters(void);
    bool bOptimise_ElyProjections(void);
    bool bOptimise_MinGroupby(void);
    bool bOptimise_OtmCalculations(void);
    bool bOptimise_PpnProjections(void);
    bool bOptimise_RplAggregByOrdAgg(void);
    bool bOptimise_RplAggregBySortOrdAgg(void);
    bool bOptimise_RplMaxByTop(void);
    bool bOptimise_RplScanByIndexScan(void);
    bool bOptimise_RplSortDescLimitByTop(void);
    bool bOptimise_RplSortLimitOnOriginByAllLevels(void);
    bool bOptimise_SeqJoinOrder(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddBoard(Board* pBoard);
    bool bBoardExists(eBK eBoard, i8 i_SubBoard = C_UNKNOWN);
    i8   nBoards(void);
    CardCtl* pCardCtl(void);
    void vClearMarks(void);
    Board* pGetBoard(eBK eBoard, i8 i_Board = C_UNKNOWN);
    Board* pGetBoard(i8 i_Board);
    Board* pGetBoard(Stz* szBoardNm);
    bool bIsFlat(void);
    void vIsFlat(bool bIsFlat);
    Optimiser* pOptimiser(void);
    char cPrefix(void);
    void vPrefix(char cPrefix);

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);



    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vS10_T00_Begin(void);

    void vS26_T10_SubQuery_Analysis_Of_Main_Query(void);
    void vS26_T20_SubQuery_Analysis_Of_Sub_Query(void);

    void vS32_T_SubQuery_Analysis(void);



    void vS75_T20_Morph_keyExists_filter_To_constraint(void);
    void vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_80_Index_Scan
    bool bS80_T00_Test(bool bEnabled);
    void vS80_T00_Skip(void);
    void vS80_T00_Begin(void);
    void vS80_T30_Morph_scan_To_indexScan(void);

    // vSeries_90_Stream_Ordering
    bool bS90_T00_Test(bool bEnabled);
    void vS90_T00_Skip(void);
    void vS90_T00_Begin(void);
    void vS90_T30_Derive_order(void);

    // vSeries_A0_Make_Weavable
    void vSA0_T00_Begin(void);

    void vAssert_Board_any(void);
    void vAssert_Board_mqA(void);
    void vAssert_Board_sqB(void);

    void vST00_Begin(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vStreamAnalysisReset(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

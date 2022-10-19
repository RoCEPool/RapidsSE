/*============================================================================*/
/* Transformer.cpp --                                                           */
/*============================================================================*/

#include "Transformer.hpp"
#include "Transforms.hpp"
#include "Optimiser.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Transformer,CN_TRANSFORMER)
__aNewFunction(Transformer,CN_TRANSFORMER)
__vDeleteFunction(Transformer,CN_TRANSFORMER)
__aFooterFunction(Transformer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Transformer::Transformer(Bench* pBench)
: OptimiserTool(CN_TRANSFORMER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  m_pBench       = pBench;
  m_pOptimiser   = m_pBench->pOptimiser();
  m_pBoard       = nullptr;

  m_pCardCtl     = m_pOptimiser->pCardCtl();

  m_pTransforms  = nullptr;

  m_pCurator     = nullptr;

  m_bInteractive = false;

  _checkInitialised_(CN_TRANSFORMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transformer::~Transformer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  delete m_pTransforms;

  delete m_pCurator;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform                                                                  */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();

  //m_pBoard = m_pOptimiser->pHeadBoard();c

  m_pTransforms  = new Transforms(m_pBench);
  m_pCurator     = new Curator(m_pBench);

  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pJobTags->bHasName("<transform3>"_Stz)) vTransform3();
  else vTransform3();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform 2                                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform3(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  //vTrace("Transformer running vTransform3\n"_Stz);

  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);
  //m_pTransforms->vExpandedForm(false);

  vPhase_1_ReducePipeline   (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_2_TableAnalysis    (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_3_WherePredicates  (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_U_ExistsSubqueries (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_4_InitialJoin      (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_5_JoinOrdering     (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_6_IndexScan        (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_7_StreamOrdering   (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_8_FilterRefinement (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);
  vPhase_Z_RestatePipeline  (C_PRINT_PHASE,C_PRINT_BOARD,C_TRACE_N);

  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);
  //m_pTransforms->vExpandedForm(false);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_1_ReducePipeline(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);

  for (i8 i_SubBoard = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_SubBoard);
       i_SubBoard++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_SubBoard);

    m_pTransforms->vDelete_Redundant_stage (bPrintBoard,bTrace);
    m_pTransforms->vDelete_absJoin         (bPrintBoard,bTrace);
    m_pTransforms->vDelete_combined        (bPrintBoard,bTrace);
    m_pTransforms->vDelete_send            (bPrintBoard,bTrace);
    m_pTransforms->vCull_scan_Leaving_table(bPrintBoard,bTrace);
  }

  m_pBench->vPrintBoards(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_2_TableAnalysis(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_tableSyn_ForAll_table(bPrintBoard,bTrace);

  m_pTransforms->vAdd_tablePtn_UnderAll_table(bPrintBoard,bTrace);

  m_pTransforms->vAdd_stream_UnderAll_table(bPrintBoard,bTrace);

  m_pTransforms->vCalc_ordinalRange_ForAll_stream(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_3_WherePredicates(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vConvert_where_calc_To_where_calcBool(bPrintBoard,bTrace);

  m_pTransforms->vMark_where_pred_Wrt_table(bPrintBoard,bTrace);

  m_pTransforms->vMove_Marked_where_pred_To_table_filter(bPrintBoard,bTrace);

  m_pTransforms->vDelete_Empty_where(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_U_ExistsSubqueries(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vRework_Exists_SubQueries(bPrintBoard,bTrace);

  m_pTransforms->vCopy_Up_SubQueries(bPrintBoard,bTrace);

  m_pTransforms->vResolve_Parameter_Linkage(bPrintBoard,bTrace);

  m_pTransforms->vResolve_Parameter_Terms(bPrintBoard,C_TRACE_Y);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_4_InitialJoin(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_indexJoin_UnderAllJoinable_table(bPrintBoard,bTrace);

  m_pTransforms->vConvert_idxCol_name_Of_table_To_idxCol_ord(bPrintBoard,bTrace);

  m_pTransforms->vAdd_joinPred_UnderAll_indexJoin(bPrintBoard,bTrace);

  m_pTransforms->vCull_filter_pred_MovedUpTo_joinPred(bPrintBoard,bTrace);

  m_pTransforms->vDelete_Empty_filter(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_5_JoinOrdering(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vExtract_tableList_From_queryBoard(bPrintBoard,bTrace);

  // add <term/<columnRef> to all ordinal <term>
  m_pTransforms->vAdd_columnRef_Under_term(bPrintBoard,bTrace);

  m_pTransforms->vExtract_joinPredList(bPrintBoard,bTrace);
  m_pTransforms->vCompute_coPtnPredList(bPrintBoard,bTrace);
  m_pTransforms->vCompute_coPtnJoinList(bPrintBoard,bTrace);
  m_pTransforms->vCompute_crossPtnJoinList(bPrintBoard,bTrace);
  m_pTransforms->vCompute_ptndJoinSet(bPrintBoard,bTrace);

  m_pTransforms->vCopy_ptndJoinSet(bPrintBoard,bTrace);
  m_pTransforms->vGenerate_ptndPermutations(bPrintBoard,bTrace);
  m_pTransforms->vRemove_invalidPermutations(bPrintBoard,bTrace);

  m_pTransforms->vSetup_costEst(bPrintBoard,bTrace);
  m_pTransforms->vEstimate_ptndJoins(bPrintBoard,bTrace);
  m_pTransforms->vEstimate_network(bPrintBoard,bTrace);
  m_pTransforms->vEstimate_joinOrder(bPrintBoard,bTrace);
  m_pTransforms->vExtract_joinOrderSeq(bPrintBoard,bTrace);
  */

  m_pTransforms->vConvert_table_To_scan_Or_indexJoin(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_6_IndexScan(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  /*
 *
 *    <scan>       <scan>
 *      <table>    <table>
 *      <stream>   <indexList>
 *      <filter>   <stream>
 *                 <filter>
 *
 */

  m_pTransforms->vInsert_indexList_ToAll_scan(bPrintBoard,bTrace);

  m_pTransforms->vConvert_idxCol_name_Of_scan_To_idxCol_ord(bPrintBoard,bTrace);

  m_pTransforms->vMark_scan_pred_bounds(bPrintBoard,bTrace);

  m_pTransforms->vMark_scan_filter_term_WhichMatchIndex(bPrintBoard,bTrace);

  m_pTransforms->vAdd_scanPred(bPrintBoard,C_TRACE_Y);

  m_pTransforms->vCull_Marked_cmpPred(bPrintBoard,bTrace);

  m_pTransforms->vPromote_scan_To_indexScan(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_7_StreamOrdering(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vRemove_sort_IfSameAs_gby(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_8_FilterRefinement(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vIndividuate_filter(bPrintBoard,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_Z_RestatePipeline(bool bPrintPhase, bool bPrintBoard, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vReinstate_stages(bPrintBoard,bTrace);

  m_pTransforms->vReinstate_combined(bPrintBoard,bTrace);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

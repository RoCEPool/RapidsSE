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

  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  vPhase_10_ReducePipeline  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_20_AccessMetadata  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_30_StreamAnalysis  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_40_PredicatePullup (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_50_FlattenExists   (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_60_InitialJoin     (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_70_JoinOrdering    (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_80_IndexScan       (C_PRINT_PHASE,   C_PRINT_XFORM, C_TRACE_Y);
  vPhase_90_StreamOrdering  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_A0_FilterRefinement(C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_Z0_RestatePipeline (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_10_ReducePipeline(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);

  for (i8 i_SubBoard = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_SubBoard);
       i_SubBoard++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_SubBoard);

    m_pTransforms->vDelete_Redundant_stage (bPrintXform,bTrace);
    m_pTransforms->vDelete_absJoin         (bPrintXform,bTrace);
    m_pTransforms->vDelete_combined        (bPrintXform,bTrace);
    m_pTransforms->vDelete_send            (bPrintXform,bTrace);
    m_pTransforms->vCull_scan_Leaving_table(bPrintXform,bTrace);
  }

  m_pCardCtl->vReset();

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_20_AccessMetadata(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_tablePtn_UnderAll_table    (bPrintXform,bTrace);
  m_pTransforms->vAdd_stream_UnderAll_table      (bPrintXform,bTrace);

  m_pCardCtl->vReset();
  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_30_StreamAnalysis(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_tableSyn_ForAll_table      (bPrintXform,bTrace);
  m_pTransforms->vCalc_ordinalRange_ForAll_stream(bPrintXform,bTrace);

  m_pCardCtl->vReset();

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_40_PredicatePullup(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vConvert_where_calc_To_where_calcBool  (bPrintXform,bTrace);
  m_pTransforms->vMark_where_pred_Wrt_table             (bPrintXform,bTrace);
  m_pTransforms->vMove_Marked_where_pred_To_table_filter(bPrintXform,bTrace);
  m_pTransforms->vDelete_Empty_where                    (bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_50_FlattenExists(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vRework_Exists_SubQueries (bPrintXform,bTrace);
  m_pTransforms->vCopy_Up_SubQueries       (bPrintXform,bTrace);
  m_pTransforms->vResolve_Parameter_Linkage(bPrintXform,bTrace);
  m_pTransforms->vResolve_Parameter_Terms  (bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_60_InitialJoin(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_indexJoin_UnderAllJoinable_table      (bPrintXform,bTrace);
  m_pTransforms->vConvert_idxCol_name_Of_table_To_idxCol_ord(bPrintXform,bTrace);
  m_pTransforms->vAdd_joinPred_UnderAll_indexJoin           (bPrintXform,bTrace);
  m_pTransforms->vCull_filter_pred_MovedUpTo_joinPred       (bPrintXform,bTrace);
  m_pTransforms->vDelete_Empty_filter                       (bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_70_JoinOrdering(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vExtract_tableList_From_queryBoard(bPrintXform,bTrace);

  // add <term/<columnRef> to all ordinal <term>
  m_pTransforms->vAdd_columnRef_Under_term(bPrintXform,bTrace);

  m_pTransforms->vExtract_joinPredList(bPrintXform,bTrace);
  m_pTransforms->vCompute_coPtnPredList(bPrintXform,bTrace);
  m_pTransforms->vCompute_coPtnJoinList(bPrintXform,bTrace);
  m_pTransforms->vCompute_crossPtnJoinList(bPrintXform,bTrace);
  m_pTransforms->vCompute_ptndJoinSet(bPrintXform,bTrace);

  m_pTransforms->vCopy_ptndJoinSet(bPrintXform,bTrace);
  m_pTransforms->vGenerate_ptndPermutations(bPrintXform,bTrace);
  m_pTransforms->vRemove_invalidPermutations(bPrintXform,bTrace);

  m_pTransforms->vSetup_costEst(bPrintXform,bTrace);
  m_pTransforms->vEstimate_ptndJoins(bPrintXform,bTrace);
  m_pTransforms->vEstimate_network(bPrintXform,bTrace);
  m_pTransforms->vEstimate_joinOrder(bPrintXform,bTrace);
  m_pTransforms->vExtract_joinOrderSeq(bPrintXform,bTrace);
  */

  m_pTransforms->vConvert_table_To_scan_Or_indexJoin(bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_80_IndexScan(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
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

  m_pTransforms->vInsert_indexList_ToAll_scan              (bPrintXform,bTrace);
  m_pTransforms->vConvert_idxCol_name_Of_scan_To_idxCol_ord(bPrintXform,bTrace);
  m_pTransforms->vMark_scan_pred_bounds                    (bPrintXform,bTrace);
  m_pTransforms->vMark_scan_filter_term_WhichMatchIndex    (bPrintXform,bTrace);
  m_pTransforms->vAdd_scanPred                             (bPrintXform,bTrace);
  m_pTransforms->vCull_Marked_cmpPred                      (bPrintXform,bTrace);
  m_pTransforms->vPromote_scan_To_indexScan                (bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_90_StreamOrdering(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vRemove_sort_IfSameAs_gby(bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_A0_FilterRefinement(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vIndividuate_filter(bPrintXform,bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_Z0_RestatePipeline(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(bPrintPhase,__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vReinstate_stages  (bPrintXform, bTrace);
  m_pTransforms->vReinstate_combined(bPrintXform, bTrace);

  if (bPrintBench(bPrintPhase,__func__))
    m_pTransforms->vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Transformer::bPrintBench(bool bPrintPhase, const char* azFuncNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  if (AM::JOB::bHasTag("<cue>"_Stz)) {
    Stz* szTagCue = AM::JOB::szTagValue("<cue>"_Stz);
    if (szTagCue->bSame((Stz*)azFuncNm))
      return true;
  }

  return bPrintPhase;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

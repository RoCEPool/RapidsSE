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

  vPhase_10_ReducePipeline  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_20_AccessMetadata  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_30_StreamAnalysis  (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_40_PredicatePullup (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_50_FlattenExists   (C_PRINT_PHASE, C_QUIET_XFORM, C_TRACE_N);
  vPhase_60_InitialJoin     (C_PRINT_PHASE,   C_PRINT_XFORM, C_TRACE_Y);
  vPhase_70_JoinOrdering    (C_PRINT_PHASE,   C_PRINT_XFORM, C_TRACE_Y);
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

  vPhase(BK_SUBPIPE, 0, bPrintPhase,__func__);

  m_pCardCtl->vReset();
  m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,0);

  for (i8 i_SubBoard = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_SubBoard);
       i_SubBoard++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_SubBoard);

    m_pTransforms->vP10_T10_Delete_Redundant_stage(bPrintXform, bTrace);
    m_pTransforms->vP10_T20_Delete_absJoin(bPrintXform, bTrace);
    m_pTransforms->vP10_T30_Delete_combined(bPrintXform, bTrace);
    m_pTransforms->vP10_T40_Delete_send(bPrintXform, bTrace);
    m_pTransforms->vP10_T50_Cull_scan_Leaving_table(bPrintXform, bTrace);
  }

  vEndPhase();
  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_20_AccessMetadata(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pCardCtl->vPrintSubCards(CK_STREAM, true);

  m_pTransforms->vP20_T10_Add_tableSyn_ForAll_table(bPrintXform, bTrace);
  m_pTransforms->vP20_T20_Add_tablePtn_UnderAll_table(bPrintXform, bTrace);
  m_pTransforms->vP20_T30_Add_stream_UnderAll_table(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_30_StreamAnalysis(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pTransforms->vP30_T10_Calc_ordinalRange_ForAll_stream(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_40_PredicatePullup(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pCardCtl->vPrintProperties(CK_STREAM, false);
  m_pCardCtl->vPrintSubCards(CK_STREAM, false);

  m_pTransforms->vP40_T10_Convert_where_calc_To_where_calcBool  (bPrintXform, bTrace);
  m_pTransforms->vP40_T20_Mark_where_pred_Wrt_table             (bPrintXform, bTrace);
  m_pTransforms->vP40_T30_Move_Marked_where_pred_To_table_filter(bPrintXform, bTrace);
  m_pTransforms->vP40_T40_Delete_Empty_where                    (bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_50_FlattenExists(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pTransforms->vP50_T10_Rework_Exists_SubQueries(bPrintXform, bTrace);
  m_pTransforms->vP50_T20_Copy_Up_SubQueries(bPrintXform, bTrace);
  m_pTransforms->vP50_T30_Resolve_Parameter_Linkage(bPrintXform, bTrace);
  m_pTransforms->vP50_T40_Resolve_Parameter_Terms(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_60_InitialJoin(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pCardCtl->vPrintSubCards(CK_FILTER, true);

  m_pTransforms->vP60_T10_Add_indexJoin_UnderAllJoinable_table(bPrintXform, bTrace);
  m_pTransforms->vP60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord(bPrintXform, bTrace);
  m_pTransforms->vP60_T30_Add_joinPred_UnderAll_indexJoin(bPrintXform, bTrace);
  m_pTransforms->vP60_T40_Cull_filter_pred_MovedUpTo_joinPred(bPrintXform, bTrace);
  m_pTransforms->vP60_T50_Delete_Empty_filter(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_70_JoinOrdering(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pCardCtl->vPrintSubCards(CK_FILTER, false);

/*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vP70_T00_Extract_tableList_From_queryBoard(bPrintXform, bTrace);

  // add <term/<columnRef> to all ordinal <term>
  //m_pTransforms->vAdd_columnRef_Under_term(bPrintXform,bTrace);

  m_pTransforms->vP70_T10_Extract_joinPredList(bPrintXform, bTrace);
  m_pTransforms->vP70_T20_Compute_coPtnPredList(bPrintXform, bTrace);
  m_pTransforms->vP70_T30_Compute_coPtnJoinList(bPrintXform, bTrace);
  m_pTransforms->vP70_T40_Compute_crossPtnJoinList(bPrintXform, bTrace);
  m_pTransforms->vP70_T50_Compute_ptndJoinSet(bPrintXform, bTrace);

  m_pTransforms->vP70_T60_Copy_ptndJoinSet(bPrintXform, bTrace);
  m_pTransforms->vP70_T70_Generate_ptndPermutations(bPrintXform, bTrace);
  m_pTransforms->vP70_T80_Remove_invalidPermutations(bPrintXform, bTrace);

  m_pTransforms->vP70_T90_Setup_costEst(bPrintXform, bTrace);
  m_pTransforms->vP70_T92_Estimate_ptndJoins(bPrintXform, bTrace);
  m_pTransforms->vP70_T94_Estimate_network(bPrintXform, bTrace);
  m_pTransforms->vP70_T96_Estimate_joinOrder(bPrintXform, bTrace);
  m_pTransforms->vP70_T98_Extract_joinOrderSeq(bPrintXform, bTrace);
*/

  m_pTransforms->vP70_T99_Convert_table_To_scan_Or_indexJoin(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_80_IndexScan(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pCardCtl->vPrintByKind(CK_STREAM, true);
  m_pCardCtl->vPrintSubCards(CK_STREAM, false);

  /*
 *
 *    <scan>       <scan>
 *      <table>    <table>
 *      <stream>   <indexList>
 *      <filter>   <stream>
 *                 <filter>
 *
 */

  m_pTransforms->vP80_T10_Insert_indexList_ToAll_scan(bPrintXform, bTrace);
  m_pTransforms->vP80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(bPrintXform, bTrace);
  m_pTransforms->vP80_T30_Mark_scan_pred_bounds(bPrintXform, bTrace);
  m_pTransforms->vP80_T40_Mark_scan_filter_term_WhichMatchIndex(bPrintXform, bTrace);
  m_pTransforms->vP80_T50_Add_scanPred(bPrintXform, bTrace);
  m_pTransforms->vP80_T60_Cull_Marked_cmpPred(bPrintXform, bTrace);
  m_pTransforms->vP80_T70_Promote_scan_To_indexScan(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_90_StreamOrdering(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pTransforms->vP90_T10_Remove_sort_IfSameAs_gby(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_A0_FilterRefinement(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pTransforms->vPA0_T10_Individuate_filter(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_Z0_RestatePipeline(bool bPrintPhase, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vPhase(BK_SUBPIPE, 0, bPrintPhase, __func__);

  m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  m_pCardCtl->vPrintSubCards(CK_GBYELIST, true);

  m_pTransforms->vPZ0_T10_Reinstate_stages(bPrintXform, bTrace);
  m_pTransforms->vPZ0_T20_Reinstate_combined(bPrintXform, bTrace);

  vEndPhase();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vEndPhase(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase(eBK eBoard, i8 i_SubBoard, bool bPrintPhase, const char* azPhase)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(azPhase);
  m_pTransforms->vPrintPhase(bPrintPhase);
  m_pTransforms->vUsingBoard(eBoard, i_SubBoard);

  m_pBench->vClearMarks();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Transformer::bPrintBench(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  Stz* szPhase = m_pTransforms->szPhase();

  if (AM::JOB::bHasTag("<cue>"_Stz)) {
    Stz* szTagCue = AM::JOB::szTagValue("<cue>"_Stz);
    if (szTagCue->bSame((Stz*)szPhase))
      return true;
  }

  bool bPrintXform = m_pTransforms->bPrintXform();
  
  return bPrintXform;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPrintBench(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  if (bPrintBench())
    m_pTransforms->vPrintBench(eBoard,i_SubBoard);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

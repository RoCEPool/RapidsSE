/*============================================================================*/
/* Transformer.cpp --                                                           */
/*============================================================================*/

#include "Transformer.hpp"
#include "Xtransforms.hpp"
#include "Optimiser.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle --------------------------Ï-------------------------------------*/
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
  m_pJobTags     = MY::pJob()->pJobTags();
  m_bInteractive = m_pJobTags->bHasName("<interactive>"_Stz);

  m_pTransforms  = new Xtransforms(m_pBench);

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

  if (m_pJobTags->bHasName("<transform3>"_Stz))
    vTransform3();
  else
    vTransform3();
}

/*----------------------------------------------------------------------------*/
/* transforms                                                                 */
/*----------------------------------------------------------------------------*/
Xtransforms* Transformer::pTransforms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  return m_pTransforms;
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

  vHand_10_ReducePipeline  (C_PRINT_HAND, C_PRINT_XFORM, C_TRACE_N);
  vHand_20_AccessMetadata  (C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
  vHand_30_StreamAnalysis  (C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
  vHand_40_PredicatePullup (C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
  vHand_50_FlattenExists   (C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
  vHand_60_InitialJoin     (C_PRINT_HAND, C_PRINT_XFORM, C_TRACE_Y);
  vHand_70_JoinOrdering    (C_PRINT_HAND, C_PRINT_XFORM, C_TRACE_Y);
  vHand_80_IndexScan       (C_PRINT_HAND, C_PRINT_XFORM, C_TRACE_Y);
  vHand_90_StreamOrdering  (C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
  vHand_A0_FilterRefinement(C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
  vHand_Z0_RestatePipeline (C_PRINT_HAND, C_QUIET_XFORM, C_TRACE_N);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_10_ReducePipeline(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pCardCtl->vReset();
  m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,0);

  for (i8 i_SubBoard = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_SubBoard);
       i_SubBoard++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_SubBoard);

    m_pTransforms->vH10_T10_Delete_Redundant_stage(bPrintXform, bTrace);
    m_pTransforms->vH10_T15_Hide_Ignorable_stage(bPrintXform, bTrace);
    m_pTransforms->vH10_T20_Delete_absJoin(bPrintXform, bTrace);
    m_pTransforms->vH10_T30_Delete_combined(bPrintXform, bTrace);
    m_pTransforms->vH10_T40_Delete_send(bPrintXform, bTrace);
    m_pTransforms->vH10_T50_Cull_scan_Leaving_table(bPrintXform, bTrace);
  }

  vEndHand();
  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_20_AccessMetadata(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pCardCtl->vPrintSubCards(CK_STREAM, true);

  m_pTransforms->vH20_T10_Add_tableSyn_ForAll_table(bPrintXform, bTrace);
  m_pTransforms->vH20_T20_Add_tablePtn_UnderAll_table(bPrintXform, bTrace);
  m_pTransforms->vH20_T30_Add_stream_UnderAll_table(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_30_StreamAnalysis(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pTransforms->vSetPrintByKind(CK_STREAM, true);

  m_pTransforms->vH30_T10_Calc_ordinalRange_ForAll_stream(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_40_PredicatePullup(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pCardCtl->vPrintProperties(CK_STREAM, false);
  m_pCardCtl->vPrintSubCards(CK_STREAM, false);

  m_pTransforms->vH40_T10_Convert_where_calc_To_where_calcBool  (bPrintXform, bTrace);
  m_pTransforms->vH40_T20_Mark_where_pred_Wrt_table             (bPrintXform, bTrace);
  m_pTransforms->vH40_T30_Move_Marked_where_pred_To_table_filter(bPrintXform, bTrace);
  m_pTransforms->vH40_T40_Delete_Empty_where                    (bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_50_FlattenExists(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pTransforms->vH50_T10_Rework_Exists_SubQueries(bPrintXform, bTrace);
  m_pTransforms->vH50_T20_Copy_Up_SubQueries(bPrintXform, bTrace);
  m_pTransforms->vH50_T30_Resolve_Parameter_Linkage(bPrintXform, bTrace);
  m_pTransforms->vH50_T40_Resolve_Parameter_Terms(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_60_InitialJoin(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pCardCtl->vPrintSubCards(CK_FILTER, true);

  m_pTransforms->vH60_T10_Add_indexJoin_UnderAllJoinable_table(bPrintXform, bTrace);
  m_pTransforms->vH60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord(bPrintXform, bTrace);
  m_pTransforms->vH60_T30_Add_joinPred_UnderAll_indexJoin(bPrintXform, bTrace);
  m_pTransforms->vH60_T40_Cull_filter_pred_MovedUpTo_joinPred(bPrintXform, bTrace);
  m_pTransforms->vH60_T50_Delete_Empty_filter(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_70_JoinOrdering(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pCardCtl->vPrintSubCards(CK_FILTER, false);

/*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vH70_T00_Extract_tableList_From_queryBoard(bPrintXform, bTrace);

  // add <term/<columnRef> to all ordinal <term>
  //m_pTransforms->vAdd_columnRef_Under_term(bPrintXform,bTrace);

  m_pTransforms->vH70_T10_Extract_joinPredList(bPrintXform, bTrace);
  m_pTransforms->vH70_T20_Compute_coPtnPredList(bPrintXform, bTrace);
  m_pTransforms->vH70_T30_Compute_coPtnJoinList(bPrintXform, bTrace);
  m_pTransforms->vH70_T40_Compute_crossPtnJoinList(bPrintXform, bTrace);
  m_pTransforms->vH70_T50_Compute_ptndJoinSet(bPrintXform, bTrace);

  m_pTransforms->vH70_T60_Copy_ptndJoinSet(bPrintXform, bTrace);
  m_pTransforms->vH70_T70_Generate_ptndPermutations(bPrintXform, bTrace);
  m_pTransforms->vH70_T80_Remove_invalidPermutations(bPrintXform, bTrace);

  m_pTransforms->vH70_T90_Setup_costEst(bPrintXform, bTrace);
  m_pTransforms->vH70_T92_Estimate_ptndJoins(bPrintXform, bTrace);
  m_pTransforms->vH70_T94_Estimate_network(bPrintXform, bTrace);
  m_pTransforms->vH70_T96_Estimate_joinOrder(bPrintXform, bTrace);
  m_pTransforms->vH70_T98_Extract_joinOrderSeq(bPrintXform, bTrace);
*/

  m_pTransforms->vH70_T99_Convert_table_To_scan_Or_indexJoin(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_80_IndexScan(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

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

  m_pTransforms->vH80_T10_Insert_indexList_ToAll_scan(bPrintXform, bTrace);
  m_pTransforms->vH80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(bPrintXform, bTrace);
  m_pTransforms->vH80_T30_Mark_scan_pred_bounds(bPrintXform, bTrace);
  m_pTransforms->vH80_T40_Mark_scan_filter_term_WhichMatchIndex(bPrintXform, bTrace);
  m_pTransforms->vH80_T50_Add_scanPred(bPrintXform, bTrace);
  m_pTransforms->vH80_T60_Cull_Marked_cmpPred(bPrintXform, bTrace);
  m_pTransforms->vH80_T70_Promote_scan_To_indexScan(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_90_StreamOrdering(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pTransforms->vH90_T10_Remove_sort_IfSameAs_gby(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_A0_FilterRefinement(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pTransforms->vHA0_T10_Individuate_filter(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_Z0_RestatePipeline(bool bPrintHand, bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, bPrintHand, __func__);

  m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  m_pCardCtl->vPrintSubCards(CK_GBYELIST, true);

  m_pTransforms->vHZ0_T10_Reinstate_stages(bPrintXform, bTrace);
  m_pTransforms->vHZ0_T20_Reinstate_combined(bPrintXform, bTrace);

  vEndHand();
  vPrintBench(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vEndHand(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vTransformation("n/a"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand(eBK eBoard, i8 i_SubBoard, bool bPrintHand, const char* azHand)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vHand(azHand);
  m_pTransforms->vPrintHand(bPrintHand);
  m_pTransforms->vUsingBoard(eBoard, i_SubBoard);

  m_pBench->vClearMarks();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Transformer::bPrintBench(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  Stz* szHand = m_pTransforms->szHand();

  if (AM::JOB::bHasTag("<cue>"_Stz)) {
    Stz* szTagCue = AM::JOB::szTagValue("<cue>"_Stz);
    if (szTagCue->bSame((Stz*)szHand))
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

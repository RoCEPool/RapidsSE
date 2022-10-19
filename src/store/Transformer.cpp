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

  vPhase_1_ReducePipeline   (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_2_TableAnalysis    (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_3_WherePredicates  (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_U_ExistsSubqueries (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_4_InitialJoin      (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_5_JoinOrdering     (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_6_IndexScan        (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_7_StreamOrdering   (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_8_FilterRefinement (C_DONT_PRINT,C_DONT_TRACE);

  vPhase_Z_RestatePipeline  (C_DONT_PRINT,C_DONT_TRACE);

  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);
  //m_pTransforms->vExpandedForm(false);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_1_ReducePipeline(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);

  for (i8 i_SubBoard = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_SubBoard);
       i_SubBoard++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_SubBoard);

    m_pTransforms->vDelete_Redundant_stage(bPrint,bTrace);

    m_pTransforms->vDelete_absJoin(bPrint,bTrace);

    m_pTransforms->vDelete_combined(bPrint,bTrace);

    m_pTransforms->vDelete_send(bPrint,bTrace);

    m_pTransforms->vCull_scan_Leaving_table(bPrint,bTrace);
  }

  m_pBench->vPrintBoards(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_2_TableAnalysis(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_tableSyn_ForAll_table(bPrint,bTrace);

  m_pTransforms->vAdd_tablePtn_UnderAll_table(bPrint,bTrace);

  m_pTransforms->vAdd_stream_UnderAll_table(bPrint,bTrace);

  m_pTransforms->vCalc_ordinalRange_ForAll_stream(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_3_WherePredicates(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vConvert_where_calc_To_where_calcBool(bPrint,bTrace);

  m_pTransforms->vMark_where_pred_Wrt_table(bPrint,bTrace);

  m_pTransforms->vMove_Marked_where_pred_To_table_filter(bPrint,bTrace);

  m_pTransforms->vDelete_Empty_where(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_U_ExistsSubqueries(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vRework_Exists_SubQueries(bPrint,bTrace);

  m_pTransforms->vCopy_Up_SubQueries(bPrint,bTrace);

  m_pTransforms->vResolve_Parameter_Linkage(bPrint,bTrace);

  m_pTransforms->vResolve_Parameter_Terms(bPrint,true);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_4_InitialJoin(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vAdd_indexJoin_UnderAllJoinable_table(bPrint,bTrace);

  m_pTransforms->vConvert_idxCol_name_Of_table_To_idxCol_ord(bPrint,bTrace);

  m_pTransforms->vAdd_joinPred_UnderAll_indexJoin(bPrint,bTrace);

  m_pTransforms->vCull_filter_pred_MovedUpTo_joinPred(bPrint,bTrace);

  m_pTransforms->vDelete_Empty_filter(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_5_JoinOrdering(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vExtract_tableList_From_queryBoard(bPrint,bTrace);

  // add <term/<columnRef> to all ordinal <term>
  m_pTransforms->vAdd_columnRef_Under_term(bPrint,bTrace);

  m_pTransforms->vExtract_joinPredList(bPrint,bTrace);
  m_pTransforms->vCompute_coPtnPredList(bPrint,bTrace);
  m_pTransforms->vCompute_coPtnJoinList(bPrint,bTrace);
  m_pTransforms->vCompute_crossPtnJoinList(bPrint,bTrace);
  m_pTransforms->vCompute_ptndJoinSet(bPrint,bTrace);

  m_pTransforms->vCopy_ptndJoinSet(bPrint,bTrace);
  m_pTransforms->vGenerate_ptndPermutations(bPrint,bTrace);
  m_pTransforms->vRemove_invalidPermutations(bPrint,bTrace);

  m_pTransforms->vSetup_costEst(bPrint,bTrace);
  m_pTransforms->vEstimate_ptndJoins(bPrint,bTrace);
  m_pTransforms->vEstimate_network(bPrint,bTrace);
  m_pTransforms->vEstimate_joinOrder(bPrint,bTrace);
  m_pTransforms->vExtract_joinOrderSeq(bPrint,bTrace);
  */

  m_pTransforms->vConvert_table_To_scan_Or_indexJoin(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_6_IndexScan(bool bPrint, bool bTrace)
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

  m_pTransforms->vInsert_indexList_ToAll_scan(bPrint,bTrace);

  m_pTransforms->vConvert_idxCol_name_Of_scan_To_idxCol_ord(bPrint,bTrace);

  m_pTransforms->vMark_scan_pred_bounds(bPrint,bTrace);

  m_pTransforms->vMark_scan_filter_term_WhichMatchIndex(bPrint,bTrace);

  m_pTransforms->vAdd_scanPred(bPrint,true);

  m_pTransforms->vCull_Marked_cmpPred(bPrint,bTrace);

  m_pTransforms->vPromote_scan_To_indexScan(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_7_StreamOrdering(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vRemove_sort_IfSameAs_gby(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_8_FilterRefinement(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vIndividuate_filter(bPrint,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_Z_RestatePipeline(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase(__func__);
  m_pTransforms->vUsingBoard(BK_SUBPIPE, 0);

  m_pTransforms->vReinstate_stages(bPrint,bTrace);

  m_pTransforms->vReinstate_combined(bPrint,bTrace);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

  m_pTransforms->vExpandedForm(BK_QUERY,false);

  vPhase_1_ReducePipeline(false);

  vPhase_2_TableAnalysis(false);

  vPhase_3_WherePredicates(false);

  vPhase_4_InitialJoin(false);

  vPhase_5_JoinOrdering(false);

  vPhase_6_IndexScan(false);

  vPhase_7_StreamOrdering(false);

  vPhase_8_FilterRefinement(false);

  vPhase_Z_RestatePipeline(false);

  m_pTransforms->vExpandedForm(BK_QUERY,false);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_1_ReducePipeline(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vDelete_Redundant_stage(BK_QUERY,bTrace);

  m_pTransforms->vDelete_absJoin(BK_QUERY,bTrace);

  m_pTransforms->vDelete_combined(BK_QUERY,bTrace);

  m_pTransforms->vDelete_send(BK_QUERY,bTrace);

  m_pTransforms->vCull_scan_Leaving_table(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_2_TableAnalysis(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vAdd_tableSyn_ForAll_table(BK_QUERY,bTrace);

  m_pTransforms->vAdd_tablePtn_UnderAll_table(BK_QUERY,bTrace);

  m_pTransforms->vAdd_stream_UnderAll_table(BK_QUERY,bTrace);

  m_pTransforms->vCalc_ordinalRange_ForAll_stream(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_3_WherePredicates(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vConvert_where_calc_To_where_calcBool(BK_QUERY,bTrace);

  m_pTransforms->vMark_where_pred_Wrt_table(BK_QUERY,bTrace);

  m_pTransforms->vMove_Marked_where_pred_To_table_filter(BK_QUERY,bTrace);

  m_pTransforms->vDelete_Empty_where(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_4_InitialJoin(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vAdd_indexJoin_UnderAllJoinable_table(BK_QUERY,bTrace);

  m_pTransforms->vConvert_idxCol_name_Of_table_To_idxCol_ord(BK_QUERY,bTrace);

  m_pTransforms->vAdd_joinPred_UnderAll_indexJoin(BK_QUERY,bTrace);

  m_pTransforms->vCull_filter_pred_MovedUpTo_joinPred(BK_QUERY,bTrace);

  m_pTransforms->vDelete_Empty_filter(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_5_JoinOrdering(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vExtract_tableList_From_queryBoard(BK_QUERY,bTrace);

  // add <term/<columnRef> to all ordinal <term>
  m_pTransforms->vAdd_columnRef_Under_term(BK_QUERY,bTrace);

  m_pTransforms->vExtract_joinPredList(BK_QUERY,bTrace);
  m_pTransforms->vCompute_coPtnPredList(BK_QUERY,bTrace);
  m_pTransforms->vCompute_coPtnJoinList(BK_QUERY,bTrace);
  m_pTransforms->vCompute_crossPtnJoinList(BK_QUERY,bTrace);
  m_pTransforms->vCompute_ptndJoinSet(BK_QUERY,bTrace);

  m_pTransforms->vCopy_ptndJoinSet(BK_QUERY,bTrace);
  m_pTransforms->vGenerate_ptndPermutations(BK_QUERY,bTrace);
  m_pTransforms->vRemove_invalidPermutations(BK_QUERY,bTrace);

  m_pTransforms->vSetup_costEst(BK_QUERY,bTrace);
  m_pTransforms->vEstimate_ptndJoins(BK_QUERY,bTrace);
  m_pTransforms->vEstimate_network(BK_QUERY,bTrace);
  m_pTransforms->vEstimate_joinOrder(BK_QUERY,bTrace);
  m_pTransforms->vExtract_joinOrderSeq(BK_QUERY,bTrace);
  */

  m_pTransforms->vConvert_table_To_scan_Or_indexJoin(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_6_IndexScan(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  /*
 *
 *    <scan>       <scan>
 *      <table>    <table>
 *      <stream>   <indexList>
 *      <filter>   <stream>
 *                 <filter>
 *
 */

  m_pTransforms->vInsert_indexList_ToAll_scan(BK_QUERY,bTrace);

  m_pTransforms->vConvert_idxCol_name_Of_scan_To_idxCol_ord(BK_QUERY,bTrace);

  m_pTransforms->vMark_scan_pred_bounds(BK_QUERY,bTrace);

  m_pTransforms->vMark_scan_filter_term_WhichMatchIndex(BK_QUERY, bTrace);

  m_pTransforms->vAdd_scanPred(BK_QUERY,true);
  /*
   m_pTransforms->vCull_markedPred(BK_QUERY,bTrace);
   m_pTransforms->vPromote_scan_To_indexScan(BK_QUERY,bTrace);
   */
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_7_StreamOrdering(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vRemove_sort_IfSameAs_gby(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_8_FilterRefinement(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vIndividuate_filter(BK_QUERY,bTrace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_Z_RestatePipeline(bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vReinstate_stages(BK_QUERY,bTrace);

  m_pTransforms->vReinstate_combined(BK_QUERY,bTrace);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

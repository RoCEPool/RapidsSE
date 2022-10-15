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

  m_pTransforms->vExpandedForm();

  vPhase_1_ReducePipeline();

  vPhase_2_TableAnalysis();

  vPhase_3_WherePredicates();

  vPhase_4_InitialJoin();

  vPhase_5_JoinOrdering();

  vPhase_6_IndexScan();

  vPhase_7_StreamOrdering();

  vPhase_8_FilterRefinement();

  vPhase_Z_RestatePipeline();

  m_pTransforms->vExpandedForm();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_1_ReducePipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vDelete_Redundant_stage();

  m_pTransforms->vDelete_absJoin();

  m_pTransforms->vDelete_combined();

  m_pTransforms->vDelete_send();

  m_pTransforms->vCull_scan_Leaving_table();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_2_TableAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vAdd_tableSyn_ForAll_table();

  m_pTransforms->vAdd_tablePtn_UnderAll_table();

  m_pTransforms->vAdd_stream_UnderAll_table();

  m_pTransforms->vCalc_ordinalRange_ForAll_stream();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_3_WherePredicates(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vConvert_where_calc_To_where_calcBool();

  m_pTransforms->vMark_where_pred_Wrt_table();

  m_pTransforms->vMove_Marked_where_pred_To_table_filter();

  m_pTransforms->vDelete_Empty_where();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_4_InitialJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vAdd_indexJoin_UnderAllJoinable_table();

  m_pTransforms->vConvert_idxCol_name_Of_table_To_idxCol_ord();

  m_pTransforms->vAdd_joinPred_UnderAll_indexJoin();

  m_pTransforms->vCull_filter_pred_MovedUpTo_joinPred();

  m_pTransforms->vDelete_Empty_filter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_5_JoinOrdering(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vExtract_tableList_From_queryBoard();

  // add <term/<columnRef> to all ordinal <term>
  m_pTransforms->vAdd_columnRef_Under_term();

  m_pTransforms->vExtract_joinPredList();
  m_pTransforms->vCompute_coPtnPredList();
  m_pTransforms->vCompute_coPtnJoinList();
  m_pTransforms->vCompute_crossPtnJoinList();
  m_pTransforms->vCompute_ptndJoinSet();

  m_pTransforms->vCopy_ptndJoinSet();
  m_pTransforms->vGenerate_ptndPermutations();
  m_pTransforms->vRemove_invalidPermutations();

  m_pTransforms->vSetup_costEst();
  m_pTransforms->vEstimate_ptndJoins();
  m_pTransforms->vEstimate_network();
  m_pTransforms->vEstimate_joinOrder();
  m_pTransforms->vExtract_joinOrderSeq();
  */

  m_pTransforms->vConvert_table_To_scan_Or_indexJoin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_6_IndexScan(void)
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

  m_pTransforms->vInsert_indexList_ToAll_scan();

  m_pTransforms->vConvert_idxCol_name_Of_scan_To_idxCol_ord();

  m_pTransforms->vMark_scan_pred_bounds();

  m_pTransforms->vMark_scan_pred_ordinals();
 /*
  m_pTransforms->vAdd_scanPred();
  m_pTransforms->vCull_markedPred();
  m_pTransforms->vPromote_scan_To_indexScan();
  */
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_7_StreamOrdering(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vRemove_sort_IfSameAs_gby();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_8_FilterRefinement(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vIndividuate_filter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vPhase_Z_RestatePipeline(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vPhase((Stz*)__func__);

  m_pTransforms->vReinstate_stages();

  m_pTransforms->vReinstate_combined();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

  m_pBench        = pBench;
  m_pOptimiser    = m_pBench->pOptimiser();
  m_pBoard        = nullptr;

  m_pCardCtl      = m_pOptimiser->pCardCtl();

  m_pTransforms   = nullptr;

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

  vTrace("Transformer running vTransform2\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // QUERY BOARD -- preliminaries

  // combine {{==<stage>}}->{<stage>}
  m_pTransforms->vLinearizeAll_stage();

  // delete *<absJoin>
  m_pTransforms->vDeleteAll_absJoin();

  // delete *<combined>
  m_pTransforms->vDeleteAll_combined();

  // delete *<send>
  m_pTransforms->vDeleteAll_send();

  // remove *<scan>
  m_pTransforms->vCullAll_scan();

  // add <tableSyn> to all <table>
  m_pTransforms->vAdd_tableSyn_ToAll_table();

  // add <tablePtn> to all <table>
  m_pTransforms->vAdd_tablePtn_ToAll_table();

  // append <stream> & <streamColList> to all <scan>
  m_pTransforms->vAppend_stream_ToAll_table();

  // calculate /<stream>:iOrdLo,iOrdHi properties
  m_pTransforms->vCalc_ordinalRanges_OfAll_stream();

  // convert all <filter>/<calc> to <filter>/<calcBool>
  m_pTransforms->vConvert_calc_OfAll_filter_To_calcBool();

  // each <scan> <absJoin> becomes <absJoin>/<scan>
  //m_pTransforms->vSubsume_scan_Under_absJoin();

  // convert $n to $$c.n
  //m_pTransforms->vConvert_streamColOrdinal_To_tableColOrdinal();



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


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>
  m_pTransforms->vAssignAll_predCmp_To_table();
  m_pTransforms->vMoveAll_predCmp_To_table();

  // delete any <filter> which is effectively empty
  m_pTransforms->vDelete_Empty_filter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pTransforms->vInsert_indexJoin_IntoAll_table();
  m_pTransforms->vConvertAll_idxColNm_To_streamColOrd();
  m_pTransforms->vFormAll_joinPred_of_indexJoin();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pTransforms->vCullAll_filterPred_Of_indexJoin();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Index Scan

  /*
   *
   *    <scan>       <scan>
   *      <table>    <table>
   *      <stream>   <indexList>
   *      <filter>   <stream>
   *                 <filter>
   *
   */

  m_pTransforms->vAppend_indexList_ToAll_table();
  m_pTransforms->vMark_scan_pred_bounds();
  m_pTransforms->vMark_scan_pred_ordinals();

  m_pTransforms->vAdd_scanPred();
  m_pTransforms->vCull_markedPred();
  //m_pTransforms->vPromote_scan_To_indexScan();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pTransforms->vDelete_empty_sub_filter();

  m_pTransforms->vRefactorAll_table_Into_join();

  m_pTransforms->vCull_sby_SameAs_gby();

  m_pTransforms->vIndividuate_filter();

  m_pTransforms->vAdd_stages();

  m_pTransforms->vAdd_combined();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

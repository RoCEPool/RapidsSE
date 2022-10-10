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

  m_pColumnar    = nullptr;
  m_pCombiner    = nullptr;
  m_pCurator     = nullptr;
  m_pEstimator   = nullptr;
  m_pFiltrate    = nullptr;
  m_pIndexer     = nullptr;
  m_pJoiner      = nullptr;
  m_pPartitioner = nullptr;
  m_pPermutor    = nullptr;
  m_pScanning    = nullptr;
  m_pSender      = nullptr;
  m_pSorter      = nullptr;
  m_pStager      = nullptr;
  m_pStreamer    = nullptr;
  m_pTabular     = nullptr;

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

  delete m_pColumnar;
  delete m_pCombiner;
  delete m_pCurator;
  delete m_pFiltrate;
  delete m_pIndexer;
  delete m_pJoiner;
  delete m_pPartitioner;
  delete m_pPermutor;
  delete m_pScanning;
  delete m_pSender;
  delete m_pSorter;
  delete m_pStager;
  delete m_pStreamer;
  delete m_pTabular;
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
  m_pColumnar    = new Columnar(m_pBench);
  m_pCombiner    = new Combiner(m_pBench);
  m_pCurator     = new Curator(m_pBench);
  m_pEstimator   = new Estimator(m_pBench);
  m_pFiltrate    = new Filtrate(m_pBench);
  m_pIndexer     = new Indexer(m_pBench);
  m_pJoiner      = new Joiner(m_pBench);
  m_pPartitioner = new Partitioner(m_pBench);
  m_pPermutor    = new Permutor(m_pBench);
  m_pScanning    = new Scanning(m_pBench);
  m_pSender      = new Sender(m_pBench);
  m_pSorter      = new Sorter(m_pBench);
  m_pStager      = new Stager(m_pBench);
  m_pStreamer    = new Streamer(m_pBench);
  m_pTabular     = new Tabular(m_pBench);

  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

       if (pJobTags->bHasName("<transform2>"_Stz)) vTransform2();
  else if (pJobTags->bHasName("<transform3>"_Stz)) vTransform3();
  else vTransform3();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform 2                                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  vTrace("Transformer running vTransform2\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // QUERY BOARD -- preliminaries

  // combine {{==<stage>}}->{<stage>}
  m_pStager->vLinearize_stages();

  // delete *<absJoin>
  m_pJoiner->vDeleteAll_absJoin();

  // delete *<combined>
  m_pCombiner->vDeleteAll_combined();

  // delete *<send>
  m_pSender->vDeleteAll_send();

  // add <tableSyn> to all <table>
  m_pTabular->vAdd_tableSyn_To_table();

  // add <tablePtn> to all <table>
  m_pTabular->vAdd_tablePtn_To_table();

  // append <stream> & <streamColList> to all <scan>
  m_pStreamer->vAppend_stream_To_scan();

  // calculate /<stream>:iOrdLo,iOrdHi properties
  m_pStreamer->vCalc_OrdinalRangesOf_stream();

  // convert all <filter>/<calc> to <filter>/<calcBool>
  m_pFiltrate->vConvert_calc_To_calcBool();

  // each <scan> <absJoin> becomes <absJoin>/<scan>
  //m_pJoiner->vSubsume_scan_Under_absJoin();

  // convert $n to $$c.n
  //m_pColumnar->vConvert_streamColOrdinal_To_tableColOrdinal();



  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pPartitioner->vExtract_tableList_From_queryBoard();

  // add <term/<columnRef> to all ordinal <term>
  m_pColumnar->vAdd_columnRef_Under_term();

  m_pPartitioner->vExtract_joinPredList();
  m_pPartitioner->vCompute_coPtnPredList();
  m_pPartitioner->vCompute_coPtnJoinList();
  m_pPartitioner->vCompute_crossPtnJoinList();
  m_pPartitioner->vCompute_ptndJoinSet();

  m_pPermutor->vCopy_ptndJoinSet();
  m_pPermutor->vGenerate_ptndPermutations();
  m_pPermutor->vRemove_invalidPermutations();

  m_pEstimator->vSetup_costEst();
  m_pEstimator->vEstimate_ptndJoins();
  m_pEstimator->vEstimate_network();
  m_pEstimator->vEstimate_joinOrder();
  m_pEstimator->vExtract_joinOrderSeq();
*/


 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>
  m_pFiltrate->vAssign_predCmp_leftwards();
  m_pFiltrate->vCopy_predCmp_leftwards();

  // delete any <filter> which is effectively empty
  m_pFiltrate->vDelete_Empty_filter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vInsert_preliminary_indexJoin();
  m_pJoiner->vConvertAll_idxColNm_To_streamColOrdinal();
  m_pJoiner->vForm_joinPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pFiltrate->vCull_filterPred_Of_indexJoin();


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

  m_pIndexer->vAppend_indexList_To_scan();
  m_pIndexer->vMark_scan_pred_bounds();
  m_pIndexer->vMark_scan_pred_ordinals();

  m_pScanning->vAdd_scanPred();
  m_pScanning->vCull_markedPred();
  //m_pScanning->vPromote_scan_To_indexScan();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pFiltrate->vDelete_empty_sub_filter();

  m_pJoiner->vRefactor_absJoin_Into_indexJoin();

  m_pSorter->vCull_sby_SameAs_gby();

  m_pFiltrate->vIndividuate_filter();

  m_pStager->vAdd_stages();

  m_pCombiner->vAdd_combined();
}

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
  m_pTransforms->vLinearize_stages();

  // delete *<absJoin>
  m_pTransforms->vDeleteAll_absJoin();

  // delete *<combined>
  m_pTransforms->vDeleteAll_combined();

  // delete *<send>
  m_pTransforms->vDeleteAll_send();

  // add <tableSyn> to all <table>
  m_pTransforms->vAdd_tableSyn_To_table();

  // add <tablePtn> to all <table>
  m_pTransforms->vAdd_tablePtn_To_table();

  // append <stream> & <streamColList> to all <scan>
  m_pTransforms->vAppend_stream_To_scan();

  // calculate /<stream>:iOrdLo,iOrdHi properties
  m_pTransforms->vCalc_OrdinalRangesOf_stream();

  // convert all <filter>/<calc> to <filter>/<calcBool>
  m_pTransforms->vConvert_calc_To_calcBool();

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
  m_pTransforms->vAssign_predCmp_leftwards();
  m_pTransforms->vCopy_predCmp_leftwards();

  // delete any <filter> which is effectively empty
  m_pTransforms->vDelete_Empty_filter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pTransforms->vInsert_preliminary_indexJoin();
  m_pTransforms->vConvertAll_idxColNm_To_streamColOrdinal();
  m_pTransforms->vForm_joinPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pTransforms->vCull_filterPred_Of_indexJoin();


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

  m_pTransforms->vAppend_indexList_To_scan();
  m_pTransforms->vMark_scan_pred_bounds();
  m_pTransforms->vMark_scan_pred_ordinals();

  m_pTransforms->vAdd_scanPred();
  m_pTransforms->vCull_markedPred();
  //m_pTransforms->vPromote_scan_To_indexScan();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pTransforms->vDelete_empty_sub_filter();

  m_pTransforms->vRefactor_absJoin_Into_indexJoin();

  m_pTransforms->vCull_sby_SameAs_gby();

  m_pTransforms->vIndividuate_filter();

  m_pTransforms->vAdd_stages();

  m_pTransforms->vAdd_combined();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

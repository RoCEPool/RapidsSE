/*============================================================================*/
/* Transformer.cpp --                                                           */
/*============================================================================*/

#include "Transformer.hpp"
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

  m_pCardCtl     = m_pOptimiser->pCardCtl();

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

  //m_pBoard = m_pOptimiser->pHeadBoard();

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
  else vTransform2();
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

  Board* pBoard_query = m_pBench->pGetBoard(BK_QUERY);

  m_pCardCtl->vPrintSubCards(CK_AGGELIST, false);
  m_pCardCtl->vPrintSubCards(CK_GBYELIST, false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pStager->vLinearize_Stages();
  m_pCombiner->vDelete_Combines();
  m_pSender->vDelete_Sends();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert all <filter> from calc form to calcBool
  m_pFiltrate->vCalc_to_calcBool();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append <stream> to all <scan> and then to all <absJoin>
  m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_DEFAULT, true);

  //m_pCardCtl->vPrintSubCards(CK_EXPR,0);

  m_pCardCtl->vPrintForm(CK_PREDAND, CP_CARDFORM);
  m_pCardCtl->vPrintForm(CK_PREDCMP, CP_CARDFORM);
  m_pCardCtl->vPrintForm(CK_PREDOR, CP_CARDFORM);
  m_pCardCtl->vPrintForm(CK_TERM, CP_CARDFORM);

  m_pStreamer->vAppendStream_to_Scan();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // each <scan> <absJoin> becomes <absJoin>/<scan>
  m_pJoiner->vSubsume_scan();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  m_pStreamer->vOrdinal_Range();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pTabular->vAdd_TableSyn();
  m_pTabular->vAdd_TablePtn();
  m_pPartitioner->vExtract_tableList();

  m_pColumnar->vAdd_ColumnRef();

  m_pPartitioner->vExtract_joinPredList();

  m_pCardCtl->vPrintForm(CK_COLUMNREF,CP_COMPRESSED);
  m_pCardCtl->vPrintForm(CK_COPTNJOIN,CP_COMPRESSED);
  m_pCardCtl->vPrintForm(CK_CROSSPTNJOIN,CP_COMPRESSED);
  //m_pCardCtl->vPrintForm(CK_PREDCMP,CP_COMPRESSED);

  m_pPartitioner->vCompute_coPtnPredList();
  m_pPartitioner->vCompute_coPtnJoinList();
  m_pPartitioner->vCompute_crossPtnJoinList();

  m_pPartitioner->vCompute_ptndJointSet();


  m_pPermutor->vCopy_ptndJoinSet();
  m_pPermutor->vGenerate_ptndPermutations();
  m_pPermutor->vRemove_invalidPermutations();

  m_pEstimator->vSetup_costEst();
  m_pEstimator->vEstimate_ptndJoins();
  m_pEstimator->vEstimate_network();
  m_pEstimator->vEstimate_joinOrder();
  m_pEstimator->vExtract_joinOrderSeq();



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>
  m_pFiltrate->vAssignLeftwards();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pFiltrate->vCopyLeftwards();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any <filter> which is effectively empty
  m_pFiltrate->vDeleteEmpty_filter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_DEFAULT, true);
  m_pCardCtl->vPrintForm(CK_PREDAND, CP_DEFAULT);
  m_pCardCtl->vPrintForm(CK_PREDCMP, CP_DEFAULT);
  m_pCardCtl->vPrintForm(CK_PREDOR, CP_DEFAULT);
  m_pCardCtl->vPrintForm(CK_TERM, CP_DEFAULT);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppendPrelim_indexJoin();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vMark_idxCol();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vForm_joinPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_DEFAULT, true);

  m_pFiltrate->vCull_indexJoin_filterPred();


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

  m_pIndexer->vAppend_indexList_to_scan();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Indexer::vAppend_indexList_to_scan"_Stz);

  m_pIndexer->vMark_scan_pred_bounds();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Indexer::vMark_scan_pred_bounds"_Stz);

  m_pIndexer->vMark_scan_pred_ordinals();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Indexer::vMark_scan_pred_ordinals"_Stz);

  m_pScanning->vAdd_scanPred();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Scanning::vAdd_scanPred"_Stz);

  m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_CARDFORM, false);

  m_pScanning->vCull_markedPred();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Scanning::vCull_markedPred"_Stz);

  m_pScanning->vPromote_scan_to_indexScan();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Scanning::vPromote_scan_to_indexScan"_Stz);




  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pFiltrate->vDeleteEmpty_sub_filter();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Filtrate::vDeleteEmpty_sub_filter"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vReduce_absJoin();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Joiner::vReduce_absJoin"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pSorter->vCull_sby_SameAs_gby();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Sorter::vCull_sby_SameAs_gby"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pFiltrate->vIndividuate_filter();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Filtrate::vIndividuate_filter"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pStager->vAdd_Stages2();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Stager::vAdd_Stages"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  m_pCardCtl->vPrintSubCards(CK_GBYELIST, true);

  m_pCombiner->vAdd_combined3();
  if (m_bInteractive)
    pBoard_query->vPrintBoard("Combiner::vAdd_combined2"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

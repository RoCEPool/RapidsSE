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
Transformer::Transformer(Optimiser* pOptimiser)
: OptimiserTool(CN_TRANSFORMER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  m_pOptimiser   = pOptimiser;

  m_pBench       = m_pOptimiser->pBench();
  m_pBoard       = nullptr;

  m_pCombiner    = nullptr;
  m_pCurator     = nullptr;
  m_pFiltrate    = nullptr;
  m_pJoiner      = nullptr;
  m_pPredicator  = nullptr;
  m_pSender      = nullptr;
  m_pStager      = nullptr;
  m_pStreamer    = nullptr;

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

  delete m_pCurator;
  delete m_pFiltrate;
  delete m_pJoiner;
  delete m_pPredicator;
  delete m_pSender;
  delete m_pStager;
  delete m_pStreamer;
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

  m_pBoard = m_pOptimiser->pHeadBoard();

  m_pCombiner    = new Combiner(m_pBench);
  m_pCurator     = new Curator(m_pBench);
  m_pFiltrate    = new Filtrate(m_pBench);
  m_pJoiner      = new Joiner(m_pBench);
  m_pPredicator  = new Predicator(m_pBench);
  m_pSender      = new Sender(m_pBench);
  m_pStager      = new Stager(m_pBench);
  m_pStreamer    = new Streamer(m_pBench);
  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

       if (pJobTags->bHasName("<transform1>"_Stz)) vTransform1();
  else if (pJobTags->bHasName("<transform2>"_Stz)) vTransform2();
  else vTransform1();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform 1                                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  vTrace("Transformer running vTransform1\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pStager->vLinearize_Stages();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Stager::vLinearize_Stages()"_Stz);

  m_pCombiner->vDelete_Combines();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vDelete_Combines()"_Stz);

  m_pSender->vDelete_Sends();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sender::vDelete_Sends()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert all <filter> from calc form to calcBool
  m_pFiltrate->vCalc_to_calcBool();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vCalc_to_calcBool()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append <stream> to all <scan> and then to all <absJoin>
  m_pStreamer->vAppend_Streams();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Streamer::vAppend_Streams()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vInsert_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vInsert_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vResolve_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vResolve_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_indexPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_indexPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_index();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_index()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove <predCmp> (in <joinPred>) from all <filter>
  m_pFiltrate->vRemove_predCmp();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vRemove_predCmp()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any <filter> which is effectively empty
  m_pFiltrate->vDeleteEmpty_filter();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vDeleteEmpty_filter()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vDelete_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vConvert_absJoin();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vConvert_absJoin()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vDelete_scan_R();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_scan_R()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pSender->vReadd_Sends();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sender::vReadd_Sends()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCombiner->vAdd_combined();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vAdd_combined()"_Stz);

}

/*----------------------------------------------------------------------------*/
/* transform 1                                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  vTrace("Transformer running vTransform2\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pStager->vLinearize_Stages();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Stager::vLinearize_Stages()"_Stz);

  m_pCombiner->vDelete_Combines();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vDelete_Combines()"_Stz);

  m_pSender->vDelete_Sends();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sender::vDelete_Sends()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert all <filter> from calc form to calcBool
  m_pFiltrate->vCalc_to_calcBool();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vCalc_to_calcBool()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append <stream> to all <scan> and then to all <absJoin>
  m_pOptimiser->pCardCtl()->vSubCards(CK_STREAM,0);
  m_pOptimiser->pCardCtl()->vPinForm(CK_STREAM,1);

  //m_pOptimiser->pCardCtl()->vSubCards(CK_EXPR,0);

  m_pOptimiser->pCardCtl()->vPinForm(CK_PREDAND,1);
  m_pOptimiser->pCardCtl()->vPinForm(CK_PREDCMP,1);
  m_pOptimiser->pCardCtl()->vPinForm(CK_PREDOR,1);
  m_pOptimiser->pCardCtl()->vPinForm(CK_TERM,1);

  m_pStreamer->vAppendStream_to_Scan();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Streamer::vAppendStream_to_Scan()"_Stz);





  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // each <scan> <absJoin> becomes <absJoin>/<scan>
  m_pJoiner->vSubsume_scan();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vSubsume_scan()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  m_pStreamer->vOrdinal_Range();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Streamer::vOrdinal_Range()"_Stz);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>
  m_pFiltrate->vAssignLeftwards();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vAssignLeftwards()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pFiltrate->vCopyLeftwards();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vCopyLeftwards()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any <filter> which is effectively empty
  m_pFiltrate->vDeleteEmpty_filter();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vDeleteEmpty_filter()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pOptimiser->pCardCtl()->vSubCards(CK_STREAM,1);
  m_pOptimiser->pCardCtl()->vPinForm(CK_STREAM,0);
  m_pOptimiser->pCardCtl()->vPinForm(CK_PREDAND,0);
  m_pOptimiser->pCardCtl()->vPinForm(CK_PREDCMP,0);
  m_pOptimiser->pCardCtl()->vPinForm(CK_PREDOR,0);
  m_pOptimiser->pCardCtl()->vPinForm(CK_TERM,0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppendPrelim_indexJoin();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppendPrelim_indexJoin()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vMark_idxCol();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vMark_idxCol()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vForm_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vForm_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pOptimiser->pCardCtl()->vSubCards(CK_STREAM,0);
  m_pOptimiser->pCardCtl()->vPinForm(CK_STREAM,1);

  m_pJoiner->vReduce_absJoin();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vReduce_absJoin()"_Stz);


  //############################################################################

/*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vInsert_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vInsert_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vResolve_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vResolve_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_indexPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_indexPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_index();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_index()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove <predCmp> (in <joinPred>) from all <filter>
  m_pFiltrate->vRemove_predCmp();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vRemove_predCmp()"_Stz);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vDelete_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_joinPred()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vConvert_absJoin();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vConvert_absJoin()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vDelete_scan_R();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_scan_R()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pSender->vReadd_Sends();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sender::vReadd_Sends()"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

  m_pCombiner->vAdd_combined();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vAdd_combined()"_Stz);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

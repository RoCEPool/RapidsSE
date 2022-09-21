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
  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  m_pBoard = m_pOptimiser->pHeadBoard();

  m_pCombiner   = new Combiner(m_pBench);
  m_pCurator    = new Curator(m_pBench);
  m_pFiltrate   = new Filtrate(m_pBench);
  m_pJoiner     = new Joiner(m_pBench);
  m_pPredicator = new Predicator(m_pBench);
  m_pSender     = new Sender(m_pBench);
  m_pStager     = new Stager(m_pBench);
  m_pStreamer   = new Streamer(m_pBench);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTransform1();
  //vTransform2();
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pStager->vLinearize_Stages();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Stager::vLinearize_Stages() >> "_Stz);

  m_pCombiner->vDelete_Combines();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vDelete_Combines() >> "_Stz);

  m_pSender->vDelete_Sends();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sender::vDelete_Sends() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pFiltrate->vCalc_to_calcBool();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vCalc_to_calcBool() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append <stream> to all <scan> and then to all <absJoin>
  m_pStreamer->vAppend_Streams();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Streamer::vAppend_Streams() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vInsert_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vInsert_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vResolve_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vResolve_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_indexPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_indexPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_index();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_index() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove <predCmp> (in <joinPred>) from all <filter>
  m_pFiltrate->vRemove_predCmp();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vRemove_predCmp() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any <filter> which is effectively empty
  m_pFiltrate->vDeleteEmpty_filter();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vDeleteEmpty_filter() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vDelete_joinPred();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vConvert_absJoin();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vConvert_absJoin() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vDelete_scan_R();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_scan_R() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pSender->vReadd_Sends();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Sender::vReadd_Sends() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCombiner->vAdd_combined();
  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vAdd_combined() >> "_Stz);

}

/*----------------------------------------------------------------------------*/
/* transform 1                                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  bool bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pStager->vLinearize_Stages();
  if (bInteractive)
    m_pBoard->vPrintBoard("Stager::vLinearize_Stages() >> "_Stz);

  m_pCombiner->vDelete_Combines();
  if (bInteractive)
    m_pBoard->vPrintBoard("Combiner::vDelete_Combines() >> "_Stz);

  m_pSender->vDelete_Sends();
  if (bInteractive)
    m_pBoard->vPrintBoard("Sender::vDelete_Sends() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pFiltrate->vCalc_to_calcBool();
  if (bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vCalc_to_calcBool() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append <stream> to all <scan> and then to all <absJoin>
  m_pStreamer->vAppend_Streams();
  if (bInteractive)
    m_pBoard->vPrintBoard("Streamer::vAppend_Streams() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vInsert_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vInsert_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vResolve_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vResolve_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_indexPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_indexPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_index();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_index() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove <predCmp> (in <joinPred>) from all <filter>
  m_pFiltrate->vRemove_predCmp();
  if (bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vRemove_predCmp() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any <filter> which is effectively empty
  m_pFiltrate->vDeleteEmpty_filter();
  if (bInteractive)
    m_pBoard->vPrintBoard("Filtrate::vDeleteEmpty_filter() >> "_Stz);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vDelete_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vConvert_absJoin();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vConvert_absJoin() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pJoiner->vDelete_scan_R();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_scan_R() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pSender->vReadd_Sends();
  if (bInteractive)
    m_pBoard->vPrintBoard("Sender::vReadd_Sends() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCombiner->vAdd_combined();
  if (bInteractive)
    m_pBoard->vPrintBoard("Combiner::vAdd_combined() >> "_Stz);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

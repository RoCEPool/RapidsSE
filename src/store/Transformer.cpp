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
  m_pStreamer    = nullptr;

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
  bool bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  m_pBoard = m_pOptimiser->pHeadBoard();

  m_pCombiner   = new Combiner(m_pBench);
  m_pCurator    = new Curator(m_pBench);
  m_pFiltrate   = new Filtrate(m_pBench);
  m_pJoiner     = new Joiner(m_pBench);
  m_pPredicator = new Predicator(m_pBench);
  m_pStreamer   = new Streamer(m_pBench);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pCurator->vLinearize_Stages();
  if (bInteractive)
    m_pBoard->vPrintBoard("Curator::vLinearize_Stages() >> "_Stz);

  m_pCurator->vDelete_Combines();
  if (bInteractive)
    m_pBoard->vPrintBoard("Curator::vDelete_Combines() >> "_Stz);

  m_pCurator->vDelete_Sends();
  if (bInteractive)
    m_pBoard->vPrintBoard("Curator::vDelete_Sends() >> "_Stz);

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

  m_pCurator->vReadd_Sends();
  if (bInteractive)
    m_pBoard->vPrintBoard("Curator::vReadd_Sends() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCombiner->vAdd_combined();
  if (bInteractive)
    m_pBoard->vPrintBoard("Combiner::vAdd_combined() >> "_Stz);

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

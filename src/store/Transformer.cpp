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

  m_pBooler      = nullptr;
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

  delete m_pBooler;
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

  m_pBooler     = new Booler(m_pBench);
  m_pCurator    = new Curator(m_pBench);
  m_pJoiner     = new Joiner(m_pBench);
  m_pPredicator = new Predicator(m_pBench);
  m_pStreamer   = new Streamer(m_pBench);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pCurator->vInitial_Simplification();
  if (bInteractive)
    m_pBoard->vPrintBoard("Curator::vInitial_Simplification() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pBooler->vBool();
  if (bInteractive)
    m_pBoard->vPrintBoard("Booler::vBool() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pStreamer->vStream();
  if (bInteractive)
    m_pBoard->vPrintBoard("Streamer::vStream() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pPredicator->vInsert_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Predicator::vInsert_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  m_pJoiner->vAppend_indexPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_indexPred() >> "_Stz);
  m_pJoiner->vAppend_index();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_index() >> "_Stz);


  m_pJoiner->vDelete_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 }


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

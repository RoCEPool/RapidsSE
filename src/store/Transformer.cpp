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
  m_pBoard       = m_pBench->pHead();
  m_pCard_SubSet = nullptr;

  _checkInitialised_(CN_TRANSFORMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transformer::~Transformer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);
  
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
  m_pCard_SubSet = m_pBoard->pCard_SubSet();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Simplifier* pSimplifier  = new Simplifier(m_pBench);
  pSimplifier->vSimplify();
  if (bInteractive)
    m_pBoard->vPrintBoard("Simplifier::vSimplify() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Booler* pBooler  = new Booler(m_pBench);
  pBooler->vBool();
  if (bInteractive)
    m_pBoard->vPrintBoard("Booler::vBool() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Streamer* pStreamer  = new Streamer(m_pBench);
  pStreamer->vStream();
  if (bInteractive)
    m_pBoard->vPrintBoard("Streamer::vStream() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Predicator* pPredicator  = new Predicator(m_pBench);
  pPredicator->vInsert_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Predicator::vInsert_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Joiner* pJoiner  = new Joiner(m_pBench);
  pJoiner->vAppend_indexPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_indexPred() >> "_Stz);
  pJoiner->vAppend_index();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppend_index() >> "_Stz);


  pJoiner->vDelete_joinPred();
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vDelete_joinPred() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pBooler;
  delete pJoiner;
  delete pPredicator;
  delete pSimplifier;
  delete pStreamer;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  delete pSimplifier;
  if (bInteractive)
    m_pBoard->vPrintBoard("Simplifier::vSimplify() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Booler* pBooler  = new Booler(m_pBench);
  pBooler->vBool();
  delete pBooler;
  if (bInteractive)
    m_pBoard->vPrintBoard("Booler::vBool() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Streamer* pStreamer  = new Streamer(m_pBench);
  pStreamer->vStream();
  delete pStreamer;
  if (bInteractive)
    m_pBoard->vPrintBoard("Streamer::vStream() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Predicator* pPredicator  = new Predicator(m_pBench);
  pPredicator->vDiscover();
  delete pPredicator;
  if (bInteractive)
    m_pBoard->vPrintBoard("Predicator::vDiscover() >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Joiner* pJoiner  = new Joiner(m_pBench);
  pJoiner->vJoin();
  delete pJoiner;
  if (bInteractive)
    m_pBoard->vPrintBoard("Joiner::vJoin() >> "_Stz);

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

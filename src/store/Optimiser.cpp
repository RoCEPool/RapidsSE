/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"

#include "SP_PipelineSpec.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Optimiser,CN_OPTIMISER)
__aNewFunction(Optimiser,CN_OPTIMISER)
__vDeleteFunction(Optimiser,CN_OPTIMISER)
__aFooterFunction(Optimiser)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Optimiser::Optimiser(PipelineSpec* pPipelineSpec_original)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_pPipelineSpec_original = pPipelineSpec_original;
  
  m_pBench   = new Bench;
  m_pDissect = new Dissect;
  m_pTools   = new Tools;
  m_pWeave   = new Weave;

  _checkInitialised_(CN_OPTIMISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  delete m_pDissect;
  delete m_pTools;

  vDeleteAll();
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ruler                                                           */
/*----------------------------------------------------------------------------*/
void Optimiser::vRepair(Object* pObject)
{
  _static_

  Optimiser* pOptimiser = _pOptimiser(pObject);

  delete pOptimiser;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get pPipelineSpec_original                                                 */
/*----------------------------------------------------------------------------*/
PipelineSpec* Optimiser::pPipelineSpec_original(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  return m_pPipelineSpec_original;
}

/*----------------------------------------------------------------------------*/
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void Optimiser::vRun(i8 iOptimiserLvl)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  bool bCue   = true;
  bool bPrint = true;

  if (! iOptimiserLvl)
    return;
 
    
  SP_PipelineSpec spPipelineSpec(m_pPipelineSpec_original);
    
  vTrace("Optimiser: %s\n"_Stz,spPipelineSpec.sz());
    
  if (1 == iOptimiserLvl)
    return;
  
  m_pDissect->vDissectSpec(m_pBench,m_pPipelineSpec_original);
  
  m_pTools->vRenumberCards(this);
  if (bPrint)
    m_pTools->vPrintBench(this);
  if (bCue)
    Cue::vGetReturn("dissect>> "_Stz);
  if (2 >= iOptimiserLvl)
    return;
  
  
  
  
  
  
  
  
  
  
  m_pTools->vGenerativeStreamDefs(this);
  m_pTools->vRenumberCards(this);
  if (bPrint)
    m_pTools->vPrintBench(this);
  if (bCue)
    Cue::vGetReturn("expand>> "_Stz);
  
  m_pTools->vSubstituteOrdinals(this);
  m_pTools->vRenumberCards(this);
  if (bPrint)
    m_pTools->vPrintBench(this);
  if (bCue)
    Cue::vGetReturn("ordinals>> "_Stz);
  
  m_pTools->vProjectiveStreamDefs(this);
  m_pTools->vRenumberCards(this);
  if (bPrint)
    m_pTools->vPrintBench(this);
  if (bCue)
    Cue::vGetReturn("reproject>> "_Stz);
    
  m_pTools->vStreamOrder(this);
  m_pTools->vRenumberCards(this);
  if (bPrint)
    m_pTools->vPrintBench(this);
  if (bCue)
    Cue::vGetReturn("streamOrder>> "_Stz);
    
//  m_pTools->vRenumberCards(this);
//  if (bCue)
//    Cue::vGetReturn("rework>> "_Stz);

  Board* pBoard = pGet(0);
  m_pWeave->vReassemble(pBoard);
  if (bCue)
    Cue::vGetReturn("synth>> "_Stz);  
}




/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* call closure for each card                                                 */
/*----------------------------------------------------------------------------*/
void Optimiser::vWalkUnits(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  ListExoIter2_t<Board>* pBench_I = pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    pBoard->vWalk(f_vClosure);
  }
  delete pBench_I;
}

/*----------------------------------------------------------------------------*/
/* call closure for each board                                                */
/*----------------------------------------------------------------------------*/
void Optimiser::vWalkAllCards(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  ListExoIter2_t<Board>* pBench_I = pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    pBoard->vWalkAllCards(f_vClosure);
  }
  delete pBench_I;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

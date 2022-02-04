/*============================================================================*/
/* Optimiser.cpp --                                                               */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"
#include "Weave.hpp"

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
  
  m_pTools = new Tools;

  _checkInitialised_(CN_OPTIMISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
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
  
  bool bCue = false;
  bool bPrint = true;

  if (! iOptimiserLvl)
    return;
 
  if (1 == iOptimiserLvl) {
    
    SP_PipelineSpec spPipelineSpec(m_pPipelineSpec_original);
    
    vTrace("Optimiser: %s\n"_Stz,spPipelineSpec.sz());
    
    return;
  }
  
  this->vDissect(m_pPipelineSpec_original);
  this->vRenumber();
  if (bPrint)
    this->vPrintBench();
  if (bCue)
    Cue::vGetReturn("dissect>> "_Stz);
  if (2 >= iOptimiserLvl)
    return;
  
  
  
  this->vExpand();
  this->vRenumber();
  if (bPrint)
    this->vPrintBench();
  if (bCue)
    Cue::vGetReturn("expand>> "_Stz);
  
  this->vOrdinals();
  this->vRenumber();
  if (bPrint)
    this->vPrintBench();
  if (bCue)
    Cue::vGetReturn("ordinals>> "_Stz);
  
  this->vReproject();
  this->vRenumber();
  if (bPrint)
    this->vPrintBench();
  if (bCue)
    Cue::vGetReturn("reproject>> "_Stz);
    
  this->vStreamOrder();
  this->vRenumber();
  if (bPrint)
    this->vPrintBench();
  if (bCue)
    Cue::vGetReturn("streamOrder>> "_Stz);
    
  this->vRework();
  this->vRenumber();
  if (bCue)
    Cue::vGetReturn("rework>> "_Stz);

  this->vSynthesize();
  if (bCue)
    Cue::vGetReturn("synth>> "_Stz);
}




/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* vDissect the pipeline spec                                                 */
/*----------------------------------------------------------------------------*/
void Optimiser::vDissect(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
      
  Dissect* pDissect = new Dissect(this);
  pDissect->vDissectSpec(pPipelineSpec);
  delete pDissect;
}

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

/*----------------------------------------------------------------------------*/
/* expand                                                                     */
/*----------------------------------------------------------------------------*/
void Optimiser::vExpand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_pTools->vGenerativeStreamDefs(this);
}

/*----------------------------------------------------------------------------*/
/* substitute ordinals                                                        */
/*----------------------------------------------------------------------------*/
void Optimiser::vOrdinals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_pTools->vSubstituteOrdinals(this);
}

/*----------------------------------------------------------------------------*/
/* renumber                                                                   */
/*----------------------------------------------------------------------------*/
void Optimiser::vRenumber(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_pTools->vRenumberCards(this);
}

/*----------------------------------------------------------------------------*/
/* reproject                                                                  */
/*----------------------------------------------------------------------------*/
void Optimiser::vReproject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_pTools->vProjectiveStreamDefs(this);
}

/*----------------------------------------------------------------------------*/
/* rework                                                                     */
/*----------------------------------------------------------------------------*/
void Optimiser::vRework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* stream order                                                               */
/*----------------------------------------------------------------------------*/
void Optimiser::vStreamOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  m_pTools->vStreamOrder(this);
}

/*----------------------------------------------------------------------------*/
/* reassemble                                                                 */
/*----------------------------------------------------------------------------*/
void Optimiser::vSynthesize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
        
  Board* pBoard = pGet(0);
  Weave* pWeave = new Weave(pBoard);
  pWeave->vReassemble();
  delete pWeave;
}

/*----------------------------------------------------------------------------*/
/* print bench                                                                */
/*----------------------------------------------------------------------------*/
void Optimiser::vPrintBench(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  i8 nBoards = nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board* pBoard = pGet(i_Board);
    
    Ribbon* pRibbon = new Ribbon(0);
  
    pBoard->vPinBoard(pRibbon,i_Board);
    pRibbon->vPrintRibbon();
  
    delete pRibbon;
  }
  
  //s256 sIn;
  //fgets(sIn.azChar(),128,stdin);
  
  //Cue::vGetReturn("bench> "_Stz);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

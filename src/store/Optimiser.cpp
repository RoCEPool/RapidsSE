/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"

#include "Dynamo.hpp"

#include "SP_PipeSpec.hpp"

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
Optimiser::Optimiser(TagSet* p_JobTags)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_p_JobTags = p_JobTags;
  m_pUnOptPipeSpec = nullptr;
  
  m_pCardCtl = new CardCtl;
  
  m_pBench   = new Bench(this);
  m_pDissect = new Dissect(this);
  m_pDrafter = new Drafter(this);
  m_pWeave   = new Weave(this);

  m_pPaste   = new Card_SubSet;

  _checkInitialised_(CN_OPTIMISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  delete m_pCardCtl;
  
  delete m_pBench;
  delete m_pDissect;
  delete m_pDrafter;
  delete m_pWeave;
  
  delete m_pPaste;
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the optimiser                                                       */
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
/* get cardCtl                                                                */
/*----------------------------------------------------------------------------*/
CardCtl* Optimiser::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pCardCtl;
}

/*----------------------------------------------------------------------------*/
/* dissect                                                                    */
/*----------------------------------------------------------------------------*/
void Optimiser::vDissect(VertexSpec* pm_pUnOptPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
   
  m_pUnOptPipeSpec = pm_pUnOptPipeSpec;
  
  SP_PipeSpec spPipeSpec(m_pUnOptPipeSpec);
  vTrace("Optimiser: %s\n"_Stz,spPipeSpec.sz());
      
  Board* pBoard = m_pDissect->pDissectSpec(m_pUnOptPipeSpec);
  
  m_pBench->vPush(pBoard);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vOptimise();

  return;
}

/*----------------------------------------------------------------------------*/
/* get drafter                                                                */
/*----------------------------------------------------------------------------*/
Drafter* Optimiser::pDrafter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pDrafter;
}

/*----------------------------------------------------------------------------*/
/* get top board                                                              */
/*----------------------------------------------------------------------------*/
Board* Optimiser::pHeadBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pHead();

  return pBoard;
}

/*----------------------------------------------------------------------------*/
/* get cardCtl                                                                */
/*----------------------------------------------------------------------------*/
TagSet* Optimiser::pJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_p_JobTags;
}

/*----------------------------------------------------------------------------*/
/* get paste buffer                                                          */
/*----------------------------------------------------------------------------*/
Card_SubSet* Optimiser::pPaste(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pPaste;
}

/*----------------------------------------------------------------------------*/
/* set m_pUnOptPipeSpec                                                   */
/*----------------------------------------------------------------------------*/
void Optimiser::vPipeSpec(VertexSpec* pm_pUnOptPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  if (m_pUnOptPipeSpec) {
    //delete m_pUnOptPipeSpec;                                        LEAK!
    m_pUnOptPipeSpec = nullptr;
  }
  
  m_pUnOptPipeSpec = pm_pUnOptPipeSpec;
}

/*----------------------------------------------------------------------------*/
/* pop & delete top board                                                     */
/*----------------------------------------------------------------------------*/
void Optimiser::vPopBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pPop();

  if (pBoard)
    delete pBoard;
}

/*----------------------------------------------------------------------------*/
/* pop & delete top board                                                     */
/*----------------------------------------------------------------------------*/
void Optimiser::vPushBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_pBench->vPush(pBoard);
}

/*----------------------------------------------------------------------------*/
/* run dynamo                                                                 */
/*----------------------------------------------------------------------------*/
void Optimiser::vToPipeSpec(VertexSpec* pPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
 
  vPipeSpec(pPipeSpec);
  
  delete m_pBench->pPop();
  
  Board* pBoard = m_pDissect->pDissectSpec(m_pUnOptPipeSpec);

  m_pBench->vAppend(pBoard);

}

/*----------------------------------------------------------------------------*/
/* get ewaved statement                                                       */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szWeave(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = this->pHeadBoard();
  m_pWeave->vWeaveOptStmt(pBoard);

  Stz* szPipeline = m_pWeave->szOptStmt();

  return szPipeline;
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Optimiser::vOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pHead();
  CardRegs* pCardRegs = pBoard->pCardRegs();

  pBoard->vPrintBoard("vDissectSpec >> "_Stz);
  
  pBoard->vResolve_Symbols();
  pBoard->vPrintBoard("vResolve_Symbols >> "_Stz);

  // rules
  
  if (pBoard->bMay_Scan_Filter_to_IndexScan()) {
    
    bool bRun = pBoard->bRule_Scan_Filter_to_IndexScan();
    UNUSED(bRun);
    
    pCardRegs->vResetCardRegs();
  }
  
  Cue::vGetReturn("$$$$$$ >>"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

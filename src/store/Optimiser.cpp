/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"

#include "Dynamo.hpp"

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
Optimiser::Optimiser(TagSet* p_JobTags)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_p_JobTags = p_JobTags;
  m_pUnOptPipelineSpec = nullptr;
  
  m_pCardCtl = new CardCtl;
  
  m_pBench   = new Bench(this);
  m_pDissect = new Dissect(this);
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
  delete m_pWeave;
  
  delete m_pPaste;
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
void Optimiser::vDissect(PipelineSpec* pUnOptPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
   
  m_pUnOptPipelineSpec = pUnOptPipelineSpec;
  
  SP_PipelineSpec spPipelineSpec(m_pUnOptPipelineSpec);
  vTrace("Optimiser: %s\n"_Stz,spPipelineSpec.sz());
      
  Board* pBoard = m_pDissect->pDissectSpec(m_pUnOptPipelineSpec);
  
  m_pBench->vPush(pBoard);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vOptimise();

  return;
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
/* set m_pUnOptPipelineSpec                                                   */
/*----------------------------------------------------------------------------*/
void Optimiser::vPipelineSpec(PipelineSpec* pUnOptPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  if (m_pUnOptPipelineSpec) {
    //delete m_pUnOptPipelineSpec;                                        LEAK!
    m_pUnOptPipelineSpec = nullptr;
  }
  
  m_pUnOptPipelineSpec = pUnOptPipelineSpec;
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
void Optimiser::vToPipelineSpec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
 
  vPipelineSpec(pPipelineSpec);
  
  delete m_pBench->pPop();
  
  Board* pBoard = m_pDissect->pDissectSpec(m_pUnOptPipelineSpec);

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

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
Optimiser::Optimiser(PipelineSpec* pUnOptPipelineSpec,
                     TagSet* p_JobTags,
                     i8 iOptimiserLvl)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_p_JobTags = p_JobTags;
  m_iOptimiserLvl = iOptimiserLvl;
  m_pUnOptPipelineSpec = pUnOptPipelineSpec;
  
  m_pBench   = new Bench;
  m_pCardCtl = new CardCtl;
  m_pDissect = new Dissect;
  m_pRules   = new Rules;
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
  
  delete m_pBench;
  delete m_pCardCtl;
  delete m_pDissect;
  delete m_pRules;
  delete m_pWeave;
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
/* get top board                                                              */
/*----------------------------------------------------------------------------*/
Board* Optimiser::pBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pGet(0);

  return pBoard;
}

/*----------------------------------------------------------------------------*/
/* get cardCtl                                                                */
/*----------------------------------------------------------------------------*/
CardCtl* Optimiser::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pCardCtl;
}

/*----------------------------------------------------------------------------*/
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void Optimiser::vOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  if (! m_iOptimiserLvl)
    return;
 
    
  SP_PipelineSpec spPipelineSpec(m_pUnOptPipelineSpec);
  vTrace("Optimiser: %s\n"_Stz,spPipelineSpec.sz());
    
  if (1 == m_iOptimiserLvl)
    return;
  
  m_pDissect->vDissectSpec(m_pBench,m_pUnOptPipelineSpec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vOptimise_Board();

  return;
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
/* run dynamo                                                                 */
/*----------------------------------------------------------------------------*/
void Optimiser::vToPipelineSpec(Stz* szStmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Dynamo* pDynamo = new Dynamo(szStmt,m_p_JobTags);
  
  PipelineSpec* pPipelineSpec = pDynamo->pCompile();
  
  vPipelineSpec(pPipelineSpec);
  
  delete m_pBench->pPop();
  
  m_pDissect->vDissectSpec(m_pBench,m_pUnOptPipelineSpec);

  delete pDynamo;
}

/*----------------------------------------------------------------------------*/
/* get ewaved statement                                                       */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szWeave(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = this->pBoard();
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
void Optimiser::vOptimise_Board(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pGet(0);
  
  pBoard->vOptimiser(this,m_pCardCtl);
  
  pBoard->vPrintBoard("vDissectSpec >> "_Stz);
  
  pBoard->vResolve_Symbols();
  pBoard->vPrintBoard("vResolve_Symbols >> "_Stz);

  // rules
  
  if (pBoard->bMay_Scan_Filter_to_IndexScan()) {
    
    bool bRun = pBoard->bRule_Scan_Filter_to_IndexScan();
    UNUSED(bRun);
    
    pBoard->vResetRegisters();
  }
  
  Cue::vGetReturn("$$$$$$ >>"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

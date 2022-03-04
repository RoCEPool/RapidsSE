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
                     TagSet* p_JobTags)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_p_JobTags = p_JobTags;
  m_pUnOptPipelineSpec = pUnOptPipelineSpec;
  
  m_pCardCtl = new CardCtl;
  
  m_pBench   = new Bench(this);
  m_pDissect = new Dissect(this);
  m_pWeave   = new Weave(this);

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

  Board* pBoard = m_pBench->pHead();

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
   
  SP_PipelineSpec spPipelineSpec(m_pUnOptPipelineSpec);
  vTrace("Optimiser: %s\n"_Stz,spPipelineSpec.sz());
      
  Board* pBoard = m_pDissect->pDissectSpec(m_pUnOptPipelineSpec);
  
  m_pBench->vAppend(pBoard);

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
  
  Board* pBoard = m_pDissect->pDissectSpec(m_pUnOptPipelineSpec);

  m_pBench->vAppend(pBoard);

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

  Board* pBoard = m_pBench->pHead();
    
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

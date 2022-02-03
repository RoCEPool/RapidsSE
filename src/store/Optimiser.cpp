/*============================================================================*/
/* Optimiser.cpp --                                                               */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Bench.hpp"
#include "Cue.hpp"

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
  
  _checkInitialised_(CN_OPTIMISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
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
void Optimiser::vRun(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  Bench* pBench = new Bench;
  
  pBench->vDissect(m_pPipelineSpec_original);
  pBench->vRenumber();
  pBench->vPrintBench();
  Cue::vGetReturn("dissect>> "_Stz);

  pBench->vExpand();
  pBench->vRenumber();
  pBench->vPrintBench();
  Cue::vGetReturn("expand>> "_Stz);
  
  pBench->vOrdinals();
  pBench->vRenumber();
  pBench->vPrintBench();
  Cue::vGetReturn("ordinals>> "_Stz);
  
  pBench->vReproject();
  pBench->vRenumber();
  pBench->vPrintBench();
  Cue::vGetReturn("reproject>> "_Stz);
    
  pBench->vStreamOrder();
  pBench->vRenumber();
  pBench->vPrintBench();
  Cue::vGetReturn("streamOrder>> "_Stz);
    
  pBench->vRework();
  pBench->vRenumber();
  Cue::vGetReturn("rework>> "_Stz);

  pBench->vSynthesize();
  Cue::vGetReturn("synth>> "_Stz);

  delete pBench;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

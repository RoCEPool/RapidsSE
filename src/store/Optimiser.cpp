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
Optimiser::Optimiser(void)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
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
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void Optimiser::vRun(PipelineSpec *pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  Bench* pBench = new Bench;
  
  pBench->vDissect(pPipelineSpec);
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

/*============================================================================*/
/* Drafter.cpp --                                                               */
/*============================================================================*/

#include "Drafter.hpp"
#include "Bench.hpp"
#include "Cue.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Drafter,CN_DRAFTER)
__aNewFunction(Drafter,CN_DRAFTER)
__vDeleteFunction(Drafter,CN_DRAFTER)
__aFooterFunction(Drafter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Drafter::Drafter(void)
: Object(CN_DRAFTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
    
  _checkInitialised_(CN_DRAFTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Drafter::~Drafter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ruler                                                           */
/*----------------------------------------------------------------------------*/
void Drafter::vRepair(Object* pObject)
{
  _static_

  Drafter* pDrafter = _pDrafter(pObject);

  delete pDrafter;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void Drafter::vRun(PipelineSpec *pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
  
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

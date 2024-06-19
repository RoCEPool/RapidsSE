/*============================================================================*/
/* Steps_PtnMap.cpp - partition map step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LglPtnMap.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PtnMap(void)
{
  vAddStep("ptnMap::adjust"_Stz,     &Step::vPtnMapAdjust);
  vAddStep("ptnmap::ptnsInGrid"_Stz, &Step::vPtnMapPtnsInGrid);
  vAddStep("ptnmap::see"_Stz,        &Step::vPtnMapSee);
}

/*----------------------------------------------------------------------------*/
/* ptnStream::adjust                                                          */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapAdjust(void)
{
  ENTRY();
  
  LglPtnMap* pPtnMap = AM::pLglPtnMap();
  
  pPtnMap->vAdjustMap(AM::GRID::nKnownNodes());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnmap::ptnsInGrid -- get number of partitions in the grid                 */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapPtnsInGrid(void)
{
  ENTRY();
  
  m_oDataStack.vPushInt(AM::PTNMAP::nPtnsInGrid());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnmap::see -- see the ptnmap                                              */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapSee(void)
{
  ENTRY();
  
  LglPtnMap* pPtnMap = AM::pLglPtnMap();
  
  pPtnMap->vSee();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

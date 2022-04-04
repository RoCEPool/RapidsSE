/*============================================================================*/
/* Steps_PtnMap.cpp - partition map step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "PtnMap.hpp"
#include "BucketMap.hpp"

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
/* ptnStream::bufferNumber                                                    */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapAdjust(void)
{
  ENTRY();
  
  PtnMap* pPtnMap = AM::pPtnMap();
  
  pPtnMap->vAdjustMap(AM::GRID::nKnownNodes());

  BucketMap* pBktMap = AM::BKTMAP::pBucketMap();
  pBktMap->vAdjustMap(pPtnMap->nPtnsInGrid());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnmap::ptnsInGrid -- get number of partitions in the grid                 */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapPtnsInGrid(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::PTNMAP::nPtnsInGrid());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnmap::see -- see the ptnmap                                              */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapSee(void)
{
  ENTRY();
  
  PtnMap* pPtnMap = AM::pPtnMap();
  
  pPtnMap->vSee();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

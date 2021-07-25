/*============================================================================*/
/* Steps_PtnMap.cpp - partition map step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "PtnMap.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PtnMap(void)
{
  vAddStep("ptnMap::adjust"_Stz,     &Step::vPtnMapAdjust);
  vAddStep("ptnmap::partitions"_Stz, &Step::vPtnMapPartitions);

}

/*----------------------------------------------------------------------------*/
/* ptnStream::bufferNumber                                                    */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapAdjust(void)
{
  ENTRY();
  
  PtnMap* pPtnMap = AM::pPtnMap();
  
  pPtnMap->vDumpDataBuffers();
  pPtnMap->vAdjustDataBuffers(AM::GRID::nKnownNodes(),AM::nDegOfPar());
  pPtnMap->vDumpDataBuffers();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnmap::partitions -- get number of partitions in the grid                   */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapPartitions(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::PTNMAP::nPtnsInGrid());
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

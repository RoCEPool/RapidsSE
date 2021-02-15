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
  vAddStep("ptnMap::adjust"_Stz, &Step::vPtnMapAdjust);
}

/*----------------------------------------------------------------------------*/
/* ptnStream::bufferNumber                                                    */
/*----------------------------------------------------------------------------*/
void Step::vPtnMapAdjust(void)
{
  ENTRY();
  
  PtnMap* pPtnMap = AM::pPtnMap();
  
  pPtnMap->vDumpDataBuffers();
  pPtnMap->vAdjustDataBuffers(AM::nConnectedNodes(),AM::nDegOfPar());
  pPtnMap->vDumpDataBuffers();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

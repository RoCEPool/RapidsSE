/*============================================================================*/
/* Steps_Ticking.cpp - ticking device steps group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Ticking(void)
{
  vAddStep("ticking::getTick"_Stz,      &Step::vTickingGetTick);
  vAddStep("ticking::ticksPerSec"_Stz,  &Step::vTickingTicksPerSec);
}

/*----------------------------------------------------------------------------*/
/* ticking::getTick -- get tick from ticking device                           */
/*----------------------------------------------------------------------------*/
void Step::vTickingGetTick(void)
{
  ENTRY();
  
  i8 iTick = TickingDevice::iGetTick();
  
  m_pDataStack->vPushInt(iTick);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ticking::ticksPerSec -- get ticks per second from ticking device           */
/*----------------------------------------------------------------------------*/
void Step::vTickingTicksPerSec(void)
{
  ENTRY();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  m_pDataStack->vPushInt(iTicksPerSec);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

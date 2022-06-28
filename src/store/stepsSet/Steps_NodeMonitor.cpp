/*============================================================================*/
/* Steps_NodeMonitor.cpp - Node Monitor step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_NodeMonitor(void)
{
  vAddStep("NodeMonitor::DummyCall"_Stz,   &Step::vNodeMonitorDummyCall);
}

/*----------------------------------------------------------------------------*/
/* NodeMonitor::DummyCall -- placeholder                                      */
/*----------------------------------------------------------------------------*/
void Step::vNodeMonitorDummyCall(void)
{
  ENTRY();
  vTrace("Test print\n"_Stz);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
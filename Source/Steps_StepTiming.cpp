/*============================================================================*/
/* Steps_StepTiming.cpp - StepTiming step group                               */
/*============================================================================*/

#include "Steps.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StepTiming(void)
{
  vAddStep("StepTiming::reset"_Stz, &Step::vStepTimingReset);
  vAddStep("StepTiming::start"_Stz, &Step::vStepTimingStart);
  vAddStep("StepTiming::write"_Stz, &Step::vStepTimingWrite);
}

/*----------------------------------------------------------------------------*/
/* StepTiming::reset -- reset steps                                           */
/*----------------------------------------------------------------------------*/
void Step::vStepTimingReset(MP_Task mpTask)
{
  ENTRY("vStepTimingReset"_Stz);
  
#ifndef WITH_ENTRY_EXIT
  
  vTrace("WARNING: WITH_ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (AM::pStepTiming())
    AM::pStepTiming()->vReset();
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StepTiming::start -- start step tracking                                   */
/*----------------------------------------------------------------------------*/
void Step::vStepTimingStart(MP_Task mpTask)
{
  ENTRY("vStepTimingStart"_Stz);
  
#ifndef WITH_ENTRY_EXIT
  
  vTrace("WARNING: WITH_ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (! AM::pStepTiming())
    AM::vStepTiming(new StepTiming);
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StepTiming::write -- write steps                                          */
/*----------------------------------------------------------------------------*/
void Step::vStepTimingWrite(MP_Task mpTask)
{
  ENTRY("vStepTimingWritevStepTimingWrite"_Stz);

#ifndef WITH_ENTRY_EXIT
  
  vTrace("WARNING: WITH_ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (AM::pStepTiming())
    AM::pStepTiming()->vWrite();
  
#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

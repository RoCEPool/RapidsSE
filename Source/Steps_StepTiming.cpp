/*============================================================================*/
/* Steps_StepTiming.cpp - StepTiming step group                               */
/*============================================================================*/

#include "Steps.h"

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
void Step::vStepTimingReset(void)
{
  ENTRY();
  
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
void Step::vStepTimingStart(void)
{
  ENTRY();
  
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
void Step::vStepTimingWrite(void)
{
  ENTRY();

#ifndef WITH_ENTRY_EXIT
  
  vTrace("WARNING: WITH_ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else

  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  if (AM::pStepTiming())
    AM::pStepTiming()->vWrite(sPath);
  
#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

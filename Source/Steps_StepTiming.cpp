/*============================================================================*/
/* Steps_StepTiming.cpp - StepTiming step group                               */
/*============================================================================*/

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
/* StepTiming::reset -- reset steps                                          */
/*----------------------------------------------------------------------------*/
void Step::vStepTimingReset(MP_Task mpTask)
{
  ENTRY("vStepTimingReset"_Stz);
  
#ifndef __ENTRY_EXIT
  
  vTrace("WARNING: __ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (gpAttachMap->m_pStepTiming)
    gpAttachMap->m_pStepTiming->vReset();
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StepTiming::start -- start step tracking                                  */
/*----------------------------------------------------------------------------*/
void Step::vStepTimingStart(MP_Task mpTask)
{
  ENTRY("vStepTimingStart"_Stz);
  
#ifndef __ENTRY_EXIT
  
  vTrace("WARNING: __ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (! gpAttachMap->m_pStepTiming)
    gpAttachMap->m_pStepTiming = new StepTiming;
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StepTiming::write -- write steps                                          */
/*----------------------------------------------------------------------------*/
void Step::vStepTimingWrite(MP_Task mpTask)
{
  ENTRY("vStepTimingWritevStepTimingWrite"_Stz);

#ifndef __ENTRY_EXIT
  
  vTrace("WARNING: __ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (gpAttachMap->m_pStepTiming)
    gpAttachMap->m_pStepTiming->vWrite();
  
#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

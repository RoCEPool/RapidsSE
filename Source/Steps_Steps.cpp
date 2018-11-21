/*============================================================================*/
/* Steps_Steps.cpp - steps step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Steps(void)
{
  vAddStep("steps::reset"_Stz, &Step::vStepsReset);
  vAddStep("steps::start"_Stz, &Step::vStepsStart);
  vAddStep("steps::write"_Stz, &Step::vStepsWrite);
}

/*----------------------------------------------------------------------------*/
/* steps::reset -- reset steps                                                */
/*----------------------------------------------------------------------------*/
void Step::vStepsReset(MP_Task mpTask)
{
  ENTRY("vStepsReset"_Stz);
  
  if (gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::start -- start step tracking                                        */
/*----------------------------------------------------------------------------*/
void Step::vStepsStart(MP_Task mpTask)
{
  ENTRY("vStepsStart"_Stz);
  
  if (! gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker = new StepTracker;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::write -- write steps                                                */
/*----------------------------------------------------------------------------*/
void Step::vStepsWrite(MP_Task mpTask)
{
  ENTRY("vStepsWrite"_Stz);

  if (gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker->vWrite();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

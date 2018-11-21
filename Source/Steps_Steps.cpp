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
  vAddStep("steps::reset"_Stz, &Task::vStepsReset);
  vAddStep("steps::start"_Stz, &Task::vStepsStart);
  vAddStep("steps::write"_Stz, &Task::vStepsWrite);
}

/*----------------------------------------------------------------------------*/
/* steps::reset -- reset steps                                                */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(MP_Task mpTask)
{
  ENTRY("vStepsReset"_Stz);
  
  if (gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::start -- start step tracking                                        */
/*----------------------------------------------------------------------------*/
void Task::vStepsStart(MP_Task mpTask)
{
  ENTRY("vStepsStart"_Stz);
  
  if (! gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker = new StepTracker;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::write -- write steps                                                */
/*----------------------------------------------------------------------------*/
void Task::vStepsWrite(MP_Task mpTask)
{
  ENTRY("vStepsWrite"_Stz);

  if (gpAttachMap->m_pStepTracker) {
    File xFile("/Rapids/StepTracker.csv"_Stz);
    gpAttachMap->m_pStepTracker->vWrite(xFile);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

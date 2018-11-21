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
  vAddStep("steps::write"_Stz, &Task::vStepsWrite);
}

/*----------------------------------------------------------------------------*/
/* steps::reset -- reset steps                                                */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(MP_Task mpTask)
{
  ENTRY("vStepsReset"_Stz);
  
  gpAttachMap->m_pStepTracker->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::write -- write steps                                                */
/*----------------------------------------------------------------------------*/
void Task::vStepsWrite(MP_Task mpTask)
{
  ENTRY("vStepsList"_Stz);
  
  File xFile("/Rapids/StepTracker.csv"_Stz);
  gpAttachMap->m_pStepTracker->vWrite(xFile);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  vAddStep("steps::list"_Stz,  &Task::vStepsList,  false);
  vAddStep("steps::reset"_Stz, &Task::vStepsReset, false);
}

/*----------------------------------------------------------------------------*/
/* steps::list -- list steps                                                  */
/*----------------------------------------------------------------------------*/
void Task::vStepsList(MP_Task mpTask)
{
ENTRY("vStepsList"_Stz);

  File xFile;
  xFile.bOpen("steps.csv"_Stz, C_OPEN_WRITE);
  
  gpAttachMap->m_pStepArray->vList(xFile);
  
  xFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::reset -- reset steps                                               */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(MP_Task mpTask)
{
  ENTRY("vStepsReset"_Stz);
  
  gpAttachMap->m_pStepArray->vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

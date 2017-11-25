/*============================================================================*/
/* Steps_Steps.cpp - steps step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Steps(void)
{
  vAddPrimitive("steps::list"_Stz,  &vStepsList,  false);
  vAddPrimitive("steps::reset"_Stz, &vStepsReset, false);
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

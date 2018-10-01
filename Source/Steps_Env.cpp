/*============================================================================*/
/* Steps_Env.cpp - env step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// env: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Env(void)
{
  vAddStep("env@"_Stz,            &Task::vEnvGet,           false);
}

/*----------------------------------------------------------------------------*/
/* env@ -- get environment variable                                           */
/*----------------------------------------------------------------------------*/
void Task::vEnvGet(MP_Task mpTask)
{
  ENTRY("vEnvGet"_Stz);
  
  s256 sEnvName;
  
  if ( ! mpTask->m_pDataStack->bPop(sEnvName) )
    THROW_INVALID_DSTACK("env@"_Stz);
  
  s256 sEnvVal;
  
  if (! Env::bGet(sEnvName,sEnvVal))
    sEnvVal = ""_Stz;
  
  mpTask->m_pDataStack->vPushS256(sEnvVal);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

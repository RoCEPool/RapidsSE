/*============================================================================*/
/* Steps_Env.cpp - env step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Env(void)
{
  vAddStep("env@"_Stz,            &Step::vEnvGet);
}

/*----------------------------------------------------------------------------*/
/* env@ -- get environment variable                                           */
/*----------------------------------------------------------------------------*/
void Step::vEnvGet(MP_Task mpTask)
{
  ENTRY("vEnvGet"_Stz);
  
  s32 sEnvName;
  
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

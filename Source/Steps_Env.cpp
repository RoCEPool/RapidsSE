/*============================================================================*/
/* Steps_Env.cpp - env step group                                             */
/*============================================================================*/

#include "Steps.h"

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
void Step::vEnvGet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnvGet"_Stz);
  
  s32 sEnvName;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sEnvName) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  s256 sEnvVal;
  
  if (! Env::bGet(sEnvName,sEnvVal))
    sEnvVal = ""_Stz;
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sEnvVal);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

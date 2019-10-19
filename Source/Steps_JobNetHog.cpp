/*============================================================================*/
/* Steps_JobNetHog.cpp - job_netHog step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobNetHog(void)
{
  vAddStep("job_nethog.hogNo@"_Stz,     &Step::vJob_NetHogHogNoFetch);
}

/*----------------------------------------------------------------------------*/
/* job_nodeInfo.nodeName@ -- fetch node name                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_NetHogHogNoFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_NetHogHogNoFetch"_Stz);
  
  Object* pPtrJob_NetHog;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_NetHog) )
    THROW_DSTACK();
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pPtrJob_NetHog);
  
  pForth->m_pDataStack->vPushI8(pJob_NetHog->m_iHogNo);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

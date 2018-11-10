/*============================================================================*/
/* Steps_JobNetHog.cpp - job_netHog step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobNetHog(void)
{
  vAddStep("job_nethog.hogNo@"_Stz,     &Task::vJob_NetHogHogNoFetch);
}

/*----------------------------------------------------------------------------*/
/* job_nodeInfo.nodeName@ -- fetch node name                                  */
/*----------------------------------------------------------------------------*/
void Task::vJob_NetHogHogNoFetch(MP_Task mpTask)
{
  ENTRY("vJob_NetHogHogNoFetch"_Stz);
  
  Object* pPtrJob_NetHog;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_NetHog) )
    THROW_INVALID_DSTACK("job_nethog.hogNo@"_Stz);
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pPtrJob_NetHog);
  
  mpTask->m_pDataStack->vPushInt(pJob_NetHog->m_iHogNo);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

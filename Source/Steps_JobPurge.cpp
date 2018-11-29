/*============================================================================*/
/* Steps_JobPurge.cpp - job_purge step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPurge(void)
{
  vAddStep("job_purge.purge"_Stz,     &Step::vJob_PurgePurge);
}

/*----------------------------------------------------------------------------*/
/* job_purge.purge -- purge table                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgePurge(MP_Task mpTask)
{
  ENTRY("vJob_PurgePurge"_Stz);
  
  Object* pPtrJobPurge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobPurge) )
    THROW_INVALID_DSTACK("job_purge.purge"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJobPurge);
  
  pJob_Purge->vPurge();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

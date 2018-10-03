/*============================================================================*/
/* Steps_job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubLoader(void)
{
  vAddStep("Job_subLoader.jobLoadTable@"_Stz, &Task::vJob_SubLoaderJobLoadTableFetch, false);
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.jobLoadTable@ -- fetch the job load table                    */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderJobLoadTableFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderJobLoadTableFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subLoader.jobLoadTable@"_Stz);
  Job_SubLoader* pJob = _pJob_SubLoader(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pJob_LoadTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

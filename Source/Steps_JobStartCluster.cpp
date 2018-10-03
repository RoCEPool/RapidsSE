/*============================================================================*/
/* Steps_JobStartCluster.cpp - job_startCluster step group                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobStartCluster(void)
{
  vAddStep("job_startCluster.spawnNodes@"_Stz, &Task::vJob_StartClusterSpawnNodesFetch, false);
}

/*----------------------------------------------------------------------------*/
/* job_startCluster.spawnNodes@ --fetch spawnNodes from job_startCluster      */
/*----------------------------------------------------------------------------*/
void Task::vJob_StartClusterSpawnNodesFetch(MP_Task mpTask)
{
  ENTRY("vJob_StartClusterSpawnNodesFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_startCluster.spawnNodes@"_Stz);
  Job_StartCluster* pJob_StartCluster = _pJob_StartCluster(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_StartCluster->m_bSpawnNodes);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

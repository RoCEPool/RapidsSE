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
void Task::vAddPrimitives_JobStartCluster(void)
{
  vAddPrimitive("job_startCluster.spawnNodes@"_Stz, &vJobStartClusterSpawnNodesFetch, false);
}

/*----------------------------------------------------------------------------*/
/* job_startCluster.spawnNodes@ --fetch spawnNodes from job_startCluster      */
/*----------------------------------------------------------------------------*/
void Task::vJobStartClusterSpawnNodesFetch(MP_Task mpTask)
{
  ENTRY("vJobStartClusterSpawnNodesFetch"_Stz);
  
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

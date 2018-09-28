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
void Task::vAddPrimitives_JobCreateIndex(void)
{
  vAddPrimitive("job_createIndex.isUnique@"_Stz, &vJobCreateIndexIsUniqueFetch,  false);
}

/*----------------------------------------------------------------------------*/
/* job_createIndex.isUnique@ -- fetch column type from create table job       */
/*----------------------------------------------------------------------------*/
void Task::vJobCreateIndexIsUniqueFetch(MP_Task mpTask)
{
  ENTRY("vJobCreateIndexIsUniqueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_createIndex.isUnique@"_Stz);
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_CreateIndex->m_bIsUnique);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

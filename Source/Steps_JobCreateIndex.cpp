/*============================================================================*/
/* Steps_JobCreateIndex.cpp - jobCreateIndex step group                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobCreateIndex(void)
{
  vAddStep("job_createIndex.isUnique@"_Stz, &Step::vJob_CreateIndexIsUniqueFetch);
}

/*----------------------------------------------------------------------------*/
/* job_createIndex.isUnique@ -- fetch column type from create table job       */
/*----------------------------------------------------------------------------*/
void Step::vJob_CreateIndexIsUniqueFetch(MP_Task mpTask)
{
  ENTRY("vJob_CreateIndexIsUniqueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_createIndex.isUnique@"_Stz);
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob_CreateIndex->m_bIsUnique);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

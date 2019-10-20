/*============================================================================*/
/* Steps_JobCreateIndex.cpp - jobCreateIndex step group                       */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_CreateIndexIsUniqueFetch(void)
{
  ENTRY("vJob_CreateIndexIsUniqueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJobPtr);
  
  m_pDataStack->vPushI8(pJob_CreateIndex->m_bIsUnique);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

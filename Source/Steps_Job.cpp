/*============================================================================*/
/* Steps_job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Job(void)
{
  vAddPrimitive("job.error@"_Stz,     &vJobErrorFetch,     false);
  vAddPrimitive("job.error!"_Stz,     &vJobErrorStore,     false);
  vAddPrimitive("job.execute@"_Stz,   &vJobExecuteFetch,   false);
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error                                          */
/*----------------------------------------------------------------------------*/
void Task::vJobErrorFetch(MP_Task mpTask)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.error@"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->eJobError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error                                          */
/*----------------------------------------------------------------------------*/
void Task::vJobErrorStore(MP_Task mpTask)
{
  ENTRY("vJobErrorStore"_Stz);
  
  Object* pJobPtr;
  
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  
  i8 iJobError;
  
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobError) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  pJob->vJobError((eEC)iJobError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.execute@ -- fetch the job word to be executed                          */
/*----------------------------------------------------------------------------*/
void Task::vJobExecuteFetch(MP_Task mpTask)
{
  ENTRY("vJobExecuteFetch"_Stz);
  
  Object* pJobPtr;
  
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.execute@"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  s32 sExecute;
  sExecute = pJob->sJobExecute();
  
  mpTask->m_pDataStack->vPushS32(sExecute);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

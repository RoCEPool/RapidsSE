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
void Task::vAddPrimitives_Job(void)
{
  vAddPrimitive("job.addOriginator"_Stz,    &vJobAddOriginator,    false);
  vAddPrimitive("job.addParticipant"_Stz,   &vJobAddParticipant,   false);
  vAddPrimitive("job.acksRemaining!"_Stz,   &vJobAcksRemaining,    false);
  vAddPrimitive("job.class"_Stz,            &vJobClass,            false);
  vAddPrimitive("job.delete"_Stz,           &vJobDelete,           false);
  vAddPrimitive("job.decAcksRemaining"_Stz, &vJobDecAcksRemaining, false);
  vAddPrimitive("job.error@"_Stz,           &vJobErrorFetch,       false);
  vAddPrimitive("job.error!"_Stz,           &vJobErrorStore,       false);
  vAddPrimitive("job.execute@"_Stz,         &vJobExecuteFetch,     false);
  vAddPrimitive("job.findOriginator"_Stz,   &vJobFindOriginator,   false);
  vAddPrimitive("job.findParticipant"_Stz,  &vJobFindParticipant,  false);
  vAddPrimitive("job.id@"_Stz,              &vJobIdFetch,          false);
  vAddPrimitive("job.objectName@"_Stz,      &vJobObjectNameFetch,  false);
  vAddPrimitive("job.newBegun"_Stz,         &vJobNewBegun,         false);
  vAddPrimitive("job.newEnd"_Stz,           &vJobNewEnd,           false);
  vAddPrimitive("job.newEnded"_Stz,         &vJobNewEnded,         false);
  vAddPrimitive("job.newExecute"_Stz,       &vJobNewExecute,       false);
  vAddPrimitive("job.newExecuted"_Stz,      &vJobNewExecuted,      false);
  vAddPrimitive("job::originator@"_Stz,     &vJobOriginatorFetch,  false);
  vAddPrimitive("job.participant@"_Stz,     &vJobParticipantFetch, false);
  vAddPrimitive("job.participant!"_Stz,     &vJobParticipantStore, false);
  vAddPrimitive("job.removeOriginator"_Stz, &vJobRemoveOriginator, false);
  vAddPrimitive("job.removeParticipant"_Stz,&vJobRemoveParticipant,false);
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Task::vJobAddOriginator(MP_Task mpTask)
{
  ENTRY("vJobAddOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.addOriginator"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  gpAttachMap->m_pOriginatingJobs->vAddJob(pJob);
  i8 iJobId = gpAttachMap->m_pOriginatingJobs->iNextJobId();
  pJob->m_iJobId = iJobId;
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addParticipant -- add job to participant job set                       */
/*----------------------------------------------------------------------------*/
void Task::vJobAddParticipant(MP_Task mpTask)
{
  ENTRY("vJobAddParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.addParticipant"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  gpAttachMap->m_pParticipatingJobs->vAddJob(pJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.acksRemaining! -- set acks remaining for this job                      */
/*----------------------------------------------------------------------------*/
void Task::vJobAcksRemaining(MP_Task mpTask)
{
  ENTRY("vJobAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iAcksRemaing;
  if ( ! mpTask->m_pDataStack->bPop(iAcksRemaing) )
  THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  
  pJob->m_ziAcksRemaining = iAcksRemaing;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClass(MP_Task mpTask)
{
  ENTRY("vJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.class"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Task::vJobDelete(MP_Task mpTask)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job.class"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  switch (pJob->eClass()) {
    
    case CN_JOB__BEGUN:
    {
      Job__Begun* pJobBegun = _pJob__Begun(pJobPtr);
      delete pJobBegun;
      break;
    }
    
    case CN_JOB__END:
    {
      Job__End* pJobEnd = _pJob__End(pJobPtr);
      delete pJobEnd;
      break;
    }
    
    case CN_JOB__ENDED:
    {
      Job__Ended* pJobEnded = _pJob__Ended(pJobPtr);
      delete pJobEnded;
      break;
    }
    
    case CN_JOB__ERROR:
    {
      Job__Error* pJobError = _pJob__Error(pJobPtr);
      delete pJobError;
      break;
    }
    
    case CN_JOB__EXECUTE:
    {
      Job__Execute* pJobExecute = _pJob__Execute(pJobPtr);
      delete pJobExecute;
      break;
    }
    
    case CN_JOB__EXECUTED:
    {
      Job__Executed* pJobExecuted = _pJob__Executed(pJobPtr);
      delete pJobExecuted;
      break;
    }
    
    case CN_JOB_G35:
    {
      Job_G35* pJobG35 = _pJob_G35(pJobPtr);
      delete pJobG35;
      break;
    }
    
    case CN_JOB_CREATESCHEMA:
    {
      Job_CreateSchema* pJobCreateSchema = _pJob_CreateSchema(pJobPtr);
      delete pJobCreateSchema;
      break;
    }
      
    default:
      ASSERT(0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.decAcksRemaining -- decrement acks remaining for this job              */
/*----------------------------------------------------------------------------*/
void Task::vJobDecAcksRemaining(MP_Task mpTask)
{
  ENTRY("vJobDecAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.decAcksRemaining"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  --pJob->m_ziAcksRemaining;
  
  mpTask->m_pDataStack->vPushInt(*pJob->m_ziAcksRemaining);
  
  EXIT();
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
  Job__Error* pJob__Error = _pJob__Error(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob__Error->eErrorCode());
  
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
  Job__Error* pJob__Error = _pJob__Error(pJobPtr);
  
  i8 iJobError;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobError) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  
  pJob__Error->vErrorCode((eEC)iJobError);
  
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

/*----------------------------------------------------------------------------*/
/* job.findOriginator -- find job from originator job set                     */
/*----------------------------------------------------------------------------*/
void Task::vJobFindOriginator(MP_Task mpTask)
{
  ENTRY("vJobFindOriginatort"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job.findOriginator"_Stz);
  
  Job__Begun* pFindJob = new Job__Begun;
  pFindJob->m_iJobId = iJobId;
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pOriginatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  delete pFindJob;
  ASSERT(pJob);
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.findPartcipant -- find job from participant job set                    */
/*----------------------------------------------------------------------------*/
void Task::vJobFindParticipant(MP_Task mpTask)
{
  ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job.findPartcipant"_Stz);
  
  Job__Begun* pFindJob = new Job__Begun;
  pFindJob->m_iJobId = iJobId;
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  delete pFindJob;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Task::vJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.objectNamed@ -- fetch the object name of the job                       */
/*----------------------------------------------------------------------------*/
void Task::vJobObjectNameFetch(MP_Task mpTask)
{
  ENTRY("vJobObjectNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.objectNamed@"_Stz);
  Job_CreateSchema* pJob = _pJob_CreateSchema(pJobPtr);
  
  mpTask->m_pDataStack->vPushS32(pJob->m_sObjectName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newBegun -- new a job__begun for this job                              */
/*----------------------------------------------------------------------------*/
void Task::vJobNewBegun(MP_Task mpTask)
{
  ENTRY("vJobNewBegun"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newBegun"_Stz);
  
  Job* pJobBegun = new Job__Begun;
  pJobBegun->m_iJobId = iJobId;
  pJobBegun->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnd - new a job__end for this job                                   */
/*----------------------------------------------------------------------------*/
void Task::vJobNewEnd(MP_Task mpTask)
{
  ENTRY("vJobNewEnd"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newEnd"_Stz);
  
  Job* pJobEnd = new Job__End;
  pJobEnd->m_iJobId = iJobId;
  pJobEnd->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnded -- new a job__ended for this job                              */
/*----------------------------------------------------------------------------*/
void Task::vJobNewEnded(MP_Task mpTask)
{
  ENTRY("vJobNewEnded"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newEnded"_Stz);
  
  Job* pJobEnded = new Job__Ended;
  pJobEnded->m_iJobId = iJobId;
  pJobEnded->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecute -- new a job__execute for this job                          */
/*----------------------------------------------------------------------------*/
void Task::vJobNewExecute(MP_Task mpTask)
{
  ENTRY("vJobNewExecute"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newExecute"_Stz);
  
  Job* pJobExecute = new Job__Execute;
  pJobExecute->m_iJobId = iJobId;
  pJobExecute->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecuted -- new a job__executed for this job                        */
/*----------------------------------------------------------------------------*/
void Task::vJobNewExecuted(MP_Task mpTask)
{
  ENTRY("vJobNewExecuted"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newExecuted"_Stz);
  
  Job* pJobExecuted = new Job__Executed;
  pJobExecuted->m_iJobId = iJobId;
  pJobExecuted->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobExecuted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::originator@ -- fetch the originator node from the job                  */
/*----------------------------------------------------------------------------*/
void Task::vJobOriginatorFetch(MP_Task mpTask)
{
  ENTRY("vJobOriginatorFetch"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::originator@@"_Stz);
  
  mpTask->m_pDataStack->vPushInt(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Task::vJobParticipantFetch(MP_Task mpTask)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partcipant@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Task::vJobParticipantStore(MP_Task mpTask)
{
  ENTRY("vJobParticipantStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partcipant!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iParticipantNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iParticipantNode) )
    THROW_INVALID_DSTACK("job.partcipant!"_Stz);
  
  pJob->m_iParticipantNode = iParticipantNode;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeOriginator -- remove job from originator job set                 */
/*----------------------------------------------------------------------------*/
void Task::vJobRemoveOriginator(MP_Task mpTask)
{
  ENTRY("vJobRemoveOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  gpAttachMap->m_pOriginatingJobs->vRemoveJob(pJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removePartcipant -- remove job from participant job set                */
/*----------------------------------------------------------------------------*/
void Task::vJobRemoveParticipant(MP_Task mpTask)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  gpAttachMap->m_pParticipatingJobs->vRemoveJob(pJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_JobTruncateTable.cpp - job_truncateTable step group                  */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobTruncateTable(void)
{
  vAddStep("job_subTruncate.truncateJobId@"_Stz,&Task::vJob_SubTruncateTruncateJobIdFetch, false);
  vAddStep("job_subTruncate.slot@"_Stz,         &Task::vJob_SubTruncateSlotFetch,          false);
  
  vAddStep("Job_truncateTable.spawnSub"_Stz, &Task::vJob_TruncateTableSpawnSub,  false);
  vAddStep("Job_truncateTable.task@"_Stz,    &Task::vJob_TruncateTableTaskFetch, false);
  vAddStep("Job_truncateTable.task!"_Stz,    &Task::vJob_TruncateTableTaskStore, false);
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.slot@ -- fetch the job slot                                */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubTruncateSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubTruncateSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subTruncate.slot@"_Stz);
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.truncateJobId@ -- fetch the truncate job id                */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubTruncateTruncateJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubTruncateTruncateJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subTruncate.truncateJobId@"_Stz);
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iTruncateJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.spawnSub -- spawn sub truncate job                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_TruncateTableSpawnSub(MP_Task mpTask)
{
  ENTRY("vJob_TruncateTableSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.spawnSub"_Stz);
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_truncateTable.spawnSub"_Stz);
  
  Session* pSession = pJob_TruncateTable->m_pSession;
  i8 iTruncateJobId = pJob_TruncateTable->m_iJobId;
  
  JobClass* pJobClass_SubTruncate = JobClass::pFind(CN_JOB_SUBTRUNCATE);
  
  Job_SubTruncate* pJob_SubTruncate =
  static_cast<Job_SubTruncate*>(pJobClass_SubTruncate->pPreJob(nullptr));
  
  pJob_SubTruncate->vSeed(SS_SEED_RUN);
  
  pJob_SubTruncate->m_iTruncateJobId = iTruncateJobId;
  pJob_SubTruncate->m_sObjectName = pJob_TruncateTable->m_sObjectName;
  pJob_SubTruncate->m_iSlot = iSlot;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  RapidsSE_bSpawnJob(pJob_SubTruncate,SS_SEED_RUN,nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.task@ -- fetch task from job_truncateTable               */
/*----------------------------------------------------------------------------*/
void Task::vJob_TruncateTableTaskFetch(MP_Task mpTask)
{
  ENTRY("vJob_TruncateTableTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.task@"_Stz);
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  Task* pTask = pJob_TruncateTable->m_pTask;
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.task! -- store task in job_truncateTable                 */
/*----------------------------------------------------------------------------*/
void Task::vJob_TruncateTableTaskStore(MP_Task mpTask)
{
  ENTRY("vJob_TruncateTableTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.task!"_Stz);
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.task!"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_TruncateTable->m_pTask = pTask;
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

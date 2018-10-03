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
void Step::vAddSteps_JobLoadTable(void)
{
  vAddStep("job_loadTable.addInvalidRows"_Stz,&Task::vJob_LoadTableAddInvalidRows,false);
  vAddStep("job_loadTable.addValidRows"_Stz,  &Task::vJob_LoadTableAddValidRows,  false);
  vAddStep("job_loadTable.filePath@"_Stz,     &Task::vJob_LoadTableFilePathFetch, false);
  vAddStep("job_loadTable.task@"_Stz,         &Task::vJob_LoadTableTaskFetch,     false);
  vAddStep("job_loadTable.task!"_Stz,         &Task::vJob_LoadTableTaskStore,     false);
  
  vAddStep("Job_subLoader.jobLoadTable@"_Stz, &Task::vJob_SubLoaderJobLoadTableFetch, false);
}

/*----------------------------------------------------------------------------*/
/* job_loadTable.addInvalidRows -- add invalid rows to load table's invalid   */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableAddInvalidRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableAddInvalidRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_loadTable.addInvalidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  i8 iInvalidRows;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iInvalidRows) )
    THROW_INVALID_DSTACK("job_loadTable.addInvalidRows"_Stz);
  
  pJob_LoadTable->m_ziInvalidRows.iAdd(iInvalidRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_loadTable.addValidRows -- add invalid rows to load table's valid rows  */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableAddValidRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableAddValidRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  i8 iValidRows;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iValidRows) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  
  pJob_LoadTable->m_ziValidRows.iAdd(iValidRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.filePath@ -- fetch filePath from job_loadTable              */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableFilePathFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableFilePathFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadTable.filePath@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  s256 sFilePath = pJob_LoadTable->m_sFileName;
  mpTask->m_pDataStack->vPushS256(sFilePath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.task@ -- fetch task from job_loadTable                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableTaskFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadTable.task@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  Task* pTask = pJob_LoadTable->pTask();
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.task! -- store task into job_loadTable                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableTaskStore(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);

  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table@"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_LoadTable->vTask(pTask);
  
  EXIT();
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

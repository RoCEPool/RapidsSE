/*============================================================================*/
/* Steps_JobLoadTable.cpp - job_loadTable step group                          */
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
  vAddStep("job_loadTable.addInvalidRows"_Stz,&Task::vJob_LoadTableAddInvalidRows);
  vAddStep("job_loadTable.addValidRows"_Stz,  &Task::vJob_LoadTableAddValidRows);
  vAddStep("job_loadTable.chunkFile"_Stz,     &Task::vJob_LoadTableChunkFile);
  vAddStep("job_loadTable.dumpMap"_Stz,       &Task::vJob_LoadTableDumpMap);
  vAddStep("job_loadTable.filePath@"_Stz,     &Task::vJob_LoadTableFilePathFetch);
  vAddStep("job_loadTable.table!"_Stz,        &Task::vJob_LoadTableTableStore);
  vAddStep("job_loadTable.task@"_Stz,         &Task::vJob_LoadTableTaskFetch);
  vAddStep("job_loadTable.task!"_Stz,         &Task::vJob_LoadTableTaskStore);
}

/*----------------------------------------------------------------------------*/
/* job_loadTable.addInvalidRows -- add invalid rows to load table's invalid   */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableAddInvalidRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableAddInvalidRows"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_loadTable.addInvalidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  i8 iInvalidRows;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iInvalidRows) )
    THROW_INVALID_DSTACK("job_loadTable.addInvalidRows"_Stz);
  
  pJob_LoadTable->m_iInvalidRows += iInvalidRows;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_loadTable.addValidRows -- add invalid rows to load table's valid rows  */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableAddValidRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableAddValidRows"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  i8 iValidRows;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iValidRows) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  
  pJob_LoadTable->m_iValidRows += iValidRows;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.chunkFile -- chunk the file                                      */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableChunkFile(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableChunkFile"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  bool bFoundChunk = false;
  
  do {
    bFoundChunk = pJob_LoadTable->bFindChunk();
  } while (bFoundChunk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.dumpMap -- dump the load map                                     */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableDumpMap(MP_Task mpTask)
{
  ENTRY("vJob_LoaderDUmpMap"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  pJob_LoadTable->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.filePath@ -- fetch filePath from job_loadTable               */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableFilePathFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableFilePathFetch"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_LoadTable.filePath@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  s256 sFilePath = pJob_LoadTable->m_sFileName;
  mpTask->m_pDataStack->vPushS256(sFilePath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.table! -- store table into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableTableStore(MP_Task mpTask)
{
  ENTRY("vJob_LoaderTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable,pPtrTable) )
    THROW_INVALID_DSTACK("loadTable.tableStore"_Stz);
  
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_LoadTable->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.task@ -- fetch task from job_loadTable                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableTaskFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableTaskFetch"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_LoadTable.task@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  Task* pTask = pJob_LoadTable->pThisTask();
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.task! -- store task into job_loadTable                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadTableTaskStore(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableTaskStore"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table@"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_LoadTable->vThisTask(pTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

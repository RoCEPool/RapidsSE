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
void Task::vAddSteps_JobLoadTable(void)
{
  vAddStep("job_loadBuffer.insertRows"_Stz,      &vJob_LoadBufferInsertRows,          false);
  vAddStep("job_loadBuffer.jobLoadTableId@"_Stz, &vJob_LoadBufferJobLoadTableIdFetch, false);
  vAddStep("job_loadBuffer.jobLoadTable!"_Stz,   &vJob_LoadBufferJobLoadTableStore,   false);
  vAddStep("job_loadBuffer.partition@"_Stz,      &vJob_LoadBufferPartitionFetch,      false);
  vAddStep("job_loadBuffer.table!"_Stz,          &vJob_LoadBufferTableStore,          false);
  
  vAddStep("job_loadTable.addInvalidRows"_Stz,&vJob_LoadTableAddInvalidRows,false);
  vAddStep("job_loadTable.addValidRows"_Stz,  &vJob_LoadTableAddValidRows,  false);
  vAddStep("job_loadTable.filePath@"_Stz,     &vJob_LoadTableFilePathFetch, false);
  vAddStep("job_loadTable.task@"_Stz,         &vJob_LoadTableTaskFetch,     false);
  vAddStep("job_loadTable.task!"_Stz,         &vJob_LoadTableTaskStore,     false);
  
  vAddStep("Job_subLoader.jobLoadTable@"_Stz, &vJob_SubLoaderJobLoadTableFetch, false);
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.insertRows -- insert rows from working buffer into table    */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.insertRows!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  // convert partition to node slot
  i8 iPartition = pJob_LoadBuffer->iPartition();
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);

  // get table, partition and working buffer
  Table* pTable = pJob_LoadBuffer->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  WorkingBuffer* pWorkingBuffer = pJob_LoadBuffer->m_pWorkingBuffer;
  
  i8 iTxId = 0;
  
  // insert the working buffer
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // we loaded one additional buffer from a subloader
  pPartition->vIncTotalBuffersLoaded();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  i8 nBuffersFromSubLoader = pJob_LoadBuffer->nBuffersFromSubLoader();
  if (C_UNKNOWN != nBuffersFromSubLoader)
    pPartition->vAddTotalBuffersLoadable(nBuffersFromSubLoader);
  
  // have all buffers been loaded?
  bool bAllBuffersLoaded = pPartition->bAllBuffersLoaded();
  //if (bAllBuffersLoaded)
  //  vTrace("Buffers loaded\n"_Stz);

  mpTask->m_pDataStack->vPushInt(bAllBuffersLoaded);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.jobLoadTableId@ -- fetch load table job id                  */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferJobLoadTableIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferLoadJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.loadJobId@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 iJobId = pJob_LoadBuffer->iJobLoadTableId();
  mpTask->m_pDataStack->vPushInt(iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.jobLoadTable! -- store job_loadTable                        */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferJobLoadTableStore(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferJobLoadTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.jobLoadTable!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.jobLoadTable!"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);

  pJob_LoadBuffer->vJobLoadTable(pJob_LoadTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.partition@ -- fetch partition no. from job_loadBuffer       */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.partition@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 i_Partition = pJob_LoadBuffer->iPartition();
  mpTask->m_pDataStack->vPushInt(i_Partition);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.table! -- store table into job_loadBuffer                   */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferTableStore(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_LoadBuffer->vTable(pTable);
  
  EXIT();
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

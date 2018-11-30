/*============================================================================*/
/* Steps_JobInsertRow.cpp - job step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobInsertRow(void)
{
  vAddStep("Job_InsertTable.insertRowJob@"_Stz, &Step::vJob_InsertTableInsertRowJobFetch);
  
  vAddStep("job_InsertRow.danglingBuffer"_Stz,   &Step::vJob_InsertRowDanglingBuffer);
  vAddStep("job_insertRow.partition!"_Stz,       &Step::vJob_InsertRowPartitionStore);
  vAddStep("job_insertRow.table!"_Stz,           &Step::vJob_InsertRowTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.InsertRowJob@ -- fetch insertRow jobid from job              */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableInsertRowJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertTableInsertRowJobFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_InsertTable.InsertRowJob@"_Stz);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob_InsertTable->m_pJobInsertRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.danglingBuffer--ensure there's a dangling buffer in partition */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowDanglingBuffer"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_InsertRow.danglingBuffer"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  if ( ! pJob_InsertRow->m_pWorkingBuffer[pJob_InsertRow->m_iPartition] )
    pJob_InsertRow->vNewWorkingBuffer(pJob_InsertRow->m_pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.partition! -- store partition into job_insertRow             */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowPartitionStore(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowPartitionStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertRow.partition!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("job_InsertRow.partition!"_Stz);
  
  pJob_InsertRow->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.table! -- store table into job_insertRow                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowTableStore(MP_Task mpTask)
{
  ENTRY("vJobInsertRowTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job_InsertRow.table!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
  THROW_INVALID_DSTACK("job_InsertRow.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InsertRow->m_pTable = pTable;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

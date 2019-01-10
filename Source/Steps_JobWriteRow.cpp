/*============================================================================*/
/* Steps_JobWriteRow.cpp - job step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobWriteRow(void)
{
  vAddStep("job_writeRow.parentJobId@"_Stz,  &Step::vJob_WriteRowParentJobIdFetch);
  vAddStep("job_writeRow.insertRow"_Stz,     &Step::vJob_WriteRowInsertRow);
  vAddStep("job_writeRow.hashRow"_Stz,       &Step::vJob_WriteRowHashRow);
  vAddStep("job_WriteRow.partition@"_Stz,    &Step::vJob_WriteRowPartitionFetch);
  vAddStep("job_WriteRow.table!"_Stz,        &Step::vJob_WriteRowTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.parentJobId@ -- fetch parent jobid from job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowParentJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_WriteRowParentJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.parentJobId@"_Stz);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  i8 iParentJobId = pJob_WriteRow->m_iParentJobId;
  RASSERT(C_UNKNOWN != iParentJobId);
  
  mpTask->m_pDataStack->vPushInt(iParentJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.hashRow -- hash row to a partition                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowHashRow(MP_Task mpTask)
{
  ENTRY("vJob_WriteRowHashRow"_Stz);
  
  Object* pJobWriteRowPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobWriteRowPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.hashRow"_Stz);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobWriteRowPtr);
  
  Object* pJobInsertRowPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobInsertRowPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.hashRow"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobInsertRowPtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first format the row
  bool bBlankLine;
  bool bResult = pJob_WriteRow->bFormatRow(pJob_InsertRow->m_aRawData,
                                           pJob_InsertRow->m_nRawDataBytes,
                                           bBlankLine);
  
  if ( ! bResult )
    mpTask->m_pDataStack->vPushInt(0);
  else {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hash the record to a partition
    pJob_WriteRow->m_iPartitionId = pJob_WriteRow->m_pTable->iRowPartition(pJob_WriteRow->m_aRecord);
    pJob_InsertRow->m_iPartitionId  = pJob_WriteRow->m_iPartitionId ;
    
    mpTask->m_pDataStack->vPushInt(1);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.insertRow -- insert row into the table                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowInsertRow(MP_Task mpTask)
{
  ENTRY("vJob_WriteRowInsertRow"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.insertRow!"_Stz);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  pJob_WriteRow->vInsertRow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.partition@ -- fetch partition no. from job_WriteRow           */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_WriteRowPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.partition@"_Stz);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  i8 i_Partition = pJob_WriteRow->m_iPartitionId;
  mpTask->m_pDataStack->vPushInt(i_Partition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.table! -- store table into job_WriteRow                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowTableStore(MP_Task mpTask)
{
  ENTRY("vJob_WriteRowTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.table!"_Stz);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_WriteRow.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_WriteRow->m_pTable = pTable;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_JobWriteRow.cpp - job step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobWriteRow(void)
{
  vAddStep("job_writeRow.parentJobId@"_Stz,  &Step::vJob_WriteRowParentJobIdFetch);
  vAddStep("job_writeRow.insertRow"_Stz,     &Step::vJob_WriteRowInsertRow);
  vAddStep("job_writeRow.encodeRow"_Stz,     &Step::vJob_WriteRowEncodeRow);
  vAddStep("job_WriteRow.partition@"_Stz,    &Step::vJob_WriteRowPartitionFetch);
  vAddStep("job_WriteRow.table!"_Stz,        &Step::vJob_WriteRowTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.parentJobId@ -- fetch parent jobid from job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowParentJobIdFetch(void)
{
  ENTRY(); // ENTRY("vJob_WriteRowParentJobIdFetch"_Stz);
  
  Object* pJobPtr = m_pDataStack->pPopPtr(__func__); Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  i8 iParentJobId = pJob_WriteRow->m_iParentJobId;
  RASSERT(C_UNKNOWN != iParentJobId);
  
  m_pDataStack->vPushInt(iParentJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_writeRow.encodeRow -- hash row to a partition                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowEncodeRow(void)
{
  ENTRY(); // ENTRY("vJob_WriteRowEncodeRow"_Stz);
  
  Object* pJobWriteRowPtr = m_pDataStack->pPopPtr(__func__); Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobWriteRowPtr);
  
  Object* pJobInsertRowPtr = m_pDataStack->pPopPtr(__func__); Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobInsertRowPtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first encode the row
  bool bBlankLine;
  //vTrace("sRecord %s\n"_Stz, pJob_InsertRow->m_pRecord->szRecStr());
  bool bResult = pJob_WriteRow->bExtractRow(pJob_InsertRow->m_pRecord->szRecStr(),
                                            bBlankLine);
  
  if ( ! bResult )
    m_pDataStack->vPushInt(0);
  else {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // use row hash to get partition
    i8 iHash = pJob_WriteRow->m_pRow->m_iHash;
    Table* pTable = pJob_WriteRow->m_pTable;
    pJob_WriteRow->m_iPartitionId = pTable->iToPartition(iHash);
    pJob_InsertRow->m_iPartitionId = pJob_WriteRow->m_iPartitionId ;

    // now delete the rowCompactors and the row from writeRow job before
    // forwarding to another node
    delete pJob_WriteRow->m_pRow;
    pJob_WriteRow->m_pRow = nullptr;
    
    for (i8 i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++) {
      delete pJob_WriteRow->m_paRowCompactor[i_Ptn];
      pJob_WriteRow->m_paRowCompactor[i_Ptn] = nullptr;
    }
    
    m_pDataStack->vPushInt(1);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.insertRow -- insert row into the table                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowInsertRow(void)
{
  ENTRY(); // ENTRY("vJob_WriteRowInsertRow"_Stz);
  
  Object* pJobPtr = m_pDataStack->pPopPtr(__func__); Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  pJob_WriteRow->vInsertRow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.partition@ -- fetch partition no. from job_WriteRow           */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowPartitionFetch(void)
{
  ENTRY(); // ENTRY("vJob_WriteRowPartitionFetch"_Stz);
  
  Object* pJobPtr = m_pDataStack->pPopPtr(__func__); Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  i8 i_Partition = pJob_WriteRow->m_iPartitionId;
  m_pDataStack->vPushInt(i_Partition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.table! -- store table into job_WriteRow                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowTableStore(void)
{
  ENTRY(); // ENTRY("vJob_WriteRowTableStore"_Stz);
  
  Object* pJobPtr = m_pDataStack->pPopPtr(__func__); Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  Object* pTablePtr = m_pDataStack->pPopPtr(__func__); Table* pTable = _pTable(pTablePtr);
  
  pJob_WriteRow->m_pTable = pTable;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

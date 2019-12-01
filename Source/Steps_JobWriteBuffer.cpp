/*============================================================================*/
/* Steps_JobWriteBuffer.cpp - job step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobWriteBuffer(void)
{
  vAddStep("job_WriteBuffer.parentJobId@"_Stz,  &Step::vJob_WriteBufferParentJobIdFetch);
  vAddStep("job_WriteBuffer.insertRows"_Stz,    &Step::vJob_WriteBufferInsertRows);
  vAddStep("job_WriteBuffer.isIngesting"_Stz,   &Step::vJob_WriteBufferIsIngesting);
  vAddStep("job_WriteBuffer.isInserting"_Stz,   &Step::vJob_WriteBufferIsInserting);
  vAddStep("job_WriteBuffer.isLoading"_Stz,     &Step::vJob_WriteBufferIsLoading);
  vAddStep("job_WriteBuffer.partition@"_Stz,    &Step::vJob_WriteBufferPartitionFetch);
  vAddStep("job_WriteBuffer.table!"_Stz,        &Step::vJob_WriteBufferTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.parentJobId@ -- fetch parent jobid from job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferParentJobIdFetch(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferParentJobIdFetch"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  i8 iParentJobId = pJob_WriteBuffer->m_iParentJobId;
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  RASSERT(DS_INGEST== eDataSource || C_UNKNOWN != iParentJobId);
  m_pDataStack->vPushInt(iParentJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.insertRows -- insert rows from working buffer into table   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferInsertRows(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferInsertRows"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  bool bAllBuffersWritten = pJob_WriteBuffer->bInsertRows();
  
  m_pDataStack->vPushInt(bAllBuffersWritten);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.isIngesting -- is writebuffer being ingesting              */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferIsIngesting(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferIsIngesting"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  m_pDataStack->vPushInt(eDataSource == DS_INGEST ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.isInserting -- is writebuffer being inserted               */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferIsInserting(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferIsInserting"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  m_pDataStack->vPushInt(eDataSource == DS_INSERT ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.isLoading -- is writebuffer being loaded                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferIsLoading(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferIsLoading"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  m_pDataStack->vPushInt(eDataSource == DS_LOAD ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.partition@ -- fetch partition no. from job_writeBuffer     */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferPartitionFetch(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferPartitionFetch"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  i8 i_Partition = pJob_WriteBuffer->m_iPartitionId;
  m_pDataStack->vPushInt(i_Partition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.table! -- store table into job_writeBuffer                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferTableStore(void)
{
  ENTRY(); // ENTRY("vJob_WriteBufferTableStore"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  Object* pTablePtr; m_pDataStack->vPopPtr(pTablePtr,__func__);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_WriteBuffer->m_pTable = pTable;
  
  if (pJob_WriteBuffer->m_pOperand) {
    ASSERT(pJob_WriteBuffer->m_pOperand->eClass() == CN_CRWBUFFER);
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pJob_WriteBuffer->m_pOperand);
    pCrwBuffer->vRestoreTable(pTable);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

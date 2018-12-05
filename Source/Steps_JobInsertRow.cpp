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
  
  vAddStep("job_insertRow.table!"_Stz,           &Step::vJob_InsertRowTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.InsertRowJob@ -- fetch insertRow jobid from job            */
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

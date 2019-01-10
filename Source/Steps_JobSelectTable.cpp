/*============================================================================*/
/* Steps_JobSelectTable.cpp - jobSelectTable step group                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSelectTable(void)
{
  vAddStep("Job_selectTable.partition@"_Stz,   &Step::vJob_SelectTablePartitionFetch);
  vAddStep("Job_selectTable.table!"_Stz,       &Step::vJob_SelectTableTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_selectTable.partition@ -- fetch partition no from the job              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectTablePartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.partition@"_Stz);
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);

  mpTask->m_pDataStack->vPushI8(pJob_SelectTable->m_iPartition);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Job_selectTable.table! -- store table in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectTableTableStore(MP_Task mpTask)
{
  ENTRY("vJob_SelectTableTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_selectTable.table!"_Stz);
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_selectTable.table"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_SelectTable->m_pTable = pTable;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

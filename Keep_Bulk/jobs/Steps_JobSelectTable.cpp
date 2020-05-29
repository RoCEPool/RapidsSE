/*============================================================================*/
/* Steps_JobSelectTable.cpp - jobSelectTable step group                       */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_SelectTablePartitionFetch(void)
{
  ENTRY("vJob_FetchRowsPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);

  m_pDataStack->vPushI8(pJob_SelectTable->m_iPartition);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Job_selectTable.table! -- store table in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectTableTableStore(void)
{
  ENTRY("vJob_SelectTableTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);
  
  Object* pTablePtr;
  if ( ! m_pDataStack->bPopObject(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  pJob_SelectTable->m_pTable = pTable;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

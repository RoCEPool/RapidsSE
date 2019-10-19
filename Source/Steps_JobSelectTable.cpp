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
void Step::vJob_SelectTablePartitionFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_FetchRowsPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);

  mpTask->m_pForth->m_pDataStack->vPushI8(pJob_SelectTable->m_iPartition);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Job_selectTable.table! -- store table in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectTableTableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SelectTableTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pTablePtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_SelectTable->m_pTable = pTable;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

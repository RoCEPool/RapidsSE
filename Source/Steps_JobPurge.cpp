/*============================================================================*/
/* Steps_JobPurge.cpp - job_purge step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPurge(void)
{
  vAddStep("job_purge.purge"_Stz,    &Step::vJob_PurgePurge);
  vAddStep("job_purge.table!"_Stz,   &Step::vJob_PurgeTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_purge.purge -- purge table                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgePurge(MP_Task mpTask)
{
  ENTRY("vJob_PurgePurge"_Stz);
  
  Object* pPtrJobPurge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobPurge) )
    THROW_INVALID_DSTACK("job_purge.purge"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJobPurge);
  
  pJob_Purge->vPurge();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.table! -- store table into job_purge                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeTableStore(MP_Task mpTask)
{
  ENTRY("vJob_PurgeTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Purge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Purge,pPtrTable) )
    THROW_INVALID_DSTACK("job_purge.tableStore"_Stz);
  
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJob_Purge);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Purge->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_JobLoadTable.cpp - job_loadTable step group                          */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobLoadTable(void)
{
  vAddStep("job_loadTable.chunkFile"_Stz,     &Step::vJob_LoadTableChunkFile);
  vAddStep("job_loadTable.dumpMap"_Stz,       &Step::vJob_LoadTableDumpMap);
  vAddStep("job_loadTable.filePath@"_Stz,     &Step::vJob_LoadTableFilePathFetch);
  vAddStep("job_loadTable.table!"_Stz,        &Step::vJob_LoadTableTableStore);
}

/*----------------------------------------------------------------------------*/
/* loadTable.chunkFile -- chunk the file                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableChunkFile(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableChunkFile"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  bool bFoundChunk = false;
  
  do {
    bFoundChunk = pJob_LoadTable->bFindChunk();
  } while (bFoundChunk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.dumpMap -- dump the load map                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableDumpMap(MP_Task mpTask)
{
  ENTRY("vJob_LoaderDUmpMap"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  pJob_LoadTable->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.filePath@ -- fetch filePath from job_loadTable               */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableFilePathFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadTableFilePathFetch"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_LoadTable.filePath@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  s256 sFilePath = pJob_LoadTable->m_sFileName;
  mpTask->m_pDataStack->vPushS256(sFilePath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.table! -- store table into loader                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableTableStore(MP_Task mpTask)
{
  ENTRY("vJob_LoaderTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable,pPtrTable) )
    THROW_INVALID_DSTACK("loadTable.tableStore"_Stz);
  
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_LoadTable->vTable(pTable);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

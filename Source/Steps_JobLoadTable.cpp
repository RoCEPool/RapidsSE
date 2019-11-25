/*============================================================================*/
/* Steps_JobLoadTable.cpp - job_loadTable step group                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobLoadTable(void)
{
  vAddStep("job_loadTable.begin"_Stz,         &Step::vJob_LoadTableBegin);
  vAddStep("job_loadTable.chunk"_Stz,         &Step::vJob_LoadTableChunk);
  vAddStep("job_loadTable.dumpMap"_Stz,       &Step::vJob_LoadTableDumpMap);
  vAddStep("job_loadTable.end"_Stz,           &Step::vJob_LoadTableEnd);
  vAddStep("job_loadTable.filePath@"_Stz,     &Step::vJob_LoadTableFilePathFetch);
  vAddStep("job_loadTable.table!"_Stz,        &Step::vJob_LoadTableTableStore);
}

/*----------------------------------------------------------------------------*/
/* loadTable.begin -- begin load of file                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableBegin(void)
{
  ENTRY(); // ENTRY("vJob_LoadTableBegin"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopPtr(pPtrJob_LoadTable) )
    THROW_DSTACK();
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  bool bBegun = pJob_LoadTable->bBegin();
  
  m_pDataStack->vPushInt(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.chunk -- chunk the file                                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableChunk(void)
{
  ENTRY(); // ENTRY("vJob_LoadTableChunk"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopPtr(pPtrJob_LoadTable) )
    THROW_DSTACK();
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  bool bCallAgain = pJob_LoadTable->bChunk();
  
  m_pDataStack->vPushInt(bCallAgain);
  
 EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.dumpMap -- dump the load map                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableDumpMap(void)
{
  ENTRY(); // ENTRY("vJob_LoaderDUmpMap"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopPtr(pPtrJob_LoadTable) )
    THROW_DSTACK();
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  pJob_LoadTable->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.end -- end load of file                                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableEnd(void)
{
  ENTRY(); // ENTRY("vJob_LoadTableEnd"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopPtr(pPtrJob_LoadTable) )
    THROW_DSTACK();
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  pJob_LoadTable->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.filePath@ -- fetch filePath from job_loadTable               */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableFilePathFetch(void)
{
  ENTRY(); // ENTRY("vJob_LoadTableFilePathFetch"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopPtr(pPtrJob_LoadTable) )
    THROW_DSTACK();
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  s256 sFilePath = pJob_LoadTable->szFilePath();
  m_pDataStack->vPushS256(sFilePath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.table! -- store table into loader                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadTableTableStore(void)
{
  ENTRY(); // ENTRY("vJob_LoaderTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopPtr(pPtrJob_LoadTable,pPtrTable) )
    THROW_DSTACK();
  
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_LoadTable->vTable(pTable);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

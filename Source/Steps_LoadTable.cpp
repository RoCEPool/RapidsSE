/*============================================================================*/
/* Steps_LoadTable.cpp - load table step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*  gpAttachMap;

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Loader(void)
{
  vAddStep("loadTable.chunkFile"_Stz,      &Task::vLoadTableChunkFile,      false);
  vAddStep("loadTable.delete"_Stz,         &Task::vLoadTableDelete,         false);
  vAddStep("loadTable.dumpMap"_Stz,        &Task::vLoadTableDumpMap,        false);
  vAddStep("loadTable::new"_Stz,           &Task::vLoadTableNew,            false);
  vAddStep("loadTable.table!"_Stz,         &Task::vLoadTableTableStore,     false);
}

/*----------------------------------------------------------------------------*/
/* loadTable.chunkFile -- chunk the file                                      */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableChunkFile(MP_Task mpTask)
{
  ENTRY("vLoaderChunkFile"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loadTable.chunkFile"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  bool bFoundChunk = false;
  
  do {
    bFoundChunk = pLoadTable->bFindChunk();
  } while (bFoundChunk);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableDelete(MP_Task mpTask)
{
  ENTRY("vLoaderDelete"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loadTable.delete"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  delete pLoadTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.dumpMap -- dump the load map                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableDumpMap(MP_Task mpTask)
{
  ENTRY("vLoaderDUmpMap"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loadTable.dumpMap"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  pLoadTable->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable::new -- new a table loader                                       */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableNew(MP_Task mpTask)
{
  ENTRY("vLoaderNew"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("loadTable::new"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  LoadTable* pLoadTable = new LoadTable(pJob_LoadTable);
  pJob_LoadTable->m_pLoadTable = pLoadTable;

  mpTask->m_pDataStack->vPushObject(pLoadTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loadTable.table! -- store table into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableTableStore(MP_Task mpTask)
{
  ENTRY("vLoaderTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable,pPtrTable) )
    THROW_INVALID_DSTACK("loadTable.tableStore"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoadTable->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  vAddStep("loader.chunkFile"_Stz,      &Task::vLoadTableChunkFile,      false);
  vAddStep("loader.delete"_Stz,         &Task::vLoadTableDelete,         false);
  vAddStep("loader.dumpMap"_Stz,        &Task::vLoadTableDumpMap,        false);
  vAddStep("loader::new"_Stz,           &Task::vLoadTableNew,            false);
  vAddStep("loader.spawnSubLoader"_Stz, &Task::vLoadTableSpawnSubLoader, false);
  vAddStep("loader.table!"_Stz,         &Task::vLoadTableTableStore,     false);
}

/*----------------------------------------------------------------------------*/
/* loader.chunkFile -- chunk the file                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableChunkFile(MP_Task mpTask)
{
  ENTRY("vLoaderChunkFile"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loader.chunkFile"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  bool bFoundChunk = false;
  
  do {
    bFoundChunk = pLoadTable->bFindChunk();
  } while (bFoundChunk);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableDelete(MP_Task mpTask)
{
  ENTRY("vLoaderDelete"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loader.delete"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  delete pLoadTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.dumpMap -- dump the load map                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableDumpMap(MP_Task mpTask)
{
  ENTRY("vLoaderDUmpMap"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loader.dumpMap"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  pLoadTable->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader::new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableNew(MP_Task mpTask)
{
  ENTRY("vLoaderNew"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("loader::new"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);

  LoadTable* pLoadTable = new LoadTable(pJob_LoadTable);
  pJob_LoadTable->m_pLoadTable = pLoadTable;

  mpTask->m_pDataStack->vPushObject(pLoadTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.spawnSubLoader -- spawn a new subloader                             */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableSpawnSubLoader(MP_Task mpTask)
{
  ENTRY("vLoaderSpawnSubLoader"_Stz);
  
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable) )
    THROW_INVALID_DSTACK("loader.spawnSubLoader"_Stz);
  LoadTable* pLoadTable = _pLoader(pPtrLoadTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job_LoadTable* pJob_LoadTable = pLoadTable->pJob_LoadTable();
  SEitf* pSEitf = pJob_LoadTable->m_pSEitf;
    
  //! this needs to be optimised
  JobClass* pJobClass_SubLoader = JobClass::pFind(CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader =
    static_cast<Job_SubLoader*>(pJobClass_SubLoader->pPreJob(pSEitf));
  
  pJob_SubLoader->vSeed(SS_SEED_RUN);

  pJob_SubLoader->m_sObjectName = pJob_LoadTable->m_sObjectName;
  pJob_SubLoader->vJob_LoadTable(pJob_LoadTable);
  
  SubLoader* pSubLoader = new SubLoader(pJob_SubLoader,pLoadTable);
  pJob_SubLoader->vLoaderSub(pSubLoader);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubLoader,SS_SEED_RUN,pSEitf);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.table! -- store table into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoadTableTableStore(MP_Task mpTask)
{
  ENTRY("vLoaderTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrLoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoadTable,pPtrTable) )
    THROW_INVALID_DSTACK("loader.tableStore"_Stz);
  
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

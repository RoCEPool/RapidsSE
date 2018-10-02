/*============================================================================*/
/* Steps_Loader.cpp - loader step group                                       */
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
  vAddStep("loader.chunkFile"_Stz,      &Task::vLoaderChunkFile,      false);
  vAddStep("loader.delete"_Stz,         &Task::vLoaderDelete,         false);
  vAddStep("loader.dumpMap"_Stz,        &Task::vLoaderDumpMap,        false);
  vAddStep("loader::new"_Stz,           &Task::vLoaderNew,            false);
  vAddStep("loader.spawnSubLoader"_Stz, &Task::vLoaderSpawnSubLoader, false);
  vAddStep("loader.table!"_Stz,         &Task::vLoaderTableStore,     false);
}

/*----------------------------------------------------------------------------*/
/* loader.chunkFile -- chunk the file                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderChunkFile(MP_Task mpTask)
{
  ENTRY("vLoaderChunkFile"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.chunkFile"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoader);
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
void Task::vLoaderDelete(MP_Task mpTask)
{
  ENTRY("vLoaderDelete"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.delete"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoader);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  delete pLoadTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.dumpMap -- dump the load map                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDumpMap(MP_Task mpTask)
{
  ENTRY("vLoaderDUmpMap"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.dumpMap"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoader);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  pLoadTable->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader::new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(MP_Task mpTask)
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
void Task::vLoaderSpawnSubLoader(MP_Task mpTask)
{
  ENTRY("vLoaderSpawnSubLoader"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.spawnSubLoader"_Stz);
  LoadTable* pLoadTable = _pLoader(pPtrLoader);
  
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
void Task::vLoaderTableStore(MP_Task mpTask)
{
  ENTRY("vLoaderTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader,pPtrTable) )
    THROW_INVALID_DSTACK("loader.tableStore"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoader);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoadTable->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

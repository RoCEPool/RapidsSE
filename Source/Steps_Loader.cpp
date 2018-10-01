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
void Task::vAddSteps_Loader(void)
{
  vAddStep("loader.addRecordsInvalid"_Stz, &Task::vLoaderAddRecordsInvalid,false);
  vAddStep("loader.addRecordsValid"_Stz,   &Task::vLoaderAddRecordsValid, false);
  vAddStep("loader.chunkFile"_Stz,         &Task::vLoaderChunkFile,       false);
  vAddStep("loader.delete"_Stz,            &Task::vLoaderDelete,          false);
  vAddStep("loader.dumpMap"_Stz,           &Task::vLoaderDumpMap,         false);
  vAddStep("loader.findChunk"_Stz,         &Task::vLoaderFindChunk,       false);
  vAddStep("loader.init"_Stz,              &Task::vLoaderInit,            false);
  vAddStep("loader::new"_Stz,              &Task::vLoaderNew,             false);
  vAddStep("loader.newSub"_Stz,            &Task::vLoaderNewSub,          false);
  vAddStep("loader.path!"_Stz,             &Task::vLoaderPathStore,       false);
  vAddStep("loader.pathDistributed!"_Stz,  &Task::vLoaderPathDistributedStore,false);
  vAddStep("loader.recordsInvalid@"_Stz,   &Task::vLoaderRecordsInvalidFetch, false);
  vAddStep("loader.recordsValid@"_Stz,     &Task::vLoaderRecordsValidFetch,   false);
  vAddStep("loader.seitf@"_Stz,            &Task::vLoaderSEitfFetch,      false);
  vAddStep("loader.seitf!"_Stz,            &Task::vLoaderSEitfStore,      false);
  vAddStep("loader.spawnSubLoader"_Stz,    &Task::vLoaderSpawnSubLoader,  false);
  vAddStep("loader.table!"_Stz,            &Task::vLoaderTableStore,      false);
}

/*----------------------------------------------------------------------------*/
/* loader.addRecordsInvalid -- add recordsInvalid                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderAddRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vLoaderAddRecordsInvalid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.addRecordsInvalid"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  i8 iInvalidRecords;
  mpTask->m_pDataStack->bPopAsInt(iInvalidRecords);
  
  pLoader->vAddInvalidRecords(iInvalidRecords);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.addRecordsValid -- add recordsValid                                 */
/*----------------------------------------------------------------------------*/
void Task::vLoaderAddRecordsValid(MP_Task mpTask)
{
  ENTRY("vLoaderAddRecordsInvalid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.addRecordsInvalid"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  i8 iValidRecords;
  mpTask->m_pDataStack->bPopAsInt(iValidRecords);
  
  pLoader->vAddValidRecords(iValidRecords);
  
  EXIT();
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
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFoundChunk = false;
  
  do {
    bFoundChunk = pLoader->bFindChunk();
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
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
  
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
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vDumpMap();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.findChunk -- find next chunk                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFindChunk(MP_Task mpTask)
{
  ENTRY("vLoaderFindChunk"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.findChunk"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFoundChunk  = pLoader->bFindChunk();
  
  mpTask->m_pDataStack->vPushInt(bFoundChunk);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(MP_Task mpTask)
{
  ENTRY("vLoaderInit"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.init"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader(nullptr);
  
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

  Loader* pLoader = new Loader(pJob_LoadTable);
  pJob_LoadTable->m_pLoader = pLoader;

  mpTask->m_pDataStack->vPushObject(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.newSub -- new a loader sub                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNewSub(MP_Task mpTask)
{
  ENTRY("vLoaderNewSub"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.newSub"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  LoaderSub* pLoaderSub = new LoaderSub(nullptr,pLoader);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vLoader(pLoader);
  
  mpTask->m_pDataStack->vPushObject(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.path! -- store file path into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathStore(MP_Task mpTask)
{
  ENTRY("vLoaderPathStore"_Stz);
  
  ValWrk xWspPath;
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader,xWspPath) )
    THROW_INVALID_DSTACK("loader.path!"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = _pStrVal(xWspPath.pNestedValueSelf());
  
  pLoader->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.pathDistributed! -- store file path into loader                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathDistributedStore(MP_Task mpTask)
{
  ENTRY("vLoaderPathStore"_Stz);
  
  ValWrk xWspPath;
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader,xWspPath) )
    THROW_INVALID_DSTACK("loader.pathDistributed!"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = _pStrVal(xWspPath.pNestedValueSelf());
  
  s256 sNewPath;
  sprintf(sNewPath.aChar(), "%s_%lld", sPath.aChar(),gpAttachMap->m_pGrid->iThisNode());
  pLoader->vPath(sNewPath);
  vTrace("LOADER opening file %s\n"_Stz, sNewPath.aChar());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.recordsInvalid@ -- fetch recordsInvalid                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderRecordsInvalidFetch(MP_Task mpTask)
{
  ENTRY("vLoaderRecordsInvalidFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.recordsInvalid@"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  mpTask->m_pDataStack->vPushInt(pLoader->iInvalidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.recordsValid@ -- fetch recordsValid                                 */
/*----------------------------------------------------------------------------*/
void Task::vLoaderRecordsValidFetch(MP_Task mpTask)
{
  ENTRY("vLoaderRecordsValidFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.recordsValid@"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  mpTask->m_pDataStack->vPushInt(pLoader->iValidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.seitf@ -- fetch seitf from loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSEitfFetch(MP_Task mpTask)
{
  ENTRY("vLoaderSEitfFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.seitf@"_Stz);
  Loader* pLoader = _pLoader(pPtrLoader);
  
  SEitf* pSEitf = pLoader->pSEitf();
  
  mpTask->m_pDataStack->vPushObject(pSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.seitf! -- store seitf into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSEitfStore(MP_Task mpTask)
{
  ENTRY("vLoaderSEitfStore"_Stz);
  
  Object* pPtrLoader;
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader,pPtrSEitf) )
    THROW_INVALID_DSTACK("loader.seitf!"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  pLoader->vSEitf(pSEitf);
  
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
  Loader* pLoader = _pLoader(pPtrLoader);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job_LoadTable* pJob_LoadTable = pLoader->pJob_LoadTable();
  SEitf* pSEitf = pJob_LoadTable->m_pSEitf;
    
  //! this needs to be optimised
  JobClass* pJobClass_SubLoader = JobClass::pFind(CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader =
    static_cast<Job_SubLoader*>(pJobClass_SubLoader->pPreJob(pSEitf));
  
  pJob_SubLoader->vSeed(SS_SEED_RUN);

  pJob_SubLoader->m_sObjectName = pJob_LoadTable->m_sObjectName;
  pJob_SubLoader->vJob_LoadTable(pJob_LoadTable);
  
  LoaderSub* pLoaderSub = new LoaderSub(pJob_SubLoader,pLoader);
  pJob_SubLoader->vLoaderSub(pLoaderSub);
  
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
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoader->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Steps_Loader.cpp - loader step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Loader(void)
{
  vAddPrimitive("loader.addRecordsInvalid"_Stz, &vLoaderAddRecordsInvalid,false);
  vAddPrimitive("loader.addRecordsValid"_Stz,   &vLoaderAddRecordsValid, false);
  vAddPrimitive("loader.delete"_Stz,            &vLoaderDelete,          false);
  vAddPrimitive("loader.findChunk"_Stz,         &vLoaderFindChunk,       false);
  vAddPrimitive("loader.init"_Stz,              &vLoaderInit,            false);
  vAddPrimitive("loader::new"_Stz,              &vLoaderNew,             false);
  vAddPrimitive("loader.newSub"_Stz,            &vLoaderNewSub,          false);
  vAddPrimitive("loader.path!"_Stz,             &vLoaderPathStore,       false);
  vAddPrimitive("loader.recordsInvalid@"_Stz,   &vLoaderRecordsInvalidFetch, false);
  vAddPrimitive("loader.recordsValid@"_Stz,     &vLoaderRecordsValidFetch,   false);
  vAddPrimitive("loader.seitf@"_Stz,            &vLoaderSEitfFetch,      false);
  vAddPrimitive("loader.seitf!"_Stz,            &vLoaderSEitfStore,      false);
  vAddPrimitive("loader.table!"_Stz,            &vLoaderTableStore,      false);
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
  
  bool bFoundChunk = pLoader->bFindChunk();
  
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
  
  new(pLoader) Loader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader::new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(MP_Task mpTask)
{
  ENTRY("vLoaderNew"_Stz);
  
  Loader* pLoader = new Loader;
  ASSERT(CN_LOADER == pLoader->eClass());
  
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
  
  LoaderSub* pLoaderSub = new LoaderSub;
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

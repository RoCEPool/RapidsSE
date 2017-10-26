/*============================================================================*/
/* Steps_Loader.cpp - loader step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Loader(void)
{
  vAddPrimitive("loader.delete"_Stz,      &vLoaderDelete,      false);
  vAddPrimitive("loader.findChunk"_Stz,   &vLoaderFindChunk,   false);
  vAddPrimitive("loader.init"_Stz,        &vLoaderInit,        false);
  vAddPrimitive("loader::new"_Stz,        &vLoaderNew,         false);
  vAddPrimitive("loader.newSub"_Stz,      &vLoaderNewSub,      false);
  vAddPrimitive("loader.path!"_Stz,       &vLoaderPathStore,   false);
  vAddPrimitive("loader.table!"_Stz,      &vLoaderTableStore,  false);
  vAddPrimitive("loader.tx@"_Stz,         &vLoaderTxFetch,     false);
  vAddPrimitive("loader.tx!"_Stz,         &vLoaderTxStore,     false);
  vAddPrimitive("loader.txref@"_Stz,      &vLoaderTxRefFetch,  false);
  vAddPrimitive("loader.txref!"_Stz,      &vLoaderTxRefStore,  false);
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

/*----------------------------------------------------------------------------*/
/* loader.tx@ -- fetch tx from loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxFetch(MP_Task mpTask)
{
  ENTRY("vLoaderTxFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.txref@"_Stz);
  Loader* pLoader = _pLoader(pPtrLoader);
  
  Tx* pTx = pLoader->pTx();
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.tx! -- store tx into loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxStore(MP_Task mpTask)
{
  ENTRY("vLoaderTxStore"_Stz);
  
  Object* pPtrLoader;
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader,pPtrTx) )
    THROW_INVALID_DSTACK("loader.txref!"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  
  Tx* pTx = _pTx(pPtrTx);
  
  pLoader->vTx(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref@ -- fetch txref from loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefFetch(MP_Task mpTask)
{
  ENTRY("vLoaderTxRefFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.txref@"_Stz);
  Loader* pLoader = _pLoader(pPtrLoader);
  
  TxRef* pTxRef = pLoader->pTxRef();
  
  mpTask->m_pDataStack->vPushObject(pTxRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref! -- store txref into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefStore(MP_Task mpTask)
{
  ENTRY("vLoaderTxRefStore"_Stz);
  
  Object* pPtrLoader;
  Object* pPtrTxRef;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader,pPtrTxRef) )
    THROW_INVALID_DSTACK("loader.txref!"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pLoader->vTxRef(pTxRef);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

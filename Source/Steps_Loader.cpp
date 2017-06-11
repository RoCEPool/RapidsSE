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

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Loader(void)
{
  vAddPrimitive("loader.delete"_Str,      &vLoaderDelete,      false);
  vAddPrimitive("loader.findChunk"_Str,   &vLoaderFindChunk,   false);
  vAddPrimitive("loader.init"_Str,        &vLoaderInit,        false);
  vAddPrimitive("loader.new"_Str,         &vLoaderNew,         false);
  vAddPrimitive("loader.newSub"_Str,      &vLoaderNewSub,      false);
  vAddPrimitive("loader.path!"_Str,       &vLoaderPathStore,   false);
  vAddPrimitive("loader.table!"_Str,      &vLoaderTableStore,  false);
  vAddPrimitive("loader.txref!"_Str,      &vLoaderTxRefStore,  false);
}

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDelete(NNP_Task nnpTask)
{
  ENTRY("vLoaderDelete"_Str);
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.delete"_Str);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.findChunk -- find next chunk                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFindChunk(NNP_Task nnpTask)
{
  ENTRY("vLoaderFindChunk"_Str);
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.findChunk"_Str);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFoundChunk = pLoader->bFindChunk();
  
  nnpTask->m_pDataStack->vPushInt(bFoundChunk);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(NNP_Task nnpTask)
{
  ENTRY("vLoaderInit"_Str);
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.init"_Str);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(NNP_Task nnpTask)
{
  ENTRY("vLoaderNew"_Str);
  
  Loader* pLoader = new Loader;
  ASSERT(CN_LOADER == pLoader->eClass());
  
  nnpTask->m_pDataStack->vPushObject(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.newSub -- new a loader sub                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNewSub(NNP_Task nnpTask)
{
  ENTRY("vLoaderNewSub"_Str);
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.newSub"_Str);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vLoader(pLoader);
  
  nnpTask->m_pDataStack->vPushObject(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.path! -- store file path into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderPathStore"_Str);
  
  ValWrk xWspPath;
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader,xWspPath) )
    THROW_INVALID_DSTACK("loader.path!"_Str);
  
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
void Task::vLoaderTableStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTableStore"_Str);
  
  Object* pPtrTable;
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader,pPtrTable) )
    THROW_INVALID_DSTACK("loader.tableStore"_Str);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoader->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref! -- store txref into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTxRefStore"_Str);
  
  Object* pPtrLoader;
  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader,pPtrTxRef) )
    THROW_INVALID_DSTACK("loader.txref!"_Str);
  
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
